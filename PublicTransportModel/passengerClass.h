#pragma once
#include "Defines.h"

using namespace System;
using namespace System::Drawing;
using namespace System::Windows::Forms;

public ref class Passenger {
private:
	int _xPos;
	int _yPos;

	int _state; // 0 - выбор машины; 1 - ожидание машины; 2 - в пункте прибытия
	Point^ _endPoint;
	int _endCrossroadIndex;
	int _endVerticeIndex;

	int _startCrossroadIndex;
	int _startVerticeIndex;

public:
	Passenger() {
		_xPos = 0;
		_yPos = 0;
		_state = 0;
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

	property Point^ endPoint {
		Point^ get() { return _endPoint; }
		void set(Point^ _value) { _endPoint = _value; }
	}

	property int endCrossroadIndex {
		int get() { return _endCrossroadIndex; }
		void set(int _value) { _endCrossroadIndex = _value; }
	}

	property int endVerticeIndex {
		int get() { return _endVerticeIndex; }
		void set(int _value) { _endVerticeIndex = _value; }
	}

	property int startCrossroadIndex {
		int get() { return _startCrossroadIndex; }
		void set(int _value) { _startCrossroadIndex = _value; }
	}

	property int startVerticeIndex {
		int get() { return _startVerticeIndex; }
		void set(int _value) { _startVerticeIndex = _value; }
	}

};