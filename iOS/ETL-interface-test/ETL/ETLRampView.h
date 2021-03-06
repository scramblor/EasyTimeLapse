//
//  ETLRampView.h
//  ETL
//
//  Created by Carll Hoffman on 8/26/12.
//  Copyright (c) 2012 Microsoft. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <QuartzCore/QuartzCore.h>
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>

#import "ETLThumb.h"

@interface ETLRampView : UIView

@property (nonatomic, assign) NSInteger easeIn;
@property (nonatomic, assign) NSInteger easeOut;
@property (nonatomic, assign) NSInteger initial;
@property (nonatomic, assign) NSInteger final;

@end
