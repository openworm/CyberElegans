#include "CConnector.h"
#include "Physics.h"
#include <windows.h>														// Header File For Windows
#include <gl\gl.h>															// Header File For The OpenGL32 Library
#include <gl\glu.h>															// Header File For The GLu32 Library

extern Vector3D ort1,ort2,ort3,vcenter,pos_rc;
extern float scale;

CConnector::CConnector(CMassPoint * i_p1, CMassPoint * i_p2)
{
	status = 1;
	p1 = i_p1;
	p2 = i_p2;
}

CConnector & CConnector::operator=(const CConnector & i_Connector)
{
	p1 = i_Connector.p1;
	p2 = i_Connector.p2;

	return * this;
}

Vector3D CConnector::getP1Pos()
{
	return p1->getPos();
}
Vector3D CConnector::getP2Pos()
{
	return p2->getPos();
}

void CConnector::applyForceP1(Vector3D i_force)
{
	p1->applyForce(i_force);
}
void CConnector::applyForceP2(Vector3D i_force)
{
	p2->applyForce(i_force);
}

/*
class CSpring
{
public:
	float length0;
	float stiffnessCoefficient;
	float frictionCoefficient;
	CMassPoint* p1; 
	CMassPoint* p2; 

	CSpring(float l0, float stiffCoeff, float frictCoeff, CMassPoint *p1, CMassPoint *p2)
	{
		this->length0 = l0;
		if(l0 <= 0)	length0 = (p1->pos - p2->pos).length()*fabs(l0);
		this->stiffnessCoefficient = stiffCoeff;
		this->frictionCoefficient = frictCoeff;
		this->p1 =  p1;
		this->p2 =  p2;
	}

	CSpring operator=(const CSpring &newspring) 
	{ 
		this->p1 = newspring.p1;
		this->p2 = newspring.p2;

		return *this;
	}
};
*/

CSpring::CSpring(const double i_length, const double i_stiffnessCoefficient, const double i_frictionCoefficient,\
						 CMassPoint *i_p1, CMassPoint *i_p2):CConnector(i_p1, i_p2)
{
	length = i_length;
	if(i_length <= 0)// AUTODETECT
		length = (i_p1->getPos() - i_p2->getPos()).length()*fabs(i_length);
	
	stiffnessCoefficient = i_stiffnessCoefficient;
	frictionCoefficient = i_frictionCoefficient;
}

CSpring & CSpring::operator=(const CSpring &i_Spring)
{
	CConnector::operator =(i_Spring);
	length = i_Spring.length;
	stiffnessCoefficient = i_Spring.stiffnessCoefficient;
	frictionCoefficient = i_Spring.frictionCoefficient;
	return * this;
}

void CSpring::draw()
{
	
	
	//Vector3D rpos = (ort1*p1->getX() + ort2*p1->getY() + ort3*(GroundHeight-0.1))*scale + vcenter;
	Vector3D rpos  = (ort1*(p1->getX()-pos_rc.x) + ort2*(p1->getY()-pos_rc.y) + ort3*(GroundHeight-0.1-pos_rc.z))*scale + vcenter;
	Vector3D rpos2 = (ort1*(p2->getX()-pos_rc.x) + ort2*(p2->getY()-pos_rc.y) + ort3*(GroundHeight-0.1-pos_rc.z))*scale + vcenter;
	//rpos = (ort1*p2->getX() + ort2*p2->getY() + ort3*(GroundHeight-0.1))*scale + vcenter;
/*
	glLineWidth(5);
	glBegin(GL_LINES);
	glColor3ub(150, 150, 150);
	glVertex3d(	rpos.x,rpos.y,rpos.z);
	glVertex3d(	rpos2.x,rpos2.y,rpos2.z);
	glEnd();

	glLineWidth(3);
	glBegin(GL_LINES);
	glColor3ub(80, 80, 80);
	glVertex3d(	rpos.x,rpos.y,rpos.z);
	glVertex3d(	rpos2.x,rpos2.y,rpos2.z);
	glEnd();
*/
	glLineWidth(2);
	glBegin(GL_LINES);
	glColor3ub(140, 100, 55);
	glVertex3d(	rpos.x,rpos.y,rpos.z);
	glVertex3d(	rpos2.x,rpos2.y,rpos2.z);
	glEnd();
	//shadow drawed

	rpos2 = rpos;

	glLineWidth(2);

	if(status == 0)
	{
		glColor3ub(255, 0, 0);
	}
	else
	{
		//glColor3ub(255, 225, 0);
		glColor3ub(75,75, 75);
	}

	glBegin(GL_LINES);
	
				//rpos = (ort1*p1->getX() + ort2*p1->getY() + ort3*p1->getZ())*scale + vcenter;
				rpos = (ort1*(p1->getX()-pos_rc.x) + ort2*(p1->getY()-pos_rc.y) + ort3*(p1->getZ()-pos_rc.z))*scale + vcenter;
	
				glVertex3d(	rpos.x, rpos.y, rpos.z );

				//rpos = (ort1*p2->getX() + ort2*p2->getY() + ort3*p2->getZ())*scale + vcenter;
				rpos = (ort1*(p2->getX()-pos_rc.x) + ort2*(p2->getY()-pos_rc.y) + ort3*(p2->getZ()-pos_rc.z))*scale + vcenter;
				glVertex3d(	rpos.x, rpos.y, rpos.z );
	glEnd();

	//Springs drawed

	
}

void CSpring::update()
{
	if(status == 1)
	{
		Vector3D springVector;
		springVector = p1->getPos() - p2->getPos();

		double r = springVector.length();

		if((r <= 0.05)||(r >= 2))
		{
			status = 0;
			return;
		}
//Now Spring can be broken;
		
	
		Vector3D force;	
		if(r != 0)
		{
			force = (springVector / r) * (r - length) * (- stiffnessCoefficient);
		}
			
		force += -(p1->getVel() - p2->getVel()) * frictionCoefficient;

		p1->applyForce(force);
		p2->applyForce(-force);

	/*
		//Ground 
		Vector3D p1Pos;
		Vector3D p2Pos;
		Vector3D p1Vel;
		Vector3D p2Vel;
		Vector3D slip;
		double coeff;

		p1Pos = p1->getPos();
		p2Pos = p2->getPos();
		p1Vel = p1->getVel();
		p2Vel = p2->getVel();
		slip = p2Pos - p1Pos;
	
		if(p1Pos.z <= 0)
		{
			p1Vel.z = 0;
			slip.z = 0;	
			if(p1Vel.scaleM(slip) / p1Vel.meas() / slip.meas() > 0.95)
			{
				p1->applyForce(- p1Vel * (0 * GroundFrictionConstant));					
			}
			else
			{
				p1->applyForce(- p1Vel * GroundFrictionConstant);
			}
		
		}

		if(p2Pos.z <= 0)
		{
			p2Vel.z = 0;
			slip.z = 0;
			if(p2Vel.scaleM(slip) / p2Vel.meas() / slip.meas() < - 0.95)
			{
				p2->applyForce(- p2Vel * (0 * GroundFrictionConstant));					
			}
			else
			{
				p2->applyForce(- p2Vel * GroundFrictionConstant);
			}

		}
*/


	}
}
CMuscle::CMuscle(const double i_strength, CMassPoint *i_p1, CMassPoint *i_p2, std::string i_name):CConnector(i_p1, i_p2), CSynapse(1.0,i_name)
{
	strength = i_strength;
	status = 1;
	//mclass_id = i_mclass_id;
	name = i_name;
	length = (i_p1->getPos() - i_p2->getPos()).length();
	//this->
	//synapse->name = i_name;
	drawPos = (i_p1->getPos()*2 + i_p2 ->getPos()*98) / 100;
}

CMuscle & CMuscle::operator=(const CMuscle &i_muscle)
{

	CConnector::operator =(i_muscle);
	strength = i_muscle.strength;
	return * this;
}

extern bool mode;

void CMuscle::draw()
{

	double pi = 3.14159;
//	double colorCoef;
	GLUquadricObj *quadObj; 
	quadObj = gluNewQuadric(); 

	/*
	if(strength >= income - threshold)
	{
		colorCoef = (income > threshold) * (income - threshold) / strength;
	}
	else
	{
		colorCoef = 1;
	}
	*/

	//if(activity == 1)
	//{
		//glColor3ub(250 / MStrength * (income - threshold) , 0, 0);
	//}
	//else 
	//{
	//	glColor3ub(50 + (int)(150 *colorCoef), 70, 70);
	//}

	float act;
	int r,g,b;
	r = 80;
	g = 80;
	b = 80;
	/*
	double dl = (getP1Pos()-getP2Pos()).length() - getLength0();
	
	if( (act = getActivity())<=1 || dl < 0.0) 
	{

		r += (int)((float)r*2.f*act);
	}
	*/
	r += (255-80)*min(getActivity(),1.f);

	/**/
	//double dl = (getP1Pos()-getP2Pos()).length() - getLength0();
	//if( dl < 0.0) g+= -dl*300;
	/*if( dl < 0.0) { r+= -(int)(dl*500); g+= (int)dl*200; b += (int)dl*200; }
	if(r>255) r = 255;
	if(g<0) g = 0;
	if(b<0) b = 0;*/
	/**/
		//(i_p1->getPos() - i_p2->getPos()).length()
	
	glColor3ub(r,g,b);
	//Vector3D rp1 = (ort1*p1->getX() + ort2*p1->getY() + ort3*p1->getZ())*scale + vcenter;
	Vector3D rp1 = (ort1*(p1->getX()-pos_rc.x) + ort2*(p1->getY()-pos_rc.y) + ort3*(p1->getZ()-pos_rc.z))*scale + vcenter;
	//Vector3D rp2 = (ort1*p2->getX() + ort2*p2->getY() + ort3*p2->getZ())*scale + vcenter;
	Vector3D rp2 = (ort1*(p2->getX()-pos_rc.x) + ort2*(p2->getY()-pos_rc.y) + ort3*(p2->getZ()-pos_rc.z))*scale + vcenter;

	Vector3D p = rp2 - rp1;
	double h = p.length();// /2;

	glPushMatrix();
	
	glTranslated(rp1.x, rp1.y, rp1.z);
	glRotated(180.0f*(double)atan2(p.x,p.z)/pi,0,1,0); 
	glRotated(-180.0f*(double)atan2(p.y,sqrt(p.x*p.x+p.z*p.z))/pi,1,0,0); 
	
	if(mode==0)
		gluCylinder(quadObj, 0, 0.02*scale, h, 16, 16);
	else
		gluCylinder(quadObj, 0, 0.05*scale, h, 16, 16);

	glPopMatrix();

	//p = p1->getPos() - p2->getPos();
	/*
	p = rp1 - rp2;

	glPushMatrix();

	glTranslated(rp2.x, rp2.y, rp2.z);
	glRotated(180.0f*(double)atan2(p.x,p.z)/pi,0,1,0); 
	glRotated(-180.0f*(double)atan2(p.y,sqrt(p.x*p.x+p.z*p.z))/pi,1,0,0);

	gluCylinder(quadObj, 0, 0.05*scale, h, 16, 16);

	glPopMatrix();
	*/

	gluDeleteQuadric(quadObj);
}

void CMuscle::update()
{
	
	drawPos = (p1->getPos()*2 + p2->getPos()*98) / 100;

	//if(activity>0)
	if(income>0)
	{
		Vector3D musclforce = p1->getPos() - p2->getPos();
		musclforce.unitize();
		
		if(income >= threshold)
		{
			income = threshold;
		}

		p1->applyForce(-musclforce * (5*income));
		p2->applyForce( musclforce * (5*income));

		income *= 0.9;

		/*if(strength >= income - threshold)
		{
			if(income >= threshold)
			{
				income = threshold;
			}
			p1->applyForce(-musclforce * (income - threshold));
			p2->applyForce(musclforce * (income - threshold));
		}
		else
		{
			p1->applyForce(-musclforce * strength);
			p2->applyForce(musclforce * strength);
		}*/
	}
}

void CMuscle::activate(float value)
{
	income  += value;
	//activity = value;
}

void CMuscle::disactivate()
{
	//activity = 0;
}

std::string CMuscle::getName()
{
	return name;
}

double CMuscle::getLength0()
{
	return length;
}