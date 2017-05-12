#pragma once

#include "OSG_Common.h"

class OSG_App;

class CRenderingThread : public OpenThreads::Thread
{
public:
	CRenderingThread(OSG_App* ptr);
	virtual ~CRenderingThread();

	virtual void run();

protected:
	OSG_App* _ptr;
	bool _done;
};
