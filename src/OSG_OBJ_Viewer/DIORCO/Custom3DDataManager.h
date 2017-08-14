#pragma once

#include "./OSG_Common.h"
#include "../Defines.h"

#include "tiny_obj_loader.h"

class Custom3DDataManager {

private:
	Custom3DDataManager();
	~Custom3DDataManager();

public:
	static ref_ptr<Group> Custom3DDataManager::LoadObjFile(const string& objFileName, const string& baseDir, bool flipYZ, float scale, const string& name);
	static ref_ptr<Group> Custom3DDataManager::LoadObjFileVertexOnly(const string& objFileName, const string& baseDir, bool flipYZ, float scale, const string& name);

	static bool Custom3DDataManager::LoadObjFileVertexOnly(const string& objFileName, const string& baseDir, RAW_DATA_INFO &sRawDataInfo);
	static bool Custom3DDataManager::LoadSTLFile(const string& stlFileName, RAW_DATA_INFO &sRawDataInfo);
	static ref_ptr<Group> LoadSTLFile(const string& objFileName, bool flipYZ, float scale, const string& name);

	static void SaveObjFile(ref_ptr<Vec3Array> pVertices, const string& objFileName);
	static void SaveSTLFile(ref_ptr<Vec3Array> pVertices, const string& stlFileName, bool isBinary = false);
	static void SaveObjFile(RAW_DATA_INFO &sRawDataInfo, const string& objFileName);
	static void SaveSTLFile(RAW_DATA_INFO &sRawDataInfo, const string& stlFileName, bool isBinary = false);
};