#include <stdafx.h>
#include "RTTView.h"
#include "RTTViewContainer.h"

namespace DIORCO {

	RTTViewContainer::RTTViewContainer(HWND hWnd, osgViewer::Viewer* pViewer)
		: m_hWnd(hWnd), m_pViewer(pViewer)
	{
		RECT windowRect;
		RECT clientRect;

		// Get the current window size
		::GetWindowRect(m_hWnd, &windowRect);
		::GetClientRect(m_hWnd, &clientRect);

		// Setup the traits parameters
		m_x = clientRect.left;
		m_y = windowRect.bottom - clientRect.bottom;
		m_width = clientRect.right - clientRect.left;
		m_height = clientRect.bottom - clientRect.top;
		
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
		m_pCameraMain = m_pViewer->getCamera();

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
		//m_pCameraMain->setProjectionMatrixAsPerspective(
		//	30.0f, static_cast<double>(traits->width)/static_cast<double>(traits->height), 1.0, 1000.0);
		m_pCameraMain->setProjectionMatrixAsOrtho2D(-m_width * 0.5f, m_width * 0.5f, -m_height * 0.5f, m_height * 0.5f);

		//int inheritanceMask = 
  //        (osgUtil::SceneView::VariablesMask::ALL_VARIABLES &
  //        ~osgUtil::SceneView::VariablesMask::CULL_MASK);
		//m_pCameraMain->setInheritanceMask(inheritanceMask);
		//m_pCameraMain->setCullMask(0x01);
	}

	osg::ref_ptr<osg::Camera> RTTViewContainer::GetCameraMain()
	{
		return m_pCameraMain;
	}

	osg::ref_ptr<RTTView> RTTViewContainer::CreateView(const string& name, float width, float height)
	{
		if(m_views.count(name) == 0) {
			m_views[name] = new RTTView(name, width, height, m_pViewer, m_pCameraMain);
		}
		return m_views[name];
	}

	osg::ref_ptr<RTTView> RTTViewContainer::GetView(const string& name)
	{
		if(m_views.count(name) != 0) {
			return m_views[name];
		}
		return nullptr;
	}

	osg::ref_ptr<RTTView> RTTViewContainer::GetViewUnderCursor()
	{
		osg::ref_ptr<osgUtil::RayIntersector> picker = new osgUtil::RayIntersector(osgUtil::Intersector::WINDOW, m_mousePosition.x, m_height - m_mousePosition.y); 
		osgUtil::IntersectionVisitor iv(picker.get()); 
		m_pCameraMain->accept(iv); 
		if (picker->containsIntersections()) 
		{
			auto i = picker->getIntersections().begin();
			auto e = picker->getIntersections().end();

			for(; i != e; i++) {
				auto element = (*i);
				auto pView = GetView(element.drawable->getName());
				if(pView != nullptr) return pView;
			}
		}
		return nullptr;
	}

	CPoint RTTViewContainer::WindowToView(RTTView* pView, CPoint windowPoint)
	{
		return CPoint(windowPoint.x / m_width * pView->GetWidth(), windowPoint.y / m_height * pView->GetHeight());
	}

	int RTTViewContainer::GetX()
	{
		return m_x;
	}

	int RTTViewContainer::GetY()
	{
		return m_y;
	}

	int RTTViewContainer::GetWidth()
	{
		return m_width;
	}

	int RTTViewContainer::GetHeight()
	{
		return m_height;
	}

	void RTTViewContainer::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
	}

	void RTTViewContainer::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
	}

	void RTTViewContainer::OnLButtonDown(UINT nFlags, CPoint point)
	{
	}

	void RTTViewContainer::OnLButtonUp(UINT nFlags, CPoint point)
	{
	}

	void RTTViewContainer::OnMButtonDown(UINT nFlags, CPoint point)
	{
	}

	void RTTViewContainer::OnMButtonUp(UINT nFlags, CPoint point)
	{
	}

	void RTTViewContainer::OnRButtonDown(UINT nFlags, CPoint point)
	{
	}

	void RTTViewContainer::OnRButtonUp(UINT nFlags, CPoint point)
	{
	}

	void RTTViewContainer::OnMouseMove(UINT nFlags, CPoint point)
	{
		m_mousePosition = point;
	}

	void RTTViewContainer::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
	{
	}
}