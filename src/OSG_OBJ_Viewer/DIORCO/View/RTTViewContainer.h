#pragma once

#include <OSG_Common.h>
#include <DIORCO/DIORCO_Common.h>

namespace DIORCO {

	class RTTView;

	class RTTViewContainer {
	public:
		RTTViewContainer(HWND hWnd, osgViewer::Viewer* pViewer);
		~RTTViewContainer();

		osg::ref_ptr<osg::Camera> GetCameraMain();
		osg::ref_ptr<RTTView> CreateView(const string& name, float width, float height);
		osg::ref_ptr<RTTView> GetView(const string& name);

		int GetX();
		int GetY();
		int GetWidth();
		int GetHeight();

	private:
		HWND m_hWnd;
		osgViewer::Viewer* m_pViewer;
		int m_x;
		int m_y;
		int m_width;
		int m_height;

		map<string, osg::ref_ptr<RTTView>> m_views;

		osg::ref_ptr<osg::Camera> m_pCameraMain;
	};

}