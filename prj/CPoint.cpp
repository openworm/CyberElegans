#include "CPoint.h"
#include "Physics.h"
#include <windows.h>														// Header File For Windows
#include <gl\gl.h>															// Header File For The OpenGL32 Library
#include <gl\glu.h>															// Header File For The GLu32 Library

extern Vector3D ort1,ort2,ort3,vcenter,pos_rc;
extern float scale;
/// реализайия класса CPoint и CMassPoint
CPoint::CPoint(const Vector3D & i_pos)
{
	pos = i_pos;
}

CPoint::~CPoint()
{

}

CPoint & CPoint::operator =(const CPoint & i_point)
{
	pos = i_point.pos;
	return * this;
}

//Возвращает координаты 
double CPoint::getX()
{
	return pos.x;
}

double CPoint::getY()
{
	return pos.y;
}

double CPoint::getZ()
{
	return pos.z;
}

//Возвращает координаты end
Vector3D CPoint::getPos()
{
	return pos;
}

Vector3D * CPoint::getAPos()
{
	return &pos;
}


void CPoint::setPos(Vector3D & i_pos)
{
	pos = i_pos;
}

CMassPoint::CMassPoint(const double i_mass, const Vector3D &i_pos/*, const Vector3D &i_vel, const Vector3D &i_force*/)\
:CPoint(i_pos)
{	
	mass = i_mass;
	vel   = Vector3D(0,0,0);//i_vel; 
	force = Vector3D(0,0,0);//i_force; 
}

CMassPoint::~CMassPoint()
{

}

CMassPoint & CMassPoint::operator=(const CMassPoint & i_mpoint)
{
	CPoint::operator = (i_mpoint);
	vel = i_mpoint.vel;
	force = i_mpoint.force;
	return *this;
}

void CMassPoint::select()
{
	GLUquadricObj *quadObj; 
	quadObj = gluNewQuadric(); 

	glColor3ub(200, 0, 0);
	
	glPushMatrix();
	Vector3D rpos = (ort1*(pos.x-pos_rc.x) + ort2*(pos.y-pos_rc.y) + ort3*(pos.z-pos_rc.z))*scale + vcenter;
	glTranslated(	rpos.x, rpos.y,	rpos.z    );

	gluSphere(quadObj,0.03f/*0.03f*/*scale,8,8);
	glPopMatrix();

	gluDeleteQuadric(quadObj);
}

void CMassPoint::draw()
{
	GLUquadricObj *quadObj; 
	quadObj = gluNewQuadric();

	glColor3ub(100, 100, 100);//задаем цвет точки
	
	glPushMatrix();//перемещаем пространство замарочки OpenGL
	Vector3D rpos = (ort1*(pos.x-pos_rc.x) + ort2*(pos.y-pos_rc.y) + ort3*(pos.z-pos_rc.z))*scale + vcenter;
	glTranslated(	rpos.x, rpos.y,	rpos.z    );

	gluSphere(quadObj,0.02f*scale,8,8);
	glPopMatrix();//перемещаем пространство обратно замарочки OpenGL

	gluDeleteQuadric(quadObj);
}

void CMassPoint::init()
{
	force = Vector3D(0,0,0);
}

extern int meet_obstacle;
extern float direction;

void CMassPoint::update()
{
	applyForce(Vector3D(0,0,Gravity * mass));

	if (pos.z <= GroundHeight)		
	{
		Vector3D v;					
		v = vel;		
		v.z = 0;
		if (vel.z < 0)							
		{
			v = vel;
			v.x = 0;								
			v.y = 0;
			applyForce(-v * GroundAbsorptionConstant);		
			applyForce(Vector3D(0, 0, GroundRepulsionConstant) * (GroundHeight - pos.z));
		}
	}

	double r = sqrt(pos.x*pos.x + pos.y*pos.y);
	if ( r >= 10)		
	{
		meet_obstacle++;

		if(meet_obstacle>5)
		{
			//direction *= -1;
			meet_obstacle = 0;
		}
		Vector3D v;					
		Vector3D rad_vect = pos;					
		rad_vect.unitize();
		
		v = vel;		
		v.z = 0;

		double radial_component =  v*rad_vect;// +---------------->|--------

		if (radial_component > 0)							
		{
			v = -pos;
			v.z = 0;
			applyForce( v * GroundRepulsionConstant * (r-10));
		}
	}
	applyForce(-vel * AirFrictionCoefficient);
//Air friction added
}

void CMassPoint::applyForce(const Vector3D & i_uppforce)
{
	force += i_uppforce; 
}
void CMassPoint::timeTick(const double dt) //Iteration ???
{
	pos += vel * dt;
	vel += (force / mass) * dt;
}


Vector3D CMassPoint::getVel()
{
	return vel;
}

Vector3D CMassPoint::getForce()
{
	return force;
}

double CMassPoint::getMass()
{
	return mass;
}