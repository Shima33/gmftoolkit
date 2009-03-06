#include <windows.h>
#include <commctrl.h>
#include <string.h>
#include <stdio.h>
#include "cGMF.h"
#include "populate.h"
extern TV_INSERTSTRUCT tvi;
extern HWND hwndTV;
extern char* buf;
extern cGMF* gmf;

TVITEM HTREEITEMToTVITEM(HTREEITEM item)
{
	TVITEM ret;
	memset(&ret,0,sizeof(ret));
	ret.hItem = item;
	SendMessage(hwndTV, TVM_GETITEM, TVGN_CARET,(LPARAM)&ret);
	return ret;
}

/*void* getPointer(HTREEITEM item)
{
	return (void*)HTREEITEMToTVITEM(item).LOWORD(lParam);
}*/

char *RGBToC(RGB color)
{
	char* ret = (char*)malloc(sizeof(char)*20);
	sprintf(ret, "%02X%02X%02X", color.red, color.green, color.blue);
	return ret;
}

HTREEITEM Add()
{
	return (HTREEITEM)SendMessage(hwndTV, TVM_INSERTITEM, 0, (LPARAM)(LPTVINSERTSTRUCT)&tvi);
}

HTREEITEM AddLabel(char* label, void* structure, int type)
{
	tvi.item.pszText = label;
	Uni *uni = new Uni();
	uni->object = structure;
	uni->type = type;
	uni->label = label;
	tvi.item.lParam = (LPARAM)uni;
	return Add();	
}

HTREEITEM AddRGB(char* label, RGB* property)
{
	sprintf(buf, "%s: #%s", label, RGBToC(*property));
	tvi.item.pszText = buf;
	Uni *uni = new Uni();
	uni->object = property;
	uni->type = 4;
	uni->label = label;
	tvi.item.lParam = (LPARAM)uni;
	return Add();	
}

HTREEITEM AddInt(char* label, int* property)
{
	sprintf(buf, "%s: %i", label, *property);
	tvi.item.pszText = buf;
	//tvi.item.lParam = MAKELPARAM(property, 1);
	Uni *uni = new Uni();
	uni->object = property;
	uni->label = label;
	uni->type = 1;
	tvi.item.lParam = (LPARAM)uni;
	return Add();
}

HTREEITEM AddFloat(char* label, float* property)
{
	sprintf(buf, "%s: %f", label, *property);
	tvi.item.pszText = buf;
	Uni *uni = new Uni();
	uni->object = property;
	uni->type = 2;
	uni->label = label;
	tvi.item.lParam = (LPARAM)uni;
	return Add();
}

HTREEITEM AddString(char* label, string* property)
{
	tvi.item.pszText = StrToC((string)label + ": " +  *property);
	Uni *uni = new Uni();
	uni->object = property;
	uni->label = label;
	uni->type = 3;
	tvi.item.lParam = (LPARAM)uni;
	return Add();
}

void ReadTM(cGMFTM* tm)
{
	HTREEITEM oldparent = tvi.hParent;
	tvi.hParent = AddLabel("Transformation Matrix", tm, TM);
	AddString("Name", &tm->name);
	
	sprintf(buf, "Row 0: %f %f %f", tm->matrix[0][0], tm->matrix[0][1], tm->matrix[0][2], tm->matrix[0][3]);
	AddLabel(buf, tm, TMROW);
	sprintf(buf, "Row 1: %f %f %f", tm->matrix[1][0], tm->matrix[1][1], tm->matrix[1][2], tm->matrix[1][3]);
	AddLabel(buf, tm, TMROW);
	sprintf(buf, "Row 2: %f %f %f", tm->matrix[2][0], tm->matrix[2][1], tm->matrix[2][2], tm->matrix[2][3]);
	AddLabel(buf, tm, TMROW);
	sprintf(buf, "Row 3: %f %f %f", tm->matrix[3][0], tm->matrix[3][1], tm->matrix[3][2], tm->matrix[3][3]);
	AddLabel(buf, tm, TMROW);
	
	tvi.hParent = oldparent;
}

void ReadMesh(cGMFMesh* mesh)
{
	HTREEITEM OldParent = tvi.hParent;
	AddInt("TimeValue", &mesh->timeValue);
	AddInt("Number of Vertices", &mesh->numVerts);
	AddInt("Number of CVertices", &mesh->numCVerts);
	AddInt("Number of TVertices", &mesh->numTVerts);
	AddInt("Number of Faces", &mesh->numFaces);
	
	tvi.hParent = AddLabel("Vertex List", &mesh->tverts, 0);
	int i;
	for (i = 0; i < mesh->numVerts; i++)
	{
		sprintf(buf, "%3i.   X: %10f   Y: %10f   Z: %10f", i, mesh->verts[i]->vert[0], mesh->verts[i]->vert[1], mesh->verts[i]->vert[2]);
		AddLabel(buf, mesh->verts[i], 0);
	}
	
	if(mesh->numTVerts>0)
	{
		tvi.hParent = OldParent;
		tvi.hParent = AddLabel("TVertex List", &mesh->tverts, 0);
		for (i = 0; i < mesh->numTVerts; i++)
		{
			sprintf(buf, "%3i.   X: %10f   Y: %10f   Z: %10f", i, mesh->tverts[i]->vert[0], mesh->tverts[i]->vert[1], mesh->tverts[i]->vert[2]);
			AddLabel(buf, mesh->tverts[i], 0);
		}
		tvi.hParent = OldParent;
		tvi.hParent = AddLabel("TFace List", &mesh->tfaces, 0);
		for (i = 0; i < mesh->numFaces; i++)
		{
			sprintf(buf, "%3i.   A: %10i   B: %10i   C: %10i", i, mesh->tfaces[i]->vertex[0], mesh->tfaces[i]->vertex[1], mesh->tfaces[i]->vertex[2]);
			AddLabel(buf, mesh->tfaces[i], 0);
		}
	}
	
	tvi.hParent = OldParent;
	tvi.hParent = AddLabel("Face List", &mesh->faces, 0);
	for (i = 0; i < mesh->numFaces; i++)
	{
		sprintf(buf, "%3i.   A: %10i   B: %10i   C: %10i   Material ID: %3i", i, mesh->faces[i]->vertex[0], mesh->faces[i]->vertex[1], mesh->faces[i]->vertex[2], mesh->faces[i]->matlid);
		AddLabel(buf, mesh->verts[i], 0);
	}
	
	tvi.hParent = OldParent;
	HTREEITEM Normals = AddLabel("Normals List", &mesh->normals, 0);	
	int j;
	for (i = 0; i < mesh->numFaces; i++)
	{
		tvi.hParent = Normals;
		sprintf(buf, "%3i   %10f   %10f   %10f", i, mesh->normals[i]->crd[0], mesh->normals[i]->crd[1], mesh->normals[i]->crd[2]);
		tvi.hParent = AddLabel(buf, mesh->normals[i], 0);
		for(j = 0; j < 3; j++)
		{
			sprintf(buf, "%3i   %10f   %10f   %10f", mesh->normals[i]->vnormals[j]->i, mesh->normals[i]->vnormals[j]->crd[0], mesh->normals[i]->vnormals[j]->crd[1], mesh->normals[i]->vnormals[j]->crd[2]);
			AddLabel(buf, mesh->normals[i]->vnormals[j], 0);
		}
	}
	
	if(mesh->secondMaterial)
	{
		tvi.hParent = OldParent;
		tvi.hParent = AddLabel("Mapping Channel", NULL, 0);
		for (i = 0; i < mesh->secondMaterialAmount; i++)
		{
			HTREEITEM Channel = AddInt("Mapping Channel", &mesh->mappingChannel[i]->number);
			tvi.hParent = Channel;
			tvi.hParent = AddLabel("TVertex List", mesh->mappingChannel[i]->tverts, 0);
			for(j = 0; j < mesh->mappingChannel[i]->numTVerts; j++)
			{
				sprintf(buf, "%3i.   X: %10f   Y: %10f   Z: %10f", j, mesh->mappingChannel[i]->tverts[j]->vert[0], mesh->mappingChannel[i]->tverts[j]->vert[1], mesh->mappingChannel[i]->tverts[j]->vert[2]);
				AddLabel(buf, mesh->mappingChannel[i]->tverts[j], 0);
			}
			tvi.hParent = Channel;
			tvi.hParent = AddLabel("TFace List", &mesh->tfaces, 0);
			for (j = 0; j < mesh->mappingChannel[i]->numTFaces; j++)
			{
				sprintf(buf, "%3i.   A: %10i   B: %10i   C: %10i", j, mesh->mappingChannel[i]->tfaces[j]->vertex[0], mesh->mappingChannel[i]->tfaces[j]->vertex[1], mesh->mappingChannel[i]->tfaces[j]->vertex[2]);
				AddLabel(buf, mesh->mappingChannel[i]->tfaces[j], 0);
			}
		}
	}
	if(mesh->numCVerts > 0)
	{
		tvi.hParent = OldParent;
		tvi.hParent = AddLabel("Color Vertex List", NULL, 0);
		for(i = 0; i < mesh->numCVerts; i++)
		{
			sprintf(buf, "%3i.   X: %10f   Y: %10f   Z: %10f", i, mesh->cverts[i]->vert[0], mesh->cverts[i]->vert[1], mesh->cverts[i]->vert[2]);
			AddLabel(buf, mesh->cverts[i], 0);
		}
		tvi.hParent = OldParent;
		/*tvi.hParent = AddLabel("Color Face List", NULL, 0);
		for(i = 0; i < mesh->numFaces; i++)
		{
			sprintf(buf, "%3i.   A: %10i   B: %10i   C: %10i", i, mesh->cfaces[i]->vertex[0], mesh->cfaces[i]->vertex[1], mesh->cfaces[i]->vertex[2]);
			AddLabel(buf, mesh->cfaces[i], 0);
		}*/
	}
	tvi.hParent = OldParent;
	AddInt("Backface Cull", &mesh->backFace);
	AddInt("Material Reference", &mesh->matRef);
}

HTREEITEM ReadTextures(HTREEITEM parent, cGMFTextureList* list)
{
	int j;
	for (j = 0; j < list->number; j++)
	{
		tvi.hParent = parent;
		tvi.hParent = AddLabel(StrToC(list->textures[j]->name), NULL, 0); 
		AddString("Name", &list->textures[j]->name);
		AddString("Class", &list->textures[j]->classname);
		AddString("Bitmap", &list->textures[j]->bitmap);
		AddFloat("Amount", &list->textures[j]->amount);
		
		switch (list->textures[j]->style == 1)
		{
			case 5:
				AddInt("Style (selfillum)", &list->textures[j]->style);
				break;
			case 6:
				AddInt("Style (opacity)", &list->textures[j]->style);
				break;
			case 1:
			default:
				AddInt("Style (diffuse)", &list->textures[j]->style);
				break;
		}
		if (list->textures[j]->type == 4)
			AddInt("Type (screen)", &list->textures[j]->type);
		else
			AddInt("Type (other)", &list->textures[j]->type);
	
		HTREEITEM Temp = tvi.hParent;
		tvi.hParent = AddLabel("UVW Data", NULL, 0);
		AddFloat("UVW U Offset", &list->textures[j]->UO);
		AddFloat("UVW V Offset", &list->textures[j]->VO);
		AddFloat("UVW U Tiling", &list->textures[j]->UT);
		AddFloat("UVW V Tiling", &list->textures[j]->VT);
		AddFloat("UVW Angle", &list->textures[j]->UVWAngle);
		AddFloat("UVW Blur", &list->textures[j]->UVWBlur);
		AddFloat("UVW Blur Offset", &list->textures[j]->UVWBlurOffset);
		AddFloat("UVW Noise Amount", &list->textures[j]->UVWNoiseAmount);
		AddFloat("UVW Noise Size", &list->textures[j]->UVWNoiseSize);
		AddInt("UVW Noise Level", &list->textures[j]->UVWNoiseLevel);
		AddFloat("UVW Noise Phase", &list->textures[j]->UVWNoisePhase);
		tvi.hParent = Temp;
		
		if (list->textures[j]->bitmapFilter == 0)
			AddInt("Bitmap Filter (pyramidal)", &list->textures[j]->bitmapFilter);
		else
			AddInt("Bitmap Filter (SAT)", &list->textures[j]->bitmapFilter);
			
		AddInt("Bitmap Channel", &list->textures[j]->bitmapChannel);
	}
}

HTREEITEM ReadMaterials(HTREEITEM parent, cGMFMaterialList* list)
{
	int i;
	for (i = 0; i < list->number; i++)
	{
		tvi.hParent = parent;
		
		HTREEITEM Parent = AddString("Material", &list->materials[i]->name);
		
		tvi.hParent = Parent;
		
		AddInt("Ref no", &list->materials[i]->ref);
		AddString("Name", &list->materials[i]->name);
		AddString("Class", &list->materials[i]->classname);
		AddRGB("Ambient", &list->materials[i]->ambient);
		AddRGB("Diffuse", &list->materials[i]->diffuse);
		AddRGB("Specular", &list->materials[i]->specular);
		AddFloat("Shine", &list->materials[i]->shine);
		AddFloat("Shine Strength", &list->materials[i]->shineStrength);
		AddFloat("Transparency", &list->materials[i]->transparency);
		AddFloat("Wire Size", &list->materials[i]->wiresize);
		
		if(list->materials[i]->texNum > 0)
		{
			HTREEITEM Textures = AddLabel("Textures", &list->materials[i]->textureList, 0);
			ReadTextures(Textures, list->materials[i]->textureList);	
		}
		if(list->materials[i]->matNum > 0)
		{
			HTREEITEM Materials = AddLabel("Materials", &list->materials[i]->materialList, 0);
			ReadMaterials(Materials, list->materials[i]->materialList);
		}
	}
}
