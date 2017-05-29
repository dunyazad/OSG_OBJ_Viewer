// MFC_OSG.cpp : implementation of the OSG_App class
//
#include "stdafx.h"
#include "OSG_App.h"

#include <DIORCO/tiny_obj_loader.h>

OSG_App::OSG_App(HWND hWnd) : m_hWnd(hWnd)
{
}

OSG_App::~OSG_App()
{
	mViewer->setDone(true);
	Sleep(1000);
	mViewer->stopThreading();

	auto i = m_pRTTs.begin();
	auto e = m_pRTTs.end();
	for(;i != e; i++) {
		if((*i)) delete(*i);
	}

	delete mViewer;

	if(m_pCameraController) delete m_pCameraController;
}

void OSG_App::Initialize()
{
	InitScene();
	InitCameraConfig();
	InitLight();
	InitModels();

	m_pMultiControllerSet = new DIORCO::MultiControllerSet(m_pViewContainer, m_pRoot, m_pViewContainer->GetCameraMain().get());
}

void OSG_App::InitModels()
{
	m_pMainObject = new osg::MatrixTransform;
	m_pRoot->addChild(m_pMainObject.get());

#define ___CUSTOM_LOADER___
#ifdef ___CUSTOM_LOADER___
	m_pMainObject = LoadObjFile("..\\..\\res\\F-15K\\F-15K.obj", "..\\..\\res\\F-15K", true, 0.001f);
	//m_pMainObject = LoadObjFile("..\\..\\res\\Init\\Init.obj", "..\\..\\res\\Init", true, 0.1f);

	m_pRoot->addChild(m_pMainObject.get());
	//m_pMainObject = LoadObjFile("..\\..\\res\\SSModel_0_7780_0\\MdBone_7780.obj", "..\\..\\res\\SSModel_0_7780_0", true, 0.1f);
	//m_pRoot->addChild(m_pMainObject.get());
#else
	osg::ref_ptr<osg::Node> m_pTargetObjectModel = this->LoadModel("..\\..\\res\\F-15K\\F-15K.obj");
	//osg::ref_ptr<osg::Node> m_pTargetObjectModel = this->LoadModel("..\\..\\res\\Init\\Init.obj");
	m_pMainObject->addChild(m_pTargetObjectModel);
	osg::Matrix scale = osg::Matrix::scale(0.5, 0.5, 0.5);
	m_pMainObject->setMatrix(scale);
#endif

	//m_pMxGroup = new osg::MatrixTransform();
	//m_pRoot->addChild(m_pMxGroup);
	//m_pMxBone = LoadObjFile("..\\..\\res\\SSModel_0_7780_0\\MxBone_7780.OBJ", "..\\..\\res\\SSModel_0_7780_0", false, 0.1f);
	//m_pMxGroup->addChild(m_pMxBone);
	//m_pT11 = LoadObjFile("..\\..\\res\\SSModel_0_7780_0\\local_t11_7780.OBJ", "..\\..\\res\\SSModel_0_7780_0", false, 0.1f);
	//m_pMxGroup->addChild(m_pT11);
	//m_pT12 = LoadObjFile("..\\..\\res\\SSModel_0_7780_0\\local_t12_7780.OBJ", "..\\..\\res\\SSModel_0_7780_0", false, 0.1f);
	//m_pMxGroup->addChild(m_pT12);
	//m_pT13 = LoadObjFile("..\\..\\res\\SSModel_0_7780_0\\local_t13_7780.OBJ", "..\\..\\res\\SSModel_0_7780_0", false, 0.1f);
	//m_pMxGroup->addChild(m_pT13);
	//m_pT14 = LoadObjFile("..\\..\\res\\SSModel_0_7780_0\\local_t14_7780.OBJ", "..\\..\\res\\SSModel_0_7780_0", false, 0.1f);
	//m_pMxGroup->addChild(m_pT14);
	//m_pT15 = LoadObjFile("..\\..\\res\\SSModel_0_7780_0\\local_t15_7780.OBJ", "..\\..\\res\\SSModel_0_7780_0", false, 0.1f);
	//m_pMxGroup->addChild(m_pT15);
	//m_pT16 = LoadObjFile("..\\..\\res\\SSModel_0_7780_0\\local_t16_7780.OBJ", "..\\..\\res\\SSModel_0_7780_0", false, 0.1f);
	//m_pMxGroup->addChild(m_pT16);
	//m_pT17 = LoadObjFile("..\\..\\res\\SSModel_0_7780_0\\local_t17_7780.OBJ", "..\\..\\res\\SSModel_0_7780_0", false, 0.1f);
	//m_pMxGroup->addChild(m_pT17);
	//m_pT21 = LoadObjFile("..\\..\\res\\SSModel_0_7780_0\\local_t21_7780.OBJ", "..\\..\\res\\SSModel_0_7780_0", false, 0.1f);
	//m_pMxGroup->addChild(m_pT21);
	//m_pT22 = LoadObjFile("..\\..\\res\\SSModel_0_7780_0\\local_t22_7780.OBJ", "..\\..\\res\\SSModel_0_7780_0", false, 0.1f);
	//m_pMxGroup->addChild(m_pT22);
	//m_pT23 = LoadObjFile("..\\..\\res\\SSModel_0_7780_0\\local_t23_7780.OBJ", "..\\..\\res\\SSModel_0_7780_0", false, 0.1f);
	//m_pMxGroup->addChild(m_pT23);
	//m_pT24 = LoadObjFile("..\\..\\res\\SSModel_0_7780_0\\local_t24_7780.OBJ", "..\\..\\res\\SSModel_0_7780_0", false, 0.1f);
	//m_pMxGroup->addChild(m_pT24);
	//m_pT25 = LoadObjFile("..\\..\\res\\SSModel_0_7780_0\\local_t25_7780.OBJ", "..\\..\\res\\SSModel_0_7780_0", false, 0.1f);
	//m_pMxGroup->addChild(m_pT25);
	//m_pT26 = LoadObjFile("..\\..\\res\\SSModel_0_7780_0\\local_t26_7780.OBJ", "..\\..\\res\\SSModel_0_7780_0", false, 0.1f);
	//m_pMxGroup->addChild(m_pT26);
	//m_pT27 = LoadObjFile("..\\..\\res\\SSModel_0_7780_0\\local_t27_7780.OBJ", "..\\..\\res\\SSModel_0_7780_0", false, 0.1f);
	//m_pMxGroup->addChild(m_pT27);

	//m_pMdGroup = new osg::MatrixTransform();
	//m_pRoot->addChild(m_pMdGroup);
	//m_pMdBone = LoadObjFile("..\\..\\res\\SSModel_0_7780_0\\MdBone_7780.OBJ", "..\\..\\res\\SSModel_0_7780_0", false, 0.1f);
	//m_pMdGroup->addChild(m_pMdBone);
	//m_pT31 = LoadObjFile("..\\..\\res\\SSModel_0_7780_0\\local_t31_7780.OBJ", "..\\..\\res\\SSModel_0_7780_0", false, 0.1f);
	//m_pMdGroup->addChild(m_pT31);
	//m_pT32 = LoadObjFile("..\\..\\res\\SSModel_0_7780_0\\local_t32_7780.OBJ", "..\\..\\res\\SSModel_0_7780_0", false, 0.1f);
	//m_pMdGroup->addChild(m_pT32);
	//m_pT33 = LoadObjFile("..\\..\\res\\SSModel_0_7780_0\\local_t33_7780.OBJ", "..\\..\\res\\SSModel_0_7780_0", false, 0.1f);
	//m_pMdGroup->addChild(m_pT33);
	//m_pT34 = LoadObjFile("..\\..\\res\\SSModel_0_7780_0\\local_t34_7780.OBJ", "..\\..\\res\\SSModel_0_7780_0", false, 0.1f);
	//m_pMdGroup->addChild(m_pT34);
	//m_pT35 = LoadObjFile("..\\..\\res\\SSModel_0_7780_0\\local_t35_7780.OBJ", "..\\..\\res\\SSModel_0_7780_0", false, 0.1f);
	//m_pMdGroup->addChild(m_pT35);
	//m_pT36 = LoadObjFile("..\\..\\res\\SSModel_0_7780_0\\local_t36_7780.OBJ", "..\\..\\res\\SSModel_0_7780_0", false, 0.1f);
	//m_pMdGroup->addChild(m_pT36);
	//m_pT37 = LoadObjFile("..\\..\\res\\SSModel_0_7780_0\\local_t37_7780.OBJ", "..\\..\\res\\SSModel_0_7780_0", false, 0.1f);
	//m_pMdGroup->addChild(m_pT37);
	//m_pT41 = LoadObjFile("..\\..\\res\\SSModel_0_7780_0\\local_t41_7780.OBJ", "..\\..\\res\\SSModel_0_7780_0", false, 0.1f);
	//m_pMdGroup->addChild(m_pT41);
	//m_pT42 = LoadObjFile("..\\..\\res\\SSModel_0_7780_0\\local_t42_7780.OBJ", "..\\..\\res\\SSModel_0_7780_0", false, 0.1f);
	//m_pMdGroup->addChild(m_pT42);
	//m_pT43 = LoadObjFile("..\\..\\res\\SSModel_0_7780_0\\local_t43_7780.OBJ", "..\\..\\res\\SSModel_0_7780_0", false, 0.1f);
	//m_pMdGroup->addChild(m_pT43);
	//m_pT44 = LoadObjFile("..\\..\\res\\SSModel_0_7780_0\\local_t44_7780.OBJ", "..\\..\\res\\SSModel_0_7780_0", false, 0.1f);
	//m_pMdGroup->addChild(m_pT44);
	//m_pT45 = LoadObjFile("..\\..\\res\\SSModel_0_7780_0\\local_t45_7780.OBJ", "..\\..\\res\\SSModel_0_7780_0", false, 0.1f);
	//m_pMdGroup->addChild(m_pT45);
	//m_pT46 = LoadObjFile("..\\..\\res\\SSModel_0_7780_0\\local_t46_7780.OBJ", "..\\..\\res\\SSModel_0_7780_0", false, 0.1f);
	//m_pMdGroup->addChild(m_pT46);
	//m_pT47 = LoadObjFile("..\\..\\res\\SSModel_0_7780_0\\local_t47_7780.OBJ", "..\\..\\res\\SSModel_0_7780_0", false, 0.1f);
	//m_pMdGroup->addChild(m_pT47);

}


void OSG_App::InitScene()
{
	m_pRoot = new osg::Group();
}

void OSG_App::InitLight()
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

osg::ref_ptr<osg::PositionAttitudeTransform> OSG_App::CreateLight(int index, osg::Vec4 position, osg::Vec3 direction)
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

void OSG_App::InitCameraConfig()
{
	mViewer = new osgViewer::Viewer();

	// Add a Stats Handler to the viewer
	//mViewer->addEventHandler(new osgViewer::StatsHandler);

	m_pViewContainer = new DIORCO::RTTViewContainer(m_hWnd, mViewer);
	m_pCameraController = new CameraController();

	{
		auto pView = m_pViewContainer->CreateView("UpperLeft", m_pViewContainer->GetWidth() / 3, m_pViewContainer->GetHeight() / 2);
		m_pRoot->addChild(pView);
		pView->SetPosition(-m_pViewContainer->GetWidth() / 3, m_pViewContainer->GetHeight() / 4);
		pView->GetCamera()->setClearColor(osg::Vec4f(1.0f, 0.7f, 0.7f, 1.0f));
		m_pCameraController->RegisterView(pView);
	}

	{
		auto pView = m_pViewContainer->CreateView("UpperCenter", m_pViewContainer->GetWidth() / 3, m_pViewContainer->GetHeight() / 2);
		m_pRoot->addChild(pView);
		pView->SetPosition(0, m_pViewContainer->GetHeight() / 4);
		pView->GetCamera()->setClearColor(osg::Vec4f(0.7f, 1.0f, 0.7f, 1.0f));
		m_pCameraController->RegisterView(pView);
	}

	{
		auto pView = m_pViewContainer->CreateView("UpperRight", m_pViewContainer->GetWidth() / 3, m_pViewContainer->GetHeight() / 2);
		m_pRoot->addChild(pView);
		pView->SetPosition(m_pViewContainer->GetWidth() / 3, m_pViewContainer->GetHeight() / 4);
		pView->GetCamera()->setClearColor(osg::Vec4f(0.7f, 0.7f, 1.0f, 1.0f));
		m_pCameraController->RegisterView(pView);
	}

	{
		auto pView = m_pViewContainer->CreateView("LowerLeft", m_pViewContainer->GetWidth() / 2, m_pViewContainer->GetHeight() / 2);
		m_pRoot->addChild(pView);
		pView->SetPosition(-m_pViewContainer->GetWidth() / 4, -m_pViewContainer->GetHeight() / 4);
		pView->GetCamera()->setClearColor(osg::Vec4f(0.3f, 0.3f, 0.3f, 1.0f));
		m_pCameraController->RegisterView(pView);
	}

	{
		auto pView = m_pViewContainer->CreateView("LowerRight", m_pViewContainer->GetWidth() / 2, m_pViewContainer->GetHeight() / 2);
		m_pRoot->addChild(pView);
		pView->SetPosition(m_pViewContainer->GetWidth() / 4, -m_pViewContainer->GetHeight() / 4);
		pView->GetCamera()->setClearColor(osg::Vec4f(1.0f, 1.0f, 1.0f, 1.0f));
		m_pCameraController->RegisterView(pView);
	}

	mViewer->setCamera(m_pViewContainer->GetCameraMain().get());

	mViewer->setSceneData(m_pRoot.get());

	mViewer->realize();
}

void OSG_App::PreFrameUpdate()
{
	// Due any preframe updates in this routine
}

void OSG_App::PostFrameUpdate()
{
	// Due any postframe updates in this routine
}

/*void OSG_App::Render(void* ptr)
{
OSG_App* osg = (OSG_App*)ptr;

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

osg::ref_ptr<osg::Node> OSG_App::LoadModel(std::string filename)
{
	DIORCO::TimeWatch tm_loading;
	tm_loading.start();

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


	tm_loading.end();

	printf("osgDB Loading time: %d [ms]\n", (int)tm_loading.msec());

	return model;
}

void OSG_App::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if(nChar == 49) {
		m_pCameraController->SelectView(m_pViewContainer->GetView("UpperLeft"));
	} else if(nChar == 50) {
		m_pCameraController->SelectView(m_pViewContainer->GetView("UpperCenter"));
	} else if(nChar == 51) {
		m_pCameraController->SelectView(m_pViewContainer->GetView("UpperRight"));
	} else if(nChar == 52) {
		m_pCameraController->SelectView(m_pViewContainer->GetView("LowerLeft"));
	} else if(nChar == 53) {
		m_pCameraController->SelectView(m_pViewContainer->GetView("LowerRight"));
	}

	m_pCameraController->OnKeyDown(nChar, nRepCnt, nFlags);
}

void OSG_App::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	m_pCameraController->OnKeyUp(nChar, nRepCnt, nFlags);
	m_pViewContainer->OnKeyUp(nChar, nRepCnt, nFlags);
	m_pMultiControllerSet->OnKeyUp(nChar, nRepCnt, nFlags);
}

void OSG_App::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_pCameraController->OnLButtonDown(nFlags, point);
	m_pViewContainer->OnLButtonDown(nFlags, point);
	m_pMultiControllerSet->OnLButtonDown(nFlags, point);
}

void OSG_App::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_pCameraController->OnLButtonUp(nFlags, point);
	m_pViewContainer->OnLButtonUp(nFlags, point);
	m_pMultiControllerSet->OnLButtonUp(nFlags, point);
}

void OSG_App::OnMButtonDown(UINT nFlags, CPoint point)
{
	m_pCameraController->OnMButtonDown(nFlags, point);
	m_pViewContainer->OnMButtonDown(nFlags, point);
	m_pMultiControllerSet->OnMButtonDown(nFlags, point);
}

void OSG_App::OnMButtonUp(UINT nFlags, CPoint point)
{
	m_pCameraController->OnMButtonUp(nFlags, point);
	m_pViewContainer->OnMButtonUp(nFlags, point);
	m_pMultiControllerSet->OnMButtonUp(nFlags, point);
}

void OSG_App::OnRButtonDown(UINT nFlags, CPoint point)
{
	m_pCameraController->OnRButtonDown(nFlags, point);
	m_pViewContainer->OnRButtonDown(nFlags, point);
	m_pMultiControllerSet->OnRButtonDown(nFlags, point);
}

void OSG_App::OnRButtonUp(UINT nFlags, CPoint point)
{
	m_pCameraController->OnRButtonUp(nFlags, point);
	m_pViewContainer->OnRButtonUp(nFlags, point);
	m_pMultiControllerSet->OnRButtonUp(nFlags, point);
}

void OSG_App::OnMouseMove(UINT nFlags, CPoint point)
{
	m_pCameraController->OnMouseMove(nFlags, point);
	m_pViewContainer->OnMouseMove(nFlags, point);
	m_pMultiControllerSet->OnMouseMove(nFlags, point);
}

void OSG_App::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	m_pCameraController->OnMouseWheel(nFlags, zDelta, pt);
	m_pViewContainer->OnMouseWheel(nFlags, zDelta, pt);
	m_pMultiControllerSet->OnMouseWheel(nFlags, zDelta, pt);
}

osg::ref_ptr<osg::MatrixTransform> OSG_App::LoadObjFile(const std::string& objFileName, const std::string& baseDir, bool flipYZ, float scale)
{
	std::string baseDirectory = baseDir;
	if(baseDirectory[baseDirectory.size() - 1] != '\\' || baseDirectory[baseDirectory.size() - 1] != '/') {
		baseDirectory += '\\';
	}

	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::map<std::string, GLuint> textures;

	DIORCO::TimeWatch tm_parsing;
	DIORCO::TimeWatch tm_loading;
	tm_parsing.start();
	tm_loading.start();

	std::string err;
	bool ret =
		tinyobj::LoadObj(&attrib, &shapes, &materials, &err, objFileName.c_str(), baseDirectory.c_str(), true);
	if (!err.empty()) {
		std::cerr << err << std::endl;
	}
	tm_parsing.end();

	if (!ret) {
		std::cerr << "Failed to load " << objFileName << std::endl;
		return nullptr;
	}

	printf("CustomLoader Parsing time: %d [ms]\n", (int)tm_parsing.msec());


	//osg::ref_ptr<osg::Vec3Array> va = new osg::Vec3Array;
	//int size = attrib.vertices.size();
	//for(int i = 0; i < size; i += 3) {
	//	va->push_back(osg::Vec3(attrib.vertices[i], attrib.vertices[i + 1], attrib.vertices[i + 2]));
	//}

	//osg::ref_ptr<osg::Vec3Array> na = new osg::Vec3Array;
	//size = attrib.normals.size();
	//for(int i = 0; i < size; i += 3) {
	//	na->push_back(osg::Vec3(attrib.normals[i], attrib.normals[i + 1], attrib.normals[i + 2]));
	//}

	//osg::ref_ptr<osg::Vec2Array> ta = new osg::Vec2Array;
	//size = attrib.texcoords.size();
	//for(int i = 0; i < size; i += 2) {
	//	ta->push_back(osg::Vec2(attrib.texcoords[i], attrib.texcoords[i + 1]));
	//}

	std::vector<osg::ref_ptr<osg::Material>> osgMaterials;
	std::vector<osg::ref_ptr<osg::Texture2D>> osgTextures;
	std::vector<osg::ref_ptr<osg::Vec3Array>> osgVertices;
	std::vector<osg::ref_ptr<osg::Vec3Array>> osgNormals;
	std::vector<osg::ref_ptr<osg::Vec2Array>> osgTexCoords;

	if(materials.size() > 0) {
		for (unsigned int materialIndex = 0; materialIndex < materials.size(); materialIndex++) {
			auto material = materials[materialIndex];

			osg::ref_ptr<osg::Material> osgMaterial = new osg::Material;
			osgMaterial->setColorMode(osg::Material::DIFFUSE);
			osgMaterial->setAmbient(osg::Material::FRONT_AND_BACK, osg::Vec4(material.ambient[0], material.ambient[1], material.ambient[2], 1));
			osgMaterial->setDiffuse(osg::Material::FRONT_AND_BACK, osg::Vec4(material.diffuse[0], material.diffuse[1], material.diffuse[2], 1));
			osgMaterial->setSpecular(osg::Material::FRONT_AND_BACK, osg::Vec4(material.specular[0], material.specular[1], material.specular[2], 1));
			osgMaterial->setShininess(osg::Material::FRONT_AND_BACK, material.shininess);

			if(material.diffuse_texname.size() > 0) {
				auto textureFileName = baseDirectory + material.diffuse_texname;
				osg::ref_ptr<osg::Texture2D> map_Kd = new osg::Texture2D(osgDB::readImageFile(textureFileName.c_str()));
				//map_Kd->setInternalFormatMode(osg::Texture2D::USE_S3TC_DXT1_COMPRESSION);
				//map_Kd->setUnRefImageDataAfterApply(true);
				osgTextures.push_back(map_Kd);

				osgMaterial->setDiffuse(osg::Material::FRONT_AND_BACK, osg::Vec4(1, 1, 1, 1));
			} else {
				osgTextures.push_back(nullptr);
			}
			osgMaterials.push_back(osgMaterial);

			osgVertices.push_back(new osg::Vec3Array);
			osgNormals.push_back(new osg::Vec3Array);
			osgTexCoords.push_back(new osg::Vec2Array);
		}
	}

	osg::ref_ptr<osg::MatrixTransform> matrixTransform = new osg::MatrixTransform();

	// Loop over shapes
	for (size_t s = 0; s < shapes.size(); s++) {
		// Loop over faces(polygon)
		size_t index_offset = 0;
		for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
			int fv = shapes[s].mesh.num_face_vertices[f];
			// per-face material
			auto material = shapes[s].mesh.material_ids[f];

			// Loop over vertices in the face.
			for (int v = 0; v < fv; v++) {
				// access to vertex
				tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
				if(attrib.vertices.size() > 0) {
					auto vx = attrib.vertices[3*idx.vertex_index+0] * scale;
					auto vy = attrib.vertices[3*idx.vertex_index+1] * scale;
					auto vz = attrib.vertices[3*idx.vertex_index+2] * scale;
					if(flipYZ) {
						osgVertices[material]->push_back(osg::Vec3(vx, vz, vy));
					} else {
						osgVertices[material]->push_back(osg::Vec3(vx, vy, vz));
					}
				}
				if(attrib.normals.size() > 0) {
					auto nx = attrib.normals[3*idx.normal_index+0];
					auto ny = attrib.normals[3*idx.normal_index+1];
					auto nz = attrib.normals[3*idx.normal_index+2];
					osgNormals[material]->push_back(osg::Vec3(nx, ny, nz));
				}
				if(attrib.texcoords.size() > 0) {
					auto tu = attrib.texcoords[2*idx.texcoord_index+0];
					auto tv = attrib.texcoords[2*idx.texcoord_index+1];
					osgTexCoords[material]->push_back(osg::Vec2(tu, tv));
				}
			}
			index_offset += fv;
		}
	}

	for (unsigned int materialIndex = 0; materialIndex < materials.size(); materialIndex++) {
		osg::ref_ptr<osg::Geode> geode = new osg::Geode;
		matrixTransform->addChild(geode);
		auto ss = geode->getOrCreateStateSet();
		ss->setMode(GL_LIGHTING, osg::StateAttribute::OFF);

		ss->setAttributeAndModes(osgMaterials[materialIndex].get(), osg::StateAttribute::ON);

		if(materials[materialIndex].diffuse_texname.size() > 0) {
			if(osgTextures[materialIndex]) {
				ss->setTextureAttributeAndModes(0, osgTextures[materialIndex].get(), osg::StateAttribute::ON);
			}
		}

		osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry;
		geode->addDrawable(geometry);

		geometry->setVertexArray(osgVertices[materialIndex]);
		geometry->setNormalArray(osgNormals[materialIndex]);
		geometry->setTexCoordArray(0, osgTexCoords[materialIndex]);
		osg::ref_ptr<osg::DrawElementsUInt> ia = new osg::DrawElementsUInt(osg::PrimitiveSet::TRIANGLES, 0);
		for(unsigned int i = 0; i < osgVertices[materialIndex]->size(); i++) {
			ia->push_back(i);
		}
		geometry->addPrimitiveSet(ia);
	}




	//for(unsigned int shapeIndex = 0; shapeIndex < shapes.size(); shapeIndex++) {
	//	auto shape = shapes[shapeIndex];
	//	auto mesh = shape.mesh;
	//	if(materials.size() > 0) {
	//		for (unsigned int materialIndex = 0; materialIndex < materials.size(); materialIndex++) {
	//			osg::ref_ptr<osg::Geode> geode = new osg::Geode;
	//			auto ss = geode->getOrCreateStateSet();
	//			ss->setMode(GL_LIGHTING, osg::StateAttribute::OFF);

	//			ss->setAttributeAndModes(osgMaterials[materialIndex].get(), osg::StateAttribute::ON);

	//			if(materials[materialIndex].diffuse_texname.size() > 0) {
	//				if(osgTextures[materialIndex]) {
	//					ss->setTextureAttributeAndModes(0, osgTextures[materialIndex].get(), osg::StateAttribute::ON);
	//				}
	//			}

	//			osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry;

	//			osg::ref_ptr<osg::DrawElementsUInt> ia = new osg::DrawElementsUInt(osg::PrimitiveSet::TRIANGLES, 0);
	//			for(unsigned int faceIndex = 0; faceIndex < mesh.num_face_vertices.size(); faceIndex++) {
	//				if(materialIndex == mesh.material_ids[faceIndex]) {
	//					auto i1 = shape.mesh.indices[faceIndex * 3 + 0];
	//					auto i2 = shape.mesh.indices[faceIndex * 3 + 1];
	//					auto i3 = shape.mesh.indices[faceIndex * 3 + 2];

	//					osg::ref_ptr<osg::Vec3Array> va = new osg::Vec3Array;
	//					va->push_back(osg::Vec3(attrib.vertices[i1.vertex_index * 3], attrib.vertices[i1.vertex_index * 3 + 1], attrib.vertices[i1.vertex_index * 3 + 2]));
	//					va->push_back(osg::Vec3(attrib.vertices[i2.vertex_index * 3], attrib.vertices[i2.vertex_index * 3 + 1], attrib.vertices[i2.vertex_index * 3 + 2]));
	//					va->push_back(osg::Vec3(attrib.vertices[i3.vertex_index * 3], attrib.vertices[i3.vertex_index * 3 + 1], attrib.vertices[i3.vertex_index * 3 + 2]));
	//					geometry->setVertexArray(va);

	//					osg::ref_ptr<osg::Vec3Array> na = new osg::Vec3Array;
	//					na->push_back(osg::Vec3(attrib.normals[i1.normal_index * 3], attrib.normals[i1.normal_index * 3 + 1], attrib.normals[i1.normal_index * 3 + 2]));
	//					na->push_back(osg::Vec3(attrib.normals[i2.normal_index * 3], attrib.normals[i2.normal_index * 3 + 1], attrib.normals[i2.normal_index * 3 + 2]));
	//					na->push_back(osg::Vec3(attrib.normals[i3.normal_index * 3], attrib.normals[i3.normal_index * 3 + 1], attrib.normals[i3.normal_index * 3 + 2]));
	//					geometry->setNormalArray(na);

	//					osg::ref_ptr<osg::Vec2Array> ta = new osg::Vec2Array;
	//					va->push_back(osg::Vec3(attrib.vertices[i1.vertex_index * 3], attrib.vertices[i1.vertex_index * 3 + 1], attrib.vertices[i1.vertex_index * 3 + 2]));
	//					va->push_back(osg::Vec3(attrib.vertices[i2.vertex_index * 3], attrib.vertices[i2.vertex_index * 3 + 1], attrib.vertices[i2.vertex_index * 3 + 2]));
	//					va->push_back(osg::Vec3(attrib.vertices[i3.vertex_index * 3], attrib.vertices[i3.vertex_index * 3 + 1], attrib.vertices[i3.vertex_index * 3 + 2]));
	//					geometry->setVertexArray(va);

	//					ia->push_back(shape.mesh.indices[faceIndex * 3].vertex_index);
	//					ia->push_back(shape.mesh.indices[faceIndex * 3 + 1].vertex_index);
	//					ia->push_back(shape.mesh.indices[faceIndex * 3 + 2].vertex_index);
	//					geometry->addPrimitiveSet(ia);
	//				}
	//			}

	//			geode->addDrawable(geometry);
	//			group->addChild(geode);
	//		}
	//	} else {
	//		/*osg::ref_ptr<osg::Geode> geode = new osg::Geode;
	//		geode->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
	//		osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry;

	//		geometry->setVertexArray(va);
	//		geometry->setNormalArray(na);
	//		geometry->setTexCoordArray(0, ta);

	//		osg::ref_ptr<osg::DrawElementsUInt> indexArray = new osg::DrawElementsUInt(osg::PrimitiveSet::TRIANGLES, 0);
	//		for(unsigned int i = 0; i < shape.mesh.indices.size(); i++) {
	//			indexArray->push_back(shape.mesh.indices[i].vertex_index);
	//		}

	//		geometry->addPrimitiveSet(indexArray);

	//		geode->addDrawable(geometry);
	//		group->addChild(geode);*/
	//	}
	//}



	tm_loading.end();

	printf("CustomLoader Loading time: %d [ms]\n", (int)tm_loading.msec());



	return matrixTransform;
}
