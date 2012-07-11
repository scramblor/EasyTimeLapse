/*
 * communications.cpp
 *
 * Created: 7/11/2012 1:55:23 PM
 *  Author: brandonw
 */ 

#include "ETL_V1.h"
#include "communications.h"
#include "Arduino/Arduino.h"
#include "SoftModem.h"
#include "Utils.h"

SoftModem modem;
//uint8_t bytesRead;
//uint16_t modemPacketIndex;

VariablePacket recvPacket;

extern uint8_t currentState;

uint32_t printTimer = 0;
byte printval;

uint8_t bytesRead = 0;
size_t modemPacketIndex = 0;

extern SectionConfig myConfigs[MAX_CONFIGS];
extern uint8_t numConfigs;
extern uint8_t configPointer;

void InitTransmitState() {
    SetLED(YELLOW);
    currentState = STATE_TRANSMIT;
    DebugPrint("Enter Transmit");
	
	// This is our unused audio channel. This must go to ground otherwise something
	// weird happens electrically. We could probably use the P/U resistor too.
	pinMode(flashPin, OUTPUT);
	digitalWrite(flashPin, HIGH);
	
    modemPacketIndex = 1;
	bytesRead = 0;
	modem.begin();
	printTimer = millis();
	printval = 'A';
	
	//
	// Send our begin packet
	//
	
	IPhonePacket startPacket;
    memset(&startPacket, 0, sizeof(IPhonePacket));
	startPacket.command = IOS_COMMAND_REQUESTPACKETID;
	startPacket.data = 0x1;

    startPacket.crc = crc_init();
	startPacket.crc = crc_update(startPacket.crc, &startPacket.command, 2);
	startPacket.crc = crc_finalize(startPacket.crc);
	
	modem.writeBytes((uint8_t *) &startPacket, sizeof(startPacket));
}

void LeaveTransmitState() {
	modem.end();
	InitIdleState();
}


void ProcessTransmitState() {
	
	while(modem.available()) {
        ((char *) &recvPacket)[bytesRead] = modem.read();
        bytesRead++;
		
		if (bytesRead >= 16) {
			IPhonePacket sendPacket;
			
			memset(&sendPacket, 0, sizeof(IPhonePacket));
			
			bytesRead = 0;
		
            crc_t myCrc = crc_init();
		    myCrc = crc_update(myCrc, (byte*) &recvPacket + sizeof(crc_t), sizeof(recvPacket) - sizeof(crc_t));
		    myCrc = crc_finalize(myCrc);
					
		    Serial.print(" command: ");
		    Serial.println(recvPacket.command, HEX);
					
		    Serial.print(" packetId: ");
		    Serial.println(recvPacket.packetId, HEX);
					
		    bool failCrc = false;
					
		    //if (random(2) == 0) {
			    //failCrc = true;
			    //Serial.println("force fail crc");
		    //}
					
		    if (myCrc != recvPacket.crc || failCrc) {
			    DebugPrint("Crc mismatch!");
			    Serial.print(" recv_crc = ");
			    Serial.print(recvPacket.crc, HEX);
                Serial.print(" calc_Crc = ");
			    Serial.println(myCrc, HEX);
			    Serial.println();
			    //SetLEDCycle(LED_CYCLE_CRC_ERROR);
			    
				Serial.print("Shots: ");
				Serial.println(recvPacket.basicTimelapse.shots);
				Serial.print("exposure: ");
				Serial.println(recvPacket.basicTimelapse.exposureLengthPower);
				Serial.print("interval: ");
				Serial.println(recvPacket.basicTimelapse.interval);
				
				// TODO for now we always request a packet, need specifc retry code here
		    } else {
			    Serial.print("packet success; crc = ");
				Serial.println(myCrc, HEX);
			    Serial.println();				
					
			    if (modemPacketIndex == recvPacket.packetId) {
				    switch (recvPacket.command) {
	            
	                case ETL_COMMAND_SETTINGS:
				        break;
	                case ETL_COMMAND_BASICTIMELAPSE:
				        myConfigs[configPointer].shots = recvPacket.basicTimelapse.shots;
					    myConfigs[configPointer].exposureOffset = recvPacket.basicTimelapse.exposureLengthPower;
					    myConfigs[configPointer].interval = recvPacket.basicTimelapse.interval;
				        // Increment to the next config. basic MUST come last
					    // TODO dump to EEPROM here
					    configPointer++;
					    numConfigs = configPointer;
				        break;
	                case ETL_COMMAND_BULBRAMP:
				        myConfigs[configPointer].exposureFstopChangePerMin = recvPacket.bulbRamp.exposureFstopChangePerMin;
					    myConfigs[configPointer].fstopChangeOnPress = recvPacket.bulbRamp.fstopChangeOnPress;
					    myConfigs[configPointer].fstopSinAmplitude = recvPacket.bulbRamp.fstopSinAmplitude;
				        break;
	                case ETL_COMMAND_INTERVALRAMP:
				        myConfigs[configPointer].intervalDelta = recvPacket.intervalRamp.intervalDelta;
					    myConfigs[configPointer].numRepeats = recvPacket.intervalRamp.numRepeats;
					    myConfigs[configPointer].repeatIndex = recvPacket.intervalRamp.repeatIndex;
				        break;
	                case ETL_COMMAND_HDRSHOT:
				        myConfigs[configPointer].fstopIncreasePerHDRShot = recvPacket.hdrShot.fstopIncreasePerHDRShot;
					    myConfigs[configPointer].numHDRShots = recvPacket.hdrShot.numHDRShots;
				        break;
				    case ETL_COMMAND_INVALID:
				    default:
				        Serial.print("unrecognized command: ");
					    Serial.println(recvPacket.command, HEX);
				    }	
				    modemPacketIndex++;
			    } else {
				    // TODO reqeust next packet
				}					
		    }
		
		
			// need to make sure iphone is ready to receive again
			// this should be async
			// or ideally we will fix iphone so it can send receive simulteneously...
		    delay(1000);
			
	        sendPacket.command = IOS_COMMAND_REQUESTPACKETID;
	        sendPacket.data = modemPacketIndex;

            sendPacket.crc = crc_init();
	        sendPacket.crc = crc_update(sendPacket.crc, ((uint8_t *) &sendPacket) + sizeof(crc_t), sizeof(sendPacket) - sizeof(crc_t));
	        sendPacket.crc = crc_finalize(sendPacket.crc);
	
	        modem.writeBytes((uint8_t *) &sendPacket, sizeof(sendPacket));
        }							
	}	
}

void ProcessTransmitStateTest() {
	
	//if (millis() > printTimer) {
	    ////modem.write(printval);
		////printval++;
		////printTimer += 5000;
		//
		//modem.printDebugInfo(1);
	//}
	
	while(modem.available()) {
        byte myByte = modem.read();
		
		//Serial.print("byte:");
		//Serial.println(myByte, HEX);
		
		//printTimer = millis() + 3000;
	}		
}