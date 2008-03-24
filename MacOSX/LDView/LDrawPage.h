/* LDrawPage */

#import <Cocoa/Cocoa.h>
#import "PreferencePage.h"
#import "TableViewReorder.h"

@interface LDrawPage : PreferencePage <TableViewReorder>
{
    IBOutlet NSSegmentedControl *addRemoveExtraFolder;
    IBOutlet NSTableView *extraFoldersTableView;
    IBOutlet id ldrawDirField;
	
	NSMutableArray *extraFolders;
	TableViewReorder *tableViewReorder;
}

- (void)setup;
- (void)updateLdPreferences;

- (IBAction)addRemoveExtraFolder:(id)sender;
- (IBAction)extraFolderSelected:(id)sender;
- (IBAction)ldrawFolderBrowse:(id)sender;
- (NSMutableArray *)tableRows:(TableViewReorder *)sender;

@end
