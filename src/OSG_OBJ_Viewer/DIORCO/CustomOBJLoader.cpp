#include "stdafx.h"
#include "CustomOBJLoader.h"

namespace DIORCO {

CustomOBJLoader::CustomOBJLoader()
{
}

CustomOBJLoader::~CustomOBJLoader()
{
}

osg::ref_ptr<osg::MatrixTransform> CustomOBJLoader::LoadObjFile(const std::string& objFileName, const std::string& baseDir, bool flipYZ, float scale)
{
	std::string baseDirectory = baseDir;
	if(baseDirectory[baseDirectory.size() - 1] != '\\' || baseDirectory[baseDirectory.size() - 1] != '/') {
		baseDirectory += '\\';
	}

	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::map<std::string, GLuint> textures;
 
	DIORCO::TimeWatch tm_parsing;
	DIORCO::TimeWatch tm_loading;
	tm_parsing.start();
	tm_loading.start();
 
	std::string err;
	bool ret =
		tinyobj::LoadObj(&attrib, &shapes, &materials, &err, objFileName.c_str(), baseDirectory.c_str(), true);
	if (!err.empty()) {
		std::cerr << err << std::endl;
	}
	tm_parsing.end();
 
	if (!ret) {
		std::cerr << "Failed to load " << objFileName << std::endl;
		return nullptr;
	}
 
	printf("CustomLoader Parsing time: %d [ms]\n", (int)tm_parsing.msec());


	//osg::ref_ptr<osg::Vec3Array> va = new osg::Vec3Array;
	//int size = attrib.vertices.size();
	//for(int i = 0; i < size; i += 3) {
	//	va->push_back(osg::Vec3(attrib.vertices[i], attrib.vertices[i + 1], attrib.vertices[i + 2]));
	//}

	//osg::ref_ptr<osg::Vec3Array> na = new osg::Vec3Array;
	//size = attrib.normals.size();
	//for(int i = 0; i < size; i += 3) {
	//	na->push_back(osg::Vec3(attrib.normals[i], attrib.normals[i + 1], attrib.normals[i + 2]));
	//}

	//osg::ref_ptr<osg::Vec2Array> ta = new osg::Vec2Array;
	//size = attrib.texcoords.size();
	//for(int i = 0; i < size; i += 2) {
	//	ta->push_back(osg::Vec2(attrib.texcoords[i], attrib.texcoords[i + 1]));
	//}

	std::vector<osg::ref_ptr<osg::Material>> osgMaterials;
	std::vector<osg::ref_ptr<osg::Texture2D>> osgTextures;
	std::vector<osg::ref_ptr<osg::Vec3Array>> osgVertices;
	std::vector<osg::ref_ptr<osg::Vec3Array>> osgNormals;
	std::vector<osg::ref_ptr<osg::Vec2Array>> osgTexCoords;

	if(materials.size() > 0) {
		for (unsigned int materialIndex = 0; materialIndex < materials.size(); materialIndex++) {
			auto material = materials[materialIndex];

			osg::ref_ptr<osg::Material> osgMaterial = new osg::Material;
			osgMaterial->setColorMode(osg::Material::DIFFUSE);
			osgMaterial->setAmbient(osg::Material::FRONT_AND_BACK, osg::Vec4(material.ambient[0], material.ambient[1], material.ambient[2], 1));
			osgMaterial->setDiffuse(osg::Material::FRONT_AND_BACK, osg::Vec4(material.diffuse[0], material.diffuse[1], material.diffuse[2], 1));
			osgMaterial->setSpecular(osg::Material::FRONT_AND_BACK, osg::Vec4(material.specular[0], material.specular[1], material.specular[2], 1));
			osgMaterial->setShininess(osg::Material::FRONT_AND_BACK, material.shininess);

			if(material.diffuse_texname.size() > 0) {
				auto textureFileName = baseDirectory + material.diffuse_texname;
				osg::ref_ptr<osg::Texture2D> map_Kd = new osg::Texture2D(osgDB::readImageFile(textureFileName.c_str()));
				//map_Kd->setInternalFormatMode(osg::Texture2D::USE_S3TC_DXT1_COMPRESSION);
				//map_Kd->setUnRefImageDataAfterApply(true);
				osgTextures.push_back(map_Kd);

				osgMaterial->setDiffuse(osg::Material::FRONT_AND_BACK, osg::Vec4(1, 1, 1, 1));
			} else {
				osgTextures.push_back(nullptr);
			}
			osgMaterials.push_back(osgMaterial);

			osgVertices.push_back(new osg::Vec3Array);
			osgNormals.push_back(new osg::Vec3Array);
			osgTexCoords.push_back(new osg::Vec2Array);
		}
	}

	osg::ref_ptr<osg::MatrixTransform> matrixTransform = new osg::MatrixTransform();

	// Loop over shapes
	for (size_t s = 0; s < shapes.size(); s++) {
		// Loop over faces(polygon)
		size_t index_offset = 0;
		for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
			int fv = shapes[s].mesh.num_face_vertices[f];
			// per-face material
			auto material = shapes[s].mesh.material_ids[f];

			// Loop over vertices in the face.
			for (int v = 0; v < fv; v++) {
				// access to vertex
				tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
				if(attrib.vertices.size() > 0) {
					auto vx = attrib.vertices[3*idx.vertex_index+0] * scale;
					auto vy = attrib.vertices[3*idx.vertex_index+1] * scale;
					auto vz = attrib.vertices[3*idx.vertex_index+2] * scale;
					if(flipYZ) {
						osgVertices[material]->push_back(osg::Vec3(vx, vz, vy));
					} else {
						osgVertices[material]->push_back(osg::Vec3(vx, vy, vz));
					}
				}
				if(attrib.normals.size() > 0) {
					auto nx = attrib.normals[3*idx.normal_index+0];
					auto ny = attrib.normals[3*idx.normal_index+1];
					auto nz = attrib.normals[3*idx.normal_index+2];
					osgNormals[material]->push_back(osg::Vec3(nx, ny, nz));
				}
				if(attrib.texcoords.size() > 0) {
					auto tu = attrib.texcoords[2*idx.texcoord_index+0];
					auto tv = attrib.texcoords[2*idx.texcoord_index+1];
					osgTexCoords[material]->push_back(osg::Vec2(tu, tv));
				}
			}
			index_offset += fv;
		}
	}

	for (unsigned int materialIndex = 0; materialIndex < materials.size(); materialIndex++) {
		osg::ref_ptr<osg::Geode> geode = new osg::Geode;
		matrixTransform->addChild(geode);
		auto ss = geode->getOrCreateStateSet();
		ss->setMode(GL_LIGHTING, osg::StateAttribute::OFF);

		ss->setAttributeAndModes(osgMaterials[materialIndex].get(), osg::StateAttribute::ON);

		if(materials[materialIndex].diffuse_texname.size() > 0) {
			if(osgTextures[materialIndex]) {
				ss->setTextureAttributeAndModes(0, osgTextures[materialIndex].get(), osg::StateAttribute::ON);
			}
		}

		osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry;
		geode->addDrawable(geometry);

		geometry->setVertexArray(osgVertices[materialIndex]);
		geometry->setNormalArray(osgNormals[materialIndex]);
		geometry->setTexCoordArray(0, osgTexCoords[materialIndex]);
		osg::ref_ptr<osg::DrawElementsUInt> ia = new osg::DrawElementsUInt(osg::PrimitiveSet::TRIANGLES, 0);
		for(unsigned int i = 0; i < osgVertices[materialIndex]->size(); i++) {
			ia->push_back(i);
		}
		geometry->addPrimitiveSet(ia);
	}




	//for(unsigned int shapeIndex = 0; shapeIndex < shapes.size(); shapeIndex++) {
	//	auto shape = shapes[shapeIndex];
	//	auto mesh = shape.mesh;
	//	if(materials.size() > 0) {
	//		for (unsigned int materialIndex = 0; materialIndex < materials.size(); materialIndex++) {
	//			osg::ref_ptr<osg::Geode> geode = new osg::Geode;
	//			auto ss = geode->getOrCreateStateSet();
	//			ss->setMode(GL_LIGHTING, osg::StateAttribute::OFF);

	//			ss->setAttributeAndModes(osgMaterials[materialIndex].get(), osg::StateAttribute::ON);

	//			if(materials[materialIndex].diffuse_texname.size() > 0) {
	//				if(osgTextures[materialIndex]) {
	//					ss->setTextureAttributeAndModes(0, osgTextures[materialIndex].get(), osg::StateAttribute::ON);
	//				}
	//			}

	//			osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry;

	//			osg::ref_ptr<osg::DrawElementsUInt> ia = new osg::DrawElementsUInt(osg::PrimitiveSet::TRIANGLES, 0);
	//			for(unsigned int faceIndex = 0; faceIndex < mesh.num_face_vertices.size(); faceIndex++) {
	//				if(materialIndex == mesh.material_ids[faceIndex]) {
	//					auto i1 = shape.mesh.indices[faceIndex * 3 + 0];
	//					auto i2 = shape.mesh.indices[faceIndex * 3 + 1];
	//					auto i3 = shape.mesh.indices[faceIndex * 3 + 2];

	//					osg::ref_ptr<osg::Vec3Array> va = new osg::Vec3Array;
	//					va->push_back(osg::Vec3(attrib.vertices[i1.vertex_index * 3], attrib.vertices[i1.vertex_index * 3 + 1], attrib.vertices[i1.vertex_index * 3 + 2]));
	//					va->push_back(osg::Vec3(attrib.vertices[i2.vertex_index * 3], attrib.vertices[i2.vertex_index * 3 + 1], attrib.vertices[i2.vertex_index * 3 + 2]));
	//					va->push_back(osg::Vec3(attrib.vertices[i3.vertex_index * 3], attrib.vertices[i3.vertex_index * 3 + 1], attrib.vertices[i3.vertex_index * 3 + 2]));
	//					geometry->setVertexArray(va);

	//					osg::ref_ptr<osg::Vec3Array> na = new osg::Vec3Array;
	//					na->push_back(osg::Vec3(attrib.normals[i1.normal_index * 3], attrib.normals[i1.normal_index * 3 + 1], attrib.normals[i1.normal_index * 3 + 2]));
	//					na->push_back(osg::Vec3(attrib.normals[i2.normal_index * 3], attrib.normals[i2.normal_index * 3 + 1], attrib.normals[i2.normal_index * 3 + 2]));
	//					na->push_back(osg::Vec3(attrib.normals[i3.normal_index * 3], attrib.normals[i3.normal_index * 3 + 1], attrib.normals[i3.normal_index * 3 + 2]));
	//					geometry->setNormalArray(na);

	//					osg::ref_ptr<osg::Vec2Array> ta = new osg::Vec2Array;
	//					va->push_back(osg::Vec3(attrib.vertices[i1.vertex_index * 3], attrib.vertices[i1.vertex_index * 3 + 1], attrib.vertices[i1.vertex_index * 3 + 2]));
	//					va->push_back(osg::Vec3(attrib.vertices[i2.vertex_index * 3], attrib.vertices[i2.vertex_index * 3 + 1], attrib.vertices[i2.vertex_index * 3 + 2]));
	//					va->push_back(osg::Vec3(attrib.vertices[i3.vertex_index * 3], attrib.vertices[i3.vertex_index * 3 + 1], attrib.vertices[i3.vertex_index * 3 + 2]));
	//					geometry->setVertexArray(va);

	//					ia->push_back(shape.mesh.indices[faceIndex * 3].vertex_index);
	//					ia->push_back(shape.mesh.indices[faceIndex * 3 + 1].vertex_index);
	//					ia->push_back(shape.mesh.indices[faceIndex * 3 + 2].vertex_index);
	//					geometry->addPrimitiveSet(ia);
	//				}
	//			}

	//			geode->addDrawable(geometry);
	//			group->addChild(geode);
	//		}
	//	} else {
	//		/*osg::ref_ptr<osg::Geode> geode = new osg::Geode;
	//		geode->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
	//		osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry;

	//		geometry->setVertexArray(va);
	//		geometry->setNormalArray(na);
	//		geometry->setTexCoordArray(0, ta);

	//		osg::ref_ptr<osg::DrawElementsUInt> indexArray = new osg::DrawElementsUInt(osg::PrimitiveSet::TRIANGLES, 0);
	//		for(unsigned int i = 0; i < shape.mesh.indices.size(); i++) {
	//			indexArray->push_back(shape.mesh.indices[i].vertex_index);
	//		}

	//		geometry->addPrimitiveSet(indexArray);

	//		geode->addDrawable(geometry);
	//		group->addChild(geode);*/
	//	}
	//}



	tm_loading.end();

	printf("CustomLoader Loading time: %d [ms]\n", (int)tm_loading.msec());


 
	return matrixTransform;
}


}