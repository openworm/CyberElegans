#include "CElegans.h"
#include "fstream"
#include "string"
#include <windows.h>														// Header File For Windows
#include <gl\gl.h>															// Header File For The OpenGL32 Library
#include <gl\glu.h>															// Header File For The GLu32 Library
#include <sstream>
extern Vector3D ort1,ort2,ort3,vcenter,pos_rc;

///
CElegans::CElegans(const double i_length, const int i_size)//Класс Червь
{
	size = i_size;
	time = 0;
	length = i_length;
	mPoint.reserve(200);
	spring.reserve(1000);
	muscle.reserve(100);
	neuron.reserve(300);
	//memory reserved

	std::vector <CNeuron *> neuron_vector;
	//A vector to store Neuro position levels

	//double dl = i_length;
	//double dx = dl*0.5;
	dl = i_length;
	dx = dl*0.5*73/60;
	float wp[27] = { 0.35f,0.50f,0.61f,0.68f,0.75f,0.81f,0.85f,0.88f,0.91f,0.93f,0.95f,0.97f,0.99f,
	/*Width Profile*/1.00f,0.99f,0.98f,0.97f,0.96f,0.95f,0.93f,0.91f,0.88f,0.83f,0.79f,0.70f,0.53f,0.34f };

	vshift = Vector3D(dl*size/4+0.5,0,dl*3/2);//Vector3D

	//for(int ii = 0; ii<27;ii++) wp[ii] /= 9.f/5.5f;

/**/
	addMPoint( new CMassPoint(0.05, vshift+Vector3D(  -0.7 * dx  ,  0		,  0	  )) );

	addMPoint( new CMassPoint(0.05, vshift+Vector3D( -1.5 * dx,	-0.35*dl*wp[0], -dl*wp[0]/2	  )) );
	addMPoint( new CMassPoint(0.05, vshift+Vector3D( -1.5 * dx,  0.35*dl*wp[0], -dl*wp[0]/2	  )) );	
	addMPoint( new CMassPoint(0.05, vshift+Vector3D( -1.5 * dx,  dl*wp[0]/2	, -0.35*dl*wp[0] )) );
	addMPoint( new CMassPoint(0.05, vshift+Vector3D( -1.5 * dx,  dl*wp[0]/2	,  0.35*dl*wp[0] )) );

	addMPoint( new CMassPoint(0.05, vshift+Vector3D( -1.5 * dx,	 0.35*dl*wp[0]	,  dl*wp[0]/2	  )) );
	addMPoint( new CMassPoint(0.05, vshift+Vector3D( -1.5 * dx, -0.35*dl*wp[0]	,  dl*wp[0]/2	  )) );	
	addMPoint( new CMassPoint(0.05, vshift+Vector3D( -1.5 * dx, -dl*wp[0]/2	,  0.35*dl*wp[0] )) );
	addMPoint( new CMassPoint(0.05, vshift+Vector3D( -1.5 * dx, -dl*wp[0]/2	, -0.35*dl*wp[0] )) );

	/**/
	addSpring( new CSpring(AUTODETECT, StiffCoeff, FrictCoeff, mPoint[0], mPoint[1]) );
	addSpring( new CSpring(AUTODETECT, StiffCoeff, FrictCoeff, mPoint[0], mPoint[2]) );
	addSpring( new CSpring(AUTODETECT, StiffCoeff, FrictCoeff, mPoint[0], mPoint[3]) );
	addSpring( new CSpring(AUTODETECT, StiffCoeff, FrictCoeff, mPoint[0], mPoint[4]) );
	addSpring( new CSpring(AUTODETECT, StiffCoeff, FrictCoeff, mPoint[0], mPoint[5]) );
	addSpring( new CSpring(AUTODETECT, StiffCoeff, FrictCoeff, mPoint[0], mPoint[6]) );
	addSpring( new CSpring(AUTODETECT, StiffCoeff, FrictCoeff, mPoint[0], mPoint[7]) );
	addSpring( new CSpring(AUTODETECT, StiffCoeff, FrictCoeff, mPoint[0], mPoint[8]) );
	/**/
	addSpring( new CSpring(AUTODETECT, StiffCoeff, FrictCoeff, mPoint[1], mPoint[2]) );
	addSpring( new CSpring(AUTODETECT, StiffCoeff, FrictCoeff, mPoint[2], mPoint[3]) );
	addSpring( new CSpring(AUTODETECT, StiffCoeff, FrictCoeff, mPoint[3], mPoint[4]) );
	addSpring( new CSpring(AUTODETECT, StiffCoeff, FrictCoeff, mPoint[4], mPoint[5]) );
	addSpring( new CSpring(AUTODETECT, StiffCoeff, FrictCoeff, mPoint[5], mPoint[6]) );
	addSpring( new CSpring(AUTODETECT, StiffCoeff, FrictCoeff, mPoint[6], mPoint[7]) );
	addSpring( new CSpring(AUTODETECT, StiffCoeff, FrictCoeff, mPoint[7], mPoint[8]) );
	addSpring( new CSpring(AUTODETECT, StiffCoeff, FrictCoeff, mPoint[8], mPoint[1]) );
	/**/
	addSpring( new CSpring(AUTODETECT, StiffCoeff, FrictCoeff, mPoint[1], mPoint[5]) );
	addSpring( new CSpring(AUTODETECT, StiffCoeff, FrictCoeff, mPoint[2], mPoint[6]) );
	addSpring( new CSpring(AUTODETECT, StiffCoeff, FrictCoeff, mPoint[3], mPoint[7]) );
	addSpring( new CSpring(AUTODETECT, StiffCoeff, FrictCoeff, mPoint[4], mPoint[8]) );
	addSpring( new CSpring(AUTODETECT, StiffCoeff, FrictCoeff, mPoint[1], mPoint[6]) );
	addSpring( new CSpring(AUTODETECT, StiffCoeff, FrictCoeff, mPoint[2], mPoint[5]) );
	addSpring( new CSpring(AUTODETECT, StiffCoeff, FrictCoeff, mPoint[3], mPoint[8]) );
	addSpring( new CSpring(AUTODETECT, StiffCoeff, FrictCoeff, mPoint[4], mPoint[7]) );
	table.push_back(neuron_vector);
	
	//Head created
	
	int i;

	
	for(i = 1; i < size + 1; i++)
	{
		addMPoint( new CMassPoint(0.05, vshift+Vector3D( -(1.0+i)*dx, 0		, 0 )) );

		addMPoint( new CMassPoint(0.05, vshift+Vector3D( -(1.5+i)*dx, -0.35*dl*wp[i], -0.50*dl*wp[i] )) );
		addMPoint( new CMassPoint(0.05, vshift+Vector3D( -(1.5+i)*dx,  0.35*dl*wp[i], -0.50*dl*wp[i] )) );	
		addMPoint( new CMassPoint(0.05, vshift+Vector3D( -(1.5+i)*dx,  0.50*dl*wp[i], -0.35*dl*wp[i] )) );
		addMPoint( new CMassPoint(0.05, vshift+Vector3D( -(1.5+i)*dx,  0.50*dl*wp[i],  0.35*dl*wp[i] )) );

		addMPoint( new CMassPoint(0.05, vshift+Vector3D( -(1.5+i)*dx,  0.35*dl*wp[i],  0.50*dl*wp[i] )) );
		addMPoint( new CMassPoint(0.05, vshift+Vector3D( -(1.5+i)*dx, -0.35*dl*wp[i],  0.50*dl*wp[i] )) );	
		addMPoint( new CMassPoint(0.05, vshift+Vector3D( -(1.5+i)*dx, -0.50*dl*wp[i],  0.35*dl*wp[i] )) );
		addMPoint( new CMassPoint(0.05, vshift+Vector3D( -(1.5+i)*dx, -0.50*dl*wp[i], -0.35*dl*wp[i] )) );

		//============

		addSpring( new CSpring(AUTODETECT, StiffCoeff, FrictCoeff, mPoint[(i-1)*9], mPoint[i*9]) ); 
		
		addSpring( new CSpring(AUTODETECT, StiffCoeff, FrictCoeff, mPoint[i*9 + 1], mPoint[i*9 + 2]) ); 
		addSpring( new CSpring(AUTODETECT, StiffCoeff, FrictCoeff, mPoint[i*9 + 2], mPoint[i*9 + 3]) ); 
		addSpring( new CSpring(AUTODETECT, StiffCoeff, FrictCoeff, mPoint[i*9 + 3], mPoint[i*9 + 4]) ); 
		addSpring( new CSpring(AUTODETECT, StiffCoeff, FrictCoeff, mPoint[i*9 + 4], mPoint[i*9 + 5]) ); 
		addSpring( new CSpring(AUTODETECT, StiffCoeff, FrictCoeff, mPoint[i*9 + 5], mPoint[i*9 + 6]) ); 
		addSpring( new CSpring(AUTODETECT, StiffCoeff, FrictCoeff, mPoint[i*9 + 6], mPoint[i*9 + 7]) ); 
		addSpring( new CSpring(AUTODETECT, StiffCoeff, FrictCoeff, mPoint[i*9 + 7], mPoint[i*9 + 8]) ); 
		addSpring( new CSpring(AUTODETECT, StiffCoeff, FrictCoeff, mPoint[i*9 + 8], mPoint[i*9 + 1]) ); 

		addSpring( new CSpring(AUTODETECT, StiffCoeff, FrictCoeff, mPoint[i*9 + 1], mPoint[i*9 + 0]) ); 
		addSpring( new CSpring(AUTODETECT, StiffCoeff, FrictCoeff, mPoint[i*9 + 2], mPoint[i*9 + 0]) ); 
		addSpring( new CSpring(AUTODETECT, StiffCoeff, FrictCoeff, mPoint[i*9 + 3], mPoint[i*9 + 0]) ); 
		addSpring( new CSpring(AUTODETECT, StiffCoeff, FrictCoeff, mPoint[i*9 + 4], mPoint[i*9 + 0]) ); 
		addSpring( new CSpring(AUTODETECT, StiffCoeff, FrictCoeff, mPoint[i*9 + 5], mPoint[i*9 + 0]) ); 
		addSpring( new CSpring(AUTODETECT, StiffCoeff, FrictCoeff, mPoint[i*9 + 6], mPoint[i*9 + 0]) ); 
		addSpring( new CSpring(AUTODETECT, StiffCoeff, FrictCoeff, mPoint[i*9 + 7], mPoint[i*9 + 0]) ); 
		addSpring( new CSpring(AUTODETECT, StiffCoeff, FrictCoeff, mPoint[i*9 + 8], mPoint[i*9 + 0]) ); 

		addSpring( new CSpring(AUTODETECT, StiffCoeff, FrictCoeff, mPoint[(i-1)*9 + 1], mPoint[i*9 + 0]) ); 
		addSpring( new CSpring(AUTODETECT, StiffCoeff, FrictCoeff, mPoint[(i-1)*9 + 2], mPoint[i*9 + 0]) ); 
		addSpring( new CSpring(AUTODETECT, StiffCoeff, FrictCoeff, mPoint[(i-1)*9 + 3], mPoint[i*9 + 0]) ); 
		addSpring( new CSpring(AUTODETECT, StiffCoeff, FrictCoeff, mPoint[(i-1)*9 + 4], mPoint[i*9 + 0]) ); 
		addSpring( new CSpring(AUTODETECT, StiffCoeff, FrictCoeff, mPoint[(i-1)*9 + 5], mPoint[i*9 + 0]) ); 
		addSpring( new CSpring(AUTODETECT, StiffCoeff, FrictCoeff, mPoint[(i-1)*9 + 6], mPoint[i*9 + 0]) ); 
		addSpring( new CSpring(AUTODETECT, StiffCoeff, FrictCoeff, mPoint[(i-1)*9 + 7], mPoint[i*9 + 0]) ); 
		addSpring( new CSpring(AUTODETECT, StiffCoeff, FrictCoeff, mPoint[(i-1)*9 + 8], mPoint[i*9 + 0]) ); 
		/**/
		addSpring( new CSpring(AUTODETECT, StiffCoeff/2, FrictCoeff, mPoint[(i-1)*9 + 1], mPoint[i*9 + 2]) ); 
		addSpring( new CSpring(AUTODETECT, StiffCoeff/2, FrictCoeff, mPoint[(i-1)*9 + 2], mPoint[i*9 + 3]) ); 
		addSpring( new CSpring(AUTODETECT, StiffCoeff/2, FrictCoeff, mPoint[(i-1)*9 + 3], mPoint[i*9 + 4]) ); 
		addSpring( new CSpring(AUTODETECT, StiffCoeff/2, FrictCoeff, mPoint[(i-1)*9 + 4], mPoint[i*9 + 5]) ); 
		addSpring( new CSpring(AUTODETECT, StiffCoeff/2, FrictCoeff, mPoint[(i-1)*9 + 5], mPoint[i*9 + 6]) ); 
		addSpring( new CSpring(AUTODETECT, StiffCoeff/2, FrictCoeff, mPoint[(i-1)*9 + 6], mPoint[i*9 + 7]) ); 
		addSpring( new CSpring(AUTODETECT, StiffCoeff/2, FrictCoeff, mPoint[(i-1)*9 + 7], mPoint[i*9 + 8]) ); 
		addSpring( new CSpring(AUTODETECT, StiffCoeff/2, FrictCoeff, mPoint[(i-1)*9 + 8], mPoint[i*9 + 1]) ); 
		/**/
		addSpring( new CSpring(AUTODETECT, StiffCoeff/2, FrictCoeff, mPoint[(i-1)*9 + 2], mPoint[i*9 + 1]) ); 
		addSpring( new CSpring(AUTODETECT, StiffCoeff/2, FrictCoeff, mPoint[(i-1)*9 + 3], mPoint[i*9 + 2]) ); 
		addSpring( new CSpring(AUTODETECT, StiffCoeff/2, FrictCoeff, mPoint[(i-1)*9 + 4], mPoint[i*9 + 3]) ); 
		addSpring( new CSpring(AUTODETECT, StiffCoeff/2, FrictCoeff, mPoint[(i-1)*9 + 5], mPoint[i*9 + 4]) ); 
		addSpring( new CSpring(AUTODETECT, StiffCoeff/2, FrictCoeff, mPoint[(i-1)*9 + 6], mPoint[i*9 + 5]) ); 
		addSpring( new CSpring(AUTODETECT, StiffCoeff/2, FrictCoeff, mPoint[(i-1)*9 + 7], mPoint[i*9 + 6]) ); 
		addSpring( new CSpring(AUTODETECT, StiffCoeff/2, FrictCoeff, mPoint[(i-1)*9 + 8], mPoint[i*9 + 7]) ); 
		addSpring( new CSpring(AUTODETECT, StiffCoeff/2, FrictCoeff, mPoint[(i-1)*9 + 1], mPoint[i*9 + 8]) ); 
		/**/
		table.push_back(neuron_vector);
	}
	//добавили точек и пружин для каждой секции


	//return;

addMuscle( new CMuscle(MStrength,mPoint[  0*9 + 1], mPoint[  1*9 + 1],"VL02") );
	addMuscle( new CMuscle(MStrength,mPoint[  2*9 + 1], mPoint[  1*9 + 1],"VL02") );
	addMuscle( new CMuscle(MStrength,mPoint[  2*9 + 1], mPoint[  3*9 + 1],"VL04") );
	addMuscle( new CMuscle(MStrength,mPoint[  4*9 + 1], mPoint[  3*9 + 1],"VL04") );
	addMuscle( new CMuscle(MStrength,mPoint[  4*9 + 1], mPoint[  5*9 + 1],"VL06") );
	addMuscle( new CMuscle(MStrength,mPoint[  6*9 + 1], mPoint[  5*9 + 1],"VL06") );
	addMuscle( new CMuscle(MStrength,mPoint[  6*9 + 1], mPoint[  7*9 + 1],"VL08") );
	addMuscle( new CMuscle(MStrength,mPoint[  8*9 + 1], mPoint[  7*9 + 1],"VL08") );
	addMuscle( new CMuscle(MStrength,mPoint[  8*9 + 1], mPoint[  9*9 + 1],"VL10") );
	addMuscle( new CMuscle(MStrength,mPoint[ 10*9 + 1], mPoint[  9*9 + 1],"VL10") );
	addMuscle( new CMuscle(MStrength,mPoint[ 10*9 + 1], mPoint[ 11*9 + 1],"VL12") );
	addMuscle( new CMuscle(MStrength,mPoint[ 12*9 + 1], mPoint[ 11*9 + 1],"VL12") );
	addMuscle( new CMuscle(MStrength,mPoint[ 12*9 + 1], mPoint[ 13*9 + 1],"VL14") );
	addMuscle( new CMuscle(MStrength,mPoint[ 14*9 + 1], mPoint[ 13*9 + 1],"VL14") );
	addMuscle( new CMuscle(MStrength,mPoint[ 14*9 + 1], mPoint[ 15*9 + 1],"VL16") );
	addMuscle( new CMuscle(MStrength,mPoint[ 16*9 + 1], mPoint[ 15*9 + 1],"VL16") );
	addMuscle( new CMuscle(MStrength,mPoint[ 16*9 + 1], mPoint[ 17*9 + 1],"VL18") );
	addMuscle( new CMuscle(MStrength,mPoint[ 18*9 + 1], mPoint[ 17*9 + 1],"VL18") );
	addMuscle( new CMuscle(MStrength,mPoint[ 18*9 + 1], mPoint[ 19*9 + 1],"VL20") );
	addMuscle( new CMuscle(MStrength,mPoint[ 20*9 + 1], mPoint[ 19*9 + 1],"VL20") );
	addMuscle( new CMuscle(MStrength,mPoint[ 20*9 + 1], mPoint[ 21*9 + 1],"VL21") );
	addMuscle( new CMuscle(MStrength,mPoint[ 22*9 + 1], mPoint[ 21*9 + 1],"VL21") );
	addMuscle( new CMuscle(MStrength,mPoint[ 22*9 + 1], mPoint[ 23*9 + 1],"VL22") );
	addMuscle( new CMuscle(MStrength,mPoint[ 24*9 + 1], mPoint[ 23*9 + 1],"VL22") );
	addMuscle( new CMuscle(MStrength,mPoint[ 24*9 + 1], mPoint[ 25*9 + 1],"VL23") );
	addMuscle( new CMuscle(MStrength,mPoint[ 26*9 + 1], mPoint[ 25*9 + 1],"VL23") );

	addMuscle( new CMuscle(MStrength,mPoint[  0*9 + 2], mPoint[  1*9 + 2],"VR02") );
	addMuscle( new CMuscle(MStrength,mPoint[  2*9 + 2], mPoint[  1*9 + 2],"VR02") );
	addMuscle( new CMuscle(MStrength,mPoint[  2*9 + 2], mPoint[  3*9 + 2],"VR04") );
	addMuscle( new CMuscle(MStrength,mPoint[  4*9 + 2], mPoint[  3*9 + 2],"VR04") );
	addMuscle( new CMuscle(MStrength,mPoint[  4*9 + 2], mPoint[  5*9 + 2],"VR06") );
	addMuscle( new CMuscle(MStrength,mPoint[  6*9 + 2], mPoint[  5*9 + 2],"VR06") );
	addMuscle( new CMuscle(MStrength,mPoint[  6*9 + 2], mPoint[  7*9 + 2],"VR08") );
	addMuscle( new CMuscle(MStrength,mPoint[  8*9 + 2], mPoint[  7*9 + 2],"VR08") );
	addMuscle( new CMuscle(MStrength,mPoint[  8*9 + 2], mPoint[  9*9 + 2],"VR10") );
	addMuscle( new CMuscle(MStrength,mPoint[ 10*9 + 2], mPoint[  9*9 + 2],"VR10") );
	addMuscle( new CMuscle(MStrength,mPoint[ 10*9 + 2], mPoint[ 11*9 + 2],"VR12") );
	addMuscle( new CMuscle(MStrength,mPoint[ 12*9 + 2], mPoint[ 11*9 + 2],"VR12") );
	addMuscle( new CMuscle(MStrength,mPoint[ 12*9 + 2], mPoint[ 13*9 + 2],"VR14") );
	addMuscle( new CMuscle(MStrength,mPoint[ 14*9 + 2], mPoint[ 13*9 + 2],"VR14") );
	addMuscle( new CMuscle(MStrength,mPoint[ 14*9 + 2], mPoint[ 15*9 + 2],"VR16") );
	addMuscle( new CMuscle(MStrength,mPoint[ 16*9 + 2], mPoint[ 15*9 + 2],"VR16") );
	addMuscle( new CMuscle(MStrength,mPoint[ 16*9 + 2], mPoint[ 17*9 + 2],"VR18") );
	addMuscle( new CMuscle(MStrength,mPoint[ 18*9 + 2], mPoint[ 17*9 + 2],"VR18") );
	addMuscle( new CMuscle(MStrength,mPoint[ 18*9 + 2], mPoint[ 19*9 + 2],"VR20") );
	addMuscle( new CMuscle(MStrength,mPoint[ 20*9 + 2], mPoint[ 19*9 + 2],"VR20") );
	addMuscle( new CMuscle(MStrength,mPoint[ 20*9 + 2], mPoint[ 21*9 + 2],"VR22") );
	addMuscle( new CMuscle(MStrength,mPoint[ 22*9 + 2], mPoint[ 21*9 + 2],"VR22") );
	addMuscle( new CMuscle(MStrength,mPoint[ 22*9 + 2], mPoint[ 23*9 + 2],"VR23") );
	addMuscle( new CMuscle(MStrength,mPoint[ 24*9 + 2], mPoint[ 23*9 + 2],"VR23") );
	addMuscle( new CMuscle(MStrength,mPoint[ 24*9 + 2], mPoint[ 25*9 + 2],"VR24") );
	addMuscle( new CMuscle(MStrength,mPoint[ 26*9 + 2], mPoint[ 25*9 + 2],"VR24") );

	addMuscle( new CMuscle(MStrength,mPoint[  1*9 + 3], mPoint[  2*9 + 3],"VR01") );
	addMuscle( new CMuscle(MStrength,mPoint[  3*9 + 3], mPoint[  2*9 + 3],"VR01") );
	addMuscle( new CMuscle(MStrength,mPoint[  3*9 + 3], mPoint[  4*9 + 3],"VR03") );
	addMuscle( new CMuscle(MStrength,mPoint[  5*9 + 3], mPoint[  4*9 + 3],"VR03") );
	addMuscle( new CMuscle(MStrength,mPoint[  5*9 + 3], mPoint[  6*9 + 3],"VR05") );
	addMuscle( new CMuscle(MStrength,mPoint[  7*9 + 3], mPoint[  6*9 + 3],"VR05") );
	addMuscle( new CMuscle(MStrength,mPoint[  7*9 + 3], mPoint[  8*9 + 3],"VR07") );
	addMuscle( new CMuscle(MStrength,mPoint[  9*9 + 3], mPoint[  8*9 + 3],"VR07") );
	addMuscle( new CMuscle(MStrength,mPoint[  9*9 + 3], mPoint[ 10*9 + 3],"VR09") );
	addMuscle( new CMuscle(MStrength,mPoint[ 11*9 + 3], mPoint[ 10*9 + 3],"VR09") );
	addMuscle( new CMuscle(MStrength,mPoint[ 11*9 + 3], mPoint[ 12*9 + 3],"VR11") );
	addMuscle( new CMuscle(MStrength,mPoint[ 13*9 + 3], mPoint[ 12*9 + 3],"VR11") );
	addMuscle( new CMuscle(MStrength,mPoint[ 13*9 + 3], mPoint[ 14*9 + 3],"VR13") );
	addMuscle( new CMuscle(MStrength,mPoint[ 15*9 + 3], mPoint[ 14*9 + 3],"VR13") );
	addMuscle( new CMuscle(MStrength,mPoint[ 15*9 + 3], mPoint[ 16*9 + 3],"VR15") );
	addMuscle( new CMuscle(MStrength,mPoint[ 17*9 + 3], mPoint[ 16*9 + 3],"VR15") );
	addMuscle( new CMuscle(MStrength,mPoint[ 17*9 + 3], mPoint[ 18*9 + 3],"VR17") );
	addMuscle( new CMuscle(MStrength,mPoint[ 19*9 + 3], mPoint[ 18*9 + 3],"VR17") );
	addMuscle( new CMuscle(MStrength,mPoint[ 19*9 + 3], mPoint[ 20*9 + 3],"VR19") );
	addMuscle( new CMuscle(MStrength,mPoint[ 21*9 + 3], mPoint[ 20*9 + 3],"VR19") );
	addMuscle( new CMuscle(MStrength,mPoint[ 21*9 + 3], mPoint[ 22*9 + 3],"VR21") );
	addMuscle( new CMuscle(MStrength,mPoint[ 23*9 + 3], mPoint[ 22*9 + 3],"VR21") );

	addMuscle( new CMuscle(MStrength,mPoint[  1*9 + 4], mPoint[  2*9 + 4],"DR01") );
	addMuscle( new CMuscle(MStrength,mPoint[  3*9 + 4], mPoint[  2*9 + 4],"DR01") );
	addMuscle( new CMuscle(MStrength,mPoint[  3*9 + 4], mPoint[  4*9 + 4],"DR03") );
	addMuscle( new CMuscle(MStrength,mPoint[  5*9 + 4], mPoint[  4*9 + 4],"DR03") );
	addMuscle( new CMuscle(MStrength,mPoint[  5*9 + 4], mPoint[  6*9 + 4],"DR05") );
	addMuscle( new CMuscle(MStrength,mPoint[  7*9 + 4], mPoint[  6*9 + 4],"DR05") );
	addMuscle( new CMuscle(MStrength,mPoint[  7*9 + 4], mPoint[  8*9 + 4],"DR07") );
	addMuscle( new CMuscle(MStrength,mPoint[  9*9 + 4], mPoint[  8*9 + 4],"DR07") );
	addMuscle( new CMuscle(MStrength,mPoint[  9*9 + 4], mPoint[ 10*9 + 4],"DR09") );
	addMuscle( new CMuscle(MStrength,mPoint[ 11*9 + 4], mPoint[ 10*9 + 4],"DR09") );
	addMuscle( new CMuscle(MStrength,mPoint[ 11*9 + 4], mPoint[ 12*9 + 4],"DR11") );
	addMuscle( new CMuscle(MStrength,mPoint[ 13*9 + 4], mPoint[ 12*9 + 4],"DR11") );
	addMuscle( new CMuscle(MStrength,mPoint[ 13*9 + 4], mPoint[ 14*9 + 4],"DR13") );
	addMuscle( new CMuscle(MStrength,mPoint[ 15*9 + 4], mPoint[ 14*9 + 4],"DR13") );
	addMuscle( new CMuscle(MStrength,mPoint[ 15*9 + 4], mPoint[ 16*9 + 4],"DR15") );
	addMuscle( new CMuscle(MStrength,mPoint[ 17*9 + 4], mPoint[ 16*9 + 4],"DR15") );
	addMuscle( new CMuscle(MStrength,mPoint[ 17*9 + 4], mPoint[ 18*9 + 4],"DR17") );
	addMuscle( new CMuscle(MStrength,mPoint[ 19*9 + 4], mPoint[ 18*9 + 4],"DR17") );
	addMuscle( new CMuscle(MStrength,mPoint[ 19*9 + 4], mPoint[ 20*9 + 4],"DR19") );
	addMuscle( new CMuscle(MStrength,mPoint[ 21*9 + 4], mPoint[ 20*9 + 4],"DR19") );
	addMuscle( new CMuscle(MStrength,mPoint[ 21*9 + 4], mPoint[ 22*9 + 4],"DR21") );	
	addMuscle( new CMuscle(MStrength,mPoint[ 23*9 + 4], mPoint[ 22*9 + 4],"DR21") );
	addMuscle( new CMuscle(MStrength,mPoint[ 23*9 + 4], mPoint[ 24*9 + 4],"DR23") );
	addMuscle( new CMuscle(MStrength,mPoint[ 25*9 + 4], mPoint[ 24*9 + 4],"DR23") );

	addMuscle( new CMuscle(MStrength,mPoint[  0*9 + 5], mPoint[  1*9 + 5],"DR02") );
	addMuscle( new CMuscle(MStrength,mPoint[  2*9 + 5], mPoint[  1*9 + 5],"DR02") );
	addMuscle( new CMuscle(MStrength,mPoint[  2*9 + 5], mPoint[  3*9 + 5],"DR04") );
	addMuscle( new CMuscle(MStrength,mPoint[  4*9 + 5], mPoint[  3*9 + 5],"DR04") );
	addMuscle( new CMuscle(MStrength,mPoint[  4*9 + 5], mPoint[  5*9 + 5],"DR06") );
	addMuscle( new CMuscle(MStrength,mPoint[  6*9 + 5], mPoint[  5*9 + 5],"DR06") );
	addMuscle( new CMuscle(MStrength,mPoint[  6*9 + 5], mPoint[  7*9 + 5],"DR08") );
	addMuscle( new CMuscle(MStrength,mPoint[  8*9 + 5], mPoint[  7*9 + 5],"DR08") );
	addMuscle( new CMuscle(MStrength,mPoint[  8*9 + 5], mPoint[  9*9 + 5],"DR10") );
	addMuscle( new CMuscle(MStrength,mPoint[ 10*9 + 5], mPoint[  9*9 + 5],"DR10") );
	addMuscle( new CMuscle(MStrength,mPoint[ 10*9 + 5], mPoint[ 11*9 + 5],"DR12") );
	addMuscle( new CMuscle(MStrength,mPoint[ 12*9 + 5], mPoint[ 11*9 + 5],"DR12") );
	addMuscle( new CMuscle(MStrength,mPoint[ 12*9 + 5], mPoint[ 13*9 + 5],"DR14") );
	addMuscle( new CMuscle(MStrength,mPoint[ 14*9 + 5], mPoint[ 13*9 + 5],"DR14") );
	addMuscle( new CMuscle(MStrength,mPoint[ 14*9 + 5], mPoint[ 15*9 + 5],"DR16") );
	addMuscle( new CMuscle(MStrength,mPoint[ 16*9 + 5], mPoint[ 15*9 + 5],"DR16") );
	addMuscle( new CMuscle(MStrength,mPoint[ 16*9 + 5], mPoint[ 17*9 + 5],"DR18") );
	addMuscle( new CMuscle(MStrength,mPoint[ 18*9 + 5], mPoint[ 17*9 + 5],"DR18") );
	addMuscle( new CMuscle(MStrength,mPoint[ 18*9 + 5], mPoint[ 19*9 + 5],"DR20") );
	addMuscle( new CMuscle(MStrength,mPoint[ 20*9 + 5], mPoint[ 19*9 + 5],"DR20") );
	addMuscle( new CMuscle(MStrength,mPoint[ 20*9 + 5], mPoint[ 21*9 + 5],"DR22") );
	addMuscle( new CMuscle(MStrength,mPoint[ 22*9 + 5], mPoint[ 21*9 + 5],"DR22") );
	addMuscle( new CMuscle(MStrength,mPoint[ 22*9 + 5], mPoint[ 23*9 + 5],"DR24") );
	addMuscle( new CMuscle(MStrength,mPoint[ 24*9 + 5], mPoint[ 23*9 + 5],"DR24") );

	addMuscle( new CMuscle(MStrength,mPoint[  0*9 + 6], mPoint[  1*9 + 6],"DL02") );
	addMuscle( new CMuscle(MStrength,mPoint[  2*9 + 6], mPoint[  1*9 + 6],"DL02") );
	addMuscle( new CMuscle(MStrength,mPoint[  2*9 + 6], mPoint[  3*9 + 6],"DL04") );
	addMuscle( new CMuscle(MStrength,mPoint[  4*9 + 6], mPoint[  3*9 + 6],"DL04") );
	addMuscle( new CMuscle(MStrength,mPoint[  4*9 + 6], mPoint[  5*9 + 6],"DL06") );
	addMuscle( new CMuscle(MStrength,mPoint[  6*9 + 6], mPoint[  5*9 + 6],"DL06") );
	addMuscle( new CMuscle(MStrength,mPoint[  6*9 + 6], mPoint[  7*9 + 6],"DL08") );
	addMuscle( new CMuscle(MStrength,mPoint[  8*9 + 6], mPoint[  7*9 + 6],"DL08") );
	addMuscle( new CMuscle(MStrength,mPoint[  8*9 + 6], mPoint[  9*9 + 6],"DL10") );
	addMuscle( new CMuscle(MStrength,mPoint[ 10*9 + 6], mPoint[  9*9 + 6],"DL10") );
	addMuscle( new CMuscle(MStrength,mPoint[ 10*9 + 6], mPoint[ 11*9 + 6],"DL12") );
	addMuscle( new CMuscle(MStrength,mPoint[ 12*9 + 6], mPoint[ 11*9 + 6],"DL12") );
	addMuscle( new CMuscle(MStrength,mPoint[ 12*9 + 6], mPoint[ 13*9 + 6],"DL14") );
	addMuscle( new CMuscle(MStrength,mPoint[ 14*9 + 6], mPoint[ 13*9 + 6],"DL14") );
	addMuscle( new CMuscle(MStrength,mPoint[ 14*9 + 6], mPoint[ 15*9 + 6],"DL16") );
	addMuscle( new CMuscle(MStrength,mPoint[ 16*9 + 6], mPoint[ 15*9 + 6],"DL16") );
	addMuscle( new CMuscle(MStrength,mPoint[ 16*9 + 6], mPoint[ 17*9 + 6],"DL18") );
	addMuscle( new CMuscle(MStrength,mPoint[ 18*9 + 6], mPoint[ 17*9 + 6],"DL18") );
	addMuscle( new CMuscle(MStrength,mPoint[ 18*9 + 6], mPoint[ 19*9 + 6],"DL20") );
	addMuscle( new CMuscle(MStrength,mPoint[ 20*9 + 6], mPoint[ 19*9 + 6],"DL20") );
	addMuscle( new CMuscle(MStrength,mPoint[ 20*9 + 6], mPoint[ 21*9 + 6],"DL22") );
	addMuscle( new CMuscle(MStrength,mPoint[ 22*9 + 6], mPoint[ 21*9 + 6],"DL22") );
	addMuscle( new CMuscle(MStrength,mPoint[ 22*9 + 6], mPoint[ 23*9 + 6],"DL24") );
	addMuscle( new CMuscle(MStrength,mPoint[ 24*9 + 6], mPoint[ 23*9 + 6],"DL24") );

	addMuscle( new CMuscle(MStrength,mPoint[  1*9 + 7], mPoint[  2*9 + 7],"DL01") );
	addMuscle( new CMuscle(MStrength,mPoint[  3*9 + 7], mPoint[  2*9 + 7],"DL01") );
	addMuscle( new CMuscle(MStrength,mPoint[  3*9 + 7], mPoint[  4*9 + 7],"DL03") );
	addMuscle( new CMuscle(MStrength,mPoint[  5*9 + 7], mPoint[  4*9 + 7],"DL03") );
	addMuscle( new CMuscle(MStrength,mPoint[  5*9 + 7], mPoint[  6*9 + 7],"DL05") );
	addMuscle( new CMuscle(MStrength,mPoint[  7*9 + 7], mPoint[  6*9 + 7],"DL05") );
	addMuscle( new CMuscle(MStrength,mPoint[  7*9 + 7], mPoint[  8*9 + 7],"DL07") );
	addMuscle( new CMuscle(MStrength,mPoint[  9*9 + 7], mPoint[  8*9 + 7],"DL07") );
	addMuscle( new CMuscle(MStrength,mPoint[  9*9 + 7], mPoint[ 10*9 + 7],"DL09") );
	addMuscle( new CMuscle(MStrength,mPoint[ 11*9 + 7], mPoint[ 10*9 + 7],"DL09") );
	addMuscle( new CMuscle(MStrength,mPoint[ 11*9 + 7], mPoint[ 12*9 + 7],"DL11") );
	addMuscle( new CMuscle(MStrength,mPoint[ 13*9 + 7], mPoint[ 12*9 + 7],"DL11") );
	addMuscle( new CMuscle(MStrength,mPoint[ 13*9 + 7], mPoint[ 14*9 + 7],"DL13") );
	addMuscle( new CMuscle(MStrength,mPoint[ 15*9 + 7], mPoint[ 14*9 + 7],"DL13") );
	addMuscle( new CMuscle(MStrength,mPoint[ 15*9 + 7], mPoint[ 16*9 + 7],"DL15") );
	addMuscle( new CMuscle(MStrength,mPoint[ 17*9 + 7], mPoint[ 16*9 + 7],"DL15") );
	addMuscle( new CMuscle(MStrength,mPoint[ 17*9 + 7], mPoint[ 18*9 + 7],"DL17") );
	addMuscle( new CMuscle(MStrength,mPoint[ 19*9 + 7], mPoint[ 18*9 + 7],"DL17") );
	addMuscle( new CMuscle(MStrength,mPoint[ 19*9 + 7], mPoint[ 20*9 + 7],"DL19") );
	addMuscle( new CMuscle(MStrength,mPoint[ 21*9 + 7], mPoint[ 20*9 + 7],"DL19") );
	addMuscle( new CMuscle(MStrength,mPoint[ 21*9 + 7], mPoint[ 22*9 + 7],"DL21") );	
	addMuscle( new CMuscle(MStrength,mPoint[ 23*9 + 7], mPoint[ 22*9 + 7],"DL21") );
	addMuscle( new CMuscle(MStrength,mPoint[ 23*9 + 7], mPoint[ 24*9 + 7],"DL23") );
	addMuscle( new CMuscle(MStrength,mPoint[ 25*9 + 7], mPoint[ 24*9 + 7],"DL23") );

	addMuscle( new CMuscle(MStrength,mPoint[  1*9 + 8], mPoint[  2*9 + 8],"VL01") );
	addMuscle( new CMuscle(MStrength,mPoint[  3*9 + 8], mPoint[  2*9 + 8],"VL01") );
	addMuscle( new CMuscle(MStrength,mPoint[  3*9 + 8], mPoint[  4*9 + 8],"VL03") );
	addMuscle( new CMuscle(MStrength,mPoint[  5*9 + 8], mPoint[  4*9 + 8],"VL03") );
	addMuscle( new CMuscle(MStrength,mPoint[  5*9 + 8], mPoint[  6*9 + 8],"VL05") );
	addMuscle( new CMuscle(MStrength,mPoint[  7*9 + 8], mPoint[  6*9 + 8],"VL05") );
	addMuscle( new CMuscle(MStrength,mPoint[  7*9 + 8], mPoint[  8*9 + 8],"VL07") );
	addMuscle( new CMuscle(MStrength,mPoint[  9*9 + 8], mPoint[  8*9 + 8],"VL07") );
	addMuscle( new CMuscle(MStrength,mPoint[  9*9 + 8], mPoint[ 10*9 + 8],"VL09") );
	addMuscle( new CMuscle(MStrength,mPoint[ 11*9 + 8], mPoint[ 10*9 + 8],"VL09") );
	addMuscle( new CMuscle(MStrength,mPoint[ 11*9 + 8], mPoint[ 12*9 + 8],"VL11") );
	addMuscle( new CMuscle(MStrength,mPoint[ 13*9 + 8], mPoint[ 12*9 + 8],"VL11") );
	addMuscle( new CMuscle(MStrength,mPoint[ 13*9 + 8], mPoint[ 14*9 + 8],"VL13") );
	addMuscle( new CMuscle(MStrength,mPoint[ 15*9 + 8], mPoint[ 14*9 + 8],"VL13") );
	addMuscle( new CMuscle(MStrength,mPoint[ 15*9 + 8], mPoint[ 16*9 + 8],"VL15") );
	addMuscle( new CMuscle(MStrength,mPoint[ 17*9 + 8], mPoint[ 16*9 + 8],"VL15") );
	addMuscle( new CMuscle(MStrength,mPoint[ 17*9 + 8], mPoint[ 18*9 + 8],"VL17") );
	addMuscle( new CMuscle(MStrength,mPoint[ 19*9 + 8], mPoint[ 18*9 + 8],"VL17") );
	addMuscle( new CMuscle(MStrength,mPoint[ 19*9 + 8], mPoint[ 20*9 + 8],"VL19") );
	addMuscle( new CMuscle(MStrength,mPoint[ 21*9 + 8], mPoint[ 20*9 + 8],"VL19") );
	addMuscle( new CMuscle(MStrength,mPoint[ 21*9 + 8], mPoint[ 22*9 + 8],"VL21") );	
	addMuscle( new CMuscle(MStrength,mPoint[ 23*9 + 8], mPoint[ 22*9 + 8],"VL21") );
	//добавили мускулы

	//Body added
/**/
	addMPoint( new CMassPoint(0.05, vshift+Vector3D( -1 * dx - i * dx, 0 , 0 )) );

	addSpring( new CSpring(AUTODETECT, StiffCoeff, FrictCoeff, mPoint[(i - 1) * 9 + 0], mPoint[i * 9 + 0]) );
	addSpring( new CSpring(AUTODETECT, StiffCoeff, FrictCoeff, mPoint[(i - 1) * 9 + 1], mPoint[i * 9 + 0]) );
	addSpring( new CSpring(AUTODETECT, StiffCoeff, FrictCoeff, mPoint[(i - 1) * 9 + 2], mPoint[i * 9 + 0]) );
	addSpring( new CSpring(AUTODETECT, StiffCoeff, FrictCoeff, mPoint[(i - 1) * 9 + 3], mPoint[i * 9 + 0]) );
	addSpring( new CSpring(AUTODETECT, StiffCoeff, FrictCoeff, mPoint[(i - 1) * 9 + 4], mPoint[i * 9 + 0]) );
	addSpring( new CSpring(AUTODETECT, StiffCoeff, FrictCoeff, mPoint[(i - 1) * 9 + 5], mPoint[i * 9 + 0]) );
	addSpring( new CSpring(AUTODETECT, StiffCoeff, FrictCoeff, mPoint[(i - 1) * 9 + 6], mPoint[i * 9 + 0]) );
	addSpring( new CSpring(AUTODETECT, StiffCoeff, FrictCoeff, mPoint[(i - 1) * 9 + 7], mPoint[i * 9 + 0]) );
	addSpring( new CSpring(AUTODETECT, StiffCoeff, FrictCoeff, mPoint[(i - 1) * 9 + 8], mPoint[i * 9 + 0]) );
/**/
	//добавили хвост
	//Tail added
	//    L O A D    N E U R O N    P O S I T I O N S
	std::ifstream f_pin("celegans302positions.new.txt");
	std::string name;
	std::string name2;
	std::string ctype;
	double x, y, z;
	char type;
	int clr_index = 0;

//	FILE *fout = fopen("celegans280positions.new.txt","wt");

	if(f_pin)
	{
		int i = 0, tresh = 1;//default neuron threshold value
		while(!f_pin.eof())
		{
			f_pin >> name >> x >> y >> z >> clr_index;// >> y;
			//y*=-1.2;
			//x*=0.6;
			//x = x*5795/5351-0.0916;
			//x = (x-0.028124)*1.089611;


			//y = 0;
			type = 'm';
			if(name[name.length()-1]=='L') { type = 'l';/* y =  0.7;*/ }
			if(name[name.length()-1]=='R') { type = 'r';/* y = -0.7;*/ }

			/*

			if(x<0.1) { z *= 0.7;	y*=0.7; } else
			if(x<0.2) { z *= 0.8; y*=0.8; } else
			if(x<0.3) { z *= 0.9; y*=0.9; } else
			if(x>0.9) { z *= 0.7; y*=0.7; }	else
			if(x>0.8) { z *= 0.8; y*=0.8; } else
			if(x>0.7) { z *= 0.9; y*=0.9; }; 
			*/

			//fprintf(fout,"%s\t%8.5f\t%8.5f\t%8.5f\n",name.c_str(),x,y,z);
			addNeuron(name, vshift+Vector3D( dx * ( -1.5 - x*size) , y, -dl/2+(0.045 + z/25) / 0.095 * length), (float)tresh, type,clr_index);
			i++;
		}
		f_pin.close();
	}

//	fclose(fout);

	//    L O A D    N E U R O N   CONNECTIONS
	std::ifstream fconn_in("celegans302connections.txt");
	int value;
	std::string name3;
	
	if(fconn_in)
	{
		int i = 0,j = 0;
		int status = 0;

		fconn_in >> name >> name2 >> ctype >> name3;

		while(!fconn_in.eof())
		{
			fconn_in >> name >> name2 >> ctype >> value;
			/*если тип соединения EJ или S то добоаляем аксон первому нейрону*/
			if((ctype=="EJ")||(ctype[0]=='S'))
			{
				for(i=0;i<(int)neuron.size();i++)
					if(neuron[i]->getName()==name)
					{
						status=1;
						break;
					}

				if(status)
				for(j=0;j<(int)neuron.size();j++)
					if(neuron[j]->getName()==name2)
					{
						status=2;
						break;
					}

				if(status==2)
				{
					neuron[i]->addAxon(neuron[j], value);
					//neuron[i]->
				}
			}
			/**/
		}
		fconn_in.close();
	}
	double weight;

	/**/
	std::ifstream neuroMuscleConnect_in("neuromuscle.txt");
	if(neuroMuscleConnect_in)
	{
		while(!neuroMuscleConnect_in.eof())
		{
			neuroMuscleConnect_in >> name >> name2 >> weight;
			weight = 3.0;
			this->addNeuroMuscleAxonByNames(name, name2, weight);
		}
		neuroMuscleConnect_in.close();
	}
	/**/
}	


CElegans::~CElegans()
{
	unsigned int i;
	for(i = 0; i < mPoint.size(); ++i)
	{
		delete(mPoint[i]);
	}
	for(i = 0; i < spring.size(); ++i)
	{
		delete(spring[i]);
	}
	for(i = 0; i < muscle.size(); ++i)
	{
		delete(muscle[i]);
	}
	for(i = 0; i < neuron.size(); ++i)
	{
		delete(neuron[i]);
	}
};
void CElegans::addMPoint(CMassPoint *i_mPoint)
{
	mPoint.push_back(i_mPoint);
}
void CElegans::addMPoint(const double i_mass, Vector3D &i_pos)
{
	CMassPoint * nPoint = new CMassPoint(i_mass, i_pos);
	mPoint.push_back(nPoint);
}
void CElegans::addMuscle(CMuscle *i_muscle)
{
	muscle.push_back(i_muscle);
}
void CElegans::addMuscle(const double i_strength, CMassPoint *i_p1, CMassPoint *i_p2, std::string i_name)
{
	CMuscle * nMuscle = new CMuscle(i_strength, i_p1, i_p2,i_name);
	muscle.push_back(nMuscle);
}
void CElegans::addSpring(const double i_length, const double i_stiffCoeff, const double i_frictCoeff, CMassPoint *i_p1, CMassPoint *i_p2)
{
	CSpring * nSpring = new CSpring(i_length, i_stiffCoeff, i_frictCoeff, i_p1, i_p2);
	spring.push_back(nSpring);
}
void CElegans::addSpring(CSpring *i_spring)
{
	spring.push_back(i_spring);
}

void CElegans::addNeuron(const std::string i_name, Vector3D & i_pos, const float i_threshold, const char i_type, const int clr_index)
{
	unsigned int i;

	for( i = 0; i < table.size(); i ++)
	{
		if((i_pos.x -  mPoint[9 * i + 8] ->getX()) * (i_pos.x -  mPoint[9 * i + 17] ->getX()) <=0)
		{
			break;
		}
	}
	
	i_pos.y *= 0.3 * length;
	/*
	switch(i_type)
	{
		case 'l':
			i_pos.y =  0.3 * length;
			break;
		case 'r':
			i_pos.y = -0.3 * length;
			break;
		case 'm':
			i_pos.y = 0;
			break;
	}
	*/

	double _x1 = mPoint[9 * i +  8] -> getX();
	double _x2 = mPoint[9 * i + 17] -> getX();
	double _y1 = mPoint[9 * i +  3] -> getY();
	double _y2 = mPoint[9 * i +  8] -> getY();
	double _z1 = mPoint[9 * i +  1] -> getZ();
	double _z2 = mPoint[9 * i +  6] -> getZ();

	double ratioX = (mPoint[9 * i + 8] ->getX() - i_pos.x)/fabs(_x1-_x2);//length;
	double ratioY = (mPoint[9 * i + 3] ->getY() - i_pos.y)/fabs(_y1-_y2);//length;
	double ratioZ = (i_pos.z - mPoint[9 * i + 1] ->getZ())/fabs(_z1-_z2);//length; 
	CNeuron * nNeuron = new CNeuron(i_name, i_pos, i_threshold, ratioX, ratioY, ratioZ, i_type, clr_index);
	neuron.push_back(nNeuron);
	table[i].push_back(nNeuron);


}

int CElegans::addNeuroMuscleAxonByNames(std::string i_neuronName, std::string i_muscleName, double i_weight)
{
	int i, j;
	for(i = 0; i < (int)neuron.size(); ++i)
	{
		if(i_neuronName == neuron[i] ->getName())
		{
			break;
		}
	}

	if(i == neuron.size())	return 1;

	for(j = 0; j < (int)muscle.size(); ++j)
	{
		if(i_muscleName == muscle[j] ->getName())
		{
			neuron[i]->addAxon(muscle[j], i_weight);
		}

		//if(j == muscle.size())	return 1;

	}

	return 0;
}
extern bool mode;
extern bool mode2;
extern bool mode3;
extern bool mode4;
extern bool f9;
extern bool f10;
extern bool f11;

void CElegans::draw()
{
	unsigned int i;

	for(i=0; i< neuron.size(); ++i)
	{
		//std::basic_string <char>::size_type contain;
		//contain = neuron[i]->getName().find("VB");
		//std::basic_string <char>::size_type contain1;
		//contain = neuron[i]->getName().find("DB");
		//if((neuron[i]->getName()[0] == 'V' && neuron[i]->getName()[1] == 'B')||(neuron[i]->getName()[0] == 'D' && neuron[i]->getName()[1] == 'B'))//contain != std::string::npos || contain1 != std::string::npos)
		{
		//	neuron[i]->select();
			neuron[i]->draw();	
		}
	}
	//Neurons Drawed
	//if(!mode) return;
	for(i=0; i< muscle.size(); ++i)
	{
		muscle[i]->draw();
	}
	//Muscle drawed
	if(!mode) return;
	
	
	for(i=0; i< mPoint.size(); ++i)
	{
		if(i!=10)
		mPoint[i]->draw();
	}

	mPoint[10]->select();
	for(i=0; i< spring.size(); ++i)
	{
		spring[i]->draw();
	}
	//	Springs drawed

}

extern float direction;

void CElegans::iteration(const double dt)
{
	unsigned int i;
	float Pi = 3.14159265f;
	
	if(f9){
		neuron[neuron.size() - 1] -> getSignal(1);
	}
	if(f10){
		neuron[neuron.size() - 2] -> getSignal(1);
	}
	if(f11){
		neuron[neuron.size() - 3] -> getSignal(1);
	}

	for(i = 0; i < neuron.size(); ++i)
	{
		neuron[i] ->checkActivity();
	}

	for(i = 0; i < muscle.size(); ++i)
	{
		muscle[i] ->checkActivity();
	}

	//set activity - 0||1 depending on income and thresold

	for(i = 0; i < neuron.size(); ++i)
	{
		neuron[i] ->update();
	}

	
	if(!mode2) 
	{
		return;
	}
	//Neurons 
	/*if(mode2 || mode3)
	{
		for(int k = 0; k < neuron.size(); k++)
		{
			std::basic_string <char>::size_type contain_temp;
			contain_temp = neuron[k]->getName().find("Pse_");
			if(contain_temp != std::string::npos)
			{
				if(neuron[k]->isSelected())
				{
					neuron[k]->unselect();
				}
			}
		}
	}*/

	for(i = 0; i < mPoint.size(); ++i)
	{
		mPoint[i]->init();
		//Forses=0
		mPoint[i]->update();	
	}

	//Friction

	applyFriction();
	
	for(i = 0; i < spring.size(); ++i)
	{
		spring[i]->update();
	}
		
//Spring friction added;
	for(i = 0; i < muscle.size(); ++i)
	{
		muscle[i]->update();
	}

	/**/
	if(mode3)
	{
		float neuro_signal;
		Vector3D springVector;
		Vector3D force;
		std::string m_name;
		
		int cnt8=0;
		int j=0;

		/*
		for(i=0;i<muscle.size();++i)
		{
			m_name = muscle[i]->getName();
			j = atoi(m_name.c_str()+2)-1;

			//if(j>6)
			{
			
			if( (m_name.find("VL")!=-1) || (m_name.find("VR")!=-1) )
			{
				neuro_signal = 2.3*(cos(1.5f*time*direction+(muscle.size()/8-j/2)*0.6f)+0.5f);
				springVector = muscle[i]->getP1Pos() - muscle[i]->getP2Pos();
				springVector.unitize();
				force = -springVector*neuro_signal;
				muscle[i]->applyForceP1(force);
				muscle[i]->applyForceP2(-force);
			}
			if( (m_name.find("DL")!=-1) || (m_name.find("DR")!=-1) )
			{
				neuro_signal = 2.3*(cos(1.5f*time*direction+(muscle.size()/8-j/2)*0.6f+3.14159f)+0.5f);
				springVector = muscle[i]->getP1Pos() - muscle[i]->getP2Pos();
				springVector.unitize();
				force = -springVector*neuro_signal;
				muscle[i]->applyForceP1(force);
				muscle[i]->applyForceP2(-force);
			}

			}
		}
		/**/

		/**/
		for(i=0;i<muscle.size();++i)
		{
			m_name = muscle[i]->getName();
			j = atoi(m_name.c_str()+2)-1;
			if( (m_name.find("VL")!=-1) || (m_name.find("VR")!=-1) )
			{
				neuro_signal = (float)( 0.2f * (sin(3*Pi*j/24 - Pi/2 - 0.5*time*direction)+1.f)/2 );

				
				//neuro_signal*=neuro_signal;
				if((j<2)||(j>20)) neuro_signal*=0.5;
				muscle[i]->activate(neuro_signal);
			}
			if( (m_name.find("DL")!=-1) || (m_name.find("DR")!=-1) )
			{
				neuro_signal = (float)( 0.2f * (sin(3*Pi*j/24 + Pi/2 - 0.5*time*direction)+1.f)/2 );
				//neuro_signal*=neuro_signal;
				if((j<2)||(j>20)) neuro_signal*=0.5;
				muscle[i]->activate(neuro_signal);
			}

			
		}
		/**/

	}

	if(mode4)
	{

		float neuro_signal;
		Vector3D springVector;
		Vector3D force;
		std::string m_name;
		
		int cnt8=0;
		int j=0;


					/*for(int k=0;k<neuron[i]->axon.size();k++)
					{
						neuron[i]->axon[k].send()
					}*/
		
		for(i=0;i<neuron.size();i++)
		{
			std::string n_name = neuron[i]->getName();
			if((n_name[0] == 'V' && n_name[1]=='B')||(n_name[0] == 'D' && n_name[1]=='B'))
			{
				j = atoi(n_name.c_str()+2)-1;

				if(n_name[0] == 'V' && n_name[1]=='B')
				{
					//neuro_signal = (float)(1.f*(cos(1.5f*time*direction+(muscle.size()/8-j/2)*0.6f)+0.5f));
					neuro_signal = (float)( 1.0f * (sin(2*Pi*j/10 - Pi/2 - 0.5*time*direction)+1.f)/2 );
					neuron[i]->getSignal(neuro_signal);

				}

				
				if(n_name[0] == 'D' && n_name[1]=='B')
				{
					//neuro_signal = (float)(1.f*(cos(1.5f*time*direction+(muscle.size()/8-j/2)*0.6f+3.14159f)+0.5f));
					neuro_signal = (float)( 1.0f * (sin(2*Pi*j/6 + Pi/2 - 0.5*time*direction)+1.f)/2 );
					neuron[i]->getSignal(neuro_signal);
				}
				
			}
		}
		

		
	}



	time += dt;//0.002f;
	/**/
	for(i=0; i<mPoint.size();++i)
	{
		mPoint[i]->timeTick(dt);
	}
	neuronPosCorrection();
}

void CElegans::applyFriction()
{
	Vector3D vel;
	Vector3D tangent;
	Vector3D normal;
	Vector3D p1;
	Vector3D p2;
	Vector3D p3;
	//Vector3D force;
	Vector3D dp;

	unsigned int i;

	for(i = 0; i < mPoint.size(); ++i)
	{
		if(mPoint[i]->getPos().z <= 0)
		{

			///
			vel = mPoint[i]->getVel();
			//vel = mPoint[i]->getForce();
			vel.z = 0;
			//force = mPoint[i]->getForce();
			//force.z = 0;
			//good situation check 
			if(1)
			{
				/*
				double dzetta, kappa, dzetta1, kappa1;
				if(i <= 8)
				{
					p1 = mPoint[i]->getPos();
					p1.z = 0;
					tangent = mPoint[i]->getPos() - mPoint[i+9]->getPos();
					tangent.unitize();
					tangent = tangent + p1;
				}
				if(i >= mPoint.size() - 9)
				{
					p1 = mPoint[i]->getPos();
					p1.z = 0;
					tangent = mPoint[i - 9]->getPos() - mPoint[i]->getPos();
					tangent.unitize();
					tangent = tangent + p1;
				}
				if((i > 8)&&(i < mPoint.size() - 9))
				{
					if(vel.length()>0.1)
					{
						i=i;
					}
					p1 = mPoint[i]->getPos();
					p1.z = 0;
					p2 = mPoint[i - 9]->getPos();
					p2.z = 0;
					p3 = mPoint[i + 9]->getPos();
					p3.z = 0;
					double a = (p2 - p3).length(), b = (p1 - p3).length(), c = (p1 - p2).length();
					double cosg = (b*b + c*c - a*a) / (2 * b * c);
					double cosa = sqrt((1 - cosg)/2);
					double d = sqrt(1 + pow(c,2) - 2 * c * cosa);
					dzetta = (pow(d, 2) - 1 - pow(p2.x, 2) + pow(p1.x, 2) - pow(p2.y, 2) + pow(p1.y, 2)) / (2 * (p1.x - p2.x)) - p1.x;
					kappa = (p1.y - p2.y)/(p2.x - p1.x);
					tangent.z = 0;
					
					double discr = pow(p1.y - dzetta * kappa, 2) - (pow(dzetta, 2) + pow(p1.y, 2) - 1) * (pow(kappa, 2)+1);
					if(discr < 0)
					{
						discr = 0;
					}

					tangent.y = (p1.y - dzetta * kappa - sqrt(discr)) / (pow(kappa, 2) + 1);
					tangent.x = dzetta + kappa * tangent.y + p1.x;
				
				}

				dzetta1 = (1 - pow(tangent.x, 2) + pow(p1.x, 2) - pow(tangent.y, 2) + pow(p1.y, 2)) / (2 * (p1.x - tangent.x)) - p1.x;
				kappa1 = (p1.y - tangent.y)/(tangent.x - p1.x);
				normal.z = 0;
				
				double discr = pow(p1.y - dzetta1 * kappa1, 2) - (pow(dzetta1, 2) + pow(p1.y, 2) - 1) * (pow(kappa1, 2)+1);
				if(discr < 0)
				{
					discr = 0;
				}
				normal.y = (p1.y - dzetta1 * kappa1 - sqrt(discr)) / (pow(kappa1, 2) + 1);
				normal.x = dzetta1 + kappa1 * normal.y + p1.x;
				
			
				tangent = tangent - p1;
				normal = normal - p1;


				double v1, v2, cosn, cost;

				if(vel.length() != 0)
				{
					cosn = (pow(normal.length(), 2) + pow(vel.length(), 2) - pow((vel - normal).length(), 2)) / (2 * vel.length() * normal.length());
					cost = (pow(tangent.length(), 2) + pow(vel.length(), 2) - pow((vel - tangent).length(), 2)) / (2 * vel.length() * tangent.length()); 
					v1 = vel.length() * cosn;
					v2 = vel.length() * cost;
				}
				else
				{
					v1 = 0;
					v2 = 0;
				}
				*/

				//if(force.length() != 0)
				{
					if(i <= 8) dp = mPoint[i]->getPos() - mPoint[i+9]->getPos();
					else
					if(i >= mPoint.size() - 9) dp = mPoint[i-9]->getPos() - mPoint[i]->getPos();
					else
					dp = (mPoint[i-9]->getPos() - mPoint[i+9]->getPos())/2;
					dp.z = 0;

					dp.unitize();
					tangent = dp*(vel*dp);
					normal = vel - tangent;



					/*
						tangent = mPoint[i]->getPos() - mPoint[i+9]->getPos();
						tangent.unitize();
						tangent = tangent + p1;
						*/
					


					//Vector3D force = - (normal * v1 *1.0 + tangent * (v2 * 0.01)) * GroundFrictionConstant;
					//*0.05) * GroundFrictionConstant;

					Vector3D force = - normal - tangent/32.0;// K = C_tangent/C_normal = 32 +/- 4, (Berri et. al., 2009);
					mPoint[i]->applyForce(force);
				}

				

			}
		}
	}
}


void CElegans::neuronPosCorrection()
{
	int i;
	for(i = 0; i < (int)table.size(); ++i)
	{
		Vector3D p1,p2,p3,p4,p5,p6,p7,p8, pp1,pp2,pp3,pp4, q1,q2;
		for(unsigned int j = 0; j < table[i].size(); ++j)
		{
			double ratioX = table[i][j] ->getRatioX();
			double ratioY = table[i][j] ->getRatioY();
			double ratioZ = table[i][j] ->getRatioZ();

			/*
			if(table[i][j]->getType() == 'l')
			{
				p1 = (mPoint[9 * i + 8]->getPos() * 0.2 + mPoint[9 * i + 3]->getPos() * 0.8) * (1 - ratioX)\
					+ (mPoint[9 * i + 17]->getPos() * 0.2 + mPoint[9 * i + 12]->getPos() * 0.8) * ratioX;
				p2 = (mPoint[9 * i + 7]->getPos() * 0.2 + mPoint[9 * i + 4]->getPos() * 0.8) * (1 - ratioX)\
					+ (mPoint[9 * i + 16]->getPos() * 0.2 + mPoint[9 * i + 13]->getPos() * 0.8) * ratioX;
				p3 = (mPoint[9 * i + 1]->getPos() * 0.2 + mPoint[9 * i + 2]->getPos() * 0.8) * (1 - ratioZ)\
					+ (mPoint[9 * i + 6]->getPos() * 0.2 + mPoint[9 * i + 5]->getPos() * 0.8) * ratioZ;
				p4 = (mPoint[9 * i + 10]->getPos() * 0.2 + mPoint[9 * i + 11]->getPos() * 0.8) * (1 - ratioZ)\
					+ (mPoint[9 * i + 15]->getPos() * 0.2 + mPoint[9 * i + 14]->getPos() * 0.8) * ratioZ;
			}
			if(table[i][j]->getType() == 'r')
			{
				p1 = (mPoint[9 * i + 8]->getPos() * 0.8 + mPoint[9 * i + 3]->getPos() * 0.2) * (1 - ratioX)\
					+ (mPoint[9 * i + 17]->getPos() * 0.8 + mPoint[9 * i + 12]->getPos() * 0.2) * ratioX;
				p2 = (mPoint[9 * i + 7]->getPos() * 0.8 + mPoint[9 * i + 4]->getPos() * 0.2) * (1 - ratioX)\
					+ (mPoint[9 * i + 16]->getPos() * 0.8 + mPoint[9 * i + 13]->getPos() * 0.2) * ratioX;
				p3 = (mPoint[9 * i + 1]->getPos() * 0.8 + mPoint[9 * i + 2]->getPos() * 0.2) * (1 - ratioZ)\
					+ (mPoint[9 * i + 6]->getPos() * 0.8 + mPoint[9 * i + 5]->getPos() * 0.2) * ratioZ;
				p4 = (mPoint[9 * i + 10]->getPos() * 0.8 + mPoint[9 * i + 11]->getPos() * 0.2) * (1 - ratioZ)\
					+ (mPoint[9 * i + 15]->getPos() * 0.8 + mPoint[9 * i + 14]->getPos() * 0.2) * ratioZ;
			}
			if(table[i][j]->getType() == 'm')
			{
				p1 = (mPoint[9 * i + 8]->getPos() * 0.5 + mPoint[9 * i + 3]->getPos() * 0.5) * (1 - ratioX)\
					+ (mPoint[9 * i + 17]->getPos() * 0.5 + mPoint[9 * i + 12]->getPos() * 0.5) * ratioX;
				p2 = (mPoint[9 * i + 7]->getPos() * 0.5 + mPoint[9 * i + 4]->getPos() * 0.5) * (1 - ratioX)\
					+ (mPoint[9 * i + 16]->getPos() * 0.5 + mPoint[9 * i + 13]->getPos() * 0.5) * ratioX;
				p3 = (mPoint[9 * i + 1]->getPos() * 0.5 + mPoint[9 * i + 2]->getPos() * 0.5) * (1 - ratioZ)\
					+ (mPoint[9 * i + 6]->getPos() * 0.5 + mPoint[9 * i + 5]->getPos() * 0.5) * ratioZ;
				p4 = (mPoint[9 * i + 10]->getPos() * 0.5 + mPoint[9 * i + 11]->getPos() * 0.5) * (1 - ratioZ)\
					+ (mPoint[9 * i + 15]->getPos() * 0.5 + mPoint[9 * i + 14]->getPos() * 0.5) * ratioZ;
			}
			
			q1 = p1 * (1 - ratioZ) + p2 * ratioZ;
			q2 = p3 * (1 - ratioX) + p4 * ratioX;
			table[i][j] ->setPos((q1 + q2) / 2);
			*/

			//ratioX = 0.3;
			//ratioZ = 0.3;

			p1 = (mPoint[9 * i + 8]->getPos() + mPoint[9 * i + 1]->getPos())/2;
			p2 = (mPoint[9 * i + 2]->getPos() + mPoint[9 * i + 3]->getPos())/2;
			p3 = (mPoint[9 * i + 4]->getPos() + mPoint[9 * i + 5]->getPos())/2;
			p4 = (mPoint[9 * i + 6]->getPos() + mPoint[9 * i + 7]->getPos())/2;

			p5 = (mPoint[9 * i +17]->getPos() + mPoint[9 * i +10]->getPos())/2;
			p6 = (mPoint[9 * i +11]->getPos() + mPoint[9 * i +12]->getPos())/2;
			p7 = (mPoint[9 * i +13]->getPos() + mPoint[9 * i +14]->getPos())/2;
			p8 = (mPoint[9 * i +15]->getPos() + mPoint[9 * i +16]->getPos())/2;

			pp1 = p1*(1-ratioX) + p5*(ratioX);
			pp2 = p2*(1-ratioX) + p6*(ratioX);
			pp3 = p3*(1-ratioX) + p7*(ratioX);
			pp4 = p4*(1-ratioX) + p8*(ratioX);

			q1 = pp1 * (1-ratioZ) + pp4 * (ratioZ);
			q2 = pp2 * (1-ratioZ) + pp3 * (ratioZ);

			/*
			if(table[i][j]->getType() == 'r')
			{
					table[i][j]->setPos( q1*0.8 + q2*0.2 );
					if(i< 2) table[i][j]->setPos( q1*0.60 + q2*0.40 ); else
					if(i< 4) table[i][j]->setPos( q1*0.70 + q2*0.30 ); else
					//if(i< 6) table[i][j]->setPos( q1*0.70 + q2*0.30 ); else
					if(i>24) table[i][j]->setPos( q1*0.60 + q2*0.40 ); else
					if(i>22) table[i][j]->setPos( q1*0.70 + q2*0.30 ); //else
					//if(i>20) table[i][j]->setPos( q1*0.55 + q2*0.45 ); 
			}
			else
			if(table[i][j]->getType() == 'l')
			{
					table[i][j]->setPos( q2*0.8 + q1*0.2 );
					if(i< 2) table[i][j]->setPos( q2*0.60 + q1*0.40 ); else
					if(i< 4) table[i][j]->setPos( q2*0.70 + q1*0.30 ); else
					//if(i< 6) table[i][j]->setPos( q2*0.70 + q1*0.30 ); else
					if(i>24) table[i][j]->setPos( q2*0.60 + q1*0.40 ); else
					if(i>22) table[i][j]->setPos( q2*0.70 + q1*0.30 ); //else
					//if(i>20) table[i][j]->setPos( q2*0.55 + q1*0.45 ); 
			}
			else
			if(table[i][j]->getType() == 'm')
			{
					table[i][j]->setPos( q1*0.5 + q2*0.5 );
			}
			*/

			table[i][j]->setPos( q1*ratioY + q2*(1-ratioY) );
			//table[i][j]->setPos( q1*0.5 + q2*(0.5) );
			
		}
	}	
}


void CElegans::rotateWormAroundAnterPosterAxis(float angle)
{
	Vector3D vAPaxis = mPoint[0]->getPos() - mPoint[mPoint.size()-1]->getPos();
	//ort1 = Vector3D::RotateVector1AroundVector2(ort1,Vector3D(0,0,1),alpha);
	Vector3D vNewPos;

	for(int i=1;i<(int)mPoint.size()-2;i++)
	{
		vNewPos = Vector3D::RotateVector1AroundVector2(
			mPoint[i]->getPos()-mPoint[mPoint.size()-1]->getPos(),vAPaxis,angle )
			+ mPoint[mPoint.size()-1]->getPos();
		mPoint[i]->setPos(vNewPos);
	}

	neuronPosCorrection();

	for(int i=0;i<(int)muscle.size();i++)
	{
		muscle[i]->update();
	}

	return;
}

