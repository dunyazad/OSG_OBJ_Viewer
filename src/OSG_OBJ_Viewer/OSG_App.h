#pragma once

#include "OSG_Common.h"
#include <DIORCO/DIORCO_Common.h>
#include <DIORCO/Util/Util.h>
#include <DIORCO/View/View.h>
#include <DIORCO/MultiControllerSet.h>
#include "RenderingThread.h"
#include "CameraController.h"
#include "RTT.h"

class OSG_App
{
public:
	OSG_App(HWND hWnd);
	~OSG_App();

	void Initialize();
	void InitModels();
	void InitScene();
	void InitCameraConfig();
	void InitLight();
	osg::ref_ptr<osg::PositionAttitudeTransform> CreateLight(int index, osg::Vec4 position, osg::Vec3 direction);
	void SetupWindow();
	void SetupCamera();
	void PreFrameUpdate();
	void PostFrameUpdate();
	void Done(bool value) { mDone = value; }
	bool Done() { return mDone; }
	//static void Render(void* ptr);

	osgViewer::Viewer* getViewer() { return mViewer; }

	osg::ref_ptr<osg::Node> LoadModel(std::string filename);


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

	osg::ref_ptr<osg::MatrixTransform> LoadObjFile(const std::string& objFileName, const std::string& baseDir, bool flipYZ = false, float scale = 1);

private:
	bool mDone;
	std::string m_ModelName;
	HWND m_hWnd;
	osgViewer::Viewer* mViewer;

	DIORCO::RTTViewContainer* m_pViewContainer;

	DIORCO::MultiControllerSet* m_pMultiControllerSet;

	osg::ref_ptr<osg::Group> m_pRoot;
	osg::ref_ptr<osg::MatrixTransform> m_pMainObject;

	osg::ref_ptr<osg::MatrixTransform> m_pMxGroup;
	osg::ref_ptr<osg::MatrixTransform> m_pMxBone;
	osg::ref_ptr<osg::MatrixTransform> m_pT11;
	osg::ref_ptr<osg::MatrixTransform> m_pT12;
	osg::ref_ptr<osg::MatrixTransform> m_pT13;
	osg::ref_ptr<osg::MatrixTransform> m_pT14;
	osg::ref_ptr<osg::MatrixTransform> m_pT15;
	osg::ref_ptr<osg::MatrixTransform> m_pT16;
	osg::ref_ptr<osg::MatrixTransform> m_pT17;
	osg::ref_ptr<osg::MatrixTransform> m_pT21;
	osg::ref_ptr<osg::MatrixTransform> m_pT22;
	osg::ref_ptr<osg::MatrixTransform> m_pT23;
	osg::ref_ptr<osg::MatrixTransform> m_pT24;
	osg::ref_ptr<osg::MatrixTransform> m_pT25;
	osg::ref_ptr<osg::MatrixTransform> m_pT26;
	osg::ref_ptr<osg::MatrixTransform> m_pT27;

	osg::ref_ptr<osg::MatrixTransform> m_pMdGroup;
	osg::ref_ptr<osg::MatrixTransform> m_pMdBone;
	osg::ref_ptr<osg::MatrixTransform> m_pT31;
	osg::ref_ptr<osg::MatrixTransform> m_pT32;
	osg::ref_ptr<osg::MatrixTransform> m_pT33;
	osg::ref_ptr<osg::MatrixTransform> m_pT34;
	osg::ref_ptr<osg::MatrixTransform> m_pT35;
	osg::ref_ptr<osg::MatrixTransform> m_pT36;
	osg::ref_ptr<osg::MatrixTransform> m_pT37;
	osg::ref_ptr<osg::MatrixTransform> m_pT41;
	osg::ref_ptr<osg::MatrixTransform> m_pT42;
	osg::ref_ptr<osg::MatrixTransform> m_pT43;
	osg::ref_ptr<osg::MatrixTransform> m_pT44;
	osg::ref_ptr<osg::MatrixTransform> m_pT45;
	osg::ref_ptr<osg::MatrixTransform> m_pT46;
	osg::ref_ptr<osg::MatrixTransform> m_pT47;
	
	osg::ref_ptr<osg::MatrixTransform> m_pAxisIndicator;
	osg::ref_ptr<osg::MatrixTransform> m_pObjectMaipulator;
	osg::ref_ptr<osg::MatrixTransform> m_pAxes;
	osg::ref_ptr<osg::MatrixTransform> m_pAxisHandles;

	CameraController* m_pCameraController;

	osg::ref_ptr<osg::Camera> m_pCameraMain;
	osg::ref_ptr<osg::Camera> m_pCameraLeft;
	osg::ref_ptr<osg::Camera> m_pCameraTop;
	osg::ref_ptr<osg::Camera> m_pCameraFront;

	std::vector<RTT*> m_pRTTs;
};
