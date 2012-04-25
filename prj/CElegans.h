#ifndef _C_ELEGANS_
#define _C_ELEGANS_

#include "Physics.h"
#include "CPoint.h"
#include "CNeuron.h"
#include "CConnector.h"
#include "VectorMath.h"

class CElegans
{
private:
	int size;//колличество сигментов оно сейчас статично 26 сегментов ак в натуральном червяке
	double length;//длина червяка
	double time;// временной интервал с такой переодичностью червяк двигается

	std::vector <CMassPoint *> mPoint;//все точки червяка маетериальные точки модели иммитирующие точки крепления пружин
	std::vector <CSpring *> spring;//пружины
	std::vector <CMuscle *> muscle;//мускулы
	std::vector <CNeuron *> neuron;// нейроны
	std::vector < std::vector<CNeuron *> > table;// таблица всех неронов в определенном сигменте
	Vector3D vshift;
	double dl;
	double dx;
	////
	//std::vector <Vector3D> trace;
	//int traceTest;
	////
public:
	CElegans(const double i_length, const int i_size);//конструктор задающийся по длине и колличеству сигментов
	~CElegans();//деструктор
	void draw();// отрисовка червя
	void iteration(const double dt);// итерация
	CNeuron * UpdateSelection();//обновление секции
	void ClearSelection();//чистка секции
	void UpdateSelection(int direction);//обновление секции
	void savePosition();
	void rotateWormAroundAnterPosterAxis(float angle);

private:
	CElegans();// ещё один конструктор, только приватный
	void addMPoint(CMassPoint * i_mPoint);//добавляем массовую точку 
	void addMPoint(const double i_mass, Vector3D & i_pos);//добавляем массовую точку(перегруженный метод)
	void addSpring(CSpring * i_spring);//добавляем пружину
	void addSpring(const double i_length, const double i_stiffCoeff, const double i_frictCoeff, CMassPoint * i_p1, CMassPoint * i_p2);//добавляем пружину 
	void addMuscle(CMuscle * i_muscle);//добавляем мускулу
	void addMuscle(const double i_strength, CMassPoint * i_p1, CMassPoint * i_p2, std::string name);//добавляем мускулу
	int  addNeuroMuscleAxonByNames(std::string i_neuronName, std::string i_muscleName,double i_weight);//добавляем нейромышечное соединение
	void addNeuron(CNeuron * i_neuron);//добавляем нейрон
	void addNeuron(const std::string i_name, Vector3D & i_pos, const float i_threshold, const char i_type, const int clr_index);//добавляем нейрон
	void applyFriction();
	void neuronPosCorrection();//корректировка позиций нейронов
	

};
#endif