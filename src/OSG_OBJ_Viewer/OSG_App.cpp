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
 
	delete mViewer;
 
	if(m_pCameraController) delete m_pCameraController;
}
 
void OSG_App::Initialize()
{
	InitScene();
	InitCameraConfig();
	InitLight();
	InitModels();
}
 
void OSG_App::InitModels()
{
	m_pMainObject = new osg::MatrixTransform;
	m_pRoot->addChild(m_pMainObject.get());

#define ___CUSTOM_LOADER___
#ifdef ___CUSTOM_LOADER___
	m_pMainObject = LoadObjFile("..\\..\\res\\F-15K\\F-15K.obj", "..\\..\\res\\F-15K", true, 0.001f);
	//auto group = LoadObjFile("..\\..\\res\\Init\\Init.obj", "..\\..\\res\\Init");
	m_pRoot->addChild(m_pMainObject.get());
#else
	osg::ref_ptr<osg::Node> m_pTargetObjectModel = this->LoadModel("..\\..\\res\\F-15K\\F-15K.obj");
	//osg::ref_ptr<osg::Node> m_pTargetObjectModel = this->LoadModel("..\\..\\res\\Init\\Init.obj");
	m_pMainObject->addChild(m_pTargetObjectModel);
	osg::Matrix scale = osg::Matrix::scale(0.5, 0.5, 0.5);
	m_pMainObject->setMatrix(scale);
#endif
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
	m_pCameraMain->setReferenceFrame(osg::Camera::ABSOLUTE_RF);
	m_pCameraMain->setRenderOrder(osg::Camera::POST_RENDER);
	m_pCameraMain->setClearMask(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	m_pCameraMain->setClearColor(osg::Vec4f(0.3f, 0.5f, 0.7f, 1.0f));
	m_pCameraMain->setProjectionMatrixAsPerspective(
		30.0f, static_cast<double>(traits->width)/static_cast<double>(traits->height), 1.0, 1000.0);
 
	m_pCameraController = new CameraController(mViewer, m_pRoot, m_pCameraMain);
 
	{
		auto pRTT = new RTT(16, 16, mViewer, m_pRoot, m_pCameraMain);
		auto t = osg::Matrix::translate(-20, 0, 0);
		auto r = osg::Matrix::rotate(osg::DegreesToRadians(90.0f), osg::Vec3(1, 0, 0));
		pRTT->SetImageTransform(r * t);
		pRTT->SetCameraPosition(-20, 0, 0);
		m_pRTTs.push_back(pRTT);
	}
	{
		auto pRTT = new RTT(16, 16, mViewer, m_pRoot, m_pCameraMain);
		auto t = osg::Matrix::translate(20, 0, 0);
		auto r = osg::Matrix::rotate(osg::DegreesToRadians(90.0f), osg::Vec3(1, 0, 0));
		pRTT->SetImageTransform(r * t);
		pRTT->SetCameraPosition(20, 0, 0);
		m_pRTTs.push_back(pRTT);
	}
	{
		auto pRTT = new RTT(16, 16, mViewer, m_pRoot, m_pCameraMain);
		auto t = osg::Matrix::translate(0, 0, 20);
		auto r = osg::Matrix::rotate(osg::DegreesToRadians(90.0f), osg::Vec3(1, 0, 0));
		pRTT->SetImageTransform(r * t);
		pRTT->SetCameraPosition(0, 0, 20);
		m_pRTTs.push_back(pRTT);
	}
	{
		auto pRTT = new RTT(16, 16, mViewer, m_pRoot, m_pCameraMain);
		auto t = osg::Matrix::translate(0, 0, -20);
		auto r = osg::Matrix::rotate(osg::DegreesToRadians(90.0f), osg::Vec3(1, 0, 0));
		pRTT->SetImageTransform(r * t);
		pRTT->SetCameraPosition(0, 0, -20);
		m_pRTTs.push_back(pRTT);
	}

	// Add the Camera to the Viewer
	mViewer->setCamera(m_pCameraMain.get());
 
	// Set the Scene Data
	mViewer->setSceneData(m_pRoot.get());
 
	// Realize the Viewer
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
	m_pCameraController->OnKeyDown(nChar, nRepCnt, nFlags);
}
 
void OSG_App::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	m_pCameraController->OnKeyUp(nChar, nRepCnt, nFlags);
}
 
void OSG_App::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_pCameraController->OnLButtonDown(nFlags, point);
}
 
void OSG_App::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_pCameraController->OnLButtonUp(nFlags, point);
}
 
void OSG_App::OnMButtonDown(UINT nFlags, CPoint point)
{
	m_pCameraController->OnMButtonDown(nFlags, point);
}
 
void OSG_App::OnMButtonUp(UINT nFlags, CPoint point)
{
	m_pCameraController->OnMButtonUp(nFlags, point);
}
 
void OSG_App::OnRButtonDown(UINT nFlags, CPoint point)
{
	m_pCameraController->OnRButtonDown(nFlags, point);
}
 
void OSG_App::OnRButtonUp(UINT nFlags, CPoint point)
{
	m_pCameraController->OnRButtonUp(nFlags, point);
}
 
void OSG_App::OnMouseMove(UINT nFlags, CPoint point)
{
	m_pCameraController->OnMouseMove(nFlags, point);
}
 
void OSG_App::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	m_pCameraController->OnMouseWheel(nFlags, zDelta, pt);
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
