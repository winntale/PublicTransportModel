#pragma once
#include "Defines.h"

using namespace System;
using namespace System::Drawing;
using namespace System::Windows::Forms;
using namespace System::Collections::Generic;

ref class Node {
public:
	int vertice;
	Node^ previous;
	String^ direction; // направление, куда двигались, чтобы попасть из previous в текущую ноду

	Node(int _vertice);
	Node(int _vertice, String^ _direction);

};

ref class Passenger {
private:
	int _color; // 0 - чёрный; 1 - рыжий; 2 - русый

	int _axisOffset;
	float _xPos;
	float _yPos;
	String^ _direction;

	bool _isMovingAway;
	int _moveActions;

	int _busStopIndex;
	int _goalBusStopIndex;

	int _passengerIndex;
	int _state; // состояния для такси: 0 - выбор машины; 1 - ожидание машины; 2 - в пути; 3 - в пункте прибытия
	// для автобуса: 4 - ожидание автобуса;
	int _serviceCarIndex;
	String^ _serviceCarDirection;
	int _serviceCarX;
	int _serviceCarY;

	Node^ _startNode;
	Node^ _endNode;

public:
	Passenger();
	Passenger(int _stateVal, int _BusStopIndex, int passengerIndex);

	property int color {
		int get() { return _color; }
		void set(int _value) { _color = _value; }
	}

	property int xPos {
		int get() { return _xPos; }
		void set(int _value) { _xPos = _value; }
	}

	property int yPos {
		int get() { return _yPos; }
		void set(int _value) { _yPos = _value; }
	}

	property int state {
		int get() { return _state; }
		void set(int _value) { _state = _value; }
	}

	property String^ direction {
		String^ get() { return _direction; }
		void set(String^ _value) { _direction = _value; }
	}

	property bool isMovingAway {
		bool get() { return _isMovingAway; }
		void set(bool _value) { _isMovingAway = _value; }
	}

	property int moveActions {
		int get() { return _moveActions; }
		void set(int _value) { _moveActions = _value; }
	}

	property Node^ startNode {
		Node^ get() { return _startNode; }
		void set(Node^ _value) { _startNode = _value; }
	}

	property Node^ endNode {
		Node^ get() { return _endNode; }
		void set(Node^ _value) { _endNode = _value; }
	}

	property int serviceCarIndex {
		int get() { return _serviceCarIndex; }
		void set(int _value) { _serviceCarIndex = _value; }
	}

	property String^ serviceCarDirection {
		String^ get() { return _serviceCarDirection; }
		void set(String^ _value) { _serviceCarDirection = _value; }
	}

	property int serviceCarX {
		int get() { return _serviceCarX; }
		void set(int _value) { _serviceCarX = _value; }
	}

	property int serviceCarY {
		int get() { return _serviceCarY; }
		void set(int _value) { _serviceCarY = _value; }
	}

	property int busStopIndex{
		int get() { return _busStopIndex; }
		void set(int _value) { _busStopIndex = _value; }
	}

	property int goalBusStopIndex {
		int get() { return _goalBusStopIndex; }
		void set(int _value) { _goalBusStopIndex = _value; }
	}

	void OnPassengerNoticed(int _serviceCar);
	void OnEventStop(int passIndex, int _stopAt, int _servXPos, int _servYPos, String^ _servDirection);
	void MoveAway();
	/*void OnEventTakeOn();*/
};

ref class TaxiCar {
protected:
	int _xPos;
	int _yPos;

	int _maxVelocity;
	System::String^ _direction;
	System::String^ _color;
	
	int _serviceCarIndex;
	int _state; // 0 - ожидание; 1 - в пути к пассажиру; 2 - перевозка пассажира; 3, 4  - ожидание пассажира на вход и выход из машины

	// поля для 2-ой точки
	Point^ _nextPoint;
	int _npCrossroadIndex;
	int _npVerticeIndex;

	// поля для 3-ей точки
	Point^ _nextPoint2;
	int _npCrossroadIndex2;
	int _npVerticeIndex2;

	List<Node^>^ _way;
	Passenger^ _currentClient;

	float _tripDuration; // в секундах
public:
	delegate void HandlerNoticePassenger(int serviceCarIndex);
	static event HandlerNoticePassenger^ EventNoticePassengert;

	TaxiCar();
	TaxiCar(int serviceCarIndex);

	property int maxVelocity {
		int get() { return _maxVelocity; }
		void set(int _value) {
			if (_value < 50) { _maxVelocity = 50; }
			else if (_value > 200) { _maxVelocity = 200; }
			else { _maxVelocity = _value; }
		};
	};

	property System::String^ direction {
		System::String^ get() { return _direction; }
		void set(System::String^ _value) { _direction = _value; }
	};

	property int xPos {
		int get() { return _xPos; }
		void set(int _value) { _xPos = _value; }
	}

	property int yPos {
		int get() { return _yPos; }
		void set(int _value) { _yPos = _value; }
	}

	property System::String^ color {
		System::String^ get() { return _color; }
		void set(System::String^ _value) {
			if (System::String::IsNullOrEmpty(_value)) {
				MessageBox::Show("Вы не указали цвет машины такси", "Ошибка",
					MessageBoxButtons::OK, MessageBoxIcon::Error);
			}
			else { _color = _value; }
		}
	};

	property int state {
		int get() { return _state; }
		void set(int _value) { _state = _value; }
	};

	property Point^ nextPoint {
		Point^ get() { return _nextPoint; }
		void set(Point^ _value) { _nextPoint = _value; }
	}

	property int npCrossroadIndex {
		int get() { return _npCrossroadIndex; }
		void set(int _value) { _npCrossroadIndex = _value; }
	}

	property int npVerticeIndex {
		int get() { return _npVerticeIndex; }
		void set(int _value) { _npVerticeIndex = _value; }
	}

	property Point^ nextPoint2 {
		Point^ get() { return _nextPoint2; }
		void set(Point^ _value) { _nextPoint2 = _value; }
	}

	property int npCrossroadIndex2 {
		int get() { return _npCrossroadIndex2; }
		void set(int _value) { _npCrossroadIndex2 = _value; }
	}

	property int npVerticeIndex2 {
		int get() { return _npVerticeIndex2; }
		void set(int _value) { _npVerticeIndex2 = _value; }
	}

	property List<Node^>^ way {
		List<Node^>^ get() { return _way; }
	}

	property Passenger^ currentClient {
		Passenger^ get() { return _currentClient; }
		void set(Passenger^ _value) { _currentClient = _value; }
	}

	property float tripDuration {
		float get() { return _tripDuration; }
		void set(float _value) { _tripDuration = _value; }
	}

	String^ XPointReached(array<array<Point^>^>^ Vertices, bool isTurned, String^ _direction2);
	String^ YPointReached(array<array<Point^>^>^ Vertices, bool isTurned, String^ _direction2);
	void Move(array<array<Point^>^>^ Vertices);
	bool IsContainsVertice(List<Node^>^ Nodes, Node^ newNode);
	int IndexOfVertice(List<Node^>^ Nodes, Node^ newNode);
	bool IsEqualsCrossroads(List<Node^>^ newReachable, int vertice);
	List<Node^>^ BuildPath(Node^ passNode, Node^ passEndNode);
	void WayFind(Passenger^ passenger, array<array<Point^>^>^ Vertices);
	void IfTransportIsHere(Passenger^ passenger, array<array<Point^>^>^ Vertices);
	void onTakeOn(Passenger^ passenger, array<array<Point^>^>^ Vertices);
	void MoveToPassenger(array<array<Point^>^>^ Vertices, Passenger^ passenger);
	void onDropOff(Passenger^ passenger);
	void OnTaxiChoised(int _serviceCar, Passenger^ currentClient, array<array<Point^>^>^ Vertices);
};

ref class Bus : public TaxiCar {
private:
	array<bool>^ wasIn;
	array<Node^>^ nodeContainer;
	int _stopAt;

	int _localTimer;
	bool _isBusHere;
	List<Passenger^>^ _currentClient;

public:

	delegate void HandlerStop(int passIndex, int _stopAt, int _servXPos, int _servYPos, String^ _servDirection);
	static event HandlerStop^ EventStop;

	Bus();

	property int stopAt {
		int get() { return _stopAt; }
	}

	property List<Passenger^>^ currentClient {
		List<Passenger^>^ get() { return _currentClient; }
		void set(List<Passenger^>^ _value) { _currentClient = _value; }
	}

	void WayGenerator();
	void IfTransportIsHere(array<Point^>^ busStops);
	void EventStopTrigger(List<List<Passenger^>^>^ _BusPassengers, int _stopAt, int _xPos, int _yPos, String^ _direction);
	void Move(array<array<Point^>^>^ Vertices, array<Point^>^ busStops);
};


ref class MyEnvironment {
private:
	array<Point^>^ cordinates;
	array<Point^>^ busStops;
	array<array<Point^>^>^ Vertices;
	Bus^ _Bus;
	List<TaxiCar^>^ _TaxiCars;
	List<Passenger^>^ _Passengers;
	List<List<Passenger^>^>^ _BusPassengers;

	float _localTimer;
	float _localTimer2;
	float _localTimer3;

public:
	delegate void HandlerTaxiChoise(int serviceCarIndex, Passenger^ currentClient, array<array<Point^>^>^ Vertices);
	static event HandlerTaxiChoise^ EventTaxiChoise;

	MyEnvironment();

	property Bus^ pBus {
		Bus^ get() { return _Bus; }
	}

	property float localTimer2 {
		float get() { return _localTimer2; }
		void set(float _value) { _localTimer2 = _value; }
	}

	property List<TaxiCar^>^ TaxiCars {
		List<TaxiCar^>^ get() { return _TaxiCars; }
	}

	property List<Passenger^>^ Passengers {
		List<Passenger^>^ get() { return _Passengers; }
	}

	property List<List<Passenger^>^>^ BusPassengers {
		List<List<Passenger^>^>^ get() { return _BusPassengers; }
	}

	// метод генерации массива вершин по массиву координат
	void VerticesGen();
	void DirectionSet(Point^ spawnPoint, Point^ nextPoint, int crossroadIndex1, int crossroadIndex2);
	void TaxiSpawn();
	void PassengerSpawn();
	void TaxiChoise();
	void BusActions();
	void TaxiActions();
	void BusPassengersMove();
	void PassengersAction();
	void TimerTickActions();
};