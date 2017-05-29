#include "stdafx.h"

#include "MultiControllerSet.h"
#include "CustomOBJLoader.h"

namespace DIORCO {
	MultiControllerSet::MultiControllerSet(osg::ref_ptr<osg::Group> pRoot, osg::ref_ptr<osg::Camera> pCamera)
		: m_pRoot(pRoot), m_pCamera(pCamera)
	{
		m_pArrowTX = DIORCO::CustomOBJLoader::LoadObjFile("..\\..\\res\\Arrow\\ArrowT.obj", "..\\..\\res\\Arrow", true, 0.01f);
		m_pArrowTY = DIORCO::CustomOBJLoader::LoadObjFile("..\\..\\res\\Arrow\\ArrowT.obj", "..\\..\\res\\Arrow", true, 0.01f);
		m_pArrowTZ = DIORCO::CustomOBJLoader::LoadObjFile("..\\..\\res\\Arrow\\ArrowT.obj", "..\\..\\res\\Arrow", true, 0.01f);

		m_pArrowRX = DIORCO::CustomOBJLoader::LoadObjFile("..\\..\\res\\Arrow\\ArrowR.obj", "..\\..\\res\\Arrow", true, 0.01f);
		m_pArrowRY = DIORCO::CustomOBJLoader::LoadObjFile("..\\..\\res\\Arrow\\ArrowR.obj", "..\\..\\res\\Arrow", true, 0.01f);
		m_pArrowRZ = DIORCO::CustomOBJLoader::LoadObjFile("..\\..\\res\\Arrow\\ArrowR.obj", "..\\..\\res\\Arrow", true, 0.01f);

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
