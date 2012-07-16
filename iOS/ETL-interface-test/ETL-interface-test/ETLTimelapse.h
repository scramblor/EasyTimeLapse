//
//  ETLTimelapse.h
//  ETL-interface-test
//
//  Created by Carll Hoffman on 7/12/12.
//  Copyright (c) 2012 Microsoft. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "ETLModel.h"

@interface ETLTimelapse : ETLModel
{
}

-(bool) continuousShooting;

@property (nonatomic) UInt64 shotInterval;
@property (nonatomic) UInt64 shotCount;
@property (nonatomic) NSTimeInterval clipLength;
@property (nonatomic) NSTimeInterval shootingTime;
@property (nonatomic) double clipFramesPerSecond;

@end