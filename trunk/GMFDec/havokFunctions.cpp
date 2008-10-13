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

void readSimObject(int preTabNum)
{
	getBytes(8);
	int totalLength = getInteger();
	
	tab(preTabNum); fprintf(output, "*GMID_HAVOK_SIMOBJECT\n");
	tab(preTabNum); fprintf(output, "{\n");

	if (getBytesNF(6)[5] != '\x00')
	{
		char* objName = getString();
		tab(preTabNum+1); fprintf(output, "*NODE_NAME\t%s\n", objName);
	}
	else
	{		
		getBytes(4);
		debugHex(getBytesNF(16), 16);
		tab(preTabNum+1); fprintf(output, "*NODE_NAME\t(null)\n");
	}

	float simGX = getFloat();
	float simGY = getFloat();
	float simGZ = getFloat();

	float simWorldScale = getFloat();
	float simTolerance = getFloat();
	int simResolver = getInteger();
	int simIncludeDrag = getBytes(1)[0];
	float simLinearDrag = getFloat();
	float simAngularDrag = getFloat();
	int simIncludeDeactivator = getBytes(1)[0];
	float simShortFreq = getFloat();
	float simLongFreq = getFloat();
	int simUseFastSubspace = getBytes(1)[0];
	float simUpdatesPertimeStep = getFloat();
	int simCollisionPairs = getInteger();

	tab(preTabNum+1); fprintf(output, "*GRAVITY\t%f\t%f\t%f\n", simGX, simGY, simGZ);
	tab(preTabNum+1); fprintf(output, "*WORLDSCALE\t%f\n", simWorldScale);
	tab(preTabNum+1); fprintf(output, "*SIMTOLERANCE\t%f\n", simTolerance);
	tab(preTabNum+1); fprintf(output, "*RESOLVER\t%i\n", simResolver);
	tab(preTabNum+1); fprintf(output, "*INCLUDE_DRAG\t%i\n", simIncludeDrag);
	tab(preTabNum+1); fprintf(output, "*LINEAR_DRAG\t%f\n", simLinearDrag);
	tab(preTabNum+1); fprintf(output, "*ANGULAR_DRAG\t%f\n", simAngularDrag);
	tab(preTabNum+1); fprintf(output, "*INCLUDE_DEACTIVATOR\t%i\n", simIncludeDeactivator);
	tab(preTabNum+1); fprintf(output, "*SHORTFREQ\t%f\n", simShortFreq);
	tab(preTabNum+1); fprintf(output, "*LONGFREQ\t%f\n", simLongFreq);
	tab(preTabNum+1); fprintf(output, "*USE_FAST_SUBSPACE\t%i\n", simUseFastSubspace);
	tab(preTabNum+1); fprintf(output, "*UPDATES_PER_TIMESTEP\t%f\n", simUpdatesPertimeStep);
	tab(preTabNum+1); fprintf(output, "*NUM_COLLISION_PAIRS\t%i\n", simCollisionPairs);

	tab(preTabNum); fprintf(output, "}\n");
}

void readRBCollectionList(int preTabNum)
{
	getBytes(8);
	int totalLength = getInteger();
	int rbCount = getInteger();

	tab(preTabNum); fprintf(output, "*COUNT\t%i\n", rbCount);
	tab(preTabNum); fprintf(output, "*GMID_HAVOK_RIGIBODY_LIST\n");
	tab(preTabNum); fprintf(output, "{\n");
	tab(preTabNum+1); fprintf(output, "*COUNT\t%i\n", rbCount); //Wierd

	int i;
	for (i = 0; i < rbCount; i++)
	{
		getBytes(8);
		int totalLength = getInteger();

		tab(preTabNum+1); fprintf(output, "*GMID_HAVOK_RIGIDBODY\n");
		tab(preTabNum+1); fprintf(output, "{\n");

		if (getBytesNF(6)[5] != '\x00')
		{
			char* objName = getString();
			tab(preTabNum+2); fprintf(output, "*NODE_NAME\t%s\n", objName);
		}
		else
		{		
			getBytes(4);
			debugHex(getBytesNF(16), 16);
			tab(preTabNum+2); fprintf(output, "*NODE_NAME\t(null)\n");
		}

		float rbMass = getFloat();
		float rbElast = getFloat();
		float rbFriction = getFloat();
		float rbOptimisation = getFloat();
		int rbUnyielding = getInteger();
		int rbSimulationGeo = getInteger();

		tab(preTabNum+2); fprintf(output, "*MASS\t%f\n", rbMass);
		tab(preTabNum+2); fprintf(output, "*ELASTICITY\t%f\n", rbElast);
		tab(preTabNum+2); fprintf(output, "*FRICTION\t%f\n", rbFriction);
		tab(preTabNum+2); fprintf(output, "*OPTIMIZATION_LEVEL\t%f\n", rbOptimisation);
		tab(preTabNum+2); fprintf(output, "*UNYIELDING\t%i\n", rbUnyielding);
		tab(preTabNum+2); fprintf(output, "*SIMULATION_GEOMETRY\t%i\n", rbSimulationGeo);

		if (getBytesNF(6)[5] != '\x00')
		{
			char* geometryProxyName = getString();
			tab(preTabNum+2); fprintf(output, "*GEOMETRY_PROXY_NAME\t%s\n", geometryProxyName);
		}
		else
		{		
			getBytes(4);
			debugHex(getBytesNF(16), 16);
			tab(preTabNum+2); fprintf(output, "*GEOMETRY_PROXY_NAME\t(null)\n");
		}

		int rbUseDisplayProxy = getBytes(1)[0];
		int rbDisableCollisions = getBytes(1)[0];
		int rbInactive = getBytes(1)[0];

		tab(preTabNum+2); fprintf(output, "*USE_DISPLAY_PROXY\t%i\n", rbUseDisplayProxy);
		tab(preTabNum+2); fprintf(output, "*DISABLE_COLLISIONS\t%i\n", rbDisableCollisions);
		tab(preTabNum+2); fprintf(output, "*INACTIVE\t%i\n", rbInactive);

		if (getBytesNF(6)[5] != '\x00')
		{
			char* displayProxyName = getString();
			tab(preTabNum+2); fprintf(output, "*DISPLAY_PROXY_NAME\t%s\n", displayProxyName);
		}
		else
		{		
			getBytes(4);
			debugHex(getBytesNF(16), 16);
			tab(preTabNum+2); fprintf(output, "*DISPLAY_PROXY_NAME\t(null)\n");
		}

		readObjectNodeTM(preTabNum + 2);

		int rbGeoTypeInt = getInteger();
		tab(preTabNum+2); fprintf(output, "*HAVOK_GEO_TYPE\tStandard\n");

		int rbChildrenNumber = getInteger();
		tab(preTabNum+2); fprintf(output, "*NUMBER_OF_CHILDREN\t%i\n", rbChildrenNumber);

		tab(preTabNum+1); fprintf(output, "}\n");
	}

	tab(preTabNum); fprintf(output, "}\n");
}

void readRBCollection(int preTabNum)
{
	getBytes(8);
	int totalLength = getInteger();
	
	tab(preTabNum); fprintf(output, "*GMID_HAVOK_RBCOLLECTION\n");
	tab(preTabNum); fprintf(output, "{\n");

	if (getBytesNF(6)[5] != '\x00')
	{
		char* objName = getString();
		tab(preTabNum+1); fprintf(output, "*NODE_NAME\t%s\n", objName);
	}
	else
	{		
		getBytes(4);
		debugHex(getBytesNF(16), 16);
		tab(preTabNum+1); fprintf(output, "*NODE_NAME\t(null)\n");
	}

	int rbDisabledPairs = getInteger();
	int rbSolverType = getInteger();
	
	tab(preTabNum+1); fprintf(output, "*NUM_DISABLED_PAIRS\t%i\n", rbDisabledPairs);
	tab(preTabNum+1); fprintf(output, "*SOLVER_TYPE\t%i\n", rbSolverType);

	while(1)
	{
		char* peekData = getBytesNF(8);
		if (!memcmp(peekData, "\x21\x00\x00\x00\x02\x00\x00\x00", 8))
		{
			readRBCollectionList(preTabNum + 1);
			//exit(0);
		}
		else if (!memcmp(peekData, "\x33\x00\x00\x00\x02\x00\x00\x00", 8))
		{

		}
		else
			break;
	}

	tab(preTabNum); fprintf(output, "}\n");
}