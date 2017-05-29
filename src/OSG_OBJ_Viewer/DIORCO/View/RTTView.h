#pragma once

#include <OSG_Common.h>
#include <DIORCO/DIORCO_Common.h>

namespace DIORCO {

	class RTTView : public osg::MatrixTransform {
	public:
		RTTView(const string& name, float width, float height, osgViewer::Viewer* pViewer, osg::ref_ptr<osg::Camera> mainCamera);
		~RTTView();

		inline osg::ref_ptr<osg::Camera> GetCamera() { return m_pCamera.get(); }
		inline const string& GetName() { return m_name; }
		inline float GetX() { return m_x; }
		inline float GetY() { return m_y; }
		inline float GetWidth() { return m_width; }
		inline float GetHeight() { return m_height; }

		const osg::Vec3& GetCameraPosition() const;
		void SetCameraPosition(float x, float y, float z);
		const osg::Vec3& GetCameraTarget() const;
		void SetCameraTarget(float x, float y, float z);

		void SetPosition(float x, float y);

	private:
		string m_name;
		float m_x;
		float m_y;
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