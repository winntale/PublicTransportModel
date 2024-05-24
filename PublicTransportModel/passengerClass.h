#pragma once
#include "Defines.h"

using namespace System;
using namespace System::Drawing;
using namespace System::Windows::Forms;

ref class Node;
ref class TaxiCar;

public ref class Passenger {
private:
	int _xPos;
	int _yPos;
	String^ _direction;
	int _moveActions;

	int _state;

	TaxiCar^ _serviceCar;

	Node^ _endNode;
	Node^ _startNode;

public:
	Passenger() {
		_xPos = 0;
		_yPos = 0;
		_direction = "";
		_moveActions = 0;

		_state = 0; // 0 - выбор машины; 1 - ожидание машины; 2 - на пути в пункт прибытия; 3 - в пункте прибытия
	}
	~Passenger() {};

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

	property TaxiCar^ serviceCar {
		TaxiCar^ get() { return _serviceCar; }
		void set(TaxiCar^ _value) { _serviceCar = _value; }
	}


	void MoveAway() {
		if (_serviceCar) {
			if (_serviceCar->direction::get() == "down") { _xPos = serviceCar->xPos::get() - 30; _yPos = serviceCar->yPos::get(); _direction = "left"; }
			else if (_serviceCar->direction::get() == "up") { _xPos = serviceCar->xPos::get() + 30; _yPos = serviceCar->yPos::get(); _direction = "right"; }
			else if (_serviceCar->direction::get() == "right") { _xPos = serviceCar->xPos::get(); _yPos = serviceCar->yPos::get() + 30; _direction = "down"; }
			else if (_serviceCar->direction::get() == "left") { _xPos = serviceCar->xPos::get(); _yPos = serviceCar->yPos::get() - 30; _direction = "up"; }

			_serviceCar = nullptr;
		}

		else if (!_serviceCar) {
			if (_direction == "left") { _xPos -= 3; }
			else if (_direction == "right") { _xPos += 3; }
			else if (_direction == "down") { _yPos += 3; }
			else if (_direction == "up") { _yPos -= 3; }

			_moveActions++;
		}
	}
};