#include "CNeuron.h"
#include <iostream>
#include <windows.h>														// Header File For Windows
#include <gl\gl.h>															// Header File For The OpenGL32 Library
#include <gl\glu.h>															// Header File For The GLu32 Library
#include "graphics.h"
#include "gl\glaux.h"
#include <string>

extern Vector3D ort1,ort2,ort3,vcenter,pos_rc;
extern float scale;

extern GLvoid glPrint(GLint x, GLint y, const char *string, ...);		// Where The Printing Happens
extern Vector3D vbeg,vend;

					//     0     1    2   3    4    5   6
 					//  def.  VB.. DB.. AVB  PVC  DVA
int neuron_color_r[] = {  80, 170, 000, 150, 100,  50 };
int neuron_color_g[] = { 100, 170, 128, 000, 000, 000 };
int neuron_color_b[] = { 100, 000, 255,  50, 100, 150 };

extern bool key_a;
extern bool key_r;


CSynapse::CSynapse(float i_threshold, const std::string &_name)
{
	selected = false;
	//activity = 0;
	income = 0;
	threshold = i_threshold;
	name = _name;
}

CSynapse::~CSynapse()
{

}

void CSynapse::getSignal(float i_signal)
{
	income += i_signal*0.1f;//i_signal;
}

void CSynapse::checkActivity()
{
	/*
	if(income >= threshold)
	{
		activity = 1;
	}
	else
	{
		activity = 0;
	}
	*/
	//activity = min(income,1.f);
	income = min(income,1.f);

	income *= 0.9f;
}


void CSynapse::select(){
	selected = true;
}
void CSynapse::unselect(){
	selected = false;
}
float CSynapse::getActivity()
{
	//return activity;
	return income;
}

bool CSynapse::isSelected()
{
	return selected;
}

Vector3D CSynapse::getDrawPos()
{
	//this->
	//CNeuron::getDrawPos();
	//return this->getDrawPos();
	return drawPos;
}


std::string CSynapse::getName()
{
	return name;
}


CAxon::CAxon(const double i_weight, CSynapse * i_synapse)
{
	weight = i_weight;
	synapse = i_synapse;
}


Vector3D CAxon::getPos()
{

	return synapse->getDrawPos();
}

CAxon::~CAxon()
{
}

void CAxon::send(float senderActivity)
{
	synapse->getSignal(senderActivity/**weight*/);	
}

std::string CAxon::getTargName()
{
	return synapse->getName();
}
bool CAxon::isTargSelected()
{
	return synapse ->isSelected();
}
CNeuron::CNeuron(const std::string i_name, const Vector3D & i_pos, const float i_threshold, const double i_ratioX, const double i_ratioY, const double i_ratioZ, const char i_type,const int clr_index):CPoint(i_pos), CSynapse(i_threshold,i_name)
{
	name = i_name;
	type = i_type;
	ratioX = i_ratioX;
	ratioY = i_ratioY;
	ratioZ = i_ratioZ;
	clrIndex = clr_index;
	axon.reserve(20); 
}

CNeuron::~CNeuron()
{
	for(unsigned int i = 0; i < axon.size(); ++i)
	{
		delete(axon[i]);
	}
}

void CNeuron::addAxon(CAxon * i_axon)
{
	axon.push_back(i_axon);
}

void CNeuron::addAxon(CSynapse * i_synapse, const double i_weight)
{
	addAxon(new CAxon(i_weight, i_synapse));
}


extern GLvoid glPrint2(GLdouble x, GLdouble y, GLdouble z, GLubyte r, GLubyte g, GLubyte b, const char *string, ...);

void CNeuron::draw()
{
	GLUquadricObj *quadObj; 
	quadObj = gluNewQuadric();
	int highlight = 0;

	int r,g,b;
	int pseudoneuron = 0;
	int clr_index = getColorIndex();
	float act;

	//activity

	r = neuron_color_r[clr_index];
	g = neuron_color_g[clr_index];
	b = neuron_color_b[clr_index];

	if( (act = getActivity())<1) 
	{
		r /= 3;
		g /= 3;
		b /= 3;

		r += (int)((float)r*2.f*act);
		g += (int)((float)g*2.f*act);
		b += (int)((float)b*2.f*act);
	}

	/*
	if(selected) 
	{
		r = min(255,r*5/3);
		g = min(255,g*5/3);
		b = min(255,b*5/3);
	}
	*/

	glColor3ub(r, g, b);
	 //glColor3ub(r, g, b);

/**/
	/*
	if(name[0]=='A')
	if(name[1]=='S')
	if((name[2]=='0')||(name[2]=='1'))
	{
		//highlight = 1;
		glColor3ub(r=100, g=25, b=0);
		if(selected||activity>0)
			glColor3ub(r=175, g=50, b=0);
	}
	/**/
/*
	if((name[0]=='V'))
	if(name[1]=='B')
	{
		//highlight = 1;
		glColor3ub(r=0, g=100, b=100);
		if(selected||activity>0)
			glColor3ub(r=0, g=150, b=150);
	}

	if((name[0]=='D'))
	if(name[1]=='B')
	{
		//highlight = 1;
		glColor3ub(r=100, g=100, b=0);
		if(selected||activity>0)
			glColor3ub(r=150, g=150, b=0);
	}
	*/

	/*
	if(name[0]=='A')
	if(name[1]=='V')
	if(name[2]=='B')
	{
		//highlight = 1;
		glColor3ub(r=100, g=0, b=0);
		if(selected||activity>0)
			glColor3ub(r=150, g=0, b=0);
	}
	*/
/**/
	/*
	if((name[0]=='V')||(name[0]=='D'))
	if(name[1]=='D')
	{
		//highlight = 1;
		glColor3ub(r=0, g=75, b=125);
		if(selected||activity>0)
			glColor3ub(r=0, g=125, b=200);
	}

	if((name[0]=='V')||(name[0]=='D'))
	if(name[1]=='A')
	{
		//highlight = 1;
		glColor3ub(r=100, g=100, b=0);
		if(selected||activity>0)
			glColor3ub(r=150, g=150, b=00);
	}

	/**/

	Vector3D rpos = (ort1*(pos.x-pos_rc.x) + ort2*(pos.y-pos_rc.y) + ort3*(pos.z-pos_rc.z))*scale + vcenter;
	Vector3D rpos2 = (ort1*(pos.x-pos_rc.x) + ort2*(pos.y-pos_rc.y) + ort3*(pos.z-pos_rc.z+0.1))*scale + vcenter;
	Vector3D p = rpos2 - rpos;
	double h = p.length();
	double pi= 3.14159;
	
	/**/
	if( 
		((name[0]=='D')&&(name[1]=='V')&&(name[2]=='A')) ||
		((name[0]=='A')&&(name[1]=='V')&&(name[2]=='B')) ||
		((name[0]=='P')&&(name[1]=='V')&&(name[2]=='C')) //||
		//((name[0]=='A')&&(name[1]=='V')&&(name[2]=='A')) //||
		//((name[0]=='S')&&(name[1]=='A')&&(name[2]=='B')) ||
		//((name[0]=='A')&&(name[1]=='V')&&(name[2]=='E')) ||
		//((name[0]=='A')&&(name[1]=='V')&&(name[2]=='D')) 
	)


	{
		//highlight = 1;
			//selected = 1;
		/*
		glColor3ub(r=100, g=0, b=100);
		if(selected||activity>0)
			glColor3ub(r=150, g=0, b=150);
		*/
	}
	/**/

	if((name[0]=='P')&&(name[1]=='s')&&(name[2]=='e'))
		pseudoneuron = 1;

	
	glPushMatrix();
	glTranslated(	rpos.x,	rpos.y,	rpos.z    );
	glRotated(180.0f*(double)atan2(p.x,p.z)/pi,0,1,0); 
	glRotated(-180.0f*(double)atan2(p.y,sqrt(p.x*p.x+p.z*p.z))/pi,1,0,0); 

	if(pseudoneuron)
	{
		gluSphere(quadObj,0.9*neuron_radius*scale,8,8);

		if(selected)
		{
			glColor3ub(0, 100, 0);
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			gluSphere(quadObj,1.2*neuron_radius*scale,6,6);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glColor3ub(r, g, b);
		}
		//glutWireSphere(quadObj,0.9*neuron_radius*scale,8,8);
	}
	else
	{
		gluSphere(quadObj,2*neuron_radius*scale,8,8);

		if(selected)
		{
			glColor3ub(0, 100, 0);
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			gluSphere(quadObj,2.5*neuron_radius*scale,6,6);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glColor3ub(r, g, b);
		}
	}
	glPopMatrix();

	

/**/
	if(selected)
	{
		//if(name.length()<5)
		glPrint2(rpos.x,rpos.y,rpos.z,(byte)min(255,r*1.7),(byte)min(255,g*1.7),(byte)min(255,b*1.7),name.c_str());
	}

	/*
	if(highlight)
	if(name.length()<5)
	glPrint2(rpos.x,rpos.y,rpos.z,(byte)min(255,r*1.7),(byte)min(255,g*1.7),(byte)min(255,b*1.7),name.c_str());	
	*/
/**/
	
	

	int n_axon_branches=0;
	rpos2 = Vector3D(0,0,0);
	std::string tmp_name;

	//if(selected)
	//if(name.length()>5)
	{
	
	/*
	for(unsigned int j = 0;  j < axon.size(); ++j)
	{
			rpos2+= (ort1*(axon[j]->getPos().x - pos_rc.x)
				   + ort2*(axon[j]->getPos().y - pos_rc.y)
				   + ort3*(axon[j]->getPos().z - pos_rc.z))*scale + vcenter;// точка от кт идут ветви к разным нейронам
			n_axon_branches++;
		
	}
	
	
	//glEnd();
	if(n_axon_branches>0)
	{
		rpos2 /= n_axon_branches;
		glPushMatrix();
		glColor3ub(r, g, b);
		
		glTranslated(	rpos2.x,	rpos2.y,	rpos2.z    );
		gluSphere(quadObj,0.7f*neuron_radius*scale ,8,8);
		glPopMatrix();
	}

	glColor3ub(min(255,r*1.0),min(255,g*1.0),min(255,b*1.0));
	if(n_axon_branches>0)
	{
		//rpos2 /= n_axon_branches;
		p = rpos2 - rpos;
		h = p.length();
	
		glPushMatrix();
		glTranslated(rpos.x, rpos.y, rpos.z);
		glRotated(180.0f*(double)atan2(p.x,p.z)/pi,0,1,0); 
		glRotated(-180.0f*(double)atan2(p.y,sqrt(p.x*p.x+p.z*p.z))/pi,1,0,0); 
		gluCylinder(quadObj, 0.005*scale, 0.005*scale, h, 16, 16);
		glPopMatrix();

		rpos = rpos2;
	}
	*/

	std::string tmp_name;

	int link_with_muscle = 0;

	for(unsigned int j = 0;  j < axon.size(); ++j)
	{
		link_with_muscle = 0;
		
		if((axon[j]->getTargName()[0]=='D')||(axon[j]->getTargName()[0]=='V'))
		if((axon[j]->getTargName()[1]=='R')||(axon[j]->getTargName()[1]=='L'))
		if((axon[j]->getTargName()[2]=='0')||(axon[j]->getTargName()[2]=='1')||(axon[j]->getTargName()[2]=='2'))
		{
			link_with_muscle=1;

			if((name[0]=='P')&&(name[1]=='s'&&(name[2]=='e')))
			if(name[3]=='_')
			if(((name[4]=='V')||(name[4]=='D'))&&(name[5]=='B')) link_with_muscle = 2;
		}

		if((link_with_muscle!=1)||(selected))
		{

		rpos2 = (ort1*(axon[j]->getPos().x - pos_rc.x)
			   + ort2*(axon[j]->getPos().y - pos_rc.y)
			   + ort3*(axon[j]->getPos().z - pos_rc.z))*scale + vcenter;

		p = rpos2 - rpos;
		h = p.length();
		pi = 3.14159;	
		
	/*	if(axon[j]->getTargName().length()<5)
			glPrint2(rpos2.x,rpos2.y,rpos2.z,150,150,150,axon[j] ->getTargName().c_str());*/
		
		//glPrint2(rpos2.x,rpos2.y,rpos2.z,150,150,150,axon[j] ->getTargName().c_str());
		glColor3ub((GLubyte)min(255,r*0.7),(GLubyte)min(255,g*0.7),(GLubyte)min(255,b*0.7));

		tmp_name = axon[j]->getTargName();

		if( 
			((tmp_name[0]=='D')&&(tmp_name[1]=='L'))||
			((tmp_name[0]=='D')&&(tmp_name[1]=='R'))|| 
			((tmp_name[0]=='V')&&(tmp_name[1]=='L'))|| 
			((tmp_name[0]=='V')&&(tmp_name[1]=='R')) 
		  )
		{
			glColor3ub((GLubyte)min(255,r*0.75),(GLubyte)min(255,g*0.75),(GLubyte)min(255,b*0.75));
		}

		glPushMatrix();
		glTranslated(rpos.x, rpos.y, rpos.z);
		glRotated(180.0f*(double)atan2(p.x,p.z)/pi,0,1,0); 
		glRotated(-180.0f*(double)atan2(p.y,sqrt(p.x*p.x+p.z*p.z))/pi,1,0,0); 
		gluCylinder(quadObj, 0.0025*scale, 0.001*scale, h, 16, 16);
		glPopMatrix();
		}
	}
	
	}//////////////////////////////////////

	gluDeleteQuadric(quadObj);

	return;
	///////////////////////////////////////////////////

	rpos = (ort1*(pos.x-pos_rc.x) + ort2*(pos.y-pos_rc.y) + ort3*(pos.z-pos_rc.z))*scale + vcenter;


	
	for(unsigned int j = 0;  j < axon.size(); ++j)
	{
		if(axon[j]->isTargSelected())
		{ 
			rpos2 = (ort1*(axon[j]->getPos().x - pos_rc.x)
				   + ort2*(axon[j]->getPos().y - pos_rc.y)
				   + ort3*(axon[j]->getPos().z - pos_rc.z))*scale + vcenter;

			p = rpos2 - rpos;
			h = p.length();
			glPrint2(rpos.x,rpos.y,rpos.z,150,150,150,name.c_str());
		
			glPushMatrix();
			glColor3ub(50, 50, 50);	
			glTranslated(rpos.x, rpos.y, rpos.z);
			glRotated(180.0f*(double)atan2(p.x,p.z)/pi,0,1,0); 
			glRotated(-180.0f*(double)atan2(p.y,sqrt(p.x*p.x+p.z*p.z))/pi,1,0,0); 
			gluCylinder(quadObj, 0.005*scale, 0.001*scale, h, 16, 16);
			glPopMatrix();
		}
	}
	gluDeleteQuadric(quadObj);
}

//int cnt=5;

void CNeuron::update()
{
	//return;
	drawPos = pos;

	std::string neuron_name = getName();

	if(key_a && selected) 
	{
		//activity = 1.f;
		//if(cnt>0)
		{
			//if(getName().length()<7)
			//{
				income += 0.1f;
			//	cnt--;
			//}
		}
		income = min(income,1.f);
		//selected = false;
		/*
		if(income>0.99) 
		{
			cnt--;
			if(cnt=0);
			key_a = 0;
		}*/

	}

	/*
	if(selected) 
	{
		FILE *f = fopen("act_log.txt","a+");
		//income += 0.0f;
		fprintf(f,"%s\t%f\t",neuron_name.c_str(),income);
		if(getName().length()>7) fprintf(f,"\n");
		fclose(f);
	}*/
	

	std::string tmp_name;

	if(income/*activity*/ > 0)
	{
		for(unsigned int j = 0; j < axon.size(); ++j)
		{
			tmp_name = axon[j]->getTargName();

			/*
			if( 
				((tmp_name[0]=='D')&&(tmp_name[1]=='L'))||
				((tmp_name[0]=='D')&&(tmp_name[1]=='R'))|| 
				((tmp_name[0]=='V')&&(tmp_name[1]=='L'))|| 
				((tmp_name[0]=='V')&&(tmp_name[1]=='R')) 
			  )*/
			{
				axon[j]->send(1.0*getActivity());
				//j = j;
			}
		}
	}
}

char CNeuron::getType()
{
	return type;
}

double CNeuron::getRatioX()
{
	return ratioX;
}

double CNeuron::getRatioY()
{
	return ratioY;
}

double CNeuron::getRatioZ()
{
	return ratioZ;
}

int CNeuron::getColorIndex()
{
	return clrIndex;
}

std::string CNeuron::getName()
{
	return name;
}