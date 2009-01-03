#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#include "byteFunctions.h"
#include "asciiFunctions.h"
#include "objectFunctions.h"

extern FILE* input;
extern FILE* output;

int readGMIDAttachmentPt()
{
	printInt(21);
	printInt(2);
	printBytes("\xFF\xFF\xFF\xFF", 4);
	int beginningOffset = ftell(output);
	openBracket();

	char* ptName = readString("*NODE_NAME");
	printString(ptName);
	readTM();

	fscanf(input, "USER DATA");
	fgetc(input);
	int offset = ftell(input);
	int length = 0;

	while (fgetc(input) != '}')
		length++;

	fseek(input, offset, 0);
	
	char* ptUserData = (char*)malloc(sizeof(char)*(length+1));
	fread(ptUserData, sizeof(char), length, input);

	ptUserData[length-2] = '\x00';

	if (length != 0)
		printString(ptUserData);
	else
		printInt(0);

	closeBracket();
	int endOffset = ftell(output);
	int size = endOffset - beginningOffset;
	fseek(output, beginningOffset - 4, 0);	
	printInt(size);
	fseek(output, endOffset, 0);
	return 0;
}

int readHavokSimobject()
{
	printInt(30);
	printInt(2);
	printBytes("\xFF\xFF\xFF\xFF", 4);
	int beginningOffset = ftell(output);
	openBracket();

	char* soName = readString("*NODE_NAME");
	if (strncmp("(null)", soName, 6))
		printString(soName);
	else
		printInt(0);

	bracketize();
	float soGravity[3];
	fscanf(input, "*GRAVITY\t%f %f %f\n", &soGravity[0], &soGravity[1], &soGravity[2]);

	float soWorldScale = readFloat("*WORLDSCALE");
	float soSimTolerance = readFloat("*SIMTOLERANCE");
	int soResolver = readInt("*RESOLVER");
	int soIncludeDrag = readInt("*INCLUDE_DRAG");
	float soLinearDrag = readFloat("*LINEAR_DRAG");
	float soAngularDrag = readFloat("*ANGULAR_DRAG");
	int soDeactivator = readInt("*INCLUDE_DEACTIVATOR");
	float soShortFreq = readFloat("*SHORTFREQ");
	float soLongFreq = readFloat("*LONGFREQ");
	int soUseFastSubspace = readInt("*USE_FAST_SUBSPACE");
	float soUpdatesPerTimeStep = readFloat("*UPDATES_PER_TIMESTEP");
	int soNumCollisionPairs = readInt("*NUM_COLLISION_PAIRS");

	printFloat(soGravity[0]);
	printFloat(soGravity[1]);
	printFloat(soGravity[2]);
	printFloat(soWorldScale);
	printFloat(soSimTolerance);
	printBytes((char*)&soResolver, 1);
	printInt(soIncludeDrag);
	printFloat(soLinearDrag);
	printFloat(soAngularDrag);
	printBytes((char*)&soDeactivator, 1);
	printFloat(soShortFreq);
	printFloat(soLongFreq);
	printBytes((char*)&soUseFastSubspace, 1);
	printFloat(soUpdatesPerTimeStep);
	printInt(soNumCollisionPairs);

	closeBracket();
	int endOffset = ftell(output);
	int size = endOffset - beginningOffset;
	fseek(output, beginningOffset - 4, 0);	
	printInt(size - 4);
	fseek(output, endOffset, 0);
	return 0;
}

int readHavokRB()
{
	printInt(32);
	printInt(4);
	printBytes("\xFF\xFF\xFF\xFF", 4);
	int beginningOffset = ftell(output);
	openBracket();

	char* rbName = readString("*NODE_NAME");
	printString(rbName);
	float rbMass = readFloat("*MASS");
	printFloat(rbMass);
	float rbElasticity = readFloat("*ELASTICITY");
	printFloat(rbElasticity);
	float rbFriction = readFloat("*FRICTION");
	printFloat(rbFriction);
	float rbOptimization = readFloat("*OPTIMIZATION_LEVEL");
	printFloat(rbOptimization);
	int rbUnyielding = readInt("*UNYIELDING");
	printInt(rbUnyielding);
	int rbSimulationGeometry = readInt("*SIMULATION_GEOMETRY");
	printInt(rbSimulationGeometry);
	char* rbGeometryProxyName = readString("*GEOMETRY_PROXY_NAME");
	if(strncmp(rbGeometryProxyName, "(null)", 6))
		printString(rbGeometryProxyName);
	else
		printInt(0);
	int rbUseDisplayProxy = readInt("*USE_DISPLAY_PROXY");
	printBytes((char*)&rbUseDisplayProxy, 1);
	int rbDisableCollisions = readInt("*DISABLE_COLLISIONS");
	printBytes((char*)&rbDisableCollisions, 1);
	int rbInactive = readInt("*INACTIVE");
	printBytes((char*)&rbInactive, 1);

	char* rbDisplayProxyName = readString("*DISPLAY_PROXY_NAME");
	if(strncmp(rbDisplayProxyName, "(null)", 6))
		printString(rbDisplayProxyName);
	else
		printInt(0);

	readTM();

	char* rbHavokGeoType = readString("*HAVOK_GEO_TYPE");
	if (!strncmp(rbHavokGeoType, "Standard", 8))
		printInt(0);
	else
		printInt(0);

	int rbNumberOfChildren = readInt("*NUMBER_OF_CHILDREN");

	printInt(rbNumberOfChildren);

	closeBracket();
	int endOffset = ftell(output);
	int size = endOffset - beginningOffset;
	fseek(output, beginningOffset - 4, 0);	
	printInt(0);
	fseek(output, endOffset, 0);
	return 0;
}

int readHavokRBList()
{
	printInt(33);
	printInt(2);
	printBytes("\xFF\xFF\xFF\xFF", 4);
	int beginningOffset = ftell(output);
	openBracket();

	int rblCount = readInt("*COUNT");

	printInt(rblCount);

	int i;

	for (i = 0; i < rblCount; i++)
	{
		readNothing("*GMID_HAVOK_RIGIDBODY");
		readHavokRB();
	}

	closeBracket();
	int endOffset = ftell(output);
	int size = endOffset - beginningOffset;
	fseek(output, beginningOffset - 4, 0);	
	printInt(0); //yes, this is weird, but otherwise RA2 crashes.
	fseek(output, endOffset, 0);
	return 0;
}

int readHavokDisabledPairs()
{
	printInt(51);
	printInt(2);
	printBytes("\xFF\xFF\xFF\xFF", 4);
	int beginningOffset = ftell(output);
	openBracket();

	int dpNumPairs = readInt("*COUNT");

	printInt(dpNumPairs);

	int i;
	for (i = 0; i < dpNumPairs; i++)
	{
		bracketize();
		//fscanf(input, "{ %s\t%s }");
		int body1Length = 0;
		int body2Length = 0;
		fscanf(input, "{ %i", &body1Length);
		char* body1 = (char*)malloc(sizeof(char) * body1Length);
		fscanf(input, "%s\t%i", body1, &body2Length);
		char* body2 = (char*)malloc(sizeof(char) * body1Length);
		fscanf(input, "%s }\n", body2);
		printString(body1);
		printString(body2);
	}

	closeBracket();
	int endOffset = ftell(output);
	int size = endOffset - beginningOffset;
	fseek(output, beginningOffset - 4, 0);	
	printInt(0); // WUT
	fseek(output, endOffset, 0);
	return 0;
}

int readHavokRBCollection()
{
	printInt(31);
	printInt(4);
	printBytes("\xFF\xFF\xFF\xFF", 4);
	int beginningOffset = ftell(output);
	openBracket();

	char* rbcName = readString("*NODE_NAME");
	int rbcNumDisabledPairs = readInt("*NUM_DISABLED_PAIRS");
	int rbcSolverType = readInt("*SOLVER_TYPE");
	int rbcRbCount = readInt("*COUNT");

	printString(rbcName);
	printInt(rbcNumDisabledPairs);
	printInt(rbcSolverType);

	bracketize();
	char* nextThing = (char*)malloc(sizeof(char)*128);

	while(1)
	{
		int offset = ftell(input);
		fscanf(input, "%s\n", nextThing);
		if (!strncmp(nextThing, "*GMID_HAVOK_RIGIDBODY_LIST", 26))
		{
			/*fscanf(input, "\t%*i\n");
			readNothing("*GMID_HAVOK_RIGIDBODY_LIST");*/
			readHavokRBList();
		}
		else if (!strncmp(nextThing, "*COUNT", 6))
		{
			fscanf(input, "\t%*i\n");
			readNothing("*GMID_HAVOK_RIGIDBODY_LIST");
			readHavokRBList();
		}
		else if (!strncmp(nextThing, "*GMID_HAVOK_DIS_COLLISION_PAIRS", 31))
		{
			readHavokDisabledPairs();
		}
		else if (!strncmp(nextThing, "}", 1))
		{
			fseek(input, offset, 0);
			break;
		}
		else
		{
			error(nextThing);
		}
	}

	closeBracket();
	int endOffset = ftell(output);
	int size = endOffset - beginningOffset;
	fseek(output, beginningOffset - 4, 0);	
	printInt(size);
	fseek(output, endOffset, 0);
	return 0;
}

int readHavokHingeC()
{
	printInt(47);
	printInt(2);
	printBytes("\xFF\xFF\xFF\xFF", 4);
	int beginningOffset = ftell(output);
	openBracket();

	char* cName = readString("*NODE_NAME");
	printString(cName);
	readTM();
	char* cBody1 = readString("*BODY1");
	char* cBody2 = readString("*BODY2");

	float p[3];
	bracketize();
	fscanf(input, "*POINT %f\t%f\t%f\n", &p[0], &p[1], &p[2]);

	float s[3];
	bracketize();
	fscanf(input, "*SPIN_AXIS %f\t%f\t%f\n", &s[0], &s[1], &s[2]);

	int cLimited = readInt("*IS_LIMITED");
	float cFriction = readFloat("*FRICTION");

	float a[2];
	bracketize();
	fscanf(input, "*ANGLE_LIMITS %f\t%f\n", &a[0], &a[1]);

	
	printString(cBody1);
	printString(cBody2);
	printFloat(p[0]);
	printFloat(p[1]);
	printFloat(p[2]);
	printFloat(s[0]);
	printFloat(s[1]);
	printFloat(s[2]);
	printBytes((char*)&cLimited, 1);
	printFloat(cFriction);
	printFloat(a[0]);
	printFloat(a[1]);

	closeBracket();
	int endOffset = ftell(output);
	int size = endOffset - beginningOffset;
	fseek(output, beginningOffset - 4, 0);	
	printInt(0);
	fseek(output, endOffset, 0);
	return 0;
}

int readHavokPivotC()
{
	MessageBox(NULL, "Warning! The PIVOT constraint is deprecated. Please use the WHEEL constraint instead (see decompiled carsteering for details)!\nThe file will continue to compile.", "Deprecation error", MB_ICONWARNING);
	printInt(46);
	printInt(2);
	printBytes("\xFF\xFF\xFF\xFF", 4);
	int beginningOffset = ftell(output);
	openBracket();

	char* cName = readString("*NODE_NAME");
	printString(cName);
	readTM();
	char* cBody1 = readString("*BODY1");
	char* cBody2 = readString("*BODY2");

	float p[3];
	bracketize();
	fscanf(input, "*POINT %f\t%f\t%f\n", &p[0], &p[1], &p[2]);

	float s[3];
	bracketize();
	fscanf(input, "*SPIN_AXIS %f\t%f\t%f\n", &s[0], &s[1], &s[2]);

	float unkn[8];
	bracketize();
	fscanf(input, "*UNKNOWN %f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\n", &unkn[0], &unkn[1], &unkn[2], &unkn[3], &unkn[4], &unkn[5], &unkn[6], &unkn[7]);

	
	printString(cBody1);
	printString(cBody2);
	printFloat(p[0]);
	printFloat(p[1]);
	printFloat(p[2]);
	printFloat(s[0]);
	printFloat(s[1]);
	printFloat(s[2]);

	int x;
	for (x = 0; x < 8; x++)
		printFloat(unkn[x]);

	closeBracket();
	int endOffset = ftell(output);
	int size = endOffset - beginningOffset;
	fseek(output, beginningOffset - 4, 0);	
	printInt(0);
	fseek(output, endOffset, 0);
	return 0;
}

int readHavokWheelC()
{
	printInt(46);
	printInt(2);
	printBytes("\xFF\xFF\xFF\xFF", 4);
	int beginningOffset = ftell(output);
	openBracket();

	char* cName = readString("*NODE_NAME");
	printString(cName);
	readTM();
	char* cBody1 = readString("*BODY1");
	char* cBody2 = readString("*BODY2");

	float p[3];
	bracketize();
	fscanf(input, "*POINT %f\t%f\t%f\n", &p[0], &p[1], &p[2]);

	float s[3];
	bracketize();
	fscanf(input, "*SPIN_AXIS %f\t%f\t%f\n", &s[0], &s[1], &s[2]);

	float suspAxis[3];
	bracketize();
	fscanf(input, "*SUSPENSION_AXIS %f\t%f\t%f\n", &suspAxis[0], &suspAxis[1], &suspAxis[2]);

	float suspLimits[2];
	bracketize();
	fscanf(input, "*SUSPENSION_LIMITS %f\t%f\n", &suspLimits[0], &suspLimits[1]);

	float suspFriction = readFloat("*SUSPENSION_FRICTION");

	float angSpeed = readFloat("*ANGULAR_SPEED");

	float gain = readFloat("*GAIN");

	
	printString(cBody1);
	printString(cBody2);
	printFloat(p[0]);
	printFloat(p[1]);
	printFloat(p[2]);
	printFloat(s[0]);
	printFloat(s[1]);
	printFloat(s[2]);

	printFloat(suspAxis[0]);
	printFloat(suspAxis[1]);
	printFloat(suspAxis[2]);

	printFloat(suspLimits[0]);
	printFloat(suspLimits[1]);

	printFloat(suspFriction);
	printFloat(angSpeed);
	printFloat(gain);

	closeBracket();
	int endOffset = ftell(output);
	int size = endOffset - beginningOffset;
	fseek(output, beginningOffset - 4, 0);	
	printInt(0);
	fseek(output, endOffset, 0);
	return 0;
}

int readHavokCItem()
{
	char* cType = (char*)malloc(sizeof(char)*128);
	bracketize();
	fscanf(input, "%s\n", cType);

	if (!strncmp(cType, "*GMID_HAVOK_HINGE_CONSTRAINT", 28))
	{
		readHavokHingeC();
	}
	else if (!strncmp(cType, "*GMID_HAVOK_PIVOT_CONSTRAINT", 29))
	{
		readHavokPivotC();
	}
	else if (!strncmp(cType, "*GMID_HAVOK_WHEEL_CONSTRAINT", 29))
	{
		readHavokWheelC();
	}
	else
	{
		char* message = (char*)malloc(sizeof(char)*128);
		sprintf(message, "readHavokCItem(): Unknown Constraint type %s!", cType);
		error(message);
	}
	return 0;
}

int readHavokCList()
{
	int cnlCountAgainOffset = ftell(output);
	printBytes("\xFF\xFF\xFF\xFF", 4);
	printInt(44);
	printInt(2);
	printBytes("\xFF\xFF\xFF\xFF", 4);
	int beginningOffset = ftell(output);
	openBracket();

	int clCount = readInt("*COUNT");

	printInt(clCount);

	int curOffset = ftell(output);
	fseek(output, cnlCountAgainOffset, 0);
	printInt(clCount);
	fseek(output, curOffset, 0);

	int i;

	for(i = 0; i < clCount; i++)
		readHavokCItem();


	closeBracket();
	int endOffset = ftell(output);
	int size = endOffset - beginningOffset;
	fseek(output, beginningOffset - 4, 0);	
	printInt(0);
	fseek(output, endOffset, 0);
	return 0;
}

int readHavokCSolver()
{
	printInt(42);
	printInt(3);
	printBytes("\xFF\xFF\xFF\xFF", 4);
	int beginningOffset = ftell(output);
	openBracket();

	char* csName = readString("*NODE_NAME");
	float csThreshold = readFloat("*THRESHOLD");
	char* csRBCollectionName = readString("*RB_COLLECTION_NAME");

	printString(csName);
	printFloat(csThreshold);
	printString(csRBCollectionName);

	int offset = ftell(input);
	char* next = (char*)malloc(sizeof(char)*128);
	fscanf(input, "%s", next);
	fseek(input, offset, 0);

	if (strncmp(next, "}", 1))
	{
		readNothing("*GMID_HAVOK_CONSTRAINT_LIST");
		readHavokCList();
	}
	else
		printInt(0);

	closeBracket();
	int endOffset = ftell(output);
	int size = endOffset - beginningOffset;
	fseek(output, beginningOffset - 4, 0);	
	printInt(size);
	fseek(output, endOffset, 0);
	return 0;
}
