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

	bool _isMovingAway;
	int _moveActions;

	int _state;

	int _serviceCarIndex;
	String^ _serviceCarDirection;
	int _serviceCarX;
	int _serviceCarY;

	Node^ _endNode;
	Node^ _startNode;

public:
	Passenger() {
		_xPos = 0;
		_yPos = 0;
		_direction = "";
		
		_isMovingAway = false;
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


	void MoveAway() {
		if (!_isMovingAway) {
			if (_serviceCarDirection == "down") { _xPos = _serviceCarX - 30; _yPos = _serviceCarY; _direction = "left"; }
			else if (_serviceCarDirection == "up") { _xPos = _serviceCarX + 30; _yPos = _serviceCarY; _direction = "right"; }
			else if (_serviceCarDirection == "right") { _xPos = _serviceCarX; _yPos = _serviceCarY + 30; _direction = "down"; }
			else if (_serviceCarDirection == "left") { _xPos = _serviceCarX; _yPos = _serviceCarY - 30; _direction = "up"; }

			_isMovingAway = true;
		}

		else if (_isMovingAway) {
			if (_direction == "left") { _xPos -= 3; }
			else if (_direction == "right") { _xPos += 3; }
			else if (_direction == "down") { _yPos += 3; }
			else if (_direction == "up") { _yPos -= 3; }

			_moveActions++;
		}
	}
};