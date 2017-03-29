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

#include <osg/LineSegment>
#include <osg/Geode>
#include <osg/TexGen>
#include <osg/Geometry>
#include <osg/Texture2D>
#include <osg/Material>
#include <osg/AutoTransform>
#include <osg/Notify>

#include <osg/io_utils>

#include <osgDB/Registry>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osgDB/FileNameUtils>
#include <osgUtil/Optimizer>
#include <osgUtil/RayIntersector>

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
