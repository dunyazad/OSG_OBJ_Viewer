#pragma once

#include "OSG_Common.h"

namespace DIORCO {
	class RTTView;
}

struct CameraInfo {
	osg::ref_ptr<osg::Camera> pMainCamera;
	osg::Vec3d target;
	osg::Vec3d position;
	osg::Vec3d up;

	double angleH;
	double angleV;
	double distance;

	bool lButtonDown;
	bool mButtonDown;
	bool rButtonDown;
	CPoint lastLButtonPosition;
	CPoint lastMButtonPosition;
	CPoint lastRButtonPosition;
	CPoint lastMousePosition;

	bool enabled;
};

class CameraController
{
public:
	CameraController();
	~CameraController();

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

	void RegisterView(DIORCO::RTTView* pView);
	void RegisterView(osg::ref_ptr<DIORCO::RTTView> pView);
	void SelectView(DIORCO::RTTView* pView);

private:
	std::map<DIORCO::RTTView*, CameraInfo> m_cameraInfos;
	CameraInfo* m_pSelectedCameraInfo;
	void ApplyChange();
	void ApplyChange(DIORCO::RTTView* pView);
};

