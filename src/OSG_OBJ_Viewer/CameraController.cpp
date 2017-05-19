#include "StdAfx.h"
#include "CameraController.h"
#include <DIORCO/View/View.h>

CameraController::CameraController() : m_pSelectedCameraInfo(nullptr)
{
	//m_target = osg::Vec3(0, 0, 0);
	//m_up = osg::Vec3(0, 0, 1);
	//m_angleH = 0;
	//m_angleV = 45;
	//m_distance = 50;

	//m_lButtonDown = false;
	//m_mButtonDown = false;
	//m_rButtonDown = false;



	//m_pCube = new osg::Box(osg::Vec3(0, 0, 0), 1.0f);
	//m_pCubeDrawable = new osg::ShapeDrawable(m_pCube);
	//m_pCubeGeode = new osg::Geode();
	//m_pCubeGeode->addDrawable(m_pCubeDrawable);
	//m_pCubePAT = new osg::PositionAttitudeTransform();
	//m_pCubePAT->addChild(m_pCubeGeode);
	//m_pRoot->addChild(m_pCubePAT);

//	this->ApplyChange();
}

CameraController::~CameraController()
{
}

void CameraController::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	osg::Vec3 dir = m_pSelectedCameraInfo->target - m_pSelectedCameraInfo->position;
	dir.normalize();

	osg::Vec3 right = dir ^ m_pSelectedCameraInfo->up;

	if(nChar == 'A' || nChar == 'a') {
		m_pSelectedCameraInfo->target -= right;
	} else if(nChar == 'D' || nChar == 'd') {
		m_pSelectedCameraInfo->target += right;
	} else if(nChar == 'W' || nChar == 'w') {
		m_pSelectedCameraInfo->target += dir;
	} else if(nChar == 'S' || nChar == 's') {
		m_pSelectedCameraInfo->target -= dir;
	}

	this->ApplyChange();
}

void CameraController::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	this->ApplyChange();
}
void CameraController::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_pSelectedCameraInfo->lButtonDown = true;
	m_pSelectedCameraInfo->lastLButtonPosition = point;
	this->ApplyChange();
}

void CameraController::OnLButtonUp(UINT nFlags, CPoint point)
{
	//osg::ref_ptr<osgUtil::RayIntersector> picker = new osgUtil::RayIntersector(osgUtil::Intersector::WINDOW, point.x, point.y); 
	//osgUtil::IntersectionVisitor iv(picker.get()); 
	//m_pCamera->accept(iv); 
	//if (picker->containsIntersections()) 
	//{
	//	AfxMessageBox(L"Pick");
	//}

	m_pSelectedCameraInfo->lButtonDown = false;
	this->ApplyChange();
}

void CameraController::OnMButtonDown(UINT nFlags, CPoint point)
{
	m_pSelectedCameraInfo->mButtonDown = true;
	m_pSelectedCameraInfo->lastMButtonPosition = point;
	this->ApplyChange();
}

void CameraController::OnMButtonUp(UINT nFlags, CPoint point)
{
	m_pSelectedCameraInfo->mButtonDown = false;
	this->ApplyChange();
}

void CameraController::OnRButtonDown(UINT nFlags, CPoint point)
{
	m_pSelectedCameraInfo->rButtonDown = true;
	m_pSelectedCameraInfo->lastRButtonPosition = point;
	this->ApplyChange();
}

void CameraController::OnRButtonUp(UINT nFlags, CPoint point)
{
	m_pSelectedCameraInfo->rButtonDown = false;
	this->ApplyChange();
}

void CameraController::OnMouseMove(UINT nFlags, CPoint point)
{
	CPoint delta = m_pSelectedCameraInfo->lastMousePosition - point;

	osg::Vec3 dir = m_pSelectedCameraInfo->target - m_pSelectedCameraInfo->position;
	dir.normalize();
	osg::Vec3 right = dir ^ m_pSelectedCameraInfo->up;

	if(m_pSelectedCameraInfo->lButtonDown) {
	} else if(m_pSelectedCameraInfo->mButtonDown) {
		m_pSelectedCameraInfo->target += right * delta.x * 0.25;
		m_pSelectedCameraInfo->target -= m_pSelectedCameraInfo->up * delta.y * 0.25;
	} else if(m_pSelectedCameraInfo->rButtonDown) {
		m_pSelectedCameraInfo->angleH += delta.x * 0.5;
		m_pSelectedCameraInfo->angleV -= delta.y * 0.5;

		if(m_pSelectedCameraInfo->angleV > 89) m_pSelectedCameraInfo->angleV = 89;
		if(m_pSelectedCameraInfo->angleV < -89) m_pSelectedCameraInfo->angleV = -89;
	}

	this->ApplyChange();

	m_pSelectedCameraInfo->lastMousePosition = point;
}

void CameraController::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	float delta = m_pSelectedCameraInfo->distance * 0.025;
	if(nFlags & MK_SHIFT) delta *= 10;

	if(zDelta > 0) {
		m_pSelectedCameraInfo->distance -= delta;
	} else if(zDelta < 0) {
		m_pSelectedCameraInfo->distance += delta;
	}

	this->ApplyChange();
}

void CameraController::RegisterView(DIORCO::RTTView* pView)
{
	if(m_cameraInfos.count(pView) == 0) {
		CameraInfo info;
		info.pMainCamera = pView->GetCamera().get();
		info.target = osg::Vec3(0, 0, 0);
		info.up = osg::Vec3(0, 0, 1);
		info.angleH = 0;
		info.angleV = 45;
		info.distance = 50;
		info.lButtonDown = false;
		info.mButtonDown = false;
		info.rButtonDown = false;
		m_cameraInfos[pView] = info;

		if(m_pSelectedCameraInfo == nullptr) {
			this->SelectView(pView);
		}
	}
}

void CameraController::RegisterView(osg::ref_ptr<DIORCO::RTTView> pView)
{
	this->RegisterView(pView.get());
}

void CameraController::SelectView(DIORCO::RTTView* pView)
{
	m_pSelectedCameraInfo = &m_cameraInfos[pView];
}

void CameraController::ApplyChange()
{
	osg::Matrix mH = osg::Matrix::rotate(osg::DegreesToRadians(m_pSelectedCameraInfo->angleH), osg::Vec3(0, 0, 1));
	osg::Matrix mV = osg::Matrix::rotate(osg::DegreesToRadians(m_pSelectedCameraInfo->angleV), osg::Vec3(1, 0, 0));
	m_pSelectedCameraInfo->position = m_pSelectedCameraInfo->target + osg::Vec3(0, m_pSelectedCameraInfo->distance, 0) * mV * mH;

	m_pSelectedCameraInfo->up.normalize();

	if(m_pSelectedCameraInfo->pMainCamera) {
		m_pSelectedCameraInfo->pMainCamera->setViewMatrixAsLookAt(m_pSelectedCameraInfo->position, m_pSelectedCameraInfo->target, m_pSelectedCameraInfo->up );
	}

	//m_pCubePAT->setPosition(m_target);
}

//void pick()
//{
//    osgUtil::LineSegmentIntersector::Intersections intersections;
//
//    std::string gdlist="";
//
//    if (view->computeIntersections(ea,intersections))
//    {
//        for(osgUtil::LineSegmentIntersector::Intersections::iterator hitr = intersections.begin();
//            hitr != intersections.end();
//            ++hitr)
//        {
//            std::ostringstream os;
//            if (!hitr->nodePath.empty() && !(hitr->nodePath.back()->getName().empty()))
//            {
//                // the geodes are identified by name.
//                os<<"Object \""<<hitr->nodePath.back()->getName()<<"\""<<std::endl;
//            }
//            else if (hitr->drawable.valid())
//            {
//                os<<"Object \""<<hitr->drawable->className()<<"\""<<std::endl;
//            }
//
//            os<<"        local coords vertex("<< hitr->getLocalIntersectPoint()<<")"<<"  normal("<<hitr->getLocalIntersectNormal()<<")"<<std::endl;
//            os<<"        world coords vertex("<< hitr->getWorldIntersectPoint()<<")"<<"  normal("<<hitr->getWorldIntersectNormal()<<")"<<std::endl;
//            const osgUtil::LineSegmentIntersector::Intersection::IndexList& vil = hitr->indexList;
//            for(unsigned int i=0;i<vil.size();++i)
//            {
//                os<<"        vertex indices ["<<i<<"] = "<<vil[i]<<std::endl;
//            }
//
//            gdlist += os.str();
//        }
//    }
//    setLabel(gdlist);
//}