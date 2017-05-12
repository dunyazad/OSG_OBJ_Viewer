#pragma once

#include "OSG_Common.h"

class RTT {
public:
	RTT(float width, float height, osgViewer::Viewer* pViewer, osg::ref_ptr<osg::Group> root, osg::ref_ptr<osg::Camera> mainCamera);
	~RTT();

	inline osg::ref_ptr<osg::Camera> GetCamera() { return m_pCamera.get(); }

	void SetCameraPosition(float x, float y, float z);
	void SetCameraTarget(float x, float y, float z);

	void SetImageTransform(osg::Matrix transform);

private:
	float m_width;
	float m_height;
	osg::ref_ptr<osg::Camera> m_pCamera;
	osg::ref_ptr<osg::Image> m_pImage;
	osg::ref_ptr<osg::Texture2D> m_pTexture;
	osg::ref_ptr<osg::MatrixTransform> m_pMatrixTransform;
	osg::ref_ptr<osg::Geode> m_pGeode;
	osg::ref_ptr<osg::Geometry> m_pGeometry;

	osg::Vec3 m_eye;
	osg::Vec3 m_target;

	osgViewer::Viewer* m_pViewer;
};
