#include "stdafx.h"

#include "MultiControllerSet.h"
#include "Custom3DDataManager.h"
#include <DIORCO/View/View.h>

namespace DIORCO {
	MultiControllerSet::MultiControllerSet(RTTViewContainer* pViewContainer, osg::ref_ptr<osg::Group> pRoot, osg::ref_ptr<osg::Camera> pCamera)
		: m_pViewContainer(pViewContainer), m_pRoot(pRoot), m_pCamera(pCamera)
	{
		m_pArrowTX = Custom3DDataManager::LoadObjFile("..\\..\\res\\Arrow\\ArrowT.obj", "..\\..\\res\\Arrow", true, 0.01f, "ArrowTX");
		m_pArrowTX->setName("ArrowTX");
		m_pArrowTY = Custom3DDataManager::LoadObjFile("..\\..\\res\\Arrow\\ArrowT.obj", "..\\..\\res\\Arrow", true, 0.01f, "ArrowTY");
		m_pArrowTY->setName("ArrowTY");
		m_pArrowTZ = Custom3DDataManager::LoadObjFile("..\\..\\res\\Arrow\\ArrowT.obj", "..\\..\\res\\Arrow", true, 0.01f, "ArrowTZ");
		m_pArrowTZ->setName("ArrowTZ");

		m_pArrowRX = Custom3DDataManager::LoadObjFile("..\\..\\res\\Arrow\\ArrowR.obj", "..\\..\\res\\Arrow", true, 0.01f, "ArrowRX");
		m_pArrowTX->setName("ArrowRX");
		m_pArrowRY = Custom3DDataManager::LoadObjFile("..\\..\\res\\Arrow\\ArrowR.obj", "..\\..\\res\\Arrow", true, 0.01f, "ArrowRY");
		m_pArrowTX->setName("ArrowRY");
		m_pArrowRZ = Custom3DDataManager::LoadObjFile("..\\..\\res\\Arrow\\ArrowR.obj", "..\\..\\res\\Arrow", true, 0.01f, "ArrowRZ");
		m_pArrowTX->setName("ArrowRZ");

		m_pControllerSet = new osg::MatrixTransform();

		m_pControllerSet->addChild(m_pArrowTX);
		m_pControllerSet->addChild(m_pArrowTY);
		m_pControllerSet->addChild(m_pArrowTZ);

		m_pControllerSet->addChild(m_pArrowRX);
		m_pControllerSet->addChild(m_pArrowRY);
		m_pControllerSet->addChild(m_pArrowRZ);

		m_pRoot->addChild(m_pControllerSet);
	}

	MultiControllerSet::~MultiControllerSet()
	{
	}

	void MultiControllerSet::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		//if(nChar == 49) {
		//	m_pCameraController->SelectView(m_pViewContainer->GetView("UpperLeft"));
		//} else if(nChar == 50) {
		//	m_pCameraController->SelectView(m_pViewContainer->GetView("UpperCenter"));
		//} else if(nChar == 51) {
		//	m_pCameraController->SelectView(m_pViewContainer->GetView("UpperRight"));
		//} else if(nChar == 52) {
		//	m_pCameraController->SelectView(m_pViewContainer->GetView("LowerLeft"));
		//} else if(nChar == 53) {
		//	m_pCameraController->SelectView(m_pViewContainer->GetView("LowerRight"));
		//}

		//m_pCameraController->OnKeyDown(nChar, nRepCnt, nFlags);
	}

	void MultiControllerSet::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
	}

	void MultiControllerSet::OnLButtonDown(UINT nFlags, CPoint point)
	{
	}

	void MultiControllerSet::OnLButtonUp(UINT nFlags, CPoint point)
	{
		//auto pView = m_pViewContainer->GetViewUnderCursor();
		auto pView = m_pViewContainer->GetView("UpperLeft");
		osg::ref_ptr<osgUtil::RayIntersector> picker = new osgUtil::RayIntersector(osgUtil::Intersector::WINDOW, point.x, point.y); 
		osgUtil::IntersectionVisitor iv(picker.get()); 
		pView->GetCamera()->accept(iv); 
		if (picker->containsIntersections()) 
		{
			auto i = picker->getIntersections().begin();
			auto e = picker->getIntersections().end();

			for(; i != e; i++) {
				auto element = (*i);
				std::cout << "================================================================================" << std::endl;
				std::cout << "intersect " << element.drawable->getName() << std::endl;

				auto npi = element.nodePath.begin();
				auto npe = element.nodePath.end();
				for(; npi != npe; npi++) {
					std::cout << (*npi)->getName() << std::endl;
				}
			}

			AfxMessageBox(L"Pick");
		}
	}

	void MultiControllerSet::OnMButtonDown(UINT nFlags, CPoint point)
	{
	}

	void MultiControllerSet::OnMButtonUp(UINT nFlags, CPoint point)
	{
	}

	void MultiControllerSet::OnRButtonDown(UINT nFlags, CPoint point)
	{
	}

	void MultiControllerSet::OnRButtonUp(UINT nFlags, CPoint point)
	{
	}

	void MultiControllerSet::OnMouseMove(UINT nFlags, CPoint point)
	{
	}

	void MultiControllerSet::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
	{
	}
}
