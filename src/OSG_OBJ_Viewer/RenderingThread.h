#pragma once

#include "OSG_Common.h"

class OpenSceneGraph;

class CRenderingThread : public OpenThreads::Thread
{
public:
	CRenderingThread( OpenSceneGraph* ptr );
	virtual ~CRenderingThread();

	virtual void run();

protected:
	OpenSceneGraph* _ptr;
	bool _done;
};
