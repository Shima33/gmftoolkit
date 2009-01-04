// Open Source C GMF Compiler
// Copyright (c) 2008 Sergiusz Bazanski
// http://www.q3k.org/
// GPL License
#include <stdio.h>
#include <string.h>
#include <memory.h>
#include <stdlib.h>

#include "byteFunctions.h"
#include "helperFunctions.h"
#include "objectFunctions.h"

extern FILE *source;
extern FILE *output;

#ifdef _BIG_ENDIAN
#define endian_swap32(x) ((((x) & 0xff000000) >> 24) | \
			  (((x) & 0x00ff0000) >>  8) | \
			  (((x) & 0x0000ff00) <<  8) | \
			  (((x) & 0x000000ff) << 24))
#else
#define endian_swap32(x) (x)
#endif

void readObjectGeoMesh(int preTabNum)
{
	printf("Decompiling \tMESH...\n");
	getBytes(8);

	int totalLength = getInteger();
	
	int meshTimeValue = getInteger();
	int meshNumVerts = getInteger();
	int meshNumFaces = getInteger();
	int meshNumTVerts = getInteger();
	int meshNumCVerts = getInteger();
	int meshMatRef = getInteger();
	
	int * normalOrder;
	normalOrder = new int[meshNumFaces*3];
	
	tab(preTabNum); fprintf(output, "*MESH\n");
	tab(preTabNum); fprintf(output, "{\n");
	
	tab(preTabNum+1); fprintf(output, "*TIMEVALUE\t%i\n", meshTimeValue);
	tab(preTabNum+1); fprintf(output, "*MESH_NUMVERTEX\t%i\n", meshNumVerts);
	tab(preTabNum+1); fprintf(output, "*MESH_NUMFACES\t%i\n", meshNumFaces);
	
	tab(preTabNum+1); fprintf(output, "*MESH_VERTEX_LIST\n");
	tab(preTabNum+1); fprintf(output, "{\n");
	int i;
	for(i = 0; i < meshNumVerts; i++)
	{
		float x = getFloat();
		float y = getFloat();
		float z = getFloat();
		tab(preTabNum+2); fprintf(output, "*MESH_VERTEX\t%i\t%f\t%f\t%f\n", i, x, y, z);
	}
	
	tab(preTabNum+1); fprintf(output, "}\n");
	tab(preTabNum+1); fprintf(output, "*MESH_FACE_LIST\n");
	tab(preTabNum+1); fprintf(output, "{\n");
	
	int mx, my, mz, mm;
	for(i = 0; i < meshNumFaces; i++)
	{
		mx = getInteger();
		my = getInteger();
		mz = getInteger();
		mm = getInteger();
		tab(preTabNum+2); fprintf(output, "*MESH_FACE\t% 4i\tA:   %i\tB:   %i\tC:   %i\t*MESH_MTLID %i\n", i, mx, my, mz, mm);
		
		normalOrder[i*3] = mx;
		normalOrder[(i*3)+1] = my;
		normalOrder[(i*3)+2] = mz;
	}
	
	tab(preTabNum+1); fprintf(output, "}\n");
	
	if (meshNumTVerts > 0)
	{
		tab(preTabNum+1); fprintf(output, "*MESH_NUMTVERTEX\t%i\n", meshNumTVerts);
		tab(preTabNum+1); fprintf(output, "*MESH_TVERTLIST\n");
		tab(preTabNum+1); fprintf(output, "{\n");
		
		for(i = 0; i < meshNumTVerts; i++)
		{
			float x = getFloat();
			float y = getFloat();
			float z = getFloat();
			tab(preTabNum+2); fprintf(output, "*MESH_TVERT\t%i\t%f\t%f\t%f\n", i, x, y, z);
		}

		tab(preTabNum+1); fprintf(output, "}\n");
		tab(preTabNum+1); fprintf(output, "*MESH_NUMTFACES\t%i\n", meshNumFaces);
		tab(preTabNum+1); fprintf(output, "*MESH_TFACELIST\n");
		tab(preTabNum+1); fprintf(output, "{\n");

		for(i = 0; i < meshNumFaces; i++)
		{
			int mx = getInteger();
			int my = getInteger();
			int mz = getInteger();
			tab(preTabNum+2); fprintf(output, "*MESH_TFACE\t%i\t%i\t%i\t%i\n", i, mx, my, mz);
		}
		tab(preTabNum+1); fprintf(output, "}\n");
	}

	int meshHasSecondMaterial = getInteger();
	
	if (meshHasSecondMaterial)
	{
		int meshHasSecondMaterialAmount = getInteger();
		for (i = 0; i < meshHasSecondMaterialAmount; i++)
		{	
			int meshChannelNo = getInteger();
			tab(preTabNum+1); fprintf(output, "*MESH_MAPPINGCHANNEL\t%i\n", meshChannelNo);
			tab(preTabNum+1); fprintf(output, "{\n");
			
			int meshNumTVerts = getInteger();
			getBytes(4);
			int meshNumTFaces = getInteger();
			getBytes(16);

			tab(preTabNum+2); fprintf(output, "*MESH_NUMTVERTEX\t%i\n", meshNumTVerts);
			tab(preTabNum+2); fprintf(output, "*MESH_TVERTLIST\n");
			tab(preTabNum+2); fprintf(output, "{\n");
			for(i = 0; i < meshNumTVerts; i++)
			{
				float x = getFloat();
				float y = getFloat();
				float z = getFloat();
				tab(preTabNum+3); fprintf(output, "*MESH_TVERT\t%i\t%f\t%f\t%f\n", i, x, y, z);
			}
			tab(preTabNum+2); fprintf(output, "}\n");

			tab(preTabNum+2); fprintf(output, "*MESH_NUMTFACES\t%i\n", meshNumTFaces);
			tab(preTabNum+2); fprintf(output, "*MESH_TFACELIST\n");
			tab(preTabNum+2); fprintf(output, "{\n");
				
			for(i = 0; i < meshNumFaces; i++)
			{
				int mx = getInteger();
				int my = getInteger();
				int mz = getInteger();
				tab(preTabNum+3); fprintf(output, "*MESH_TFACE\t%i\t%i\t%i\t%i\n", i, mx, my, mz);
			}
			tab(preTabNum+2); fprintf(output, "}\n");

			tab(preTabNum+1); fprintf(output, "}\n");
		}
	}
	if (meshNumCVerts > 0)
	{
		tab(preTabNum+1); fprintf(output, "*MESH_NUMCVERTEX\t%i\n", meshNumCVerts);
		tab(preTabNum+1); fprintf(output, "*MESH_CVERTLIST\n");
		tab(preTabNum+1); fprintf(output, "{\n");

		for(i = 0; i < meshNumCVerts; i++)
		{
			float x = getFloat();
			float y = getFloat();
			float z = getFloat();
			tab(preTabNum+2); fprintf(output, "*MESH_TVERT\t%i\t%f\t%f\t%f\n", i, x, y, z);
		}

		tab(preTabNum+1); fprintf(output, "}\n");

		tab(preTabNum+1); fprintf(output, "*MESH_CFACELIST\n");
		tab(preTabNum+1); fprintf(output, "{\n");

		for(i = 0; i < meshNumFaces; i++)
		{
			int mx = getInteger();
			int my = getInteger();
			int mz = getInteger();
			tab(preTabNum+2); fprintf(output, "*MESH_TFACE\t%i\t%i\ti\t%i\n", i, mx, my, mz);
		}
		tab(preTabNum+1); fprintf(output, "}\n");
	}
	
	if (meshNumFaces > 0)
	{
		tab(preTabNum+1); fprintf(output, "*MESH_NORMALS\n");
		tab(preTabNum+1); fprintf(output, "{\n");

		int j, f;
		for(i = 0; i < meshNumFaces; i++)
		{
			float x = getFloat();
			float y = getFloat();
			float z = getFloat();
			tab(preTabNum+2); fprintf(output, "*MESH_FACENORMAL\t%i\t%f\t%f\t%f\n", i, x, y, z);
			for (j = 0; j < 3; j++)
			{
				f = normalOrder[(i*3)+j];
				float x = getFloat();
				float y = getFloat();
				float z = getFloat();
				tab(preTabNum+3); fprintf(output, "*MESH_VERTEXNORMAL\t%i\t%f\t%f\t%f\n", f, x, y, z);				
			}
		}
		tab(preTabNum+1); fprintf(output, "}\n");
	}
	
	int meshBackFaceCull = getInteger();
	tab(preTabNum+1); fprintf(output, "*BACKFACE_CULL\t%i\n", meshBackFaceCull);
	tab(preTabNum+1); fprintf(output, "*MATERIAL_REF\t%i\n", meshMatRef);

	tab(preTabNum); fprintf(output, "}\n");
	delete normalOrder;
}

void readObjectGeo(int preTabNum)
{
	tab(preTabNum); fprintf(output, "*GEOMOBJECT\n");
	tab(preTabNum); fprintf(output, "{\n");

	getBytes(8);

	int totalLength = getInteger();	

	if (getBytesNF(6)[5] != '\x00')
	{
		char* objName = getString();
		tab(preTabNum+1); fprintf(output, "*NODE_NAME\t%s\n", objName);	
		printf("Decompiling GEOMOBJECT %s...\n", objName);
		//free(objName);
	}
	else
	{
		getBytes(4);
		tab(preTabNum+1); fprintf(output, "*NODE_NAME\t(null)\n");
		printf("Decompiling GEOMOBJECT...\n");
	}

	if (getBytesNF(1)[0] != '\x00')
	{
		char* objParent = getString();
		tab(preTabNum+1); fprintf(output, "*NODE_PARENT\t%s\n", objParent);
		//free(objParent);
	}

	char* temp = getBytes(1);
	int objShadeVerts = temp[0];
	//free(temp);

	tab(preTabNum+1); fprintf(output, "*NODE_SHADEVERTS\t%i\n", objShadeVerts);
	
	readObjectNodeTM(preTabNum + 1);
	readObjectGeoMesh(preTabNum + 1);
	
	tab(preTabNum); fprintf(output, "}\n");
	printf("\n");

}
