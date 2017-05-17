#include "stdafx.h"

#include "RTT.h"



struct MyCameraPostDrawCallback : public osg::Camera::DrawCallback
{
    MyCameraPostDrawCallback(osg::Image* image):
        _image(image)
    {
    }

    virtual void operator () (const osg::Camera& /*camera*/) const
    {
        //if (_image && _image->getPixelFormat()==GL_RGBA && _image->getDataType()==GL_UNSIGNED_BYTE)
        //{
        //    // we'll pick out the center 1/2 of the whole image,
        //    int column_start = _image->s()/4;
        //    int column_end = 3*column_start;

        //    int row_start = _image->t()/4;
        //    int row_end = 3*row_start;


        //    // and then invert these pixels
        //    for(int r=row_start; r<row_end; ++r)
        //    {
        //        unsigned char* data = _image->data(column_start, r);
        //        for(int c=column_start; c<column_end; ++c)
        //        {
        //            (*data) = 255-(*data); ++data;
        //            (*data) = 255-(*data); ++data;
        //            (*data) = 255-(*data); ++data;
        //            (*data) = 255; ++data;
        //        }
        //    }


        //    // dirty the image (increments the modified count) so that any textures
        //    // using the image can be informed that they need to update.
        //    _image->dirty();
        //}
        //else if (_image && _image->getPixelFormat()==GL_RGBA && _image->getDataType()==GL_FLOAT)
        //{
        //    // we'll pick out the center 1/2 of the whole image,
        //    int column_start = _image->s()/4;
        //    int column_end = 3*column_start;

        //    int row_start = _image->t()/4;
        //    int row_end = 3*row_start;

        //    // and then invert these pixels
        //    for(int r=row_start; r<row_end; ++r)
        //    {
        //        float* data = (float*)_image->data(column_start, r);
        //        for(int c=column_start; c<column_end; ++c)
        //        {
        //            (*data) = 1.0f-(*data); ++data;
        //            (*data) = 1.0f-(*data); ++data;
        //            (*data) = 1.0f-(*data); ++data;
        //            (*data) = 1.0f; ++data;
        //        }
        //    }

        //    // dirty the image (increments the modified count) so that any textures
        //    // using the image can be informed that they need to update.
        //    _image->dirty();
        //}

		//_image->dirty();
    }

    osg::Image* _image;
};




RTT::RTT(float width, float height, osgViewer::Viewer* pViewer, osg::ref_ptr<osg::Group> root, osg::ref_ptr<osg::Camera> mainCamera)
	: m_width(width), m_height(height), m_pViewer(pViewer), m_eye(10, 0, 0), m_target(0, 0, 0)
{
	m_pCamera = new osg::Camera();

	m_pCamera->setGraphicsContext(mainCamera->getGraphicsContext());

	m_pCamera->setReferenceFrame(osg::Camera::ABSOLUTE_RF);
	m_pCamera->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_pCamera->setClearColor(osg::Vec4f(0.7f, 0.5f, 0.3f, 1.0f));
	m_pCamera->setRenderTargetImplementation(osg::Camera::FRAME_BUFFER_OBJECT);
	m_pCamera->setRenderOrder(osg::Camera::PRE_RENDER);
	m_pCamera->setProjectionMatrix(mainCamera->getProjectionMatrix());
	m_pCamera->setViewport(mainCamera->getViewport());

	float viewportWidth = mainCamera->getViewport()->width();
	float viewportHeight = mainCamera->getViewport()->height();

	m_pCamera->setViewMatrixAsLookAt(m_eye, m_target, osg::Vec3(0, 1, 0));


	m_pTexture = new osg::Texture2D();
	m_pTexture->setTextureSize(viewportWidth, viewportHeight);
	m_pTexture->setInternalFormat(GL_RGBA);
	m_pTexture->setFilter(osg::Texture2D::MIN_FILTER,osg::Texture2D::LINEAR);
	m_pTexture->setFilter(osg::Texture2D::MAG_FILTER,osg::Texture2D::LINEAR);
	
	m_pCamera->attach(osg::Camera::COLOR_BUFFER, m_pTexture.get());

	m_pMatrixTransform = new osg::MatrixTransform();

	m_pGeode = new osg::Geode();
	m_pMatrixTransform->addChild(m_pGeode);

	m_pGeometry = new osg::Geometry();
	m_pGeode->addDrawable(m_pGeometry);

	osg::ref_ptr<osg::Vec3Array> vertexArray = new osg::Vec3Array();
	vertexArray->push_back(osg::Vec3(-width * 0.5f, -height * 0.5f, 0));
	vertexArray->push_back(osg::Vec3(width * 0.5f, -height * 0.5f, 0));
	vertexArray->push_back(osg::Vec3(width * 0.5f, height * 0.5f, 0));
	vertexArray->push_back(osg::Vec3(-width * 0.5f, height * 0.5f, 0));
	m_pGeometry->setVertexArray(vertexArray);

	osg::ref_ptr<osg::Vec4Array> colorArray = new osg::Vec4Array();
	colorArray->push_back(osg::Vec4(1.f, 1.f, 1.f, 1.0f));
	m_pGeometry->setColorArray(colorArray);
	m_pGeometry->setColorBinding(osg::Geometry::BIND_OVERALL);

	osg::ref_ptr<osg::Vec2Array> texCoordArray = new osg::Vec2Array();
	texCoordArray->push_back(osg::Vec2(0.f, 0.f));
	texCoordArray->push_back(osg::Vec2(1.f, 0.f));
	texCoordArray->push_back(osg::Vec2(1.f, 1.f));
	texCoordArray->push_back(osg::Vec2(0.f, 1.f));
	m_pGeometry->setTexCoordArray(0, texCoordArray);

	m_pGeometry->addPrimitiveSet(new osg::DrawArrays(GL_TRIANGLE_FAN, 0, 4));

	m_pGeode->getOrCreateStateSet()->setTextureAttributeAndModes(0, m_pTexture.get(), osg::StateAttribute::ON);

	m_pGeode->getOrCreateStateSet()->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
	osg::ref_ptr<osg::BlendFunc> blendFunc = new osg::BlendFunc;
	blendFunc->setFunction(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	m_pGeode->getOrCreateStateSet()->setAttributeAndModes(blendFunc);

	root->addChild(m_pMatrixTransform);

	m_pViewer->addSlave(m_pCamera);
}

RTT::~RTT()
{
}

const osg::Vec3& RTT::GetCameraPosition() const
{
	return m_eye;
}

void RTT::SetCameraPosition(float x, float y, float z)
{
	m_eye = osg::Vec3(x, y, z);
	m_pCamera->setViewMatrixAsLookAt(m_eye, m_target, osg::Vec3(0, 1, 0));
}

const osg::Vec3& RTT::GetCameraTarget() const
{
	return m_target;
}

void RTT::SetCameraTarget(float x, float y, float z)
{
	m_target = osg::Vec3(x, y, z);
	m_pCamera->setViewMatrixAsLookAt(m_eye, m_target, osg::Vec3(0, 1, 0));
}

void RTT::SetImageTransform(osg::Matrix transform)
{
	m_pMatrixTransform->setMatrix(transform);
}