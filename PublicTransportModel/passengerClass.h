#pragma once
#include "Defines.h"

using namespace System;
using namespace System::Drawing;
using namespace System::Windows::Forms;


//public ref class Node {
//public:
//	int vertice;
//	Node^ previous;
//	String^ direction; // направление, куда двигались, чтобы попасть из previous в текущую ноду
//
//	Node(int _vertice) {
//		vertice = _vertice;
//		previous = nullptr;
//		direction = "";
//	}
//	~Node() {}
//};

ref class Node;

public ref class Passenger {
private:
	int _xPos;
	int _yPos;

	int _state; // 0 - выбор машины; 1 - ожидание машины; 2 - в пункте прибытия
	Node^ _endNode;
	Node^ _startNode;

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

	property Node^ startNode {
		Node^ get() { return _startNode; }
		void set(Node^ _value) { _startNode = _value; }
	}

	property Node^ endNode {
		Node^ get() { return _endNode; }
		void set(Node^ _value) { _endNode = _value; }
	}

};