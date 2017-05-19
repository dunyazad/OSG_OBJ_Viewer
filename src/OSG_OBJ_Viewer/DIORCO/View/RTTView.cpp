#include <stdafx.h>
#include "RTTView.h"

namespace DIORCO {

	RTTView::RTTView(float width, float height, osgViewer::Viewer* pViewer, osg::ref_ptr<osg::Camera> mainCamera)
		: m_width(width), m_height(height), m_pViewer(pViewer), m_eye(10, 0, 0), m_target(0, 0, 0)
	{
		m_pCamera = new osg::Camera();

		m_pCamera->setGraphicsContext(mainCamera->getGraphicsContext());

		m_pCamera->setReferenceFrame(osg::Camera::ABSOLUTE_RF);
		m_pCamera->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//m_pCamera->setClearColor(osg::Vec4f(0.5f, 0.7f, 0.9f, 1.0f));
		m_pCamera->setClearColor(osg::Vec4f(1.0f, 0.5f, 0.7f, 1.0f));
		m_pCamera->setRenderTargetImplementation(osg::Camera::FRAME_BUFFER_OBJECT);
		m_pCamera->setRenderOrder(osg::Camera::PRE_RENDER);
		m_pCamera->setProjectionMatrixAsPerspective(
			30.0f, static_cast<double>(m_width)/static_cast<double>(m_height), 1.0, 1000.0);
		m_pCamera->setViewport(mainCamera->getViewport());

		float viewportWidth = mainCamera->getViewport()->width();
		float viewportHeight = mainCamera->getViewport()->height();

		m_pCamera->setViewMatrixAsLookAt(m_eye, m_target, osg::Vec3(0, 1, 0));
		/*int inheritanceMask = 
          (osgUtil::SceneView::VariablesMask::ALL_VARIABLES &
          ~osgUtil::SceneView::VariablesMask::CULL_MASK);
		m_pCamera->setInheritanceMask(inheritanceMask);*/
		m_pCamera->setCullMask(0x02); // 2면 그린다.

		m_pTexture = new osg::Texture2D();
		m_pTexture->setTextureSize(viewportWidth, viewportHeight);
		m_pTexture->setInternalFormat(GL_RGBA);
		m_pTexture->setFilter(osg::Texture2D::MIN_FILTER,osg::Texture2D::LINEAR);
		m_pTexture->setFilter(osg::Texture2D::MAG_FILTER,osg::Texture2D::LINEAR);

		m_pCamera->attach(osg::Camera::COLOR_BUFFER, m_pTexture.get());

		m_pGeode = new osg::Geode();
		this->addChild(m_pGeode);

		m_pGeometry = new osg::Geometry();
		m_pGeode->addDrawable(m_pGeometry);

		osg::ref_ptr<osg::Vec3Array> vertexArray = new osg::Vec3Array();
		vertexArray->push_back(osg::Vec3(-width * 0.5f, -height * 0.5f, 0));
		vertexArray->push_back(osg::Vec3(width * 0.5f, -height * 0.5f, 0));
		vertexArray->push_back(osg::Vec3(width * 0.5f, height * 0.5f, 0));
		vertexArray->push_back(osg::Vec3(-width * 0.5f, height * 0.5f, 0));
		m_pGeometry->setVertexArray(vertexArray);

		osg::ref_ptr<osg::Vec4Array> colorArray = new osg::Vec4Array();
		colorArray->push_back(osg::Vec4(1.f, 1.f, 1.f, 1.0f));
		m_pGeometry->setColorArray(colorArray);
		m_pGeometry->setColorBinding(osg::Geometry::BIND_OVERALL);

		osg::ref_ptr<osg::Vec2Array> texCoordArray = new osg::Vec2Array();
		texCoordArray->push_back(osg::Vec2(0.f, 0.f));
		texCoordArray->push_back(osg::Vec2(1.f, 0.f));
		texCoordArray->push_back(osg::Vec2(1.f, 1.f));
		texCoordArray->push_back(osg::Vec2(0.f, 1.f));
		m_pGeometry->setTexCoordArray(0, texCoordArray);

		m_pGeometry->addPrimitiveSet(new osg::DrawArrays(GL_TRIANGLE_FAN, 0, 4));

		m_pGeode->getOrCreateStateSet()->setTextureAttributeAndModes(0, m_pTexture.get(), osg::StateAttribute::ON);

		m_pGeode->getOrCreateStateSet()->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
		osg::ref_ptr<osg::BlendFunc> blendFunc = new osg::BlendFunc;
		blendFunc->setFunction(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		m_pGeode->getOrCreateStateSet()->setAttributeAndModes(blendFunc);
		m_pGeode->setNodeMask(0x01);

		m_pViewer->addSlave(m_pCamera);
	}

	RTTView::~RTTView()
	{
	}

	const osg::Vec3& RTTView::GetCameraPosition() const
	{
		return m_eye;
	}

	void RTTView::SetCameraPosition(float x, float y, float z)
	{
		m_eye = osg::Vec3(x, y, z);
		m_pCamera->setViewMatrixAsLookAt(m_eye, m_target, osg::Vec3(0, 1, 0));
	}

	const osg::Vec3& RTTView::GetCameraTarget() const
	{
		return m_target;
	}

	void RTTView::SetCameraTarget(float x, float y, float z)
	{
		m_target = osg::Vec3(x, y, z);
		m_pCamera->setViewMatrixAsLookAt(m_eye, m_target, osg::Vec3(0, 1, 0));
	}

	void RTTView::SetImageTransform(osg::Matrix transform)
	{
		this->setMatrix(transform);
	}

}