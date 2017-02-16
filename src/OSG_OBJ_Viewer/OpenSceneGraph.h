#pragma once

#include <iostream>



#include <osg/Group>
#include <osg/MatrixTransform>
#include <osg/PositionAttitudeTransform>
#include <osg/ShapeDrawable>
#include <osgViewer/Viewer>
#include <osgGA/TrackballManipulator>





#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgViewer/api/Win32/GraphicsWindowWin32>
#include <osgPresentation/KeyEventHandler>

#include <osg/Geode>
#include <osg/TexGen>
#include <osg/Geometry>
#include <osg/Texture2D>
#include <osg/AutoTransform>
#include <osg/Notify>

#include <osg/io_utils>

#include <osgDB/Registry>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osgDB/FileNameUtils>
#include <osgUtil/Optimizer>

#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>

#include <OpenThreads/Thread>

#include <osgGA/GUIEventHandler>
#include <osgGA/TrackballManipulator>
#include <osgGA/FlightManipulator>
#include <osgGA/DriveManipulator>
#include <osgGA/KeySwitchMatrixManipulator>
#include <osgGA/AnimationPathManipulator>
#include <osgGA/TerrainManipulator>
#include <osgGA/AnimationPathManipulator>
#include <osgGA/StateSetManipulator>
#include <osgGA/MultiTouchTrackballManipulator>

#include <osgPresentation/SlideEventHandler>
#include <osgPresentation/Cursor>




#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

#ifdef _DEBUG
#pragma comment(lib, "osgViewerd.lib")
#pragma comment(lib, "osgTextd.lib")
#pragma comment(lib, "osgGAd.lib")
#pragma comment(lib, "osgDBd.lib")
#pragma comment(lib, "osgUtild.lib")
#pragma comment(lib, "osgd.lib")
#pragma comment(lib, "OpenThreadsd.lib")
#else
#pragma comment(lib, "osgViewer.lib")
#pragma comment(lib, "osgText.lib")
#pragma comment(lib, "osgGA.lib")
#pragma comment(lib, "osgDB.lib")
#pragma comment(lib, "osgUtil.lib")
#pragma comment(lib, "osg.lib")
#pragma comment(lib, "OpenThreads.lib")
#endif


#include <string>

class OpenSceneGraph
{
public:
	OpenSceneGraph(HWND hWnd);
	~OpenSceneGraph();

	void Initialize();
	void InitManipulators();
	void InitScene();
	void InitCameraConfig();
	void InitLight();
	void SetupWindow();
	void SetupCamera();
	void PreFrameUpdate();
	void PostFrameUpdate();
	void Done(bool value) { mDone = value; }
	bool Done() { return mDone; }
	//static void Render(void* ptr);

	osgViewer::Viewer* getViewer() { return mViewer; }

	osg::ref_ptr<osg::Node> LoadModel(std::string filename);

private:
	bool mDone;
	std::string m_ModelName;
	HWND m_hWnd;
	osgViewer::Viewer* mViewer;
	osg::ref_ptr<osg::Group> m_pRoot;
	osg::ref_ptr<osg::MatrixTransform> m_pTeeth;
	
	osg::ref_ptr<osg::MatrixTransform> m_pAxisIndicator;
	osg::ref_ptr<osg::MatrixTransform> m_pObjectMaipulator;
	osg::ref_ptr<osg::MatrixTransform> m_pAxes;
	osg::ref_ptr<osg::MatrixTransform> m_pAxisHandles;

	osg::ref_ptr<osgGA::TrackballManipulator> trackball;
	osg::ref_ptr<osgGA::KeySwitchMatrixManipulator> keyswitchManipulator;

	osg::ref_ptr<osgGA::FirstPersonManipulator> fpsController;
	osg::ref_ptr<osgGA::FlightManipulator> flightController;

	osg::ref_ptr<osgGA::GUIEventHandler> keyboardEventHandler;
};

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
