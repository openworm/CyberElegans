#ifndef _C_CONNECTOR_
#define _C_CONNECTOR_

#include "CPoint.h"
#include "VectorMath.h"
#include <string>

class CConnector
{
protected:

	int status; //broken?
	CMassPoint* p1; 
	CMassPoint* p2;

public:
	void virtual draw() = 0; 
	void virtual update() = 0;
	Vector3D getP1Pos();
	Vector3D getP2Pos();
	void applyForceP1(Vector3D);
	void applyForceP2(Vector3D);

protected:
	CConnector(CMassPoint * i_p1, CMassPoint * i_p2);
	virtual CConnector & operator =(const CConnector &i_Connector);

private:
	CConnector();
};
#endif

#ifndef _C_SPRING_
#define _C_SPRING_



class CSpring: public CConnector
{
private:
	double length;
	double stiffnessCoefficient;
	double frictionCoefficient;

public:

	CSpring(const double i_length, const double i_stiffnessCoefficient, const double i_frictionCoefficient,\
		 CMassPoint *i_p1,  CMassPoint *i_p2);
	virtual CSpring & operator = (const CSpring &i_Spring);
	void virtual draw();
	void virtual update();
	
private:
	CSpring();

};
#endif

#ifndef _C_MUSCLE_
#define _C_MUSCLE_

#include "CNeuron.h"

class CMuscle: public CConnector, public CSynapse
{
private:
	double strength;
	//int mclass_id;
	std::string name;
	double length;

public:
	CMuscle(const double i_strength, CMassPoint *i_p1, CMassPoint *i_p2, std::string i_name = "");
	virtual CMuscle & operator = (const CMuscle &i_muscle);
	void virtual draw();
	void virtual update();
	void activate(float value);
	void disactivate();
	std::string getName();
	double getLength0();

private:
	CMuscle();

};
#endif
