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
	fstream oFile;
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
	cGMFObjectCSolver* cGMF::readObjectCSolver(int size);
	cGMFObjectCSolverList* cGMF::readObjectCSolverList(int size);
	cGMFObjectHingeConstraint* cGMF::readObjectHingeConstraint(int size);
	cGMFObjectWheelConstraint* cGMF::readObjectWheelConstraint(int size);
	cGMFMesh* readMesh();
	int printBytes(char* byteArray, int num);
	int printBytes(RGB in, int num);
	int printInt(int in);
	int printString(char* in);
	int printString(string in);
	int printFloat(float in);
	void saveMaterialList(cGMFMaterialList* list);
	void saveMaterial(cGMFMaterial* mat);
	void cGMF::saveObjectList(cGMFObjectList* obj);
	void cGMF::saveCSolver(cGMFObjectCSolver* csolver);
	void cGMF::saveCSolverList(cGMFObjectCSolverList* list);
	void cGMF::saveHingeConstraint(cGMFObjectHingeConstraint* c);
	void cGMF::saveWheelConstraint(cGMFObjectWheelConstraint* c);
	void cGMF::saveGeomobject(cGMFObjectGeo* o);
	void cGMF::saveMesh(cGMFMesh* m);
	void cGMF::saveAttachmentPoint(cGMFObjectAttachment* a);
	void cGMF::saveSimobject(cGMFObjectSimobject* s);
	void cGMF::saveRBCollection(cGMFObjectRBCollection* rbc);
	void cGMF::saveRBCollectionList(cGMFObjectRBCollectionList* l);
	void cGMF::saveRB(cGMFRigidBody* rb);
	void cGMF::saveCollisionList(cGMFCollisionPairList* l);
	void cGMF::saveTM(cGMFTM* tm);
public:
	void cGMF::Save(string FilePath, int Format);
	cGMF(string FilePath, int Format, int RW);
	int version, modelType;
	cGMFScene* scene;
	cGMFMaterialList* materialList;
	cGMFObjectList* objectList;
};

#endif
