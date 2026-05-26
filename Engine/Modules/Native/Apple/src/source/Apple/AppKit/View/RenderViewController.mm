//
// Module: CelestialEngine/Engine/Modules/Native/Apple/AppKit/View
// File: RenderViewController.h
// Created by: Catalin Chirosca
// Created: 2026-05-25
// Updated by: Catalin Chirosca
// Updated: 2026-05-26
//

#include "Apple/AppKit/View/RenderViewController.h"
#include "Apple/MetalCpp/MetalKit/MTKView.hpp"

@interface RenderViewController()
{
	MTL::Device* _device;
	NSRect _initialFrame;
}
@end

@implementation RenderViewController

- (instancetype) initWithFrame:(NSRect)frame device:(MTL::Device*)device {
	self = [super initWithNibName:nil bundle:nil];
	if (self) {
		_initialFrame = frame;
		_device = device;
	}
	return self;
}

-(void) loadView {
	MTK::View* mtkView = MTK::View::alloc()->init(_initialFrame, _device);
	self.view = (__bridge NSView*)mtkView;
}

-(void) viewDidLoad {
	[super viewDidLoad];
}

@end
