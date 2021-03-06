//
//  GenericSheet.mm
//  LDView
//
//  Created by Travis Cobbs on 7/29/07.
//  Copyright 2007 __MyCompanyName__. All rights reserved.
//

#import "GenericSheet.h"
#import "LDViewCategories.h"

@implementation GenericSheet

- (void)dealloc
{
	[self releaseTopLevelObjects:topLevelObjects orTopLevelObject:panel];
	[super dealloc];
}

- (id)initWithNibName:(NSString *)nibName
{
	if ((self = [super init]) != nil)
	{
		[self finishInitWithNibName:nibName];
	}
	return self;
}

- (void)finishInitWithNibName:(NSString *)nibName
{
	[self ldvLoadNibNamed:nibName topLevelObjects:&topLevelObjects];
	[topLevelObjects retain];
}

- (NSInteger)runSheetInWindow:(NSWindow *)window;
{
	NSInteger modalResult;
	
	[[NSApplication sharedApplication] beginSheet:panel modalForWindow:window modalDelegate:self didEndSelector:nil contextInfo:NULL];
	modalResult = [[NSApplication sharedApplication] runModalForWindow:panel];
	[panel orderOut:self];
	return modalResult;
}

- (IBAction)cancel:(id)sender
{
	[self stopModalWithCode:NSModalResponseCancel];
}

- (IBAction)ok:(id)sender
{
	[self stopModalWithCode:NSModalResponseOK];
}

- (void)stopModalWithCode:(NSModalResponse)returnCode
{
	[[NSApplication sharedApplication] endSheet:panel];
	[[NSApplication sharedApplication] stopModalWithCode:returnCode];
}

@end
