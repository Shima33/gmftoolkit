#ifndef cGMFSceneh
#define cGMFSceneh

#define GMID_HAVOK_RBCOLLECTION 312
#define GEOMOBJECT 24
#define GMID_ATTACHMENTPT 212
#define GMID_HAVOK_SIMOBJECT 702

#include "sRGB.h"
#include <string>
using namespace std;
class cGMFScene {
public:
	string name;
	unsigned int firstFrame, lastFrame, frameSpeed, ticksPerFrame;
	RGB backgroundStatic, ambientStatic;
};

class cGMFMaterialList;
class cGMFTextureList;

class cGMFMaterial {
public:
	string name, classname;
	int ref, shading, fallof, XPType, texNum, matNum;
	RGB ambient, diffuse, specular;
	float shine, shineStrength, transparency, wiresize, XPFallof, selfIllum;
	cGMFMaterialList* materialList;
	cGMFTextureList* textureList;
};

class cGMFMaterialList {
public:
	int number;
	cGMFMaterial** materials;
};

class cGMFTexture {
public:
	string name, classname, bitmap;
	float amount;
	int style, type, UVWNoiseLevel, bitmapFilter, bitmapChannel;
	float UO, VO, UT, VT, UVWAngle, UVWBlur, UVWBlurOffset, UVWNoiseAmount, UVWNoiseSize, UVWNoisePhase;
};

class cGMFTextureList {
public:
	int number;
	cGMFTexture** textures;
};

class cGMFTM {
public:
	string name;
	float matrix[4][3];
};

class cGMFFace {
public:
	int vertex[3];
	int matlid;
};

class cGMFVertexNormal {
public:
	int i;
	float crd[3];
};

class cGMFFaceNormal {
public:
	int i;
	float crd[3];
	cGMFVertexNormal** vnormals;
};

class cGMFVertex {
public:
	float vert[3];
};

class cGMFMeshMappingChannel {
public:
	int number, numTVerts, numTFaces;
	cGMFVertex** tverts;
	cGMFFace** tfaces;
};

class cGMFMesh {
public:
	int timeValue, numVerts, numFaces, numTVerts, numCVerts, backFace, matRef, secondMaterial, secondMaterialAmount;
	cGMFVertex** verts;
	cGMFVertex** tverts;
	cGMFVertex** cverts;
	cGMFFace** faces;
	cGMFFace** tfaces;
	cGMFFaceNormal** normals;
	cGMFMeshMappingChannel** mappingChannel;
};

class cGMFObject {
public:
	string name;
	cGMFTM* tm;
	virtual int getType() {
		return -1;
	}
};

class cGMFCollisionPair {
public:
	string body1, body2;	
};

class cGMFCollisionPairList {
public:
	int count;
	cGMFCollisionPair** cpair;	
};

class cGMFRigidBody {
public:
	string name, geomProxy, dispProxy;
	float mass, elasticity, friction, optimisation;
	int unyielding, simulationGeo, useDisplayProxy, disableCollisions, inactive, geoType, childrenNumber;
	cGMFTM* tm;
};

class cGMFObjectRBCollectionList {
public:
	int rbCount;
	cGMFRigidBody** rbs;
};

class cGMFObjectRBCollection : public cGMFObject {
public:
	int numDisabledPairs, solverType;
	int hasRBList, hasDisabledCollisionsList;
	cGMFObjectRBCollectionList* rblist;
	cGMFCollisionPairList* cplist;
	virtual int getType() {
		return 312;
	}
};

class cGMFObjectGeo : public cGMFObject {
public:
	string parent;
	int shadeVerts;
	cGMFMesh* mesh;
	virtual int getType() {
		return 24;
	}
};

class cGMFObjectAttachment : public cGMFObject {
public:
	string userData;
	virtual int getType() {
		return 212;
	}
};

class cGMFObjectSimobject : public cGMFObject {
public:
	float x, y, z, worldScale, tolerance, linearDrag, angularDrag, shortFreq, longFreq, updatesPerTimeStep;
	int resolver, includeDrag, includeDeactivator, useFastSubspace, collisionPairs;
	virtual int getType() {
		return 702;
	}
};

class cGMFObjectList {
public:
	int number;
	cGMFObject** objects;
};

#endif
