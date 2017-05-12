#pragma once

#include "OSG_Common.h"

class RTT {
public:
	RTT();
	~RTT();

private:
	osg::ref_ptr<osg::Camera> m_pCamera;
	osg::ref_ptr<osg::Image> m_pImage;
	osg::ref_ptr<osg::Texture2D> m_pTexture;
	osg::ref_ptr<osg::Geode> m_pGeode;
	osg::ref_ptr<osg::Geometry> m_pGeometry;
}