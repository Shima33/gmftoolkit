#include "cGMF.h"
#include "cGMFTypes.h"
#include <iostream>

cGMFTM* cGMF::readTM()
{
	getBytes(12);
	cGMFTM* tm = new cGMFTM();
	tm->name = getString();
	int i, j;
	for (i = 0; i < 4; i++)
		for (j = 0; j < 3; j++)
			tm->matrix[i][j] = getFloat();

	return tm;
}

cGMFObjectList* cGMF::readObjectList(int size)
{
	cGMFObjectList* list = new cGMFObjectList();
	list->number = getInteger();
	list->objects = new cGMFObject *[list->number];

	int i;
	for (i = 0; i < list->number; i++)
	{
		int id, flags, size;
		id = getInteger();
		flags = getInteger();
		size = getInteger();

		if (id == 2 && flags == 4)
			list->objects[i] = readObjectGeo(size);
		else if (id == 21 && flags == 2)
			list->objects[i] = readObjectAttachment(size);
		else if (id == 30 && flags == 2)
			list->objects[i] = readObjectSimobject(size);
		else if (id == 31 && flags == 4)
			list->objects[i] = readObjectRBCollection(size);
		else
		{
			cout << "Unknown Object Type! Skipped (this is normal, do not worry) id: " << id << " flags: " << flags << " size: " << size << endl;
			getBytes(size);
		}
	}

	return list;
}

cGMFMesh* cGMF::readMesh()
{
	getBytes(12);
	cGMFMesh* mesh = new cGMFMesh();

	mesh->timeValue = getInteger();
	mesh->numVerts = getInteger();
	mesh->numFaces = getInteger();
	mesh->numTVerts = getInteger();
	mesh->numCVerts = getInteger();
	mesh->matRef = getInteger();

	mesh->verts = new cGMFVertex *[mesh->numVerts];
	mesh->faces = new cGMFFace *[mesh->numFaces];

	int i;
	for (i = 0; i < mesh->numVerts; i++)
	{
		mesh->verts[i] = new cGMFVertex();
		mesh->verts[i]->vert[0] = getFloat();
		mesh->verts[i]->vert[1] = getFloat();
		mesh->verts[i]->vert[2] = getFloat();
	}

	for (i = 0; i <  mesh->numFaces; i++)
	{
		mesh->faces[i] = new cGMFFace();
		mesh->faces[i]->vertex[0] = getInteger();
		mesh->faces[i]->vertex[1] = getInteger();
		mesh->faces[i]->vertex[2] = getInteger();
		mesh->faces[i]->matlid = getInteger();
	}

	if (mesh->numTVerts > 0)
	{
		mesh->tverts = new cGMFVertex *[mesh->numTVerts];
		mesh->tfaces = new cGMFFace *[mesh->numFaces];

		for (i = 0; i < mesh->numTVerts; i++)
		{
			mesh->tverts[i] = new cGMFVertex();
			mesh->tverts[i]->vert[0] = getFloat();
			mesh->tverts[i]->vert[1] = getFloat();
			mesh->tverts[i]->vert[2] = getFloat();
		}

		for (i = 0; i < mesh->numFaces; i++)
		{
			mesh->tfaces[i] = new cGMFFace();
			mesh->tfaces[i]->vertex[0] = getInteger();
			mesh->tfaces[i]->vertex[1] = getInteger();
			mesh->tfaces[i]->vertex[2] = getInteger();
		}
	}

	mesh->secondMaterial = getInteger();
	if (mesh->secondMaterial)
	{
		mesh->mappingChannel = new cGMFMeshMappingChannel *[mesh->secondMaterial];
		mesh->secondMaterialAmount = getInteger();
		for (i = 0; i < mesh->secondMaterial; i++)
		{
			mesh->mappingChannel[i] = new cGMFMeshMappingChannel();
			mesh->mappingChannel[i]->number = getInteger();
			mesh->mappingChannel[i]->numTVerts = getInteger();
			getBytes(4);
			mesh->mappingChannel[i]->numTFaces = getInteger();
			getBytes(16);

			int j;
			for (j = 0; j < mesh->mappingChannel[i]->numTVerts; i++)
			{
				mesh->mappingChannel[i]->tverts[j] = new cGMFVertex();
				mesh->mappingChannel[i]->tverts[j]->vert[0] = getFloat();
				mesh->mappingChannel[i]->tverts[j]->vert[1] = getFloat();
				mesh->mappingChannel[i]->tverts[j]->vert[2] = getFloat();
			}

			for (j = 0; j < mesh->mappingChannel[i]->numTFaces; i++)
			{
				mesh->mappingChannel[i]->tfaces[j] = new cGMFFace();
				mesh->mappingChannel[i]->tfaces[j]->vertex[0] = getInteger();
				mesh->mappingChannel[i]->tfaces[j]->vertex[1] = getInteger();
				mesh->mappingChannel[i]->tfaces[j]->vertex[2] = getInteger();
				mesh->mappingChannel[i]->tfaces[j]->matlid = getInteger();
			}
		}
	}

	if (mesh->numCVerts > 0)
	{
		mesh->cverts = new cGMFVertex *[mesh->numCVerts];
		mesh->tfaces = new cGMFFace *[mesh->numFaces];
		for (i = 0; i < mesh->numCVerts; i++)
		{
			mesh->cverts[i] = new cGMFVertex();
			mesh->cverts[i]->vert[0] = getFloat();
			mesh->cverts[i]->vert[1] = getFloat();
			mesh->cverts[i]->vert[2] = getFloat();
		}

		for (i = 0; i < mesh->numFaces; i++)
		{
			mesh->tfaces[i] = new cGMFFace();
			mesh->tfaces[i]->vertex[0] = getInteger();
			mesh->tfaces[i]->vertex[1] = getInteger();
			mesh->tfaces[i]->vertex[2] = getInteger();
		}
	}

	if (mesh->numFaces > 0)
	{
		mesh->normals = new cGMFFaceNormal *[mesh->numFaces];
		for (i = 0; i < mesh->numFaces; i++)
		{
			int j;

			mesh->normals[i] = new cGMFFaceNormal();
			mesh->normals[i]->vnormals = new cGMFVertexNormal *[3];
			mesh->normals[i]->i = i;
			mesh->normals[i]->crd[0] = getFloat();
			mesh->normals[i]->crd[1] = getFloat();
			mesh->normals[i]->crd[2] = getFloat();

			for (j = 0; j < 3; j++)
			{
				mesh->normals[i]->vnormals[j] = new cGMFVertexNormal();
				mesh->normals[i]->vnormals[j]->i = mesh->faces[i]->vertex[j];
				mesh->normals[i]->vnormals[j]->crd[0] = getFloat();
				mesh->normals[i]->vnormals[j]->crd[1] = getFloat();
				mesh->normals[i]->vnormals[j]->crd[2] = getFloat();
			}
		}
	}

	mesh->backFace = getInteger();

	return mesh;
}

cGMFObjectGeo* cGMF::readObjectGeo(int size)
{
	cGMFObjectGeo* geo = new cGMFObjectGeo();
	geo->name = getString();
	geo->parent = getString();
	geo->shadeVerts = getBytes(1)[0];

	geo->tm = readTM();
	geo->mesh = readMesh();

	return geo;
}

cGMFObjectAttachment* cGMF::readObjectAttachment(int size)
{
	cGMFObjectAttachment* att = new cGMFObjectAttachment();

	att->name = getString();
	att->tm = readTM();
	att->userData = getString();

	return att;
}

cGMFObjectSimobject* cGMF::readObjectSimobject(int size)
{
	cGMFObjectSimobject* so = new cGMFObjectSimobject();

	so->name = getString();
	so->x = getFloat();
	so->y = getFloat();
	so->z = getFloat();
	so->worldScale = getFloat();
	so->tolerance = getFloat();
	so->resolver = getInteger();
	so->includeDrag = getBytes(1)[0];
	so->linearDrag = getFloat();
	so->angularDrag = getFloat();
	so->includeDeactivator = getBytes(1)[0];
	so->shortFreq = getFloat();
	so->longFreq = getFloat();
	so->useFastSubspace = getBytes(1)[0];
	so->updatesPerTimeStep = getFloat();
	so->collisionPairs = getInteger();

	return so;
}

cGMFCollisionPairList* cGMF::readCollisionPairList(int size)
{
	cGMFCollisionPairList* cplist = new cGMFCollisionPairList();
	cplist->count = getInteger();
	cplist->cpair = new cGMFCollisionPair *[cplist->count];
	int i;
	for (i = 0; i < cplist->count; i++)
	{
		cplist->cpair[i] = new cGMFCollisionPair();
		cplist->cpair[i]->body1 = getString();
		cplist->cpair[i]->body2 = getString();	
	}
	return cplist;
}

cGMFObjectRBCollectionList* cGMF::readObjectRBCollectionList(int size)
{
	cGMFObjectRBCollectionList* rbcl = new cGMFObjectRBCollectionList();
	rbcl->rbCount = getInteger();
	rbcl->rbs = new cGMFRigidBody *[rbcl->rbCount];
	int i;
	for (i = 0; i < rbcl->rbCount; i++)
	{
		getBytes(12);
		rbcl->rbs[i] = new cGMFRigidBody();
		rbcl->rbs[i]->name = getString();
		rbcl->rbs[i]->mass = getFloat();
		rbcl->rbs[i]->elasticity = getFloat();
		rbcl->rbs[i]->friction = getFloat();
		rbcl->rbs[i]->optimisation = getFloat();
		rbcl->rbs[i]->unyielding = getInteger();
		rbcl->rbs[i]->simulationGeo = getInteger();
		rbcl->rbs[i]->geomProxy = getString();
		rbcl->rbs[i]->useDisplayProxy = (int)getBytes(1)[0];
		rbcl->rbs[i]->disableCollisions = (int)getBytes(1)[0];
		rbcl->rbs[i]->inactive = (int)getBytes(1)[0];
		rbcl->rbs[i]->dispProxy = getString();
		rbcl->rbs[i]->tm = readTM();
		rbcl->rbs[i]->geoType = getInteger();
		rbcl->rbs[i]->childrenNumber = getInteger();
	}
	return rbcl;
}

cGMFObjectRBCollection* cGMF::readObjectRBCollection(int size)
{
	cGMFObjectRBCollection* rbc = new cGMFObjectRBCollection();
	rbc->name = getString();
	rbc->numDisabledPairs = getInteger();
	rbc->solverType = getInteger();

	rbc->hasRBList = 0;
	rbc->hasDisabledCollisionsList = 0;
	
	while(1)
	{
		int id = getInteger();
		int flags = getInteger();
		int lsize = getInteger();
	
		if (id == 33)
		{
			rbc->hasRBList = 1;
			rbc->rblist = readObjectRBCollectionList(lsize);
		}
		else if (id == 51)
		{
			rbc->hasDisabledCollisionsList = 1;
			rbc->cplist = readCollisionPairList(lsize);
		}
		else
			break;
	}
	return rbc;
}
