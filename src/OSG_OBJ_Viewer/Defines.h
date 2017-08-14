#pragma once

#include "OSG_Common.h"

using namespace std;
using namespace osg;

struct RAW_DATA_INFO
{
	bool bSetInfo;
	int iVertexNum;
	Vec3 * pVertexPtr;
	int iIndicesNum;
	int * pIndicesPtr;
};
