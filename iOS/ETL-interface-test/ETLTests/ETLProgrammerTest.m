//
//  ETLProgrammerTest.m
//  ETL-interface-test
//
//  Created by Carll Hoffman on 7/16/12.
//  Copyright (c) 2012 Microsoft. All rights reserved.
//

#import "TestHelper.h"
#import "ETLProgrammer.h"
#import "Common.h"

@interface ETLProgrammerTest : ETLTestCase
{
    ETLProgrammer *programmer;
    id programmerMock, deviceMock, providerMock;
}
@end

@implementation ETLProgrammerTest
- (void)setUpClass {
}

- (void)tearDownClass {
}

- (void)setUp {
    programmer = [[ETLProgrammer alloc] init];
    
    programmerMock = [OCMockObject partialMockForObject:programmer];
    MOCKS_FOR(programmer)
        //WIRE(deviceMock, ETLDeviceInterface, device)
        WIRE_PROTOCOL(providerMock, PacketProvider, packetProvider)
    END
}

- (void)tearDown {
    VERIFY_MOCK(programmerMock)
    VERIFY_MOCK(deviceMock)
}  

- (void)testValidPacketReceived
{    
    MOCKS_FOR(programmer)
        WIRE(deviceMock, ETLDeviceInterface, device)
    END
    UInt32 packetNumber = 1;
    IPhonePacket packet = {0, 0, packetNumber};
    bool crcPass = true;
    
    [[deviceMock expect] stopReader];
    [[deviceMock expect] startPlayer];    
    [[[programmerMock expect] andReturnValue:OCMOCK_VALUE(crcPass)] performSelector:@selector(isCrcValid)];
    [[programmerMock expect] sendPacketNumber:packetNumber];
    
    for (UInt32 i = 0; i < sizeof(IPhonePacket); i++) {
        [programmer receivedChar:((char *)&packet)[i]];
    }
}

- (void)testInvalidPacketReceived
{
    MOCKS_FOR(programmer)
        WIRE(deviceMock, ETLDeviceInterface, device)
    END
    UInt32 packetNumber = 0;
    IPhonePacket packet = {0, 0, packetNumber + 1};
    bool crcPass = false;
    
    [[deviceMock expect] stopReader];
    [[deviceMock expect] startPlayer];
    [[[programmerMock expect] andReturnValue:OCMOCK_VALUE(crcPass)] performSelector:@selector(isCrcValid)];
    [[programmerMock expect] sendPacketNumber:packetNumber];
    
    for (UInt32 i = 0; i < sizeof(IPhonePacket); i++) {
        [programmer receivedChar:((char *)&packet)[i]];
    }
}

- (void)testSendPacketNumber
{
    [[providerMock expect] renderPacket:0 to:[OCMArg anyPointer]];
    [programmer sendPacketNumber:0];
}
@end
