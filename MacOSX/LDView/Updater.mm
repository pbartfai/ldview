#import "Updater.h"
#import "OCLocalStrings.h"
#import "LDViewCategories.h"
#import "UpdaterAlertHandler.h"

#include <TCFoundation/TCProgressAlert.h>
#include <TCFoundation/TCStringArray.h>
#include <LDLib/LDLibraryUpdater.h>
#include <LDLib/LDUserDefaultsKeys.h>
#include <LDLoader/LDLModel.h>

#if MAC_OS_X_VERSION_MAX_ALLOWED < 1050

@interface NSFileManager (unimplemented)
- (BOOL)createDirectoryAtPath:(NSString *)aPathname withIntermediateDirectories:(BOOL)aIntermediate attributes:(NSDictionary *)aAttributes error:(NSError **)aError;
@end

#endif // Tiger

@implementation Updater

- (id) init
{
	self = [super init];
	if (self != nil)
	{
		[self ldvLoadNibNamed:@"Updater" topLevelObjects:&topLevelObjects];
		[topLevelObjects retain];
	}
	return self;
}

- (void)dealloc
{
	[self releaseTopLevelObjects:topLevelObjects orTopLevelObject:panel];
	TCObject::release(alertHandler);
	[super dealloc];
}

- (void)awakeFromNib
{
	alertHandler = new UpdaterAlertHandler(self);
}

#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
- (bool)run:(NSString *)targetDir full:(bool)fullDownload isParent:(bool)isParent
{
#ifdef _NO_BOOST
	NSRunAlertPanel(@"Error", @"The Tiger version of LDView does not support automatic LDraw library installation.", @"OK", nil, nil);
	return false;
#else // _NO_BOOST
	NSString *ldrawDir;
	if (isParent)
	{
		ldrawDir = [targetDir stringByAppendingPathComponent:@"ldraw"];
	}
	else
	{
		ldrawDir = targetDir;
	}
	NSFileManager *fileManager = [NSFileManager defaultManager];
	BOOL isDir;

	if (![fileManager fileExistsAtPath:ldrawDir isDirectory:&isDir])
	{
		BOOL created = NO;
		
		if ([fileManager respondsToSelector:@selector(createDirectoryAtPath:withIntermediateDirectories:attributes:error:)])
		{
			created = [fileManager createDirectoryAtPath:ldrawDir withIntermediateDirectories:NO attributes:nil error:NULL];
		}
		else if ([fileManager respondsToSelector:@selector(createDirectoryAtPath:attributes:)])
		{
			created = [fileManager createDirectoryAtPath:ldrawDir attributes:@{}];
		}
		if(!created)
		{
			NSRunCriticalAlertPanel([OCLocalStrings get:@"ErrorCreatingLDrawFolder"], [OCLocalStrings get:@"EnsureParentFolderWriteAccess"], [OCLocalStrings get:@"OK"], nil, nil);
			return NSModalResponseCancel;
		}
	}
	else if (!isDir)
	{
		NSRunCriticalAlertPanel([OCLocalStrings get:@"FileExists"], [NSString stringWithFormat:[OCLocalStrings get:@"FileExistsFormat"], ldrawDir], [OCLocalStrings get:@"OK"], nil, nil);
		return NSModalResponseCancel;
	}
	canceled = NO;
	done = NO;
	error = NO;
	updater = new LDLibraryUpdater;
	[progress setDoubleValue:0.0];
	[textField setStringValue:[OCLocalStrings get:@"CheckingForLibraryUpdates"]];
	updater->setLibraryUpdateKey(LAST_LIBRARY_UPDATE_KEY);
	updater->setLdrawZipPath(LDLModel::ldrawZipPath());
	updater->setLdrawDir([ldrawDir UTF8String]);
	bool zipInstall = false;
	if (fullDownload)
	{
		zipInstall = [NSAlert runModalWithTitle:[OCLocalStrings get:@"InstallZipTitle"] message:[OCLocalStrings get:@"InstallZipMessage"] defaultButton:[OCLocalStrings get:@"Yes"] alternateButton:[OCLocalStrings get:@"No"] otherButton:nil] == NSAlertFirstButtonReturn;
		updater->installLDraw(zipInstall);
	}
	else
	{
		updater->checkForUpdates();
	}
	[NSApp runModalForWindow:panel];
	[panel orderOut:self];
	TCObject::release(updater);
	updater = NULL;
	if (done && !error && !canceled)
	{
		if (zipInstall)
		{
			std::string ldrawZipPath;
			combinePath([ldrawDir UTF8String], "complete.zip", ldrawZipPath);
			LDLModel::setLDrawZipPath(ldrawZipPath);
		}
		return true;
	}
	else
	{
		return false;
	}
#endif // _NO_BOOST
}
#pragma GCC diagnostic warning "-Wdeprecated-declarations"

- (bool)checkForUpdates:(NSString *)targetDir isParent:(bool)isParent
{
	return [self run:targetDir full:false isParent:isParent];
}

- (bool)downloadLDraw:(NSString *)targetDir
{
	return [self run:targetDir full:true isParent:true];
}

- (void)setProgressValue:(NSNumber *)amount
{
	if ([amount doubleValue] == 1.0 || [amount doubleValue] == 2.0)
	{
		// When we're done, jump the progress immediately to the end without
		// animating.
		progress.usesThreadedAnimation = NO;
	}
	[progress setDoubleValue:[amount doubleValue]];
	[amount release];
}

- (void)setMessageText:(NSString *)message
{
	[textField setStringValue:message];
	[message release];
}

- (void)switchToDone
{
	[cancelButton setEnabled:NO];
	[okButton setEnabled:YES];
}

- (void)noUpdates
{
	[textField setStringValue:[OCLocalStrings get:@"LibraryUpdateUnnecessary"]];
	[self switchToDone];
}

- (void)updateFinished
{
	[textField setStringValue:[OCLocalStrings get:@"LibraryUpdateComplete"]];
	[self switchToDone];
}

- (void)updateCanceled
{
	[textField setStringValue:[OCLocalStrings get:@"LibraryUpdateCanceled"]];
	[self switchToDone];
}

- (void)updateError
{
#ifndef _NO_BOOST
	[textField setStringValue:[NSString stringWithFormat:@"%@:\n%@", [OCLocalStrings get:@"LibraryUpdateError"], [NSString stringWithUCString:updater->getError()]]];
	[self switchToDone];
#endif // _NO_BOOST
}

- (void)cancel
{
	[NSApp stopModalWithCode:NSModalResponseOK];
	TCObject::release(alertHandler);
	alertHandler = NULL;
}

- (void)progressCallback:(TCProgressAlert *)alert
{
#ifndef _NO_BOOST
	// NOTE: this gets called from a background thread.  That background thread
	// has no NSAutorleasePool in place, so don't do anything that would
	// trigger an autorelease (like [NSNumber numberWithDouble:].
	// Also, don't update the UI directly.  Instead, use
	// performSelectorOnMainThread:withObject:waitUntilDone:.
	if (alert && strcmp(alert->getSource(), LD_LIBRARY_UPDATER) == 0)
	{
		float progress = alert->getProgress();
		[self performSelectorOnMainThread:@selector(setProgressValue:) withObject:[[NSNumber alloc] initWithDouble:progress] waitUntilDone:NO];
		[self performSelectorOnMainThread:@selector(setMessageText:) withObject:[[NSString alloc] initWithUCString:alert->getMessageUC()] waitUntilDone:NO];
		if (progress == 1.0f)
		{
			if (alert->getExtraInfo())
			{
				if (strcmp((*(alert->getExtraInfo()))[0], "None") == 0)
				{
					// No updates available.
					[self performSelectorOnMainThread:@selector(noUpdates) withObject:nil waitUntilDone:YES];
				}
				else
				{
					// Success
					[self performSelectorOnMainThread:@selector(updateFinished) withObject:nil waitUntilDone:YES];
				}
			}
			else
			{
				[self performSelectorOnMainThread:@selector(updateCanceled) withObject:nil waitUntilDone:YES];
			}
			done = true;
		}
		else if (progress == 2.0f)
		{
			[self performSelectorOnMainThread:@selector(updateError) withObject:nil waitUntilDone:YES];
			error = YES;
		}
		if (canceled)
		{
			alert->abort();
			[self performSelectorOnMainThread:@selector(cancel) withObject:nil waitUntilDone:NO];
		}
	}
#endif // _NO_BOOST
}

- (IBAction)ok:(id)sender
{
	[NSApp stopModalWithCode:NSModalResponseOK];
}

- (IBAction)cancel:(id)sender
{
	canceled = true;
}

@end
