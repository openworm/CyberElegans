#ifndef _C_POINT_
#define _C_POINT_

#include  "VectorMath.h"

class CPoint
{
protected:
	Vector3D pos;

public:
	CPoint(const Vector3D &i_pos);
	virtual ~CPoint();
	virtual CPoint & operator=(const CPoint &i_point);
	void virtual draw() = 0;
	double getX();//взять X координату
	double getY();//взять Y координату
	double getZ();//взять Z координату
	Vector3D getPos();
	Vector3D * getAPos();
	void setPos(Vector3D & i_pos);
	
private:
	CPoint();
};
#endif

#ifndef _C_MASS_POINT_
#define _C_MASS_POINT_
//массовая точка, к кт может крепиться любое колличество пружин 
class CMassPoint: public CPoint
{
private:
	double mass;// масса
	Vector3D vel;//скорость
	Vector3D force;// сила приложенная в этой точке

public:
	CMassPoint(const double i_mass, const Vector3D &i_pos);//конструктор
	virtual ~CMassPoint();//деструктор
	virtual CMassPoint & operator=(const CMassPoint &i_mpoint);//переопределение оператора =
	void virtual draw();//рисование точки масс
	void virtual select();//eсли мы выбрали точку то происходит это событие
	void init();//инициализация
	void update();//обновление
	void applyForce(const Vector3D & force);//
	void timeTick(const double dt);//
	Vector3D getVel();//
	Vector3D getForce();//
	double getMass();//
	
private:
	CMassPoint();

};
#endif