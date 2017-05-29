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
		osg::ref_ptr<RTTView> GetViewUnderCursor();
		CPoint WindowToView(RTTView* pView, CPoint windowPoint);

		int GetX();
		int GetY();
		int GetWidth();
		int GetHeight();

		void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
		void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
		void OnLButtonDown(UINT nFlags, CPoint point);
		void OnLButtonUp(UINT nFlags, CPoint point);
		void OnMButtonDown(UINT nFlags, CPoint point);
		void OnMButtonUp(UINT nFlags, CPoint point);
		void OnRButtonDown(UINT nFlags, CPoint point);
		void OnRButtonUp(UINT nFlags, CPoint point);
		void OnMouseMove(UINT nFlags, CPoint point);
		void OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);

	private:
		HWND m_hWnd;
		osgViewer::Viewer* m_pViewer;
		int m_x;
		int m_y;
		int m_width;
		int m_height;
		CPoint m_mousePosition;

		map<string, osg::ref_ptr<RTTView>> m_views;

		osg::ref_ptr<osg::Camera> m_pCameraMain;
	};

}