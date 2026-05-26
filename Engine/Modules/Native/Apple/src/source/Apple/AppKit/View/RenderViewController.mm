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
#include "Apple/MetalKit/View/RenderView.h"

#import <Metal/Metal.h>

@interface RenderViewController()
{
	id<MTLDevice> _device;
	NSRect _initialFrame;
	I_ViewControllerEventHandler* _eventHandler;
}
@end

@implementation RenderViewController

- (instancetype) initWithFrame:(NSRect)frame device:(id<MTLDevice>)device {
	self = [super initWithNibName:nil bundle:nil];
	NSLog(@"RenderViewController: initWithFrame called with frame: %@ and device: %@", NSStringFromRect(frame), device);
	if (self) {
		_initialFrame = frame;
		_device = device;
	}
	return self;
}

- (void)setEventHandler:(I_ViewControllerEventHandler*)handler {
	_eventHandler = handler;
}

-(void) loadView {
	RenderView* renderView = [[RenderView alloc] initWithFrame:_initialFrame device:_device];
	self.view = renderView;
	NSLog(@"RenderViewController: loadView called, view set to RenderView with frame: %@", NSStringFromRect(_initialFrame));
}

-(void) viewDidLoad {
	[super viewDidLoad];

	NSLog(@"RenderViewController: viewDidLoad called");

	if (not _eventHandler)
		return;

	_eventHandler->DispatchViewDidLoad();
}

@end
