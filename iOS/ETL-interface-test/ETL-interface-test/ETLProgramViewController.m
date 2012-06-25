//
//  ETLProgramControllerViewController.m
//  ETL-interface-test
//
//  Created by Carll Hoffman on 6/20/12.
//  Copyright (c) 2012 Microsoft. All rights reserved.
//

#import "ETLProgramViewController.h"

@interface ETLProgramViewController ()

@end

@implementation ETLProgramViewController

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        deviceInterface = [[ETLDeviceInterface alloc] initWithReceiver:self];
    }
    return self;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    progressBarTimer = [NSTimer scheduledTimerWithTimeInterval:0.01 target:self selector:@selector(updateProgressBar:) userInfo:nil repeats:YES];
    
    [deviceInterface startProgramming];
    [deviceInterface sendCommand:command.Command data:command.Data];
    for (NSUInteger i = 0; i < command.Data; i++) {
        [deviceInterface sendSection:sections + i];
    }
}

const NSUInteger streamBitsPerDataByte = 14;
- (void)updateProgressBar:(NSTimer *)timer
{
    NSUInteger bitCount = [deviceInterface.generator numRawBitsWritten];
    
    bytesTransferred.text = [NSString stringWithFormat:@"%i bytes sent", bitCount / streamBitsPerDataByte];
    if (bitCount < totalCommandBits) {        
        programmingProgress.progress = (bitCount / (totalCommandBits * 1.0)) * 0.9;
    }
    else {
        programmingProgress.progress += 0.002;
    }
    
    if (programmingProgress.progress >= 1) {
        [timer invalidate];
        [deviceInterface stopProgramming];
        bytesTransferred.text = @"Done";
        cancelButton.hidden = true;
        
        [NSTimer scheduledTimerWithTimeInterval:2.0 target:self selector:@selector(goBack:) userInfo:NULL repeats:NO];
    }
}

- (void) setDeviceCommand:(CommandPacket)deviceCommand withSections:(SectionConfig *)commandSections
{
    command = deviceCommand;
    sections = commandSections;
    
    // TODO - review this calculation for general cases
    totalCommandBits = (sizeof(CommandPacket) + sizeof(ETlModemPacket) * command.Data) * streamBitsPerDataByte;
}

- (void)goBack:(id)sender
{
    [deviceInterface stopProgramming];
    [progressBarTimer invalidate];
    progressBarTimer = nil;
    [super goBack:sender];
}

- (void) receivedChar:(char)input
{
    NSLog(@"Received character: %c", input);
}

@end
