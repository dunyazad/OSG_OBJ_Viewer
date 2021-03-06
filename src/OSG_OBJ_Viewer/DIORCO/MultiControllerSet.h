#include <OSG_Common.h>
#include <DIORCO/DIORCO_Common.h>

namespace DIORCO {
	class RTTViewContainer;

	class MultiControllerSet {
	public:
		MultiControllerSet(RTTViewContainer* pViewContainer, osg::ref_ptr<osg::Group> pRoot, osg::ref_ptr<osg::Camera> pCamera);
		~MultiControllerSet();

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
		RTTViewContainer* m_pViewContainer;
		osg::ref_ptr<osg::Group> m_pRoot;
		osg::ref_ptr<osg::Camera> m_pCamera;

		osg::ref_ptr<osg::Group> m_pControllerSet;
		osg::ref_ptr<osg::Group> m_pArrowTX;
		osg::ref_ptr<osg::Group> m_pArrowTY;
		osg::ref_ptr<osg::Group> m_pArrowTZ;
		osg::ref_ptr<osg::Group> m_pArrowRX;
		osg::ref_ptr<osg::Group> m_pArrowRY;
		osg::ref_ptr<osg::Group> m_pArrowRZ;
	};
}
