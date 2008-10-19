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
		printf("Decompiling havok simobject %s...\n", objName);
	}
	else
	{		
		getBytes(4);
		tab(preTabNum+1); fprintf(output, "*NODE_NAME\t(null)\n");
		printf("Decompiling havok simobject...\n");
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

	tab(preTabNum+1); fprintf(output, "*GRAVITY\t%f %f %f\n", simGX, simGY, simGZ);
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
	printf("Decompiling %i rigidbodies...\n", rbCount);

	tab(preTabNum); fprintf(output, "*COUNT\t%i\n", rbCount);
	tab(preTabNum); fprintf(output, "*GMID_HAVOK_RIGIDBODY_LIST\n");
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
			printf("Decompiling havok rigidbody %s...\n", objName);
		}
		else
		{		
			getBytes(4);
			tab(preTabNum+2); fprintf(output, "*NODE_NAME\t(null)\n");
			printf("Decompiling havok rigidbody...\n");
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

		if (getBytesNF(5)[4] != '\x00')
		{
			char* geometryProxyName = getString();
			tab(preTabNum+2); fprintf(output, "*GEOMETRY_PROXY_NAME\t%s\n", geometryProxyName);
		}
		else
		{		
			getBytes(4);
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
			tab(preTabNum+2); fprintf(output, "*DISPLAY_PROXY_NAME\t(null)\n");
		}

		readObjectNodeTM(preTabNum + 2);

		int rbGeoTypeInt = getInteger();
		switch (rbGeoTypeInt)
		{
			case 1:
				tab(preTabNum+2); fprintf(output, "*HAVOK_GEO_TYPE\tPlane\n");
				break;
			default:
				tab(preTabNum+2); fprintf(output, "*HAVOK_GEO_TYPE\tStandard\n");
				break;
		}

		int rbChildrenNumber = getInteger();
		tab(preTabNum+2); fprintf(output, "*NUMBER_OF_CHILDREN\t%i\n", rbChildrenNumber);

		tab(preTabNum+1); fprintf(output, "}\n");
	}

	tab(preTabNum); fprintf(output, "}\n");
}

void readRBCollisionPairs(int preTabNum)
{
	getBytes(8);
	int totalLength = getInteger();
	int rbCount = getInteger();
	printf("Decompiling %i collision pairs...\n", rbCount);

	tab(preTabNum); fprintf(output, "*GMID_HAVOK_DIS_COLLISION_PAIRS\n");
	tab(preTabNum); fprintf(output, "{\n");
	tab(preTabNum+1); fprintf(output, "*COUNT\t%i\n", rbCount);
	
	int i;
	for (i = 0; i < rbCount; i++)
	{
		char* rbLeft = getString();
		char* rbRight = getString();
		tab(preTabNum+1); fprintf(output, "{ %s\t%s }\n", rbLeft, rbRight);
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
		printf("Decompiling havok rigidbody collection %s...\n", objName);
	}
	else
	{		
		getBytes(4);
		tab(preTabNum+1); fprintf(output, "*NODE_NAME\t(null)\n");
		printf("Decompiling havok rigidbody collection...\n");
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
			readRBCollisionPairs(preTabNum+1);
		}
		else
			break;
	}

	tab(preTabNum); fprintf(output, "}\n");
}

void readHingeConstraint(int preTabNum)
{
	tab(preTabNum); fprintf(output, "*GMID_HAVOK_HINGE_CONSTRAINT\n");
	tab(preTabNum); fprintf(output, "{\n");
	getBytes(4);
	if (getBytesNF(6)[5] != '\x00')
	{
		char* cnlName = getString();
		tab(preTabNum+1); fprintf(output, "*NODE_NAME\t%s\n", cnlName);
		printf("Decompiling havok hinge constraint %s...\n", cnlName);
	}
	else
	{		
		getBytes(4);
		tab(preTabNum+1); fprintf(output, "*NODE_NAME\t(null)\n");
		printf("Decompiling havok hinge constraint...\n");
	}

	readObjectNodeTM(preTabNum + 3);

	char* cnlBody1 = getString();
	char* cnlBody2 = getString();
	float px = getFloat();
	float py = getFloat();
	float pz = getFloat();
	float ax = getFloat();
	float ay = getFloat();
	float az = getFloat();
	int cnlIsLimited = getBytes(1)[0];
	float cnlFriction = getFloat();
	float cnlAngleLimitA = getFloat();
	float cnlAngleLimitB = getFloat();

	tab(preTabNum+1); fprintf(output, "*BODY1\t%s\n", cnlBody1);
	tab(preTabNum+1); fprintf(output, "*BODY2\t%s\n", cnlBody2);
	tab(preTabNum+1); fprintf(output, "*POINT %f\t%f\t%f\n", px, py, pz);
	tab(preTabNum+1); fprintf(output, "*SPIN_AXIS %f\t%f\t%f\n", ax, ay, az);
	tab(preTabNum+1); fprintf(output, "*IS_LIMITED %i\n", cnlIsLimited);
	tab(preTabNum+1); fprintf(output, "*FRICTION %f\n", cnlFriction);
	tab(preTabNum+1); fprintf(output, "*ANGLE_LIMITS %f\t%f\n", cnlAngleLimitA, cnlAngleLimitB);
	
	//Padding issues when dealing with CSolvers > 1;
	while(getBytesNF(1)[0] == '\x00')
		getBytes(1);

	tab(preTabNum); fprintf(output, "}\n");
}

void readPointToPoint(int preTabNum)
{
	tab(preTabNum); fprintf(output, "*GMID_HAVOK_POINTTOPOINT\n");
	tab(preTabNum); fprintf(output, "{\n");

	getBytes(4);

	if (getBytesNF(6)[5] != '\x00')
	{
		char* ptpName = getString();
		tab(preTabNum+1); fprintf(output, "*NODE_NAME\t%s\n", ptpName);
		printf("Decompiling havok point to point %s...\n", ptpName);
	}
	else
	{		
		getBytes(4);
		tab(preTabNum+1); fprintf(output, "*NODE_NAME\t(null)\n");
		printf("Decompiling havok point to point...\n");
	}

	readObjectNodeTM(preTabNum + 1);

	char* ptpBody1 = getString();
	char* ptpBody2 = getString();

	float ptp1 = getFloat();
	float ptp2 = getFloat();
	float ptp3 = getFloat();
	float ptp4 = getFloat();
	float ptp5 = getFloat();
	float ptp6 = getFloat();

	tab(preTabNum+1); fprintf(output, "*BODY1\t%s\n", ptpBody1);
	tab(preTabNum+1); fprintf(output, "*BODY2\t%s\n", ptpBody2);

	tab(preTabNum+1); fprintf(output, "*POINT 1\n");
	tab(preTabNum+1); fprintf(output, "{  %f\t%f\t%f }\n", ptp1, ptp2, ptp3);
	tab(preTabNum+1); fprintf(output, "{  %f\t%f\t%f }\n", ptp4, ptp5, ptp6);

	tab(preTabNum); fprintf(output, "}\n");
}

void readConstraintSolver(int preTabNum)
{
	getBytes(8);
	int totalLength = getInteger();

	tab(preTabNum); fprintf(output, "*GMID_HAVOK_CONSTRAINTSOLVER\n");
	tab(preTabNum); fprintf(output, "{\n");

	if (getBytesNF(6)[5] != '\x00')
	{
		char* objName = getString();
		tab(preTabNum+1); fprintf(output, "*NODE_NAME\t%s\n", objName);
		printf("Decompiling havok constraint solver collection %s...\n", objName);
	}
	else
	{		
		getBytes(4);
		tab(preTabNum+1); fprintf(output, "*NODE_NAME\t(null)\n");
		printf("Decompiling havok constraint solver collection...\n");
	}



	float cnTreshold = getFloat();
	tab(preTabNum+1); fprintf(output, "*THRESHOLD\t%f\n", cnTreshold);

	if (getBytesNF(6)[5] != '\x00')
	{
		char* cnRBName = getString();
		tab(preTabNum+1); fprintf(output, "*RB_COLLECTION_NAME\t%s\n", cnRBName);
	}
	else
	{		
		getBytes(4);
		tab(preTabNum+1); fprintf(output, "*RB_COLLECTION_NAME\t(null)\n");
	}

	int cnCount = getInteger();

	if (cnCount > 0)
	{
		char* objType = getBytes(8);
		if(!memcmp(objType, "\x2C\x00\x00\x00\x02\x00\x00\x00", 8))
		{
			int totalLength = getInteger();
			int cnlCount = getInteger();
			tab(preTabNum+1); fprintf(output, "*GMID_HAVOK_CONSTRAINT_LIST\n");
			tab(preTabNum+1); fprintf(output, "{\n");
			tab(preTabNum+2); fprintf(output, "*COUNT\t%i\n", cnlCount);
			printf("Decompiling %i constraints...\n", cnlCount);

			int i;
			for (i = 0; i < cnlCount; i++)
			{
				char* constraintType = getBytes(8);
				if (!memcmp(constraintType, "\x2F\x00\x00\x00\x02\x00\x00\x00", 8))
				{
					readHingeConstraint(preTabNum + 2);
				}
				else if (!memcmp(constraintType, "\x35\x00\x00\x00\x02\x00\x00\x00", 8))
				{
					readPointToPoint(preTabNum + 2);
				}
				
			}

			

			tab(preTabNum+1); fprintf(output, "}\n");
		}
	}
	tab(preTabNum); fprintf(output, "}\n");

}

void readAngularDashpot(int preTabNum)
{
	getBytes(8);
	int totalLength = getInteger();
	
	tab(preTabNum); fprintf(output, "*GMID_HAVOK_ANGULAR_DASHPOT\n");
	tab(preTabNum); fprintf(output, "{\n");

	if (getBytesNF(6)[5] != '\x00')
	{
		char* objName = getString();
		tab(preTabNum+1); fprintf(output, "*NODE_NAME\t%s\n", objName);
		printf("Decompiling havok angular dashpot %s...\n", objName);
	}
	else
	{		
		getBytes(4);
		tab(preTabNum+1); fprintf(output, "*NODE_NAME\t(null)\n");
		printf("Decompiling havok angular dashpot...\n");
	}

	readObjectNodeTM(preTabNum +1);

	char* adBody1 = getString();
	char* adBody2 = getString();

	float adStrength = getFloat();
	float adDamping = getFloat();

	int adAllowInterpenetrations = getBytes(1)[0];

	float adQuat1 = getFloat();
	float adQuat2 = getFloat();
	float adQuat3 = getFloat();
	float adQuat4 = getFloat();

	tab(preTabNum+1); fprintf(output, "*BODY1\t%s\n", adBody1);
	tab(preTabNum+1); fprintf(output, "*BODY2\t%s\n", adBody2);

	tab(preTabNum+1); fprintf(output, "*STRENGTH\t%f\n", adStrength);
	tab(preTabNum+1); fprintf(output, "*DAMPING\t%f\n", adDamping);

	tab(preTabNum+1); fprintf(output, "*ALLOW_INTERPENETRATIONS\t%i\n", adAllowInterpenetrations);

	tab(preTabNum+1); fprintf(output, "*QUATERNION\n");
	tab(preTabNum+1); fprintf(output, "{ %f\t%f\t%f\t%f }\n", adQuat1, adQuat2, adQuat3, adQuat4);

	tab(preTabNum); fprintf(output, "}\n");

}