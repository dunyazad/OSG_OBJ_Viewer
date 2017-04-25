// MFC_OSG.cpp : implementation of the OpenSceneGraph class
//
#include "stdafx.h"
#include "OpenSceneGraph.h"

#include "tiny_obj_loader.h"

#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

#include <ctime>

class timerutil {
 public:
#ifdef _WIN32
  typedef DWORD time_t;

  timerutil() { ::timeBeginPeriod(1); }
  ~timerutil() { ::timeEndPeriod(1); }

  void start() { t_[0] = ::timeGetTime(); }
  void end() { t_[1] = ::timeGetTime(); }

  time_t sec() { return (time_t)((t_[1] - t_[0]) / 1000); }
  time_t msec() { return (time_t)((t_[1] - t_[0])); }
  time_t usec() { return (time_t)((t_[1] - t_[0]) * 1000); }
  time_t current() { return ::timeGetTime(); }

#else
#if defined(__unix__) || defined(__APPLE__)
  typedef unsigned long int time_t;

  void start() { gettimeofday(tv + 0, &tz); }
  void end() { gettimeofday(tv + 1, &tz); }

  time_t sec() { return (time_t)(tv[1].tv_sec - tv[0].tv_sec); }
  time_t msec() {
    return this->sec() * 1000 +
           (time_t)((tv[1].tv_usec - tv[0].tv_usec) / 1000);
  }
  time_t usec() {
    return this->sec() * 1000000 + (time_t)(tv[1].tv_usec - tv[0].tv_usec);
  }
  time_t current() {
    struct timeval t;
    gettimeofday(&t, NULL);
    return (time_t)(t.tv_sec * 1000 + t.tv_usec);
  }

#else  // C timer
  // using namespace std;
  typedef clock_t time_t;

  void start() { t_[0] = clock(); }
  void end() { t_[1] = clock(); }

  time_t sec() { return (time_t)((t_[1] - t_[0]) / CLOCKS_PER_SEC); }
  time_t msec() { return (time_t)((t_[1] - t_[0]) * 1000 / CLOCKS_PER_SEC); }
  time_t usec() { return (time_t)((t_[1] - t_[0]) * 1000000 / CLOCKS_PER_SEC); }
  time_t current() { return (time_t)clock(); }

#endif
#endif

 private:
#ifdef _WIN32
  DWORD t_[2];
#else
#if defined(__unix__) || defined(__APPLE__)
  struct timeval tv[2];
  struct timezone tz;
#else
  time_t t_[2];
#endif
#endif
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

	if(m_pCameraController) delete m_pCameraController;
}

void OpenSceneGraph::Initialize()
{
	InitScene();
	InitCameraConfig();
	InitLight();
	InitModels();
}

void OpenSceneGraph::InitModels()
{
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::map<std::string, GLuint> textures;

	timerutil tm;

	tm.start();

	std::string base_dir = "./";
#ifdef _WIN32
	base_dir += "\\";
#else
	base_dir += "/";
#endif

	std::string err;
	bool ret =
		//tinyobj::LoadObj(&attrib, &shapes, &materials, &err, "E:\\Resources\\OBJ\\OOOO R8\\R8.obj", base_dir.c_str());
		tinyobj::LoadObj(&attrib, &shapes, &materials, &err, "E:\\Resources\\OBJ\\F-15K\\F-15K.obj", base_dir.c_str());
	if (!err.empty()) {
		std::cerr << err << std::endl;
	}

	tm.end();

	if (!ret) {
		std::cerr << "Failed to load " << "E:\\Resources\\OBJ\\OOOO R8\\R8.obj" << std::endl;
		return;
	}

	printf("Parsing time: %d [ms]\n", (int)tm.msec());





	osg::Geode *geode = new osg::Geode;
	osg::Geometry *g = new osg::Geometry;
	
	osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array;
	int size = attrib.vertices.size();
	for(int i = 0; i < size; i += 3) {
		v->push_back(osg::Vec3(attrib.vertices[i], attrib.vertices[i + 1], attrib.vertices[i + 2]));
	}
	g->setVertexArray(v);

	osg::ref_ptr<osg::Vec3Array> n = new osg::Vec3Array;
	size = attrib.normals.size();
	for(int i = 0; i < size; i += 3) {
		n->push_back(osg::Vec3(attrib.normals[i], attrib.normals[i + 1], attrib.normals[i + 2]));
	}
	g->setNormalArray(v);

	osg::ref_ptr<osg::Vec2Array> t = new osg::Vec2Array;
	size = attrib.texcoords.size();
	for(int i = 0; i < size; i += 2) {
		t->push_back(osg::Vec2(attrib.texcoords[i], attrib.texcoords[i + 1]));
	}
	g->setTexCoordArray(0, t);


	
	osg::DrawArrays *da = new 
		osg::DrawArrays(osg::PrimitiveSet::TRIANGLES, 0, v->size());

	g->addPrimitiveSet(da);

	geode->addDrawable(g);
	geode->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);

	m_pRoot->addChild(geode);





	//m_pF15K = new osg::MatrixTransform;
	//m_pRoot->addChild(m_pF15K.get());
	////osg::ref_ptr<osg::Node> m_pF15KModel = this->LoadModel("..\\..\\res\\F-15K\\F-15K.OBJ"); m_pF15K->addChild(m_pF15KModel);
	//osg::ref_ptr<osg::Node> m_pF15KModel = this->LoadModel("E:\\Resources\\OBJ\\OOOO R8\\R8.obj"); m_pF15K->addChild(m_pF15KModel);
	//m_pF15KModel->setName("F-15K");
	////osg::Matrix scale = osg::Matrix::scale(0.005, 0.005, 0.005);
	////m_pF15K->setMatrix(scale);

	//osg::Box* cube = new osg::Box(osg::Vec3(50, -50, 50), 1.0f);
	//osg::ShapeDrawable* cubeDrawable = new osg::ShapeDrawable(cube);
	//osg::Geode* cubeGeode = new osg::Geode();
	//cubeGeode->addDrawable(cubeDrawable);
	//m_pRoot->addChild(cubeGeode);

	// Load the Model from the model name
	//m_pTeeth = new osg::MatrixTransform;
	//m_pRoot->addChild(m_pTeeth.get());
	//osg::ref_ptr<osg::Node> tooth11 = this->LoadModel("..\\..\\res\\3D Tooth\\t11_7780.OBJ"); m_pTeeth->addChild(tooth11);
	//osg::ref_ptr<osg::Node> tooth12 = this->LoadModel("..\\..\\res\\3D Tooth\\t12_7780.OBJ"); m_pTeeth->addChild(tooth12);
	//osg::ref_ptr<osg::Node> tooth13 = this->LoadModel("..\\..\\res\\3D Tooth\\t13_7780.OBJ"); m_pTeeth->addChild(tooth13);
	//osg::ref_ptr<osg::Node> tooth14 = this->LoadModel("..\\..\\res\\3D Tooth\\t14_7780.OBJ"); m_pTeeth->addChild(tooth14);
	//osg::ref_ptr<osg::Node> tooth15 = this->LoadModel("..\\..\\res\\3D Tooth\\t15_7780.OBJ"); m_pTeeth->addChild(tooth15);
	//osg::ref_ptr<osg::Node> tooth16 = this->LoadModel("..\\..\\res\\3D Tooth\\t16_7780.OBJ"); m_pTeeth->addChild(tooth16);
	//osg::ref_ptr<osg::Node> tooth17 = this->LoadModel("..\\..\\res\\3D Tooth\\t17_7780.OBJ"); m_pTeeth->addChild(tooth17);

	//osg::ref_ptr<osg::Node> tooth21 = this->LoadModel("..\\..\\res\\3D Tooth\\t21_7780.OBJ"); m_pTeeth->addChild(tooth21);
	//osg::ref_ptr<osg::Node> tooth22 = this->LoadModel("..\\..\\res\\3D Tooth\\t22_7780.OBJ"); m_pTeeth->addChild(tooth22);
	//osg::ref_ptr<osg::Node> tooth23 = this->LoadModel("..\\..\\res\\3D Tooth\\t23_7780.OBJ"); m_pTeeth->addChild(tooth23);
	//osg::ref_ptr<osg::Node> tooth24 = this->LoadModel("..\\..\\res\\3D Tooth\\t24_7780.OBJ"); m_pTeeth->addChild(tooth24);
	//osg::ref_ptr<osg::Node> tooth25 = this->LoadModel("..\\..\\res\\3D Tooth\\t25_7780.OBJ"); m_pTeeth->addChild(tooth25);
	//osg::ref_ptr<osg::Node> tooth26 = this->LoadModel("..\\..\\res\\3D Tooth\\t26_7780.OBJ"); m_pTeeth->addChild(tooth26);
	//osg::ref_ptr<osg::Node> tooth27 = this->LoadModel("..\\..\\res\\3D Tooth\\t27_7780.OBJ"); m_pTeeth->addChild(tooth27);

	//osg::ref_ptr<osg::Node> tooth31 = this->LoadModel("..\\..\\res\\3D Tooth\\t31_7780.OBJ"); m_pTeeth->addChild(tooth31);
	//osg::ref_ptr<osg::Node> tooth32 = this->LoadModel("..\\..\\res\\3D Tooth\\t32_7780.OBJ"); m_pTeeth->addChild(tooth32);
	//osg::ref_ptr<osg::Node> tooth33 = this->LoadModel("..\\..\\res\\3D Tooth\\t33_7780.OBJ"); m_pTeeth->addChild(tooth33);
	//osg::ref_ptr<osg::Node> tooth34 = this->LoadModel("..\\..\\res\\3D Tooth\\t34_7780.OBJ"); m_pTeeth->addChild(tooth34);
	//osg::ref_ptr<osg::Node> tooth35 = this->LoadModel("..\\..\\res\\3D Tooth\\t35_7780.OBJ"); m_pTeeth->addChild(tooth35);
	//osg::ref_ptr<osg::Node> tooth36 = this->LoadModel("..\\..\\res\\3D Tooth\\t36_7780.OBJ"); m_pTeeth->addChild(tooth36);
	//osg::ref_ptr<osg::Node> tooth37 = this->LoadModel("..\\..\\res\\3D Tooth\\t37_7780.OBJ"); m_pTeeth->addChild(tooth37);

	//osg::ref_ptr<osg::Node> tooth41 = this->LoadModel("..\\..\\res\\3D Tooth\\t41_7780.OBJ"); m_pTeeth->addChild(tooth41);
	//osg::ref_ptr<osg::Node> tooth42 = this->LoadModel("..\\..\\res\\3D Tooth\\t42_7780.OBJ"); m_pTeeth->addChild(tooth42);
	//osg::ref_ptr<osg::Node> tooth43 = this->LoadModel("..\\..\\res\\3D Tooth\\t43_7780.OBJ"); m_pTeeth->addChild(tooth43);
	//osg::ref_ptr<osg::Node> tooth44 = this->LoadModel("..\\..\\res\\3D Tooth\\t44_7780.OBJ"); m_pTeeth->addChild(tooth44);
	//osg::ref_ptr<osg::Node> tooth45 = this->LoadModel("..\\..\\res\\3D Tooth\\t45_7780.OBJ"); m_pTeeth->addChild(tooth45);
	//osg::ref_ptr<osg::Node> tooth46 = this->LoadModel("..\\..\\res\\3D Tooth\\t46_7780.OBJ"); m_pTeeth->addChild(tooth46);
	//osg::ref_ptr<osg::Node> tooth47 = this->LoadModel("..\\..\\res\\3D Tooth\\t47_7780.OBJ"); m_pTeeth->addChild(tooth47);

	//osg::Matrix m1, m2;
	//m1 = osg::Matrix::rotate(osg::DegreesToRadians(-90.0f), 1, 0, 0);
	//m2 = osg::Matrix::rotate(osg::DegreesToRadians(-90.0f), 0, 0, 1);
	//m_pTeeth->setMatrix(m1 * m2);

	//m_pF15K = new osg::MatrixTransform;
	//m_pRoot->addChild(m_pF15K.get());
	////osg::ref_ptr<osg::Node> m_pF15KModel = this->LoadModel("..\\..\\res\\F-15K\\F-15K.OBJ"); m_pF15K->addChild(m_pF15KModel);
	//osg::ref_ptr<osg::Node> m_pF15KModel = this->LoadModel("..\\..\\res\\Capsule\\capsule.obj"); m_pF15K->addChild(m_pF15KModel);
	//m_pF15KModel->setName("F-15K");
	////osg::Matrix scale = osg::Matrix::scale(0.005, 0.005, 0.005);
	////m_pF15K->setMatrix(scale);


	//auto rotation = osg::Matrix::rotate(45 * osg::PI/180, osg::Vec3(0, 0, 1));

	//auto group = m_pF15KModel->asGroup();
	//for(int i = 0; i < group->getNumChildren(); i++)
	//{
	//	auto geode = group->getChild(i)->asGeode();
	//	if(nullptr != geode) {
	//		for(int j = 0; j < geode->getNumChildren(); j++) {
	//			auto geometry = geode->getChild(j)->asGeometry();
	//			if(nullptr != geometry) {
	//				osg::StateSet* ss = geometry->getOrCreateStateSet();				
	//				ss->setMode(GL_LIGHTING,osg::StateAttribute::ON);

	//				osg::ref_ptr<osg::Material> material = new osg::Material;
	//				material->setDiffuse (osg::Material::FRONT_AND_BACK, osg::Vec4(1.0, 1.0, 1.0, 1.0));
	//				material->setAmbient(osg::Material::FRONT, osg::Vec4(1.0, 1.0, 1.0, 1.0));
	//				material->setSpecular(osg::Material::FRONT, osg::Vec4(0.15, 0.15, 0.15, 1.0));
	//				material->setEmission(osg::Material::FRONT, osg::Vec4(0.4, 0.4, 0.4, 1.0));
	//				/*
	//				pMat->setDiffuse (osg::Material::FRONT_AND_BACK, osg::Vec4(1.0, 1.0, 1.0, 1.0));
	//				pMat->setAmbient(osg::Material::FRONT, osg::Vec4(1.0, 1.0, 1.0, 1.0));
	//				pMat->setSpecular(osg::Material::FRONT, osg::Vec4(0.15, 0.15, 0.15, 1.0));
	//				*/
	//				ss->setAttributeAndModes(material, osg::StateAttribute::ON|osg::StateAttribute::OVERRIDE);		

	//				osg::ref_ptr<osg::Texture2D> texture = (osg::Texture2D*) ss->getTextureAttribute(0, osg::StateAttribute::TEXTURE); 
	//				if(texture)
	//				{
	//					texture->setInternalFormatMode(osg::Texture2D::USE_S3TC_DXT1_COMPRESSION);
	//					texture->setUnRefImageDataAfterApply(true);
	//					
	//					auto filename = texture->getImage()->getFileName();
	//					//_splitpath(filename.c_str(), )

	//					osgDB::writeImageFile(*texture->getImage(), "D:\\result.bmp");
	//				}

	//				osg::Vec3Array* va = dynamic_cast<osg::Vec3Array*>(geometry->getVertexArray());
	//				if(nullptr != va) {
	//					auto vi = va->begin();
	//					auto ve = va->end();
	//					for(; vi != ve; vi++) {
	//						(*vi) = (*vi) * rotation;
	//					}
	//				}

	//				osg::Vec3Array* na = dynamic_cast<osg::Vec3Array*>(geometry->getNormalArray());
	//				if(nullptr != na) {
	//					auto ni = na->begin();
	//					auto ne = na->end();
	//					for(; ni != ne; ni++) {
	//						(*ni) = (*ni) * rotation;
	//					}
	//				}
	//			}
	//		}
	//	}
	//}

	//if(!osgDB::writeNodeFile(*m_pF15KModel, "D:\\result.obj")) {
	//	MessageBox(nullptr, L"Error", L"Error", MB_OK);
	//}

	//m_pAxisIndicator = new osg::MatrixTransform();
	//m_pCameraSub->addChild(m_pAxisIndicator.get());

	//osg::ref_ptr<osg::Node> axisIndicatorX = this->LoadModel("..\\..\\res\\Axes\\AxisXPositive.OBJ"); m_pAxisIndicator->addChild(axisIndicatorX);
	//osg::ref_ptr<osg::Node> axisIndicatorY = this->LoadModel("..\\..\\res\\Axes\\AxisYPositive.OBJ"); m_pAxisIndicator->addChild(axisIndicatorY);
	//osg::ref_ptr<osg::Node> axisIndicatorZ = this->LoadModel("..\\..\\res\\Axes\\AxisZPositive.OBJ"); m_pAxisIndicator->addChild(axisIndicatorZ);


	//m_pObjectMaipulator = new osg::MatrixTransform();
	//m_pRoot->addChild(m_pObjectMaipulator.get());
	//osg::Matrix mTemp;
	//mTemp = osg::Matrix::translate(50, -50, 50);
	//m_pObjectMaipulator->setMatrix(mTemp);




	//m_pAxes = new osg::MatrixTransform();
	//m_pObjectMaipulator->addChild(m_pAxes.get());

	//osg::ref_ptr<osg::Node> axisXPositive = this->LoadModel("..\\..\\res\\Axes\\AxisXPositive.OBJ"); m_pAxes->addChild(axisXPositive);
	//osg::ref_ptr<osg::Node> axisXNegative = this->LoadModel("..\\..\\res\\Axes\\AxisXNegative.OBJ"); m_pAxes->addChild(axisXNegative);
	//osg::ref_ptr<osg::Node> axisYPositive = this->LoadModel("..\\..\\res\\Axes\\AxisYPositive.OBJ"); m_pAxes->addChild(axisYPositive);
	//osg::ref_ptr<osg::Node> axisYNegative = this->LoadModel("..\\..\\res\\Axes\\AxisYNegative.OBJ"); m_pAxes->addChild(axisYNegative);
	//osg::ref_ptr<osg::Node> axisZPositive = this->LoadModel("..\\..\\res\\Axes\\AxisZPositive.OBJ"); m_pAxes->addChild(axisZPositive);
	//osg::ref_ptr<osg::Node> axisZNegative = this->LoadModel("..\\..\\res\\Axes\\AxisZNegative.OBJ"); m_pAxes->addChild(axisZNegative);




	//m_pAxisHandles = new osg::MatrixTransform();
	//m_pObjectMaipulator->addChild(m_pAxisHandles.get());

	//osg::ref_ptr<osg::Node> axisHandleX = this->LoadModel("..\\..\\res\\AxisHandles\\AxisHandleX.OBJ"); m_pAxisHandles->addChild(axisHandleX);
	//osg::ref_ptr<osg::Node> axisHandleY = this->LoadModel("..\\..\\res\\AxisHandles\\AxisHandleY.OBJ"); m_pAxisHandles->addChild(axisHandleY);
	//osg::ref_ptr<osg::Node> axisHandleZ = this->LoadModel("..\\..\\res\\AxisHandles\\AxisHandleZ.OBJ"); m_pAxisHandles->addChild(axisHandleZ);




	//osg::Geode *geode = new osg::Geode;
	//osg::Geometry *g = new osg::Geometry;
	//osg::Vec3Array *v = new osg::Vec3Array;

	//v->push_back(osg::Vec3(0,0,0));
	//v->push_back(osg::Vec3(100,0,0));
	//v->push_back(osg::Vec3(0,0,0));
	//v->push_back(osg::Vec3(0,0,100));

	//osg::DrawArrays *da = new 
	//osg::DrawArrays(osg::PrimitiveSet::LINES,0,v->size());

	//g->setVertexArray( v);
	//g->addPrimitiveSet( da);

	//geode->addDrawable( g);
	//geode->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);

	//m_pRoot->addChild(geode);
}


void OpenSceneGraph::InitScene()
{
	m_pRoot = new osg::Group();
}

void OpenSceneGraph::InitLight()
{
	this->CreateLight(0, osg::Vec4(-100, -100, -100, 1), osg::Vec3(-1, -1, -1));
	this->CreateLight(1, osg::Vec4(-100, 100, -100, 1), osg::Vec3(-1, 1, -1));
	this->CreateLight(2, osg::Vec4(100, -100, -100, 1), osg::Vec3(1, -1, -1));
	this->CreateLight(3, osg::Vec4(100, 100, -100, 1), osg::Vec3(1, 1, -1));
	this->CreateLight(4, osg::Vec4(-100, -100, 100, 1), osg::Vec3(-1, -1, 1));
	this->CreateLight(5, osg::Vec4(-100, 100, 100, 1), osg::Vec3(-1, 1, 1));
	this->CreateLight(6, osg::Vec4(100, -100, 100, 1), osg::Vec3(1, -1, 1));
	this->CreateLight(7, osg::Vec4(100, 100, 100, 1), osg::Vec3(1, 1, 1));
}

osg::ref_ptr<osg::PositionAttitudeTransform> OpenSceneGraph::CreateLight(int index, osg::Vec4 position, osg::Vec3 direction)
{
	osg::ref_ptr<osg::Light> light = new osg::Light();
	light->setLightNum(index);
	light->setPosition(position);
	light->setDirection(direction);
	light->setDiffuse(osg::Vec4(1.0, 1.0, 1.0, 1.0));
	light->setSpecular(osg::Vec4(1.0, 1.0, 1.0, 1.0));
	light->setAmbient(osg::Vec4(0.0, 0.0, 0.0, 1.0));

	osg::ref_ptr<osg::StateSet> lightStateSet = m_pRoot->getOrCreateStateSet();

	osg::ref_ptr<osg::LightSource> lightSource = new osg::LightSource();
	lightSource->setLight(light);
	lightSource->setLocalStateSetModes(osg::StateAttribute::ON);
	lightSource->setStateSetModes(*lightStateSet, osg::StateAttribute::ON);

	osg::ref_ptr<osg::PositionAttitudeTransform> lightTransform = new osg::PositionAttitudeTransform();
	lightTransform->addChild(lightSource);

	m_pRoot->addChild(lightTransform);

	return lightTransform;
}

void OpenSceneGraph::InitCameraConfig()
{
	// Local Variable to hold window size data
	RECT windowRect;
	RECT clientRect;

	// Create the viewer for this window
	mViewer = new osgViewer::Viewer();

	// Add a Stats Handler to the viewer
	//mViewer->addEventHandler(new osgViewer::StatsHandler);

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
	m_pCameraMain = mViewer->getCamera();

	// Assign Graphics Context to the Camera
	m_pCameraMain->setGraphicsContext(gc);

	traits->x = 0;
	traits->y = 0;
	traits->width = clientRect.right - clientRect.left;
	traits->height = clientRect.bottom - clientRect.top;

	// Set the viewport for the Camera
	m_pCameraMain->setViewport(new osg::Viewport(traits->x, traits->y, traits->width, traits->height));

	// Set projection matrix and camera attribtues
	m_pCameraMain->setClearMask(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	m_pCameraMain->setClearColor(osg::Vec4f(0.3f, 0.5f, 0.7f, 1.0f));
	m_pCameraMain->setProjectionMatrixAsPerspective(
		30.0f, static_cast<double>(traits->width)/static_cast<double>(traits->height), 1.0, 1000.0);

	m_pCameraController = new CameraController(mViewer, m_pRoot, m_pCameraMain);

	m_pCameraSub = new osg::Camera();
	m_pCameraSub->setViewport(new osg::Viewport(traits->x, traits->y, traits->width, traits->height));

	// Set projection matrix and camera attribtues
	m_pCameraSub->setClearMask(GL_DEPTH_BUFFER_BIT);
	m_pCameraSub->setRenderOrder(osg::Camera::POST_RENDER, 0);
	m_pCameraSub->setClearColor(osg::Vec4f(0.3f, 0.5f, 0.7f, 1.0f));
	m_pCameraSub->setProjectionMatrixAsPerspective(
		30.0f, static_cast<double>(traits->width)/static_cast<double>(traits->height), 1.0, 1000.0);


	//rt->setRenderTargetImplementation(osg::Camera::FRAME_BUFFER_OBJECT); 
	//rt->attach(osg::Camera::COLOR_BUFFER, tex); 
	//rt->setClearMask(0); 


	//osg::Vec3d eye( 0.0, -100.0, 50.0 );
	//osg::Vec3d center( 0.0, 0.0, 0.0 );
	//osg::Vec3d up( 0.0, 0.0, 1.0 );

	//camera->setViewMatrixAsLookAt( eye, center, up );

	// Add the Camera to the Viewer
	mViewer->setCamera(m_pCameraMain.get());
	mViewer->addSlave(m_pCameraSub);

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

void OpenSceneGraph::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	m_pCameraController->OnKeyDown(nChar, nRepCnt, nFlags);
}

void OpenSceneGraph::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	m_pCameraController->OnKeyUp(nChar, nRepCnt, nFlags);
}

void OpenSceneGraph::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_pCameraController->OnLButtonDown(nFlags, point);
}

void OpenSceneGraph::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_pCameraController->OnLButtonUp(nFlags, point);
}

void OpenSceneGraph::OnMButtonDown(UINT nFlags, CPoint point)
{
	m_pCameraController->OnMButtonDown(nFlags, point);
}

void OpenSceneGraph::OnMButtonUp(UINT nFlags, CPoint point)
{
	m_pCameraController->OnMButtonUp(nFlags, point);
}

void OpenSceneGraph::OnRButtonDown(UINT nFlags, CPoint point)
{
	m_pCameraController->OnRButtonDown(nFlags, point);
}

void OpenSceneGraph::OnRButtonUp(UINT nFlags, CPoint point)
{
	m_pCameraController->OnRButtonUp(nFlags, point);
}

void OpenSceneGraph::OnMouseMove(UINT nFlags, CPoint point)
{
	m_pCameraController->OnMouseMove(nFlags, point);
}

void OpenSceneGraph::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	m_pCameraController->OnMouseWheel(nFlags, zDelta, pt);
}
