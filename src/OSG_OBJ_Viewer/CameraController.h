#pragma once

#include "OSG_Common.h"

struct CameraInfo {
	osg::ref_ptr<osg::Camera> pCamera;
	osg::Vec3d target;
	osg::Vec3d position;
	osg::Vec3d up;
};

class CameraController
{
public:
	CameraController(osgViewer::Viewer* pViewer, osg::Group* pRoot, osg::ref_ptr<osg::Camera> pCamera);
	~CameraController(void);

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
	std::map<std::string, osg::ref_ptr<osg::Camera>> m_cameraInfoMap;
	osg::ref_ptr<osg::Camera> m_pMainCamera;
	osg::Vec3d m_target;
	osg::Vec3d m_position;
	osg::Vec3d m_up;

	double m_angleH;
	double m_angleV;
	double m_distance;
	bool m_lButtonDown;
	bool m_mButtonDown;
	bool m_rButtonDown;
	CPoint m_lastLButtonPosition;
	CPoint m_lastMButtonPosition;
	CPoint m_lastRButtonPosition;
	CPoint m_lastMousePosition;

	osgViewer::Viewer* m_pViewer;
	osg::Group* m_pRoot;
	osg::ref_ptr<osg::Box> m_pCube;
	osg::ref_ptr<osg::ShapeDrawable> m_pCubeDrawable;
	osg::ref_ptr<osg::Geode> m_pCubeGeode;
	osg::ref_ptr<osg::PositionAttitudeTransform> m_pCubePAT;

	void ApplyChange();
};

