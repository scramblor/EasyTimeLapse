//
//  ETLShotViewController.h
//  ETL-interface-test
//
//  Created by Carll Hoffman on 6/20/12.
//  Copyright (c) 2012 Microsoft. All rights reserved.
//

#import "ETLViewController.h"
#import "etl_typedefs.h"

@interface ETLShotViewController : ETLViewController
{
    CommandPacket command;
    SectionConfig sections[MAX_CONFIGS];
}

- (IBAction)goProgramDevice:(id)sender;
@end