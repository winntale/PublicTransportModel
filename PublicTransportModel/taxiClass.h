#pragma once


using namespace System;
using namespace System::Threading;
using namespace System::Drawing;
using namespace System::Windows::Forms;
using namespace System::Collections::Generic;

#include "Defines.h"
#include "passengerClass.h"

#include <time.h>

// �����, ������������ ��� ������ ����
public ref class Node {
public:
	int vertice;
	Node^ previous;
	String^ direction; // �����������, ���� ���������, ����� ������� �� previous � ������� ����

	Node(int _vertice) {
		vertice = _vertice;
		previous = nullptr;
		direction = "";
	}
	~Node() {}
};

public ref class TaxiCar {
private:
	int _xPos;
	int _yPos;

	int _maxVelocity;
	System::String^ _direction;
	System::String^ _color;
	int _fuelWaste;
	int _currentFuel;
	int _state; // 0 - ��������; 1 - ���������� ������; 2 - ������ ������� �������

	// ���� ��� 2-�� �����
	Point^ _nextPoint;
	int _npCrossroadIndex;
	int _npVerticeIndex;

	// ���� ��� 3-�� �����
	Point^ _nextPoint2;
	int _npCrossroadIndex2;
	int _npVerticeIndex2;

	List<Node^>^ _way;
	Passenger^ _currentClient;

	float _tripDuration; // � ��������

public:

	delegate void HandlerTakeOnPassenger(Passenger^ passenger, array<array<Point^>^>^ Vertices);
	delegate void HandlerDropOffPassenger();

	event HandlerTakeOnPassenger^ EventTakeOn;
	event HandlerDropOffPassenger^ EventDropOff;

	TaxiCar() {
		_xPos = 0;
		_yPos = 0;

		_fuelWaste = 1;
		_currentFuel = 100;
		_state = 0;

		_way = gcnew List<Node^>(0);
		_tripDuration = 0;

		EventTakeOn += gcnew TaxiCar::HandlerTakeOnPassenger(this, &TaxiCar::onTakeOn);
	}
	~TaxiCar() {};

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
				MessageBox::Show("�� �� ������� ���� ������ �����", "������",
					MessageBoxButtons::OK, MessageBoxIcon::Error);
			}
			else { _color = _value; }
		}
	};

	property int fuelWaste {
		int get() { return _fuelWaste; }
	};

	property int currentFuel {
		int get() { return _currentFuel; }
		void set(int _value) { _currentFuel = _value; }
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

	// ����� �������� �������� � �������� � 0 ��������� ������ (��������)
	void Move(array<array<Point^>^>^ Vertices) {
		// � ����������� �� ����������� (direction) �������� ���������� x, y
		if (_direction == "left") { _xPos -= SPEED; }
		else if (_direction == "right") { _xPos += SPEED; }

		else if (_direction == "down") { _yPos += SPEED; }
		else if (_direction == "up") { _yPos -= SPEED; }


		Random^ rndGen = gcnew Random();

		bool isTurned = false;
		String^ _direction2 = _direction;


		// ���������� 2 ��������: 1) ������ �������� �� Y	2) ������ �������� �� X
		// ������ �������� Y (��������� �� ���������)
		if (_direction == "up" || _direction == "down") {
			if ((Math::Abs(_yPos - (_nextPoint->Y - (TAXICAR_IMG_HEIGHT / 2))) < SPEED) && !isTurned) {
				_yPos = _nextPoint->Y - (TAXICAR_IMG_HEIGHT / 2); // ������������� Y

				bool a = (Vertices[_npCrossroadIndex][_npVerticeIndex]->X == Vertices[_npCrossroadIndex2][_npVerticeIndex2]->X); // ������ X � 1-� � 2-� �����
				bool b = (Vertices[_npCrossroadIndex][_npVerticeIndex]->Y == Vertices[_npCrossroadIndex2][_npVerticeIndex2]->Y); // ������ Y � 1-� � 2-� �����

				// (2) -> (1) (�� ���� (2) ���������� startPoint'��)
				int crossroadIndex1 = _npCrossroadIndex;
				int verticeIndex1 = _npVerticeIndex;

				// (3) -> (2) (�� ���� (3) ���������� nextPoint'��)
				_nextPoint = _nextPoint2;
				_npCrossroadIndex = _npCrossroadIndex2;
				_npVerticeIndex = _npVerticeIndex2;

				// ����� �������������� �������� �� ������� (2)
				if (_npCrossroadIndex > crossroadIndex1) { _direction2 = "right"; } // �������
				else if (_npCrossroadIndex < crossroadIndex1) { _direction2 = "left"; } // ������

				// (*) -> (3). �������� ����� �����, ������� ������ nextPoint2'��
				// 1) ���������� ���������� (*), ���� �� ��������� ��������������� ��������
				while (b || !a || (_npCrossroadIndex == _npCrossroadIndex2)) {
					_npCrossroadIndex2 = rndGen->Next(0, VERTEX_QUANTITY);

					for (int i = 0; i < 4; i++) {
						a = (Vertices[_npCrossroadIndex][_npVerticeIndex]->X == Vertices[_npCrossroadIndex2][i]->X);
						b = (Vertices[_npCrossroadIndex][_npVerticeIndex]->Y == Vertices[_npCrossroadIndex2][i]->Y);
						if (!b && a && (_npCrossroadIndex != _npCrossroadIndex2)) { break; }
					}
				}
				if (_npCrossroadIndex2 > _npCrossroadIndex) {
					_npVerticeIndex = Convert::ToInt16(verticeIndex1 / 2) * 2;
					_npVerticeIndex2 = 2 * rndGen->Next(0, 2);
				}
				else if (_npCrossroadIndex2 < _npCrossroadIndex) {
					_npVerticeIndex = Convert::ToInt16(verticeIndex1 / 2) * 2 + 1;
					_npVerticeIndex2 = 2 * rndGen->Next(0, 2) + 1;
				}


				_nextPoint = Vertices[_npCrossroadIndex][_npVerticeIndex];
				_nextPoint2 = Vertices[_npCrossroadIndex2][_npVerticeIndex2];

				isTurned = true;
			}
		}
		// ������ �������� X (��������� �� �����������)
		else if (_direction == "right" || _direction == "left") {
			if ((Math::Abs(_xPos - (_nextPoint->X - (TAXICAR_IMG_HEIGHT / 2))) < SPEED) && !isTurned) {
				_xPos = _nextPoint->X - (TAXICAR_IMG_HEIGHT / 2); // ������������� X

				bool a = (Vertices[_npCrossroadIndex][_npVerticeIndex]->X == Vertices[_npCrossroadIndex2][_npVerticeIndex2]->X); // ������ X � 1-� � 2-� �����
				bool b = (Vertices[_npCrossroadIndex][_npVerticeIndex]->Y == Vertices[_npCrossroadIndex2][_npVerticeIndex2]->Y); // ������ Y � 1-� � 2-� �����

				// (2) -> (1) (�� ���� (2) ���������� startPoint'��)
				int crossroadIndex1 = _npCrossroadIndex;
				int verticeIndex1 = _npVerticeIndex;

				// (3) -> (2) (�� ���� (3) ���������� nextPoint'��)
				_nextPoint = _nextPoint2;
				_npCrossroadIndex = _npCrossroadIndex2;
				_npVerticeIndex = _npVerticeIndex2;

				// ����� ������������ �������� �� ������� (2)
				if (_npCrossroadIndex > crossroadIndex1) { _direction2 = "down"; } // ����
				else if (_npCrossroadIndex < crossroadIndex1) { _direction2 = "up"; } // �����

				while (a || !b || (_npCrossroadIndex == _npCrossroadIndex2)) {
					_npCrossroadIndex2 = rndGen->Next(0, VERTEX_QUANTITY);

					for (int i = 0; i < 4; i++) {
						a = (Vertices[_npCrossroadIndex][_npVerticeIndex]->X == Vertices[_npCrossroadIndex2][i]->X);
						b = (Vertices[_npCrossroadIndex][_npVerticeIndex]->Y == Vertices[_npCrossroadIndex2][i]->Y);
						if (!a && b && (_npCrossroadIndex != _npCrossroadIndex2)) { break; }
					}
				}

				if (_npCrossroadIndex2 > _npCrossroadIndex) {
					_npVerticeIndex = (verticeIndex1 % 2) + 2;
					_npVerticeIndex2 = rndGen->Next(2, 4);
				}
				else if (_npCrossroadIndex2 < _npCrossroadIndex) {
					_npVerticeIndex = (verticeIndex1 % 2);
					_npVerticeIndex2 = rndGen->Next(0, 2);
				}

				_nextPoint = Vertices[_npCrossroadIndex][_npVerticeIndex];
				_nextPoint2 = Vertices[_npCrossroadIndex2][_npVerticeIndex2];

				isTurned = true;
			}
		}

		if (isTurned) {
			_direction = _direction2;
			isTurned = false;
		}
	}

	// ����� ��������� ���������� �� � ������� ��� (Nodes) ������� ����� newNode
	bool IsContainsVertice(List<Node^>^ Nodes, Node^ newNode) {
		bool flag = false;
		for each (Node ^ node in Nodes)
			if (node->vertice == newNode->vertice) {
				flag = true;
				break;
			}
		return flag;
	}

	// ����� ���������� ������ ������ �������� newNode � ������� Nodes �� ������������� �������� ����� (-1 ���� �� ���)
	int IndexOfVertice(List<Node^>^ Nodes, Node^ newNode) {
		int index = -1;
		for each (Node ^ node in Nodes)
			if (node->vertice == newNode->vertice) {
				index = Nodes->IndexOf(node);
				break;
			}
		return index;
	}

	// ����� ���������, ���� �� ������ ���������� � ������� newReachable
	bool IsEqualsCrossroads(List<Node^>^ newReachable, int vertice) {
		bool flag = false;
		for each (Node ^ availableNode in newReachable) {
			if ((availableNode->vertice / 10) == (vertice / 10)) {
				flag = true;
				break;
			}
		}
		return flag;
	}

	// ����� ������ ���� �� ��������� ����� �� ������ ����� ��������� passNode ���� ��������� � ������� previous
	List<Node^>^ BuildPath(Node^ passNode, Node^ passEndNode) {
		List<Node^>^ path = gcnew List<Node^>(0);
		path->Add(passEndNode);
		while (passNode != nullptr) {
			path->Add(passNode);
			passNode = passNode->previous;
		}
		return path;
	}

	// ����� ������ ���� (���������� ������ previous ��� reachable �����)
	void WayFind(Passenger^ passenger, array<array<Point^>^>^ Vertices, Label^ label11, Label^ label12) {
		label11->Text = "way";
		Random^ rndGen = gcnew Random();

		// ������ ������ ���, ������� ������ ����� ����������� � ������ �����, � ������� �� ��� ����
		List<Node^>^ reachable = gcnew List<Node^>(0);
		List<int>^ explored = gcnew List<int>(0);

		// �� ��������� ����� ������ ������� �����, � ������� �� ������ ������ ���� ������
		int startIndexes = _npCrossroadIndex * 10 + _npVerticeIndex;
		Node^ startNode = gcnew Node(startIndexes);
		startNode->direction = _direction;

		// ��������� � ������ ���������� ��� ��������� ����
		reachable->Add(startNode);

		// �� �� ����� � ������, ������� ������������ �� ���� ��������� ����� �����, ��� ��������� ��������
		Node^ passStartNode = passenger->startNode::get();
		Node^ passEndNode = passenger->endNode::get();

		int passStartIndexes = passStartNode->vertice;
		int passEndIndexes = passEndNode->vertice;

		bool a = false;
		bool b = false;

		// ���� ����� ��������, ���� ���� �����, ������� ����� �����������
		while (reachable->Count > 0) {
			// ����� ������� ����� (���� � �������� �� ������� ��������� �����)
			Node^ currentNode = reachable[rndGen->Next(0, reachable->Count)];
			int currentNodeCrossroad = currentNode->vertice / 10;
			int currentNodeVertice = currentNode->vertice % 10;

			// ������� ������ ��������������, ���� �� ����� �����, ������� ������������� ������ �����, ��� ��������� �������� (� ����������� �� ����������� ������� ���������)
			bool isPassLineReached = false;

			if ((currentNode->vertice / 10) == (passStartIndexes / 10)) {
				if (passEndNode->direction == "left") { isPassLineReached = (currentNode->vertice % 10) < 2; }
				else if (passEndNode->direction == "right") { isPassLineReached = (currentNode->vertice % 10) > 1; }
				else if (passEndNode->direction == "up") { isPassLineReached = (currentNode->vertice % 10) % 2 == 1; }
				else if (passEndNode->direction == "down") { isPassLineReached = (currentNode->vertice % 10) % 2 == 0; }
			}

			if (isPassLineReached) {
				_way = BuildPath(currentNode, passEndNode);
				_way->Reverse();
				for (int i = 0; i < _way->Count; i++) {
					label11->Text += Convert::ToString(String::Format(" {0}", _way[i]->vertice));
					label12->Text += Convert::ToString(String::Format(" {0}", _way[i]->direction));
				}
				break;
			}

			// ��� ��� �� ����� ������� �����, �� ������� � �� ������� ��������� � ������� � ������ ����������
			reachable->Remove(currentNode);
			explored->Add(currentNode->vertice);

			// ������ ������ �����, ������� ����� ������� �� ������� �����
			List<Node^>^ newReachable = gcnew List<Node^>(0);

			// �������� �� ���� ������� �����
			for (int i = 0; i < VERTEX_QUANTITY; i++) {
				for (int j = 0; j < 4; j++) {
					// ����� ������� �����, ������� ������ ������������� (������������ ��������� �����)
					int nextIndexes = i * 10 + j;
					int nextNodeCrossroad = nextIndexes / 10;
					int nextNodeVertice = nextIndexes % 10;


					a = Vertices[currentNodeCrossroad][currentNodeVertice]->X == Vertices[nextNodeCrossroad][nextNodeVertice]->X; // ������ X (������������ ��������)
					b = Vertices[currentNodeCrossroad][currentNodeVertice]->Y == Vertices[nextNodeCrossroad][nextNodeVertice]->Y; // ������ Y (�������������� ��������)

					Node^ nextNode = gcnew Node(nextIndexes);
					// ���� ������� ���� ���� ���������� � �� ���� �� ������ ������������ ��������� �����
					if (((a + b) % 2) && (explored->IndexOf(nextNode->vertice) == -1) && !IsContainsVertice(newReachable, nextNode) && (currentNodeCrossroad != i)) {
						if (currentNodeVertice == 0) {
							if (a && nextNodeCrossroad > currentNodeCrossroad) {
								nextNode->direction = "down";
								newReachable->Add(nextNode);
							}
							else if (b && nextNodeCrossroad < currentNodeCrossroad) {
								nextNode->direction = "left";
								newReachable->Add(nextNode);
							}
						}
						else if (currentNodeVertice == 1 && nextNodeCrossroad < currentNodeCrossroad) {
							if (a) { nextNode->direction = "up"; }
							else if (b) { nextNode->direction = "left"; }
							newReachable->Add(nextNode);
						}
						else if (currentNodeVertice == 2 && nextNodeCrossroad > currentNodeCrossroad) {
							if (a) { nextNode->direction = "down"; }
							else if (b) { nextNode->direction = "right"; }
							newReachable->Add(nextNode);
						}
						else if (currentNodeVertice == 3) {
							if (a && nextNodeCrossroad < currentNodeCrossroad) {
								nextNode->direction = "up";
								newReachable->Add(nextNode);
							}
							else if (b && nextNodeCrossroad > currentNodeCrossroad) {
								nextNode->direction = "right";
								newReachable->Add(nextNode);
							}
						}
					}
				}
			}

			// ������������� ������ ������� ������� �����, � ������� ����� ������� �� ������� (current)
			for (int i = 0; i < newReachable->Count; i++) {
				for each (Node ^ availableNode in newReachable->ToArray()) {
					// availablePoint - �����, � ������� ����� �������� �� current (�� ���� ���� current -> available ��� ����������)
					Point^ availablePoint = Vertices[availableNode->vertice / 10][availableNode->vertice % 10];
					Point^ currentPoint = Vertices[currentNodeCrossroad][currentNodeVertice];
					Point^ comparablePoint = Vertices[newReachable[i]->vertice / 10][newReachable[i]->vertice % 10];

					// ���� ������������ ��������� ����� (nextPoint) � �����, � ������� ����� ������� �� current (available) ��������� �� ����� �����
					// � ���������� �� current �� next ������, ��� ���������� �� current �� available
					// � ���������� nextPoint �� ����� ����������� availablePoint � currentPoint
					bool sameX = (comparablePoint->X == availablePoint->X); bool sameY = (comparablePoint->Y == availablePoint->Y);
					bool IsnpYCloser = (Math::Abs(comparablePoint->Y - currentPoint->Y) < Math::Abs(availablePoint->Y - currentPoint->Y) - 60);
					bool IsnpXCloser = (Math::Abs(comparablePoint->X - currentPoint->X) < Math::Abs(availablePoint->X - currentPoint->X) - 60);
					if (((sameX && IsnpYCloser) || (sameY && IsnpXCloser)) && (newReachable[i]->vertice / 10 != availableNode->vertice / 10) && (newReachable[i]->vertice / 10 != currentNodeCrossroad)
						&& (newReachable[i]->vertice % 10 == availableNode->vertice % 10))
					{
						newReachable[newReachable->IndexOf(availableNode)] = newReachable[i];
					} // �� ������ � ������� ��������� ����� �������� ���������������� ������� �� nextNode
				}
			}

			for each (Node ^ adjacent in newReachable) {
				if (!IsContainsVertice(reachable, adjacent)) {
					adjacent->previous = currentNode;
					reachable->Add(adjacent);
				}
			}
			newReachable = nullptr;
		}
	}

	void onTakeOn(Passenger^ passenger, array<array<Point^>^>^ Vertices) {
		Thread::Sleep(1000);
		_state = 2;
		passenger->state::set(2);

		Random^ rndGen = gcnew Random();
		int verticeIndex1 = passenger->startNode->vertice % 10;

		int crossroadIndex2 = _way[0]->vertice / 10;
		int verticeIndex2 = _way[0]->vertice % 10;

		int crossroadIndex3 = rndGen->Next(0, VERTEX_QUANTITY);
		int verticeIndex3 = rndGen->Next(0, 4);

		bool a1 = (Vertices[crossroadIndex2][verticeIndex2]->X == Vertices[crossroadIndex3][verticeIndex3]->X); // ������ X � 2-�� � 3-� �����
		bool b1 = (Vertices[crossroadIndex2][verticeIndex2]->Y == Vertices[crossroadIndex3][verticeIndex3]->Y); // ������ Y � 2-�� � 3-� �����

		// ��������� ��������� 3�� �����
		if (_way[0]->direction == "up" || _way[0]->direction == "down") { // ������ ���������� ������ �� ��������� (������������� 1-�� �����)
			while (!b1 || a1 || (crossroadIndex3 == crossroadIndex2)) { // ���� �� ������ ���� �� �����������
				crossroadIndex3 = rndGen->Next(0, VERTEX_QUANTITY);

				for (int i = 0; i < 4; i++) {
					a1 = (Vertices[crossroadIndex2][verticeIndex2]->X == Vertices[crossroadIndex3][i]->X);
					b1 = (Vertices[crossroadIndex2][verticeIndex2]->Y == Vertices[crossroadIndex3][i]->Y);
					if (b1 && !a1) { break; }
				}
			}

			if (crossroadIndex3 > crossroadIndex2) {
				verticeIndex2 = (verticeIndex1 % 2) + 2;
				verticeIndex3 = rndGen->Next(2, 4);
			}
			else if (crossroadIndex3 < crossroadIndex2) {
				verticeIndex2 = (verticeIndex1 % 2);
				verticeIndex3 = rndGen->Next(0, 2);
			}
		}
		else if (_way[0]->direction == "left" || _way[0]->direction == "right") { // ������ ���������� ������ �� �����������
			while (!a1 || b1 || (crossroadIndex3 == crossroadIndex2)) { // ���� �� ������ ���� �� ���������
				crossroadIndex3 = rndGen->Next(0, VERTEX_QUANTITY);

				for (int i = 0; i < 4; i++) {
					a1 = (Vertices[crossroadIndex2][verticeIndex2]->X == Vertices[crossroadIndex3][i]->X);
					b1 = (Vertices[crossroadIndex2][verticeIndex2]->Y == Vertices[crossroadIndex3][i]->Y);
					if (a1 && !b1) { break; }
				}
			}

			if (crossroadIndex3 > crossroadIndex2) {
				verticeIndex2 = (verticeIndex1 / 2) * 2;
				verticeIndex3 = 2 * rndGen->Next(0, 2);
			}
			else if (crossroadIndex3 < crossroadIndex2) {
				verticeIndex2 = (verticeIndex1 / 2) * 2 + 1;
				verticeIndex3 = 2 * rndGen->Next(0, 2) + 1;
			}
		}

		//���������� ����� �������
		_nextPoint = Vertices[crossroadIndex2][verticeIndex2];
		_npCrossroadIndex = crossroadIndex2;
		_npVerticeIndex = verticeIndex2;

		_nextPoint2 = Vertices[crossroadIndex3][verticeIndex3];
		_npCrossroadIndex2 = crossroadIndex3;
		_npVerticeIndex2 = verticeIndex3;
	}

	void TakeOn(Passenger^ passenger, array<array<Point^>^>^ Vertices) {
		EventTakeOn(passenger, Vertices);
	}

	void IfTaxiIsHere(Passenger^ passenger, array<array<Point^>^>^ Vertices) {
		bool isTaxiHere = false;

		if (passenger->endNode->direction == "up") { isTaxiHere = (((passenger->xPos::get() - _xPos) < 60) && (Math::Abs(_yPos - (passenger->yPos::get() - (TAXICAR_IMG_HEIGHT / 2))) < SPEED)); }
		else if (passenger->endNode->direction == "down") { isTaxiHere = (((_xPos - passenger->xPos::get()) < 60) && (Math::Abs(_yPos - (passenger->yPos::get() - (TAXICAR_IMG_HEIGHT / 2))) < SPEED)); }
		else if (passenger->endNode->direction == "right") { isTaxiHere = ((Math::Abs(_xPos - (passenger->xPos::get() - (TAXICAR_IMG_HEIGHT / 2))) < SPEED) && ((passenger->yPos::get() - _yPos) < 120)); }
		else if (passenger->endNode->direction == "left") { isTaxiHere = ((Math::Abs(_xPos - (passenger->xPos::get() - (TAXICAR_IMG_HEIGHT / 2))) < SPEED) && ((_yPos - passenger->yPos::get()) < 120)); }

		if (isTaxiHere) {
			TakeOn(passenger, Vertices);
		}
	}

	void MoveToPassenger(array<array<Point^>^>^ Vertices, Passenger^ passenger) {
		_direction = _way[0]->direction;

		if (_direction == "left") { _xPos -= SPEED; }
		else if (_direction == "right") { _xPos += SPEED; }

		else if (_direction == "down") { _yPos += SPEED; }
		else if (_direction == "up") { _yPos -= SPEED; }

		Point^ nextWayPoint = Vertices[_way[0]->vertice / 10][_way[0]->vertice % 10];

		bool isTurned = false;

			
		if (_direction == "up" || _direction == "down") {
			// ������ �������� Y (��������� �� ���������)
			if ((Math::Abs(_yPos - (nextWayPoint->Y - (TAXICAR_IMG_HEIGHT / 2))) < SPEED) && !isTurned) {
				_yPos = nextWayPoint->Y - (TAXICAR_IMG_HEIGHT / 2); // ������������� Y
				_way->Remove(_way[0]);

				isTurned = true;
			}
		}
		else if (_direction == "right" || _direction == "left") {
			// ������ �������� X (��������� �� �����������)
			if ((Math::Abs(_xPos - (nextWayPoint->X - (TAXICAR_IMG_HEIGHT / 2))) < SPEED) && !isTurned) {
				_xPos = nextWayPoint->X - (TAXICAR_IMG_HEIGHT / 2); // ������������� X
				_way->Remove(_way[0]);

				isTurned = true;
			}
		}

		if (_way->Count == 1) {
			IfTaxiIsHere(passenger, Vertices);
		}
	}

	void onDropOff(Passenger^ passenger) {
		if (tripDuration > 10) {
			Thread::Sleep(1000);
			_state = 0;
			passenger->state::set(3);
		}
	}

};