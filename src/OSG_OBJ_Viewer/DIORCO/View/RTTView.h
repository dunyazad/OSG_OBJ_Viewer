#pragma once

#include <OSG_Common.h>
#include <DIORCO/DIORCO_Common.h>

namespace DIORCO {

	class RTTView : public osg::MatrixTransform {
	public:
		RTTView(float width, float height, osgViewer::Viewer* pViewer, osg::ref_ptr<osg::Camera> mainCamera);
		~RTTView();

		inline osg::ref_ptr<osg::Camera> GetCamera() { return m_pCamera.get(); }

		const osg::Vec3& GetCameraPosition() const;
		void SetCameraPosition(float x, float y, float z);
		const osg::Vec3& GetCameraTarget() const;
		void SetCameraTarget(float x, float y, float z);

		void SetImageTransform(osg::Matrix transform);

	private:
		float m_width;
		float m_height;
		osg::ref_ptr<osg::Camera> m_pCamera;
		osg::ref_ptr<osg::Texture2D> m_pTexture;
		osg::ref_ptr<osg::Geode> m_pGeode;
		osg::ref_ptr<osg::Geometry> m_pGeometry;

		osg::Vec3 m_eye;
		osg::Vec3 m_target;

		osgViewer::Viewer* m_pViewer;
	};

}