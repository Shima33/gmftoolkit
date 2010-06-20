// Open Source C GMF Compiler
// Copyright (c) 2008 Sergiusz Bazanski
// http://www.q3k.org/
// GPL License
#include "stdafx.h"

#include "../mystdio.h"
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
	free(getBytes(8));
	int totalLength = getInteger();
	
	tab(preTabNum); fprintf(output, "*GMID_HAVOK_SIMOBJECT\n");
	tab(preTabNum); fprintf(output, "{\n");

	char* nodedata = getBytesNF(6);

	if (nodedata[5] != '\x00')
	{
		char* objName = getString();
		tab(preTabNum+1); fprintf(output, "*NODE_NAME\t%s\n", objName);
		myprintf("Decompiling GMID_HAVOK_SIMOBJECT %s...\n", objName);
		free(objName);
	}
	else
	{		
		free(getBytes(4));
		tab(preTabNum+1); fprintf(output, "*NODE_NAME\t(null)\n");
		myprintf("Decompiling GMID_HAVOK_SIMOBJECT...\n");
	}
	free(nodedata);

	float simGX = getFloat();
	float simGY = getFloat();
	float simGZ = getFloat();

	float simWorldScale = getFloat();
	float simTolerance = getFloat();
	int simResolver = getInteger();
	char* databytes = getBytes(1);
	int simIncludeDrag = databytes[0];
	free(databytes);
	float simLinearDrag = getFloat();
	float simAngularDrag = getFloat();
	databytes = getBytes(1);
	int simIncludeDeactivator = databytes[0];
	free(databytes);
	float simShortFreq = getFloat();
	float simLongFreq = getFloat();
	databytes = getBytes(1);
	int simUseFastSubspace = databytes[0];
	free(databytes);
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
	myprintf("\n");
}

void readRBCollectionList(int preTabNum)
{
	free(getBytes(8));
	int totalLength = getInteger();
	int rbCount = getInteger();
	myprintf("Decompiling \tGMID_HAVOK_RIGIDBODY_LIST %i...\n", rbCount);

	tab(preTabNum); fprintf(output, "*COUNT\t%i\n", rbCount);
	tab(preTabNum); fprintf(output, "*GMID_HAVOK_RIGIDBODY_LIST\n");
	tab(preTabNum); fprintf(output, "{\n");
	tab(preTabNum+1); fprintf(output, "*COUNT\t%i\n", rbCount); //Weird

	int i;
	for (i = 0; i < rbCount; i++)
	{
		free(getBytes(8));
		int totalLength = getInteger();

		tab(preTabNum+1); fprintf(output, "*GMID_HAVOK_RIGIDBODY\n");
		tab(preTabNum+1); fprintf(output, "{\n");

		char* databytes = getBytesNF(6);
		if (databytes[5] != '\x00')
		{
			char* objName = getString();
			tab(preTabNum+2); fprintf(output, "*NODE_NAME\t%s\n", objName);
			myprintf("Decompiling \t\tGMID_HAVOK_RIGIDBODY %s...\n", objName);
			free(objName);
		}
		else
		{		
			free(getBytes(4));
			tab(preTabNum+2); fprintf(output, "*NODE_NAME\t(null)\n");
			myprintf("Decompiling \t\tGMID_HAVOK_RIGIDBODY...\n");
		}

		free(databytes);

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

		databytes = getBytesNF(1);

		if (databytes[0] != 0)
		{
			char* geometryProxyName = getString();
			tab(preTabNum+2); fprintf(output, "*GEOMETRY_PROXY_NAME\t%s\n", geometryProxyName);
			free(geometryProxyName);
		}
		else
		{		
			free(getBytes(4));
			tab(preTabNum+2); fprintf(output, "*GEOMETRY_PROXY_NAME\t(null)\n");
		}

		free(databytes);

		databytes = getBytes(3);
		int rbUseDisplayProxy = databytes[0];
		int rbDisableCollisions = databytes[1];
		int rbInactive = databytes[2];
		free(databytes);

		tab(preTabNum+2); fprintf(output, "*USE_DISPLAY_PROXY\t%i\n", rbUseDisplayProxy);
		tab(preTabNum+2); fprintf(output, "*DISABLE_COLLISIONS\t%i\n", rbDisableCollisions);
		tab(preTabNum+2); fprintf(output, "*INACTIVE\t%i\n", rbInactive);

		databytes = getBytesNF(6);
		if (databytes[5] != '\x00')
		{
			char* displayProxyName = getString();
			tab(preTabNum+2); fprintf(output, "*DISPLAY_PROXY_NAME\t%s\n", displayProxyName);
			free(displayProxyName);
		}
		else
		{		
			free(getBytes(4));
			tab(preTabNum+2); fprintf(output, "*DISPLAY_PROXY_NAME\t(null)\n");
		}
		free(databytes);

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
	free(getBytes(8));
	int totalLength = getInteger();
	int rbCount = getInteger();
	myprintf("Decompiling GMID_HAVOK_DIS_COLLISION_PAIRS %i...\n", rbCount);

	tab(preTabNum); fprintf(output, "*GMID_HAVOK_DIS_COLLISION_PAIRS\n");
	tab(preTabNum); fprintf(output, "{\n");
	tab(preTabNum+1); fprintf(output, "*COUNT\t%i\n", rbCount);
	
	int i;
	for (i = 0; i < rbCount; i++)
	{
		char* rbLeft = getString();
		char* rbRight = getString();
		tab(preTabNum+1); fprintf(output, "{ %s\t%s }\n", rbLeft, rbRight);
		free(rbLeft);
		free(rbRight);
	}

	tab(preTabNum); fprintf(output, "}\n");
}

void readRBCollection(int preTabNum)
{
	free(getBytes(8));
	int totalLength = getInteger();
	
	tab(preTabNum); fprintf(output, "*GMID_HAVOK_RBCOLLECTION\n");
	tab(preTabNum); fprintf(output, "{\n");

	char* databytes = getBytesNF(6);
	if (databytes[5] != '\x00')
	{
		char* objName = getString();
		tab(preTabNum+1); fprintf(output, "*NODE_NAME\t%s\n", objName);
		myprintf("Decompiling GMID_HAVOK_RBCOLLECTION %s...\n", objName);
		free(objName);
	}
	else
	{		
		free(getBytes(4));
		tab(preTabNum+1); fprintf(output, "*NODE_NAME\t(null)\n");
		myprintf("Decompiling GMID_HAVOK_RBCOLLECTION...\n");
	}
	free(databytes);

	int rbDisabledPairs = getInteger();
	int rbSolverType = getInteger();
	
	tab(preTabNum+1); fprintf(output, "*NUM_DISABLED_PAIRS\t%i\n", rbDisabledPairs);
	tab(preTabNum+1); fprintf(output, "*SOLVER_TYPE\t%i\n", rbSolverType);

	while(1)
	{
		char* peekData = getBytesNF(8);
		if (!memcmp(peekData, "\x21\x00\x00\x00\x02\x00\x00\x00", 8))
		{
			free(peekData);
			readRBCollectionList(preTabNum + 1);
			//exit(0);
		}
		else if (!memcmp(peekData, "\x33\x00\x00\x00\x02\x00\x00\x00", 8))
		{
			free(peekData);
			readRBCollisionPairs(preTabNum+1);
		}
		else
		{
			free(peekData);
			break;
		}
	}

	tab(preTabNum); fprintf(output, "}\n");
	myprintf("\n");
}

void readHingeConstraint(int preTabNum)
{
	tab(preTabNum); fprintf(output, "*GMID_HAVOK_HINGE_CONSTRAINT\n");
	tab(preTabNum); fprintf(output, "{\n");
	free(getBytes(4));

	char* databytes = getBytesNF(6);
	if (databytes[5] != '\x00')
	{
		char* cnlName = getString();
		tab(preTabNum+1); fprintf(output, "*NODE_NAME\t%s\n", cnlName);
		myprintf("Decompiling \tGMID_HAVOK_HINGE_CONSTRAINT %s...\n", cnlName);
		free(cnlName);
	}
	else
	{		
		free(getBytes(4));
		tab(preTabNum+1); fprintf(output, "*NODE_NAME\t(null)\n");
		myprintf("Decompiling \tGMID_HAVOK_HINGE_CONSTRAINT...\n");
	}
	free(databytes);

	readObjectNodeTM(preTabNum + 1);

	char* cnlBody1 = getString();
	char* cnlBody2 = getString();
	float px = getFloat();
	float py = getFloat();
	float pz = getFloat();
	float ax = getFloat();
	float ay = getFloat();
	float az = getFloat();
	databytes = getBytes(1);
	int cnlIsLimited = databytes[0];
	free(databytes);
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
	//also, fix this ugliness
	databytes = getBytesNF(1);
	while(databytes[0] == '\x00')
	{
		free(databytes);
		databytes = getBytesNF(1);
		free(getBytes(1));
	}
	free(databytes);

	free(cnlBody1);
	free(cnlBody2);

	tab(preTabNum); fprintf(output, "}\n");
}

void readWheelConstraint(int preTabNum)
{
	tab(preTabNum); fprintf(output, "*GMID_HAVOK_WHEEL_CONSTRAINT\n");
	tab(preTabNum); fprintf(output, "{\n");
	free(getBytes(4));
	char* databytes = getBytesNF(6);
	if (databytes[5] != '\x00')
	{
		char* cnlName = getString();
		tab(preTabNum+1); fprintf(output, "*NODE_NAME\t%s\n", cnlName);
		myprintf("Decompiling \tGMID_HAVOK_WHEEL_CONSTRAINT %s...\n", cnlName);
		free(cnlName);
	}
	else
	{		
		free(getBytes(4));
		tab(preTabNum+1); fprintf(output, "*NODE_NAME\t(null)\n");
		myprintf("Decompiling \tGMID_HAVOK_WHEEL_CONSTRAINT...\n");
	}
	free(databytes);

	readObjectNodeTM(preTabNum + 1);

	char* cnlBody1 = getString();
	char* cnlBody2 = getString();
	float px = getFloat();
	float py = getFloat();
	float pz = getFloat();
	float ax = getFloat();
	float ay = getFloat();
	float az = getFloat();
	
	float unkn[8];
	int x;
	for(x = 0; x < 8; x++)
		unkn[x] = getFloat();

	tab(preTabNum+1); fprintf(output, "*BODY1\t%s\n", cnlBody1);
	tab(preTabNum+1); fprintf(output, "*BODY2\t%s\n", cnlBody2);
	tab(preTabNum+1); fprintf(output, "*POINT %f\t%f\t%f\n", px, py, pz);
	tab(preTabNum+1); fprintf(output, "*SPIN_AXIS %f\t%f\t%f\n", ax, ay, az);
	tab(preTabNum+1); fprintf(output, "*SUSPENSION_AXIS %f\t%f\t%f\n", unkn[0], unkn[1], unkn[2]);
	tab(preTabNum+1); fprintf(output, "*SUSPENSION_LIMITS %f\t%f\n", unkn[3], unkn[4]);
	tab(preTabNum+1); fprintf(output, "*SUSPENSION_FRICTION %f\n", unkn[5]);
	tab(preTabNum+1); fprintf(output, "*ANGULAR_SPEED %f\n", unkn[6]);
	tab(preTabNum+1); fprintf(output, "*GAIN %f\n", unkn[7]);
	
	//Padding issues when dealing with CSolvers > 1;
	databytes = getBytesNF(1);
	while(databytes[0] == '\x00')
	{
		free(databytes);
		free(getBytes(1));
		databytes = getBytesNF(1);
	}
	free(databytes);

	free(cnlBody1);
	free(cnlBody2);

	tab(preTabNum); fprintf(output, "}\n");
}

void readPointToPoint(int preTabNum)
{
	tab(preTabNum); fprintf(output, "*GMID_HAVOK_POINTTOPOINT\n");
	tab(preTabNum); fprintf(output, "{\n");

	free(getBytes(4));
	char* databytes = getBytesNF(6);
	if (databytes[5] != '\x00')
	{
		char* ptpName = getString();
		tab(preTabNum+1); fprintf(output, "*NODE_NAME\t%s\n", ptpName);
		myprintf("Decompiling \tGMID_HAVOK_POINTTOPOINT %s...\n", ptpName);
		free(ptpName);
	}
	else
	{		
		free(getBytes(4));
		tab(preTabNum+1); fprintf(output, "*NODE_NAME\t(null)\n");
		myprintf("Decompiling \tGMID_HAVOK_POINTTOPOINT...\n");
	}
	free(databytes);

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

	free(ptpBody1);
	free(ptpBody2);

	tab(preTabNum+1); fprintf(output, "*POINT 1\n");
	tab(preTabNum+1); fprintf(output, "{  %f\t%f\t%f }\n", ptp1, ptp2, ptp3);
	tab(preTabNum+1); fprintf(output, "{  %f\t%f\t%f }\n", ptp4, ptp5, ptp6);

	tab(preTabNum); fprintf(output, "}\n");
}

void readConstraintSolver(int preTabNum)
{
	free(getBytes(8));
	int totalLength = getInteger();

	tab(preTabNum); fprintf(output, "*GMID_HAVOK_CONSTRAINTSOLVER\n");
	tab(preTabNum); fprintf(output, "{\n");

	char* databytes = getBytesNF(6);
	if (databytes[5] != '\x00')
	{
		char* objName = getString();
		tab(preTabNum+1); fprintf(output, "*NODE_NAME\t%s\n", objName);
		myprintf("Decompiling GMID_HAVOK_CONSTRAINTSOLVER %s...\n", objName);
		free(objName);
	}
	else
	{		
		free(getBytes(4));
		tab(preTabNum+1); fprintf(output, "*NODE_NAME\t(null)\n");
		myprintf("Decompiling GMID_HAVOK_CONSTRAINTSOLVER...\n");
	}
	free(databytes);


	float cnTreshold = getFloat();
	tab(preTabNum+1); fprintf(output, "*THRESHOLD\t%f\n", cnTreshold);
	
	databytes = getBytesNF(6);
	if (databytes[5] != '\x00')
	{
		char* cnRBName = getString();
		tab(preTabNum+1); fprintf(output, "*RB_COLLECTION_NAME\t%s\n", cnRBName);
		free(cnRBName);
	}
	else
	{		
		free(getBytes(4));
		tab(preTabNum+1); fprintf(output, "*RB_COLLECTION_NAME\t(null)\n");
	}
	free(databytes);

	int cnCount = getInteger();

	if (cnCount > 0)
	{
		char* objType = getBytes(8);
		debugHex(objType, 8);
		if(!memcmp(objType, "\x2C\x00\x00\x00\x02\x00\x00\x00", 8))
		{
			int totalLength = getInteger();
			int cnlCount = getInteger();
			tab(preTabNum+1); fprintf(output, "*GMID_HAVOK_CONSTRAINT_LIST\n");
			tab(preTabNum+1); fprintf(output, "{\n");
			tab(preTabNum+2); fprintf(output, "*COUNT\t%i\n", cnlCount);
			//printf("Decompiling %i constraints...\n", cnlCount);

			int i;
			for (i = 0; i < cnlCount; i++)
			{
				char* constraintType = getBytes(8);
				debugHex(constraintType, 8);
				if (!memcmp(constraintType, "\x2F\x00\x00\x00\x02\x00\x00\x00", 8))
				{
					readHingeConstraint(preTabNum + 2);
				}
				else if (!memcmp(constraintType, "\x2E\x00\x00\x00\x02\x00\x00\x00", 8))
				{
					readWheelConstraint(preTabNum + 2);
				}
				else if (!memcmp(constraintType, "\x35\x00\x00\x00\x02\x00\x00\x00", 8))
				{
					readPointToPoint(preTabNum + 2);
				}
				free(constraintType);
				
			}

			

			tab(preTabNum+1); fprintf(output, "}\n");
		}
		free(objType);
	}
	tab(preTabNum); fprintf(output, "}\n");
	myprintf("\n");

}

void readAngularDashpot(int preTabNum)
{
	free(getBytes(8));
	int totalLength = getInteger();
	
	tab(preTabNum); fprintf(output, "*GMID_HAVOK_ANGULAR_DASHPOT\n");
	tab(preTabNum); fprintf(output, "{\n");

	char* databytes = getBytesNF(6);
	if (databytes[5] != '\x00')
	{
		char* objName = getString();
		tab(preTabNum+1); fprintf(output, "*NODE_NAME\t%s\n", objName);
		myprintf("Decompiling GMID_HAVOK_ANGULAR_DASHPOT %s...\n", objName);
		free(objName);
	}
	else
	{		
		free(getBytes(4));
		tab(preTabNum+1); fprintf(output, "*NODE_NAME\t(null)\n");
		myprintf("Decompiling GMID_HAVOK_ANGULAR_DASHPOT...\n");
	}
	free(databytes);

	readObjectNodeTM(preTabNum +1);

	char* adBody1 = getString();
	char* adBody2 = getString();

	float adStrength = getFloat();
	float adDamping = getFloat();

	databytes = getBytes(1);
	int adAllowInterpenetrations = databytes[0];
	free(databytes);

	float adQuat1 = getFloat();
	float adQuat2 = getFloat();
	float adQuat3 = getFloat();
	float adQuat4 = getFloat();

	tab(preTabNum+1); fprintf(output, "*BODY1\t%s\n", adBody1);
	tab(preTabNum+1); fprintf(output, "*BODY2\t%s\n", adBody2);
	free(adBody1);
	free(adBody2);

	tab(preTabNum+1); fprintf(output, "*STRENGTH\t%f\n", adStrength);
	tab(preTabNum+1); fprintf(output, "*DAMPING\t%f\n", adDamping);

	tab(preTabNum+1); fprintf(output, "*ALLOW_INTERPENETRATIONS\t%i\n", adAllowInterpenetrations);

	tab(preTabNum+1); fprintf(output, "*QUATERNION\n");
	tab(preTabNum+1); fprintf(output, "{ %f\t%f\t%f\t%f }\n", adQuat1, adQuat2, adQuat3, adQuat4);

	tab(preTabNum); fprintf(output, "}\n");
	myprintf("\n");

}
