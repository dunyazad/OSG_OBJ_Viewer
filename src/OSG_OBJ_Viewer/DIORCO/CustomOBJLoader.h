#include <DIORCO/DIORCO_Common.h>
#include <DIORCO/Util/Util.h>
#include "tiny_obj_loader.h"

#include <OSG_Common.h>

namespace DIORCO {

class CustomOBJLoader {
public:
	CustomOBJLoader(const std::string& basePath);
	~CustomOBJLoader();

	osg::ref_ptr<osg::Group> LoadObjFile(const std::string relativeFilePathFromBase);
private:
	std::string m_basePath;
};

}