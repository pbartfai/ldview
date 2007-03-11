/*
 *  AlertHandler.h
 *  LDView
 *
 *  Created by Travis Cobbs on 7/15/06.
 *  Copyright 2006 __MyCompanyName__. All rights reserved.
 *
 */

#include <TCFoundation/TCObject.h>

@class ModelWindow;

class AlertHandler: public TCObject
{
public:
	AlertHandler(ModelWindow *modelWindow);
protected:
	~AlertHandler(void);
	void dealloc(void);
	void ldlErrorCallback(LDLError *error);
	void progressAlertCallback(TCProgressAlert *alert);

	ModelWindow *modelWindow;
};
