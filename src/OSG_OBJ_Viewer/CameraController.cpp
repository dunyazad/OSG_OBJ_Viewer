#include "StdAfx.h"
#include "CameraController.h"

CameraController::CameraController(osgViewer::Viewer* pViewer, osg::Group* pRoot, osg::ref_ptr<osg::Camera> pCamera) : m_pViewer(pViewer), m_pRoot(pRoot), m_pMainCamera(pCamera)
{
	m_target = osg::Vec3(0, 0, 0);
	m_up = osg::Vec3(0, 0, 1);
	m_angleH = 0;
	m_angleV = 45;
	m_distance = 50;

	m_lButtonDown = false;
	m_mButtonDown = false;
	m_rButtonDown = false;



	//m_pCube = new osg::Box(osg::Vec3(0, 0, 0), 1.0f);
	//m_pCubeDrawable = new osg::ShapeDrawable(m_pCube);
	//m_pCubeGeode = new osg::Geode();
	//m_pCubeGeode->addDrawable(m_pCubeDrawable);
	//m_pCubePAT = new osg::PositionAttitudeTransform();
	//m_pCubePAT->addChild(m_pCubeGeode);
	//m_pRoot->addChild(m_pCubePAT);


	this->ApplyChange();
}

CameraController::~CameraController(void)
{
}

void CameraController::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	osg::Vec3 dir = m_target - m_position;
	dir.normalize();

	osg::Vec3 right = dir ^ m_up;

	if(nChar == 'A' || nChar == 'a') {
		m_target -= right;
	} else if(nChar == 'D' || nChar == 'd') {
		m_target += right;
	} else if(nChar == 'W' || nChar == 'w') {
		m_target += dir;
	} else if(nChar == 'S' || nChar == 's') {
		m_target -= dir;
	}

	this->ApplyChange();
}

void CameraController::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	this->ApplyChange();
}
void CameraController::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_lButtonDown = true;
	m_lastLButtonPosition = point;
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

	m_lButtonDown = false;
	this->ApplyChange();
}

void CameraController::OnMButtonDown(UINT nFlags, CPoint point)
{
	m_mButtonDown = true;
	m_lastMButtonPosition = point;
	this->ApplyChange();
}

void CameraController::OnMButtonUp(UINT nFlags, CPoint point)
{
	m_mButtonDown = false;
	this->ApplyChange();
}

void CameraController::OnRButtonDown(UINT nFlags, CPoint point)
{
	m_rButtonDown = true;
	m_lastRButtonPosition = point;
	this->ApplyChange();
}

void CameraController::OnRButtonUp(UINT nFlags, CPoint point)
{
	m_rButtonDown = false;
	this->ApplyChange();
}

void CameraController::OnMouseMove(UINT nFlags, CPoint point)
{
	CPoint delta = m_lastMousePosition - point;

	osg::Vec3 dir = m_target - m_position;
	dir.normalize();
	osg::Vec3 right = dir ^ m_up;

	if(m_lButtonDown) {
	} else if(m_mButtonDown) {
		m_target += right * delta.x * 0.25;
		m_target -= m_up * delta.y * 0.25;
	} else if(m_rButtonDown) {
		m_angleH += delta.x * 0.5;
		m_angleV -= delta.y * 0.5;

		if(m_angleV > 89) m_angleV = 89;
		if(m_angleV < -89) m_angleV = -89;
	}

	this->ApplyChange();

	m_lastMousePosition = point;
}

void CameraController::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	float delta = m_distance * 0.025;
	if(nFlags & MK_SHIFT) delta *= 10;

	if(zDelta > 0) {
		m_distance -= delta;
	} else if(zDelta < 0) {
		m_distance += delta;
	}

	this->ApplyChange();
}

void CameraController::ApplyChange()
{
	osg::Matrix mH = osg::Matrix::rotate(osg::DegreesToRadians(m_angleH), osg::Vec3(0, 0, 1));
	osg::Matrix mV = osg::Matrix::rotate(osg::DegreesToRadians(m_angleV), osg::Vec3(1, 0, 0));
	m_position = m_target + osg::Vec3(0, m_distance, 0) * mV * mH;

	m_up.normalize();

	m_pCamera->setViewMatrixAsLookAt( m_position, m_target, m_up );

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