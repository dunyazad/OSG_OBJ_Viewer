#include "StdAfx.h"
#include "Custom3DDataManager.h"
#include "Custom3DDataFile.h"

Custom3DDataManager::Custom3DDataManager()
{
}

Custom3DDataManager::~Custom3DDataManager()
{
}

ref_ptr<Group> Custom3DDataManager::LoadObjFile(const string& objFileName, const string& baseDir, bool flipYZ, float scale, const string& name)
{
	string baseDirectory = baseDir;
	if(baseDirectory[baseDirectory.size() - 1] != '\\' || baseDirectory[baseDirectory.size() - 1] != '/') {
		baseDirectory += '\\';
	}

	tinyobj::attrib_t attrib;
	vector<tinyobj::shape_t> shapes;
	vector<tinyobj::material_t> materials;
	map<string, GLuint> textures;

	string err;
	bool ret =
		tinyobj::LoadObj(&attrib, &shapes, &materials, &err, objFileName.c_str(), baseDirectory.c_str(), true);
	if (!err.empty()) {
		cerr << err << endl;
	}

	if (!ret) {
		cerr << "Failed to load " << objFileName << endl;
		return nullptr;
	}

	vector<ref_ptr<Material>> osgMaterials;
	vector<ref_ptr<Texture2D>> osgTextures;
	vector<ref_ptr<Vec3Array>> osgVertices;
	vector<ref_ptr<Vec3Array>> osgNormals;
	vector<ref_ptr<Vec2Array>> osgTexCoords;

	if(materials.size() == 0) {
		//material 추가
		tinyobj::material_t material;
		material.ambient[0] = 1;
		material.ambient[1] = 1;
		material.ambient[2] = 1;

		material.diffuse[0] = 1;
		material.diffuse[1] = 1;
		material.diffuse[2] = 1;

		material.specular[2] = 1;
		material.specular[2] = 1;
		material.specular[2] = 1;

		material.shininess = 0;

		materials.push_back(material);
	}
	for (unsigned int materialIndex = 0; materialIndex < materials.size(); materialIndex++) {
		auto material = materials[materialIndex];

		ref_ptr<Material> osgMaterial = new Material;
		//osgMaterial->setColorMode(Material::DIFFUSE);
		osgMaterial->setAmbient(Material::FRONT_AND_BACK, Vec4(material.ambient[0], material.ambient[1], material.ambient[2], 1));
		osgMaterial->setDiffuse(Material::FRONT_AND_BACK, Vec4(material.diffuse[0], material.diffuse[1], material.diffuse[2], 1));
		osgMaterial->setSpecular(Material::FRONT_AND_BACK, Vec4(material.specular[0], material.specular[1], material.specular[2], 1));
		osgMaterial->setShininess(Material::FRONT_AND_BACK, material.shininess);

		if(material.diffuse_texname.size() > 0) {
			auto textureFileName = baseDirectory + material.diffuse_texname;
			ref_ptr<Texture2D> map_Kd = new Texture2D(osgDB::readImageFile(textureFileName.c_str()));
			//map_Kd->setInternalFormatMode(Texture2D::USE_S3TC_DXT1_COMPRESSION);
			//map_Kd->setUnRefImageDataAfterApply(true);
			osgTextures.push_back(map_Kd);

			osgMaterial->setDiffuse(Material::FRONT_AND_BACK, Vec4(1, 1, 1, 1));
		} else {
			osgTextures.push_back(nullptr);
		}
		osgMaterials.push_back(osgMaterial);

		osgVertices.push_back(new Vec3Array);
		osgNormals.push_back(new Vec3Array);
		osgTexCoords.push_back(new Vec2Array);
	}

	ref_ptr<Group> group = new Group();

	// Loop over shapes
	for (size_t s = 0; s < shapes.size(); s++) {
		// Loop over faces(polygon)
		size_t index_offset = 0;
		for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
			int fv = shapes[s].mesh.num_face_vertices[f];
			// per-face material
			auto material = shapes[s].mesh.material_ids[f];
			if(material == -1) material = 0;

			// Loop over vertices in the face.
			for (int v = 0; v < fv; v++) {
				// access to vertex
				tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
				if(attrib.vertices.size() > 0) {
					auto vx = attrib.vertices[3*idx.vertex_index+0] * scale;
					auto vy = attrib.vertices[3*idx.vertex_index+1] * scale;
					auto vz = attrib.vertices[3*idx.vertex_index+2] * scale;
					if(flipYZ) {
						osgVertices[material]->push_back(Vec3(vx, vz, vy));
					} else {
						osgVertices[material]->push_back(Vec3(vx, vy, vz));
					}
				}
				if(attrib.normals.size() > 0) {
					auto nx = attrib.normals[3*idx.normal_index+0];
					auto ny = attrib.normals[3*idx.normal_index+1];
					auto nz = attrib.normals[3*idx.normal_index+2];
					osgNormals[material]->push_back(Vec3(nx, ny, nz));
				}
				if(attrib.texcoords.size() > 0) {
					auto tu = attrib.texcoords[2*idx.texcoord_index+0];
					auto tv = attrib.texcoords[2*idx.texcoord_index+1];
					osgTexCoords[material]->push_back(Vec2(tu, tv));
				}
			}
			index_offset += fv;
		}
	}

	for (unsigned int materialIndex = 0; materialIndex < materials.size(); materialIndex++) {
		ref_ptr<Geode> geode = new Geode;
		geode->setName(name);
		group->addChild(geode);
		auto ss = geode->getOrCreateStateSet();
		//		ss->setMode(GL_LIGHTING, StateAttribute::OFF);
		ss->setMode(GL_LIGHTING, StateAttribute::ON);

		ss->setAttributeAndModes(osgMaterials[materialIndex].get(), StateAttribute::ON);

		if(materials[materialIndex].diffuse_texname.size() > 0) {
			if(osgTextures[materialIndex]) {
				ss->setTextureAttributeAndModes(0, osgTextures[materialIndex].get(), StateAttribute::ON);
			}
		}

		ref_ptr<Geometry> geometry = new Geometry;
		//		geometry->setName(name);
		geode->addDrawable(geometry);

		geometry->setVertexArray(osgVertices[materialIndex]);
		geometry->setNormalArray(osgNormals[materialIndex]);
		geometry->setTexCoordArray(0, osgTexCoords[materialIndex]);
		ref_ptr<DrawElementsUInt> ia = new DrawElementsUInt(PrimitiveSet::TRIANGLES, 0);
		for(unsigned int i = 0; i < osgVertices[materialIndex]->size(); i++) {
			ia->push_back(i);
		}
		geometry->addPrimitiveSet(ia);
	}

	return group;
}

/*
ref_ptr<Group> Custom3DDataManager::LoadObjFileVertexOnly(const string& objFileName, const string& baseDir, bool flipYZ, float scale, const string& name)
{
string baseDirectory = baseDir;
if(baseDirectory[baseDirectory.size() - 1] != '\\' || baseDirectory[baseDirectory.size() - 1] != '/') {
baseDirectory += '\\';
}

tinyobj::attrib_t attrib;
vector<tinyobj::shape_t> shapes;
vector<tinyobj::material_t> materials;
map<string, GLuint> textures;

string err;
bool ret =
tinyobj::LoadObj(&attrib, &shapes, &materials, &err, objFileName.c_str(), baseDirectory.c_str(), true);
if (!err.empty()) {
cerr << err << endl;
}

if (!ret) {
cerr << "Failed to load " << objFileName << endl;
return nullptr;
}

vector<ref_ptr<Material>> osgMaterials;
vector<ref_ptr<Vec3Array>> osgVertices;

if(materials.size() == 0) {
//material 추가
tinyobj::material_t material;
material.ambient[0] = 1;
material.ambient[1] = 1;
material.ambient[2] = 1;

material.diffuse[0] = 1;
material.diffuse[1] = 1;
material.diffuse[2] = 1;

material.specular[2] = 1;
material.specular[2] = 1;
material.specular[2] = 1;

material.shininess = 0;

materials.push_back(material);
}
for (unsigned int materialIndex = 0; materialIndex < materials.size(); materialIndex++) {
auto material = materials[materialIndex];

ref_ptr<Material> osgMaterial = new Material;
osgMaterial->setColorMode(Material::DIFFUSE);
osgMaterial->setAmbient(Material::FRONT_AND_BACK, Vec4(material.ambient[0], material.ambient[1], material.ambient[2], 1));
osgMaterial->setDiffuse(Material::FRONT_AND_BACK, Vec4(material.diffuse[0], material.diffuse[1], material.diffuse[2], 1));
osgMaterial->setSpecular(Material::FRONT_AND_BACK, Vec4(material.specular[0], material.specular[1], material.specular[2], 1));
osgMaterial->setShininess(Material::FRONT_AND_BACK, material.shininess);

osgMaterials.push_back(osgMaterial);

osgVertices.push_back(new Vec3Array);
}

ref_ptr<Group> group = new Group();

// Loop over shapes
for (size_t s = 0; s < shapes.size(); s++) {
// Loop over faces(polygon)
size_t index_offset = 0;
for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
int fv = shapes[s].mesh.num_face_vertices[f];
// per-face material
auto material = shapes[s].mesh.material_ids[f];
if(material == -1) material = 0;

// Loop over vertices in the face.
for (int v = 0; v < fv; v++) {
// access to vertex
tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
if(attrib.vertices.size() > 0) {
auto vx = attrib.vertices[3*idx.vertex_index+0] * scale;
auto vy = attrib.vertices[3*idx.vertex_index+1] * scale;
auto vz = attrib.vertices[3*idx.vertex_index+2] * scale;
if(flipYZ) {
osgVertices[material]->push_back(Vec3(vx, vz, vy));
} else {
osgVertices[material]->push_back(Vec3(vx, vy, vz));
}
}
}
index_offset += fv;
}
}

for (unsigned int materialIndex = 0; materialIndex < materials.size(); materialIndex++) {
ref_ptr<Geode> geode = new Geode;
geode->setName(name);
group->addChild(geode);
auto ss = geode->getOrCreateStateSet();
//		ss->setMode(GL_LIGHTING, StateAttribute::OFF);
ss->setMode(GL_LIGHTING, StateAttribute::ON);

ss->setAttributeAndModes(osgMaterials[materialIndex].get(), StateAttribute::ON);

ref_ptr<Geometry> geometry = new Geometry;
//		geometry->setName(name);
geode->addDrawable(geometry);

geometry->setVertexArray(osgVertices[materialIndex]);
ref_ptr<DrawElementsUInt> ia = new DrawElementsUInt(PrimitiveSet::TRIANGLES, 0);
for(unsigned int i = 0; i < osgVertices[materialIndex]->size(); i++) {
ia->push_back(i);
}
geometry->addPrimitiveSet(ia);
}

return group;
}
*/

ref_ptr<Group> Custom3DDataManager::LoadObjFileVertexOnly(const string& objFileName, const string& baseDir, bool flipYZ, float scale, const string& name)
{
	string baseDirectory = baseDir;
	if(baseDirectory[baseDirectory.size() - 1] != '\\' || baseDirectory[baseDirectory.size() - 1] != '/') {
		baseDirectory += '\\';
	}

	tinyobj::attrib_t attrib;
	vector<tinyobj::shape_t> shapes;
	vector<tinyobj::material_t> materials;
	map<string, GLuint> textures;

	string err;
	bool ret =
		tinyobj::LoadObj(&attrib, &shapes, &materials, &err, objFileName.c_str(), baseDirectory.c_str(), true);
	if (!err.empty()) {
		cerr << err << endl;
	}

	if (!ret) {
		cerr << "Failed to load " << objFileName << endl;
		return nullptr;
	}

	int iVerticesNum = attrib.vertices.size()/3; // vec3의 개수
	if(!iVerticesNum)
		return nullptr;

	int iShapeNum = shapes.size();
	if(!iShapeNum)
		return nullptr;

	int iTotalIndicesNum = 0;	// face * 3개 (즉, 모서리의 개수)

	for(int i = 0 ; i<iShapeNum ; i++)
	{
		iTotalIndicesNum += shapes[i].mesh.indices.size();
	}

	if(!iTotalIndicesNum)
		return nullptr;

	ref_ptr<Vec3Array> pVertexArray = new Vec3Array(iVerticesNum);
	ref_ptr<DrawElementsUInt> pIndices = new DrawElementsUInt(GL_TRIANGLES, iTotalIndicesNum);

	Vec3 *pVertexBegin = (Vec3 *) (&attrib.vertices[0]);
	for(int i = 0 ; i < iVerticesNum ; i++)
		(*pVertexArray)[i].set(pVertexBegin[i]);

	int iLastIndices = 0;
	for(int j = 0 ; j < iShapeNum ; j++)
	{
		int iIndicesNum = shapes[j].mesh.indices.size();
		int iCurrentIndicesNum = 0;
		for(int i = 0 ; i<iIndicesNum ; i++)
		{
			tinyobj::index_t sIdx = shapes[j].mesh.indices[i];
			(*pIndices)[i+iLastIndices] = sIdx.vertex_index;
			iCurrentIndicesNum++;
		}
		iLastIndices+=iCurrentIndicesNum;
	}

	ref_ptr<Group> pObjGroup = new Group();
	ref_ptr<Geode> pObjGeode = new Geode;
	pObjGeode->setName(name);

	ref_ptr<Geometry> pObjGeometry = new Geometry();
	pObjGeometry->setVertexArray( pVertexArray.get() );
	pObjGeometry->addPrimitiveSet( pIndices.get() );

	// Normal 자동 생성
	osgUtil::SmoothingVisitor::smooth( *pObjGeometry );

	pObjGeode->addDrawable( pObjGeometry.get() );

	pObjGroup->addChild(pObjGeode.get());

	return pObjGroup;
}

bool Custom3DDataManager::LoadObjFileVertexOnly(const string& objFileName, const string& baseDir, RAW_DATA_INFO &sRawDataInfo)
{
	string baseDirectory = baseDir;
	if(baseDirectory[baseDirectory.size() - 1] != '\\' || baseDirectory[baseDirectory.size() - 1] != '/') {
		baseDirectory += '\\';
	}

	tinyobj::attrib_t attrib;
	vector<tinyobj::shape_t> shapes;
	vector<tinyobj::material_t> materials;
	map<string, GLuint> textures;

	string err;
	bool ret =
		tinyobj::LoadObj(&attrib, &shapes, &materials, &err, objFileName.c_str(), baseDirectory.c_str(), true);
	if (!err.empty()) {
		cerr << err << endl;
	}

	if (!ret) {
		cerr << "Failed to load " << objFileName << endl;
		return false;
	}

	int iVerticesNum = attrib.vertices.size()/3; // vec3의 개수
	if(!iVerticesNum)
		return false;

	int iShapeNum = shapes.size();
	if(!iShapeNum)
		return false;

	int iTotalIndicesNum = 0;	// face * 3개 (즉, 모서리의 개수)

	for(int i = 0 ; i<iShapeNum ; i++)
	{
		iTotalIndicesNum += shapes[i].mesh.indices.size();
	}

	if(!iTotalIndicesNum)
		return false;

	sRawDataInfo.bSetInfo = true;

	// vertex
	sRawDataInfo.iVertexNum = iVerticesNum;
	sRawDataInfo.pVertexPtr = new Vec3[iVerticesNum];
	memset(sRawDataInfo.pVertexPtr, 0, sizeof(Vec3)*iVerticesNum);
	Vec3 *pVertexBegin = (Vec3 *) (&attrib.vertices[0]);
	memcpy(sRawDataInfo.pVertexPtr, pVertexBegin, sizeof(Vec3)*iVerticesNum);

	// indices
	sRawDataInfo.iIndicesNum = iTotalIndicesNum;
	sRawDataInfo.pIndicesPtr = new int[iTotalIndicesNum];

	int iLastIndices = 0;
	for(int j = 0 ; j < iShapeNum ; j++)
	{
		int iIndicesNum = shapes[j].mesh.indices.size();
		int iCurrentIndicesNum = 0;
		for(int i = 0 ; i<iIndicesNum ; i++)
		{
			tinyobj::index_t sIdx = shapes[j].mesh.indices[i];
			sRawDataInfo.pIndicesPtr[i+iLastIndices] = sIdx.vertex_index;
			iCurrentIndicesNum++;
		}
		iLastIndices+=iCurrentIndicesNum;
	}

	return true;
}

bool Custom3DDataManager::LoadSTLFile(const string& stlFileName, RAW_DATA_INFO &sRawDataInfo)
{
	vector<Vec3> vertices;
	vector<Vec3> normals;
	vector<int> indices;

	map<Vec3, int> vertexMap;

	Custom3DDataFile file;
	file.Open(stlFileName, false);
	if (!file.isOpen()) return false;

	string word;
	file.GetWord(word);
	if(word == "solid") {
		while (file.GetWord(word))
		{
			if(word == "facet") {
				file.GetWord(word);
				Vec3 normal;
				if(word == "normal") {
					file.GetWord(word);
					normal.x() = atof(word.c_str());
					file.GetWord(word);
					normal.y() = atof(word.c_str());
					file.GetWord(word);
					normal.z() = atof(word.c_str());
				}
				file.GetWord(word);
				if(word == "outer") {
					file.GetWord(word);
					if(word == "loop") {
						file.GetWord(word);
						if(word == "vertex") {
							Vec3 v1;
							file.GetWord(word);
							v1.x() = atof(word.c_str());
							file.GetWord(word);
							v1.y() = atof(word.c_str());
							file.GetWord(word);
							v1.z() = atof(word.c_str());

							if(vertexMap.count(v1) == 0) {
								vertices.push_back(v1);
								normals.push_back(normal);
								vertexMap[v1] = vertices.size() - 1;
								indices.push_back(vertices.size() - 1);
							} else {
								auto index = vertexMap[v1];
								indices.push_back(index);
							}
						}
						file.GetWord(word);
						if(word == "vertex") {
							Vec3 v2;
							file.GetWord(word);
							v2.x() = atof(word.c_str());
							file.GetWord(word);
							v2.y() = atof(word.c_str());
							file.GetWord(word);
							v2.z() = atof(word.c_str());
							if(vertexMap.count(v2) == 0) {
								vertices.push_back(v2);
								normals.push_back(normal);
								vertexMap[v2] = vertices.size() - 1;
								indices.push_back(vertices.size() - 1);
							} else {
								auto index = vertexMap[v2];
								indices.push_back(index);
							}
						}
						file.GetWord(word);
						if(word == "vertex") {
							Vec3 v3;
							file.GetWord(word);
							v3.x() = atof(word.c_str());
							file.GetWord(word);
							v3.y() = atof(word.c_str());
							file.GetWord(word);
							v3.z() = atof(word.c_str());

							if(vertexMap.count(v3) == 0) {
								vertices.push_back(v3);
								normals.push_back(normal);
								vertexMap[v3] = vertices.size() - 1;
								indices.push_back(vertices.size() - 1);
							} else {
								auto index = vertexMap[v3];
								indices.push_back(index);
							}
						}
					}
					file.GetWord(word); // endloop
					file.GetWord(word); // endfacet
				}
			}
		}
	} else {
		file.Close();
		file.Open(stlFileName, true);

		char header[80];
		file.Read(header, 80);
		char buffer[4];
		unsigned int numberOfTriangles;
		file.Read(buffer, sizeof(int));

		numberOfTriangles = *((unsigned int*)&buffer);

		for(unsigned int i = 0; i < numberOfTriangles; i++) {
			float nx, ny, nz;
			float v1x, v1y, v1z;
			float v2x, v2y, v2z;
			float v3x, v3y, v3z;
			short attributeByteCount;

			file.Read((char*)&nx, sizeof(float));
			file.Read((char*)&ny, sizeof(float));
			file.Read((char*)&nz, sizeof(float));

			file.Read((char*)&v1x, sizeof(float));
			file.Read((char*)&v1y, sizeof(float));
			file.Read((char*)&v1z, sizeof(float));

			file.Read((char*)&v2x, sizeof(float));
			file.Read((char*)&v2y, sizeof(float));
			file.Read((char*)&v2z, sizeof(float));

			file.Read((char*)&v3x, sizeof(float));
			file.Read((char*)&v3y, sizeof(float));
			file.Read((char*)&v3z, sizeof(float));

			file.Read((char*)&attributeByteCount, sizeof(short));

			Vec3 v1(v1x, v1y, v1z);
			Vec3 v2(v2x, v2y, v2z);
			Vec3 v3(v3x, v3y, v3z);
			Vec3 n(nx, ny, nz);

			if(vertexMap.count(v1) == 0) {
				vertices.push_back(v1);
				normals.push_back(n);
				vertexMap[v1] = vertices.size() - 1;
				indices.push_back(vertices.size() - 1);
			} else {
				auto index = vertexMap[v1];
				indices.push_back(index);
			}

			if(vertexMap.count(v2) == 0) {
				vertices.push_back(v2);
				normals.push_back(n);
				vertexMap[v2] = vertices.size() - 1;
				indices.push_back(vertices.size() - 1);
			} else {
				auto index = vertexMap[v2];
				indices.push_back(index);
			}

			if(vertexMap.count(v3) == 0) {
				vertices.push_back(v3);
				normals.push_back(n);
				vertexMap[v3] = vertices.size() - 1;
				indices.push_back(vertices.size() - 1);
			} else {
				auto index = vertexMap[v3];
				indices.push_back(index);
			}
		}
	}

	sRawDataInfo.bSetInfo = true;

	// vertex
	sRawDataInfo.iVertexNum = vertices.size();
	sRawDataInfo.pVertexPtr = new Vec3[sRawDataInfo.iVertexNum];
	memset(sRawDataInfo.pVertexPtr, 0, sizeof(Vec3)*sRawDataInfo.iVertexNum);
	Vec3 *pVertexBegin = (Vec3 *) (&vertices[0]);
	memcpy(sRawDataInfo.pVertexPtr, pVertexBegin, sizeof(Vec3)*sRawDataInfo.iVertexNum);

	// indices
	sRawDataInfo.iIndicesNum = indices.size();
	sRawDataInfo.pIndicesPtr = new int[sRawDataInfo.iIndicesNum];
	memset(sRawDataInfo.pIndicesPtr, 0, sizeof(int)*sRawDataInfo.iIndicesNum);
	int *pIndexBegin = (int *) (&indices[0]);
	memcpy(sRawDataInfo.pIndicesPtr, pIndexBegin, sizeof(int)*sRawDataInfo.iIndicesNum);

	printf("Vertices : %d, Normals : %d, Indices : %d\n", vertices.size(), normals.size(), indices.size());

	return true;
}

ref_ptr<Group> Custom3DDataManager::LoadSTLFile(const std::string& stlFileName, bool flipYZ, float scale, const string& name)
{
	ref_ptr<Vec3Array> vertices = new Vec3Array;
	ref_ptr<Vec3Array> normals = new Vec3Array;
	vector<int> indices;

	map<Vec3, int> vertexMap;

	//ref_ptr<DrawElementsUInt> pIndices = new DrawElementsUInt(GL_TRIANGLES, iTotalIndicesNum);

	Custom3DDataFile file;
	file.Open(stlFileName, false);
	if (!file.isOpen()) return false;

	string word;
	file.GetWord(word);
	if(word == "solid") {
		while (file.GetWord(word))
		{
			if(word == "facet") {
				file.GetWord(word);
				Vec3 normal;
				if(word == "normal") {
					file.GetWord(word);
					normal.x() = atof(word.c_str());
					file.GetWord(word);
					normal.y() = atof(word.c_str());
					file.GetWord(word);
					normal.z() = atof(word.c_str());
				}
				file.GetWord(word);
				if(word == "outer") {
					file.GetWord(word);
					if(word == "loop") {
						file.GetWord(word);
						if(word == "vertex") {
							Vec3 v1;
							file.GetWord(word);
							v1.x() = atof(word.c_str());
							file.GetWord(word);
							v1.y() = atof(word.c_str());
							file.GetWord(word);
							v1.z() = atof(word.c_str());

							if(vertexMap.count(v1) == 0) {
								vertices->push_back(v1);
								normals->push_back(normal);
								vertexMap[v1] = vertices->size() - 1;
								indices.push_back(vertices->size() - 1);
							} else {
								auto index = vertexMap[v1];
								indices.push_back(index);
							}
						}
						file.GetWord(word);
						if(word == "vertex") {
							Vec3 v2;
							file.GetWord(word);
							v2.x() = atof(word.c_str());
							file.GetWord(word);
							v2.y() = atof(word.c_str());
							file.GetWord(word);
							v2.z() = atof(word.c_str());

							if(vertexMap.count(v2) == 0) {
								vertices->push_back(v2);
								normals->push_back(normal);
								vertexMap[v2] = vertices->size() - 1;
								indices.push_back(vertices->size() - 1);
							} else {
								auto index = vertexMap[v2];
								indices.push_back(index);
							}
						}
						file.GetWord(word);
						if(word == "vertex") {
							Vec3 v3;
							file.GetWord(word);
							v3.x() = atof(word.c_str());
							file.GetWord(word);
							v3.y() = atof(word.c_str());
							file.GetWord(word);
							v3.z() = atof(word.c_str());

							if(vertexMap.count(v3) == 0) {
								vertices->push_back(v3);
								normals->push_back(normal);
								vertexMap[v3] = vertices->size() - 1;
								indices.push_back(vertices->size() - 1);
							} else {
								auto index = vertexMap[v3];
								indices.push_back(index);
							}
						}
					}
					file.GetWord(word); // endloop
					file.GetWord(word); // endfacet
				}
			}
		}
	} else {
		file.Close();
		file.Open(stlFileName, true);

		char header[80];
		file.Read(header, 80);
		char buffer[4];
		unsigned int numberOfTriangles;
		file.Read(buffer, sizeof(int));

		numberOfTriangles = *((unsigned int*)&buffer);

		for(unsigned int i = 0; i < numberOfTriangles; i++) {
			float nx, ny, nz;
			float v1x, v1y, v1z;
			float v2x, v2y, v2z;
			float v3x, v3y, v3z;

			file.Read((char*)&nx, sizeof(float));
			file.Read((char*)&ny, sizeof(float));
			file.Read((char*)&nz, sizeof(float));

			file.Read((char*)&v1x, sizeof(float));
			file.Read((char*)&v1y, sizeof(float));
			file.Read((char*)&v1z, sizeof(float));

			file.Read((char*)&v2x, sizeof(float));
			file.Read((char*)&v2y, sizeof(float));
			file.Read((char*)&v2z, sizeof(float));

			file.Read((char*)&v3x, sizeof(float));
			file.Read((char*)&v3y, sizeof(float));
			file.Read((char*)&v3z, sizeof(float));

			Vec3 v1(v1x, v1y, v1z);
			Vec3 v2(v2x, v2y, v2z);
			Vec3 v3(v3x, v3y, v3z);
			Vec3 n(nx, ny, nz);

			if(vertexMap.count(v1) == 0) {
				vertices->push_back(v1);
				normals->push_back(n);
				vertexMap[v1] = vertices->size() - 1;
				indices.push_back(vertices->size() - 1);
			} else {
				auto index = vertexMap[v1];
				indices.push_back(index);
			}

			if(vertexMap.count(v2) == 0) {
				vertices->push_back(v2);
				normals->push_back(n);
				vertexMap[v2] = vertices->size() - 1;
				indices.push_back(vertices->size() - 1);
			} else {
				auto index = vertexMap[v2];
				indices.push_back(index);
			}

			if(vertexMap.count(v3) == 0) {
				vertices->push_back(v3);
				normals->push_back(n);
				vertexMap[v3] = vertices->size() - 1;
				indices.push_back(vertices->size() - 1);
			} else {
				auto index = vertexMap[v3];
				indices.push_back(index);
			}
		}
	}

	ref_ptr<Group> pGroup = new Group;
	ref_ptr<Geode> pGeode = new Geode;
	pGroup->addChild(pGeode);

	ref_ptr<Geometry> pGeometry = new Geometry;
	pGeode->addDrawable(pGeometry);

	pGeometry->setVertexArray(vertices);
	pGeometry->setNormalArray(normals);
	ref_ptr<DrawElementsUInt> ia = new DrawElementsUInt(PrimitiveSet::TRIANGLES, 0);
	for(unsigned int i = 0; i < vertices->getNumElements(); i++) {
		ia->push_back(i);
	}
	pGeometry->addPrimitiveSet(ia);

	osgUtil::SmoothingVisitor::smooth(*pGeometry);

	printf("Vertices : %d, Normals : %d, Indices : %d\n", vertices->size(), normals->size(), indices.size());

	return pGroup;
}

void Custom3DDataManager::SaveObjFile(ref_ptr<Vec3Array> pVertices, const string& objFileName, bool isBinary)
{
	Custom3DDataFile file;
	if(isBinary)
	{
	}
	else
	{
		file.Open(objFileName, false);
		file << "solid " << objFileName << endl;
		file.Close();
	}
}

void Custom3DDataManager::SaveSTLFile(ref_ptr<Vec3Array> pVertices, const string& objFileName, bool isBinary)
{
	Custom3DDataFile file;
	if(isBinary)
	{
		file.Open(objFileName, true);
		unsigned int numberOfTriangles = pVertices->getNumElements() / 3;
		char header[80] = "Header";
		file.Write((char*)header, 80);
		file.Write((char*)&numberOfTriangles, 4);

		for	(int i = 0; i < pVertices->size(); i += 3)
		{
			Vec3 v1 = pVertices->at(i);
			Vec3 v2 = pVertices->at(i + 1);
			Vec3 v3 = pVertices->at(i + 2);
			Vec3 d1 = v2 - v1;
			Vec3 d2 = v3 - v1;
			d1.normalize();
			d2.normalize();
			Vec3 n = d2 ^ d1;
			n.normalize();
			short dummy = 0;

			file.Write((char*)&n.x(), 4);
			file.Write((char*)&n.y(), 4);
			file.Write((char*)&n.z(), 4);

			file.Write((char*)&v1.x(), 4);
			file.Write((char*)&v1.y(), 4);
			file.Write((char*)&v1.z(), 4);

			file.Write((char*)&v2.x(), 4);
			file.Write((char*)&v2.y(), 4);
			file.Write((char*)&v2.z(), 4);

			file.Write((char*)&v3.x(), 4);
			file.Write((char*)&v3.y(), 4);
			file.Write((char*)&v3.z(), 4);

			file.Write((char*)&(dummy), 2);
		}

		file.Close();
	}
	else
	{
		file.Open(objFileName, false);
		file << "solid " << objFileName << endl;

		for	(int i = 0; i < pVertices->size(); i += 3)
		{
			Vec3 v1 = pVertices->at(i);
			Vec3 v2 = pVertices->at(i + 1);
			Vec3 v3 = pVertices->at(i + 2);
			Vec3 d1 = v2 - v1;
			Vec3 d2 = v3 - v1;
			d1.normalize();
			d2.normalize();
			Vec3 n = d2 ^ d1;
			n.normalize();

			file << "facet normal " << n.x() << " " << n.y() << " " << n.z() << endl;
			file << "outer loop" << endl;
			file << "vertex " << v1.x() << " " << v1.y() << " " << v1.z() << endl;
			file << "vertex " << v2.x() << " " << v2.y() << " " << v2.z() << endl;
			file << "vertex " << v3.x() << " " << v3.y() << " " << v3.z() << endl;
			file << "endloop" << endl;
			file << "endfacet" << endl;
		}

		file << "endsolid " << objFileName << endl;

		file.Close();
	}
}

void Custom3DDataManager::SaveObjFile(RAW_DATA_INFO &sRawDataInfo, const string& objFileName, bool isBinary)
{
}

void Custom3DDataManager::SaveSTLFile(RAW_DATA_INFO &sRawDataInfo, const string& objFileName, bool isBinary)
{
	Custom3DDataFile file;
	if(isBinary)
	{
	}
	else
	{
		file.Open(objFileName, false);
		file << "solid " << objFileName << endl;
		file.Close();
	}
}
