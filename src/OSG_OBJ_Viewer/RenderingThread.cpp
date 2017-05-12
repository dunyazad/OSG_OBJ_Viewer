#include "stdafx.h"
#include "OSG_App.h"
#include "RenderingThread.h"

CRenderingThread::CRenderingThread(OSG_App* ptr)
	:   OpenThreads::Thread(), _ptr(ptr), _done(false)
{
}

CRenderingThread::~CRenderingThread()
{
	_done = true;
	if (isRunning())
	{
		cancel();
		join();
	}
}

void CRenderingThread::run()
{
	if ( !_ptr )
	{
		_done = true;
		return;
	}

	osgViewer::Viewer* viewer = _ptr->getViewer();
	do
	{
		_ptr->PreFrameUpdate();
		viewer->frame();
		_ptr->PostFrameUpdate();
	} while ( !testCancel() && !viewer->done() && !_done );
}
