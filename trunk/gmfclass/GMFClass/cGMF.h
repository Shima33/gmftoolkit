#ifndef CGMFH
#define CGMFH

#include <string>
#include <fstream>
#include "cGMFTypes.h"

#define DllExport extern "C" __declspec (dllexport)

using namespace std;

class __declspec (dllexport) cGMF {
private:
	string cPath;
	fstream cFile;
	char* getBytes(int NumberOfBytes);
	char* getBytesNF(int NumberOfBytes);
	int getInteger();
	float getFloat();
	char* getString();
	int readMain(int id, int flag, int size);
	int readSceneInfo(int size);
	cGMFMaterialList* readMaterialList(int size);
	cGMFMaterial* readMaterial(int size);
	cGMFTextureList* readTextureList(int size);
	cGMFTexture* readTexture(int size);
	cGMFTM* readTM();
	cGMFObjectList* readObjectList(int size);
	cGMFObjectGeo* readObjectGeo(int size);
	cGMFObjectAttachment* readObjectAttachment(int size);
	cGMFObjectSimobject* cGMF::readObjectSimobject(int size);
	cGMFObjectRBCollection* cGMF::readObjectRBCollection(int size);
	cGMFObjectRBCollectionList* cGMF::readObjectRBCollectionList(int size);
	cGMFCollisionPairList* cGMF::readCollisionPairList(int size);
	cGMFMesh* readMesh();
public:
	cGMF(string FilePath, int Format, int RW);
	int version, modelType;
	cGMFScene* scene;
	cGMFMaterialList* materialList;
	cGMFObjectList* objectList;
};

#endif
