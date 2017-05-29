#include <DIORCO/DIORCO_Common.h>
#include <DIORCO/Util/Util.h>
#include "tiny_obj_loader.h"

#include <OSG_Common.h>

namespace DIORCO {

class CustomOBJLoader {
public:
	CustomOBJLoader();
	~CustomOBJLoader();

	static osg::ref_ptr<osg::MatrixTransform> LoadObjFile(const std::string& objFileName, const std::string& baseDir, bool flipYZ, float scale);
};

}