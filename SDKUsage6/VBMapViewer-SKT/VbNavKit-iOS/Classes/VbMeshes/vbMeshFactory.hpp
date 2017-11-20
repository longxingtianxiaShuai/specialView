#pragma once

#include "vbMeshManager.hpp"
#include "vbMaterialManager.hpp"
#include "vbMeshGroupManager.h"

class vbMeshFactory
{
public:
	vbMeshFactory();
	~vbMeshFactory();

	
//Mesh Generator
	static void	CreateTriangle(vbMesh* pNewMesh, unsigned char flags);
	static void	CreateRectangle(vbMesh* pNewMesh, unsigned char flags, float edge_length);

    static bool ImportMeshFromSBMv1(vbMeshManager* pMshManager, vbMaterialManager* pMtlMng, FILE* fp, int mesh_count, bool bAsStatic,  char* sbm_path, vbMeshGroupManager* pGroupMng);
};
