// MFC_OSG.cpp : implementation of the OpenSceneGraph class
//
#include "stdafx.h"
#include "OpenSceneGraph.h"


class MouseEventHandler : public osgGA::GUIEventHandler {
protected:
	float _mX, _mY;
public:
	MouseEventHandler() : osgGA::GUIEventHandler() {}


	/** OVERRIDE THE HANDLE METHOD:
	The handle() method should return true if the event has been dealt with
	and we do not wish it to be handled by any other handler we may also have
	defined. Whether you return true or false depends on the behaviour you
	want - here we have no other handlers defined so return true. **/
	virtual bool handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa) {

		//osgViewer::View* view = dynamic_cast<osgViewer::View*>(&aa);

		switch (ea.getEventType()) {


			/** HANDLE MOUSE EVENTS:
			Mouse events have associated event names, and mouse
			position co-ordinates. **/
		case osgGA::GUIEventAdapter::RELEASE: {


			/** PERFORM SOME ACTION:
			Once we have received the mouse event, we can
			perform some action. Here we perform mouse picking.

			Mouse picking can be thought of as shooting a "ray"
			from the mouse position into our scene, and determining
			which parts of the scene intersect with the ray.
			Typically we will only be interested in the firstfalse
			(if any) object that the ray hits. We can then perform
			some action.

			In this example, we are going to interact with our
			car model - if the mouse has "picked" it, we will
			start a simple rotation on it. **/



			/** PERFORM MOUSE PICKING:
			In OSG, mouse picking can be done in a few slightly
			different ways, however, the OSG documentation recommends
			using the following steps: **/


			/** 1. Create either a PolytopeIntersector, or a LineIntersector
			using the normalized mouse co-ordinates.**/
			osg::ref_ptr<osgUtil::LineSegmentIntersector> lI = new
				osgUtil::LineSegmentIntersector(osgUtil::Intersector::PROJECTION,
				ea.getXnormalized(), ea.getYnormalized());

			/** 2. Create an IntersectionVisitor, passing the
			Intersector as parameter to the constructor. **/
			osgUtil::IntersectionVisitor iV(lI);

			/** 3. Launch the IntersectionVisitor on the root node of
			the scene graph. In an OSGART application, we can
			launch it on the osgART::Scene node. **/
			////////////////////////////////////////////////////////////////////////////////////////////////////////////_scene->accept(iV);

			/** 4. If the Intersector contains any intersections
			obtain the NodePath and search it to find the
			Node of interest. **/

			if (lI->containsIntersections()) {
				osg::NodePath nP = lI->getFirstIntersection().nodePath;


				/** Here we search the NodePath for the node of interest.
				This is where we can make use of our node naming.

				If we find the Transform node named "CAR", we obtain its
				AnimationPathCallback - if it is currently paused
				we "un-pause" it, and vice-versa. **/
				for (int i = 0; i <= nP.size(); i++) {
					if (nP[i]->getName() == "CAR") {
						osg::AnimationPathCallback* cb = 
							dynamic_cast<osg::AnimationPathCallback*>(nP[i]->getUpdateCallback());

						if (cb->getPause()==true)
							cb->setPause(false);
						else cb->setPause(true);

						return true;
					}
				}
			}
											  } 
		default: return false;
		} 
	} 
};


class CustomKeyboardEventHandler : public osgGA::GUIEventHandler
{
public:
	CustomKeyboardEventHandler() : osgGA::GUIEventHandler()
	{
	}      


	/**
	OVERRIDE THE HANDLE METHOD:
	The handle() method should return true if the event has been dealt with
	and we do not wish it to be handled by any other handler we may also have
	defined. Whether you return true or false depends on the behaviour you 
	want - here we have no other handlers defined so return true.
	**/
	virtual bool handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa, osg::Object* obj, osg::NodeVisitor* nv)
	{
		switch (ea.getEventType()) {
			/** KEY EVENTS:
			Key events have an associated key and event names.
			In this example, we are interested in keys up/down/right/left arrow
			and space bar.
			If we detect a press then we modify the transformation matrix 
			of the local transform node. **/
		case osgGA::GUIEventAdapter::KEYDOWN: {

			switch (ea.getKey()) {

			case osgGA::GUIEventAdapter::KEY_Up: // Move forward 5mm
				//_driveCar->preMult(osg::Matrix::translate(0, -5, 0));
				MessageBox(nullptr, L"KEY_Up", L"Info", MB_OK);
				return true;

			case osgGA::GUIEventAdapter::KEY_Down: // Move back 5mm
				//_driveCar->preMult(osg::Matrix::translate(0, 5, 0));
				MessageBox(nullptr, L"KEY_Down", L"Info", MB_OK);
				return true; 

			case osgGA::GUIEventAdapter::KEY_Left: // Rotate 10 degrees left
				//_driveCar->preMult(osg::Matrix::rotate(osg::DegreesToRadians(10.0f), osg::Z_AXIS));
				MessageBox(nullptr, L"KEY_Left", L"Info", MB_OK);
				return true;

			case osgGA::GUIEventAdapter::KEY_Right: // Rotate 10 degrees right
				//_driveCar->preMult(osg::Matrix::rotate(osg::DegreesToRadians(-10.0f), osg::Z_AXIS));
				MessageBox(nullptr, L"KEY_Right", L"Info", MB_OK);
				return true;

			case ' ': // Reset the transformation
				//_driveCar->setMatrix(osg::Matrix::identity());
				return true;
			}

		default:
			return false;
											  }
		}
	}
};






OpenSceneGraph::OpenSceneGraph(HWND hWnd) : m_hWnd(hWnd)
{
}

OpenSceneGraph::~OpenSceneGraph()
{
	mViewer->setDone(true);
	Sleep(1000);
	mViewer->stopThreading();

	delete mViewer;
}

void OpenSceneGraph::Initialize()
{
	InitScene();
	InitLight();
	InitManipulators();
	InitCameraConfig();

	keyboardEventHandler = new CustomKeyboardEventHandler();
	mViewer->addEventHandler(keyboardEventHandler);
}

void OpenSceneGraph::InitManipulators()
{
	// Create a trackball manipulator
	//trackball = new osgGA::TrackballManipulator();

	//fpsController = new osgGA::FirstPersonManipulator();
	flightController = new osgGA::FlightManipulator();

	// Create a Manipulator Switcher
	keyswitchManipulator = new osgGA::KeySwitchMatrixManipulator;

	// Add our trackball manipulator to the switcher
	//keyswitchManipulator->addMatrixManipulator( '1', "Trackball", trackball.get());
	//keyswitchManipulator->addMatrixManipulator('1', "FPS", fpsController.get());
	keyswitchManipulator->addMatrixManipulator('1', "Flight", flightController.get());

	// Init the switcher to the first manipulator (in this case the only manipulator)
	keyswitchManipulator->selectMatrixManipulator(0);  // Zero based index Value
}


void OpenSceneGraph::InitScene()
{
	m_pRoot = new osg::Group();

	//osg::Box* cube = new osg::Box(osg::Vec3(50, -50, 50), 1.0f);
	//osg::ShapeDrawable* cubeDrawable = new osg::ShapeDrawable(cube);
	//osg::Geode* cubeGeode = new osg::Geode();
	//cubeGeode->addDrawable(cubeDrawable);
	//m_pRoot->addChild(cubeGeode);


	//// Load the Model from the model name
	m_pTeeth = new osg::MatrixTransform;
	m_pRoot->addChild(m_pTeeth.get());
	osg::ref_ptr<osg::Node> tooth11 = this->LoadModel("..\\..\\res\\3D Tooth\\t11_7780.OBJ"); m_pTeeth->addChild(tooth11);
	osg::ref_ptr<osg::Node> tooth12 = this->LoadModel("..\\..\\res\\3D Tooth\\t12_7780.OBJ"); m_pTeeth->addChild(tooth12);
	osg::ref_ptr<osg::Node> tooth13 = this->LoadModel("..\\..\\res\\3D Tooth\\t13_7780.OBJ"); m_pTeeth->addChild(tooth13);
	osg::ref_ptr<osg::Node> tooth14 = this->LoadModel("..\\..\\res\\3D Tooth\\t14_7780.OBJ"); m_pTeeth->addChild(tooth14);
	osg::ref_ptr<osg::Node> tooth15 = this->LoadModel("..\\..\\res\\3D Tooth\\t15_7780.OBJ"); m_pTeeth->addChild(tooth15);
	osg::ref_ptr<osg::Node> tooth16 = this->LoadModel("..\\..\\res\\3D Tooth\\t16_7780.OBJ"); m_pTeeth->addChild(tooth16);
	osg::ref_ptr<osg::Node> tooth17 = this->LoadModel("..\\..\\res\\3D Tooth\\t17_7780.OBJ"); m_pTeeth->addChild(tooth17);

	osg::ref_ptr<osg::Node> tooth21 = this->LoadModel("..\\..\\res\\3D Tooth\\t21_7780.OBJ"); m_pTeeth->addChild(tooth21);
	osg::ref_ptr<osg::Node> tooth22 = this->LoadModel("..\\..\\res\\3D Tooth\\t22_7780.OBJ"); m_pTeeth->addChild(tooth22);
	osg::ref_ptr<osg::Node> tooth23 = this->LoadModel("..\\..\\res\\3D Tooth\\t23_7780.OBJ"); m_pTeeth->addChild(tooth23);
	osg::ref_ptr<osg::Node> tooth24 = this->LoadModel("..\\..\\res\\3D Tooth\\t24_7780.OBJ"); m_pTeeth->addChild(tooth24);
	osg::ref_ptr<osg::Node> tooth25 = this->LoadModel("..\\..\\res\\3D Tooth\\t25_7780.OBJ"); m_pTeeth->addChild(tooth25);
	osg::ref_ptr<osg::Node> tooth26 = this->LoadModel("..\\..\\res\\3D Tooth\\t26_7780.OBJ"); m_pTeeth->addChild(tooth26);
	osg::ref_ptr<osg::Node> tooth27 = this->LoadModel("..\\..\\res\\3D Tooth\\t27_7780.OBJ"); m_pTeeth->addChild(tooth27);

	osg::ref_ptr<osg::Node> tooth31 = this->LoadModel("..\\..\\res\\3D Tooth\\t31_7780.OBJ"); m_pTeeth->addChild(tooth31);
	osg::ref_ptr<osg::Node> tooth32 = this->LoadModel("..\\..\\res\\3D Tooth\\t32_7780.OBJ"); m_pTeeth->addChild(tooth32);
	osg::ref_ptr<osg::Node> tooth33 = this->LoadModel("..\\..\\res\\3D Tooth\\t33_7780.OBJ"); m_pTeeth->addChild(tooth33);
	osg::ref_ptr<osg::Node> tooth34 = this->LoadModel("..\\..\\res\\3D Tooth\\t34_7780.OBJ"); m_pTeeth->addChild(tooth34);
	osg::ref_ptr<osg::Node> tooth35 = this->LoadModel("..\\..\\res\\3D Tooth\\t35_7780.OBJ"); m_pTeeth->addChild(tooth35);
	osg::ref_ptr<osg::Node> tooth36 = this->LoadModel("..\\..\\res\\3D Tooth\\t36_7780.OBJ"); m_pTeeth->addChild(tooth36);
	osg::ref_ptr<osg::Node> tooth37 = this->LoadModel("..\\..\\res\\3D Tooth\\t37_7780.OBJ"); m_pTeeth->addChild(tooth37);

	osg::ref_ptr<osg::Node> tooth41 = this->LoadModel("..\\..\\res\\3D Tooth\\t41_7780.OBJ"); m_pTeeth->addChild(tooth41);
	osg::ref_ptr<osg::Node> tooth42 = this->LoadModel("..\\..\\res\\3D Tooth\\t42_7780.OBJ"); m_pTeeth->addChild(tooth42);
	osg::ref_ptr<osg::Node> tooth43 = this->LoadModel("..\\..\\res\\3D Tooth\\t43_7780.OBJ"); m_pTeeth->addChild(tooth43);
	osg::ref_ptr<osg::Node> tooth44 = this->LoadModel("..\\..\\res\\3D Tooth\\t44_7780.OBJ"); m_pTeeth->addChild(tooth44);
	osg::ref_ptr<osg::Node> tooth45 = this->LoadModel("..\\..\\res\\3D Tooth\\t45_7780.OBJ"); m_pTeeth->addChild(tooth45);
	osg::ref_ptr<osg::Node> tooth46 = this->LoadModel("..\\..\\res\\3D Tooth\\t46_7780.OBJ"); m_pTeeth->addChild(tooth46);
	osg::ref_ptr<osg::Node> tooth47 = this->LoadModel("..\\..\\res\\3D Tooth\\t47_7780.OBJ"); m_pTeeth->addChild(tooth47);

	osg::Matrix m1, m2;
	m1 = osg::Matrix::rotate(osg::DegreesToRadians(-90.0f), 1, 0, 0);
	m2 = osg::Matrix::rotate(osg::DegreesToRadians(-90.0f), 0, 0, 1);
	m_pTeeth->setMatrix(m1 * m2);



	m_pAxisIndicator = new osg::MatrixTransform();
	m_pRoot->addChild(m_pAxisIndicator.get());

	osg::ref_ptr<osg::Node> axisIndicatorX = this->LoadModel("..\\..\\res\\Axes\\AxisXPositive.OBJ"); m_pAxisIndicator->addChild(axisIndicatorX);
	osg::ref_ptr<osg::Node> axisIndicatorY = this->LoadModel("..\\..\\res\\Axes\\AxisYPositive.OBJ"); m_pAxisIndicator->addChild(axisIndicatorY);
	osg::ref_ptr<osg::Node> axisIndicatorZ = this->LoadModel("..\\..\\res\\Axes\\AxisZPositive.OBJ"); m_pAxisIndicator->addChild(axisIndicatorZ);


	m_pObjectMaipulator = new osg::MatrixTransform();
	m_pRoot->addChild(m_pObjectMaipulator.get());
	osg::Matrix mTemp;
	mTemp = osg::Matrix::translate(50, -50, 50);
	m_pObjectMaipulator->setMatrix(mTemp);




	m_pAxes = new osg::MatrixTransform();
	m_pObjectMaipulator->addChild(m_pAxes.get());

	osg::ref_ptr<osg::Node> axisXPositive = this->LoadModel("..\\..\\res\\Axes\\AxisXPositive.OBJ"); m_pAxes->addChild(axisXPositive);
	osg::ref_ptr<osg::Node> axisXNegative = this->LoadModel("..\\..\\res\\Axes\\AxisXNegative.OBJ"); m_pAxes->addChild(axisXNegative);
	osg::ref_ptr<osg::Node> axisYPositive = this->LoadModel("..\\..\\res\\Axes\\AxisYPositive.OBJ"); m_pAxes->addChild(axisYPositive);
	osg::ref_ptr<osg::Node> axisYNegative = this->LoadModel("..\\..\\res\\Axes\\AxisYNegative.OBJ"); m_pAxes->addChild(axisYNegative);
	osg::ref_ptr<osg::Node> axisZPositive = this->LoadModel("..\\..\\res\\Axes\\AxisZPositive.OBJ"); m_pAxes->addChild(axisZPositive);
	osg::ref_ptr<osg::Node> axisZNegative = this->LoadModel("..\\..\\res\\Axes\\AxisZNegative.OBJ"); m_pAxes->addChild(axisZNegative);




	m_pAxisHandles = new osg::MatrixTransform();
	m_pObjectMaipulator->addChild(m_pAxisHandles.get());

	osg::ref_ptr<osg::Node> axisHandleX = this->LoadModel("..\\..\\res\\AxisHandles\\AxisHandleX.OBJ"); m_pAxisHandles->addChild(axisHandleX);
	osg::ref_ptr<osg::Node> axisHandleY = this->LoadModel("..\\..\\res\\AxisHandles\\AxisHandleY.OBJ"); m_pAxisHandles->addChild(axisHandleY);
	osg::ref_ptr<osg::Node> axisHandleZ = this->LoadModel("..\\..\\res\\AxisHandles\\AxisHandleZ.OBJ"); m_pAxisHandles->addChild(axisHandleZ);




	osg::Geode *geode = new osg::Geode;
	osg::Geometry *g = new osg::Geometry;
	osg::Vec3Array *v = new osg::Vec3Array;

	v->push_back(osg::Vec3(0,0,0));
	v->push_back(osg::Vec3(100,0,0));
	v->push_back(osg::Vec3(0,0,0));
	v->push_back(osg::Vec3(0,0,100));

	osg::DrawArrays *da = new 
	osg::DrawArrays(osg::PrimitiveSet::LINES,0,v->size());

	g->setVertexArray( v);
	g->addPrimitiveSet( da);

	geode->addDrawable( g);
	geode->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);

	m_pRoot->addChild(geode);
}

void OpenSceneGraph::InitLight()
{
	osg::Light *light = new osg::Light();
	light->setLightNum(0);
	light->setPosition(osg::Vec4(0.0, 0.0, 0.0, 1.0));
	light->setDiffuse(osg::Vec4(1.0, 1.0, 1.0, 1.0));
	light->setSpecular(osg::Vec4(1.0, 1.0, 1.0, 1.0));
	light->setAmbient(osg::Vec4(2.0, 2.0, 2.0, 2.0));

	osg::StateSet* lightStateSet = m_pRoot->getOrCreateStateSet();

	osg::LightSource* lightSource = new osg::LightSource();
	lightSource->setLight(light);
	lightSource->setLocalStateSetModes(osg::StateAttribute::ON);
	lightSource->setStateSetModes(*lightStateSet, osg::StateAttribute::ON);

	osg::PositionAttitudeTransform *lightTransform = new osg::PositionAttitudeTransform();
	lightTransform->addChild(lightSource);
	lightTransform->setPosition(osg::Vec3(0.0, -100.0, 50.0));
	
	m_pRoot->addChild(lightTransform);
}

void OpenSceneGraph::InitCameraConfig()
{
	// Local Variable to hold window size data
	RECT windowRect;
	RECT clientRect;

	// Create the viewer for this window
	mViewer = new osgViewer::Viewer();

	// Add a Stats Handler to the viewer
	mViewer->addEventHandler(new osgViewer::StatsHandler);

	// Get the current window size
	::GetWindowRect(m_hWnd, &windowRect);
	::GetClientRect(m_hWnd, &clientRect);

	// Init the GraphicsContext Traits
	osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;

	// Init the Windata Variable that holds the handle for the Window to display OSG in.
	osg::ref_ptr<osg::Referenced> windata = new osgViewer::GraphicsWindowWin32::WindowData(m_hWnd);

	// Setup the traits parameters
	traits->x = clientRect.left;
	traits->y = windowRect.bottom - clientRect.bottom;
	traits->width = clientRect.right - clientRect.left;
	traits->height = clientRect.bottom - clientRect.top;

	traits->windowDecoration = false;
	traits->doubleBuffer = true;
	traits->sharedContext = 0;
	traits->setInheritedWindowPixelFormat = true;
	traits->inheritedWindowData = windata;

	// Create the Graphics Context
	osg::GraphicsContext* gc = osg::GraphicsContext::createGraphicsContext(traits.get());

	// Init Master Camera for this View
	osg::ref_ptr<osg::Camera> camera = mViewer->getCamera();

	// Assign Graphics Context to the Camera
	camera->setGraphicsContext(gc);

	traits->x = 0;
	traits->y = 0;
	traits->width = clientRect.right - clientRect.left;
	traits->height = clientRect.bottom - clientRect.top;

	// Set the viewport for the Camera
	camera->setViewport(new osg::Viewport(traits->x, traits->y, traits->width, traits->height));

	// Set projection matrix and camera attribtues
	camera->setClearMask(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	camera->setClearColor(osg::Vec4f(0.3f, 0.5f, 0.7f, 1.0f));
	camera->setProjectionMatrixAsPerspective(
		30.0f, static_cast<double>(traits->width)/static_cast<double>(traits->height), 1.0, 1000.0);

	osg::Vec3d eye( 0.0, -100.0, 50.0 );
	osg::Vec3d center( 0.0, 0.0, 0.0 );
	osg::Vec3d up( 0.0, 0.0, 1.0 );

	camera->setViewMatrixAsLookAt( eye, center, up );

	// Add the Camera to the Viewer
	//mViewer->addSlave(camera.get());
	mViewer->setCamera(camera.get());

	// Add the Camera Manipulator to the Viewer
	//mViewer->setCameraManipulator(keyswitchManipulator.get());

	// Set the Scene Data
	mViewer->setSceneData(m_pRoot.get());

	// Realize the Viewer
	mViewer->realize();

	// Correct aspect ratio
	/*double fovy,aspectRatio,z1,z2;
	mViewer->getCamera()->getProjectionMatrixAsPerspective(fovy,aspectRatio,z1,z2);
	aspectRatio=double(traits->width)/double(traits->height);
	mViewer->getCamera()->setProjectionMatrixAsPerspective(fovy,aspectRatio,z1,z2);*/
}

void OpenSceneGraph::PreFrameUpdate()
{
	// Due any preframe updates in this routine
}

void OpenSceneGraph::PostFrameUpdate()
{
	// Due any postframe updates in this routine
}

/*void OpenSceneGraph::Render(void* ptr)
{
OpenSceneGraph* osg = (OpenSceneGraph*)ptr;

osgViewer::Viewer* viewer = osg->getViewer();

// You have two options for the main viewer loop
//      viewer->run()   or
//      while(!viewer->done()) { viewer->frame(); }

//viewer->run();
while(!viewer->done())
{
osg->PreFrameUpdate();
viewer->frame();
osg->PostFrameUpdate();
//Sleep(10);         // Use this command if you need to allow other processes to have cpu time
}

// For some reason this has to be here to avoid issue:
// if you have multiple OSG windows up
// and you exit one then all stop rendering
AfxMessageBox("Exit Rendering Thread");

_endthread();
}*/

osg::ref_ptr<osg::Node> OpenSceneGraph::LoadModel(std::string filename)
{
	//// Load the Model from the model name
	osg::ref_ptr<osg::Node> model = osgDB::readNodeFile(filename);
	if (!model)
	{
		return nullptr;
	}
	//model->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);

	// Optimize the model
	osgUtil::Optimizer optimizer;
	optimizer.optimize(model.get());
	optimizer.reset();

	return model;
}

CRenderingThread::CRenderingThread( OpenSceneGraph* ptr )
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
