#pragma once

#include "OSG_Common.h"
#include <DIORCO/DIORCO_Common.h>
#include <DIORCO/Util/Util.h>
#include "RenderingThread.h"
#include "CameraController.h"

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

	osg::ref_ptr<osg::Group> LoadObjFile(const std::string& objFileName, const std::string& baseDir, bool flipYZ = false, float scale = 1);

private:
	bool mDone;
	std::string m_ModelName;
	HWND m_hWnd;
	osgViewer::Viewer* mViewer;
	osg::ref_ptr<osg::Group> m_pRoot;
	osg::ref_ptr<osg::MatrixTransform> m_pMainObject;
	
	osg::ref_ptr<osg::MatrixTransform> m_pAxisIndicator;
	osg::ref_ptr<osg::MatrixTransform> m_pObjectMaipulator;
	osg::ref_ptr<osg::MatrixTransform> m_pAxes;
	osg::ref_ptr<osg::MatrixTransform> m_pAxisHandles;

	CameraController* m_pCameraController;

	osg::ref_ptr<osg::Camera> m_pCameraMain;
	osg::ref_ptr<osg::Camera> m_pCameraLeft;
	osg::ref_ptr<osg::Camera> m_pCameraTop;
	osg::ref_ptr<osg::Camera> m_pCameraFront;
};
