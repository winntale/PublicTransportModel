#pragma once


using namespace System;
using namespace System::Drawing;
using namespace System::Windows::Forms;
using namespace System::Collections::Generic;

#include "Defines.h"
#include "passengerClass.h"


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

public:
	TaxiCar() {
		_xPos = 0;
		_yPos = 0;

		_fuelWaste = 1;
		_currentFuel = 100;
		_state = 0;

		_way = gcnew List<Node^>(0);
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


	// ����� ������ ����

	bool IsContainsVertice(List<Node^>^ Nodes, Node^ newNode) {
		bool flag = false;
		for each (Node^ node in Nodes)
			if (node->vertice == newNode->vertice) {
				flag = true;
				break;
			}
		return flag;
	}

	int IndexOfVertice(List<Node^>^ Nodes, Node^ newNode) {
		int index = -1;
		for each (Node^ node in Nodes)
			if (node->vertice == newNode->vertice) {
				index = Nodes->IndexOf(node);
				break;
			}
		return index;
	}

	List<Node^>^ BuildPath(Node^ passNode) {
		List<Node^>^ path = gcnew List<Node^>(0);

		while (passNode != nullptr) {
			path->Add(passNode);
			passNode = passNode->previous;
		}
		return path;
	}

	void CrossroadRemover(List<Node^>^ newReachable, int currentNodeCrossroad, bool flag) {
		// ���� ���� � ��������� true, �� ��������� ������ ���������� � ��������� ������, ��� �������
		if (flag) {
			for each (Node^ availableNode in newReachable->ToArray()) {
				int availableCrossroad = availableNode->vertice / 10;

				if (availableCrossroad <= currentNodeCrossroad)
					newReachable->Remove(availableNode);
			}
		}
		else {
			for each (Node^ availableNode in newReachable->ToArray()) {
				int availableCrossroad = availableNode->vertice / 10;

				if (availableCrossroad >= currentNodeCrossroad)
					newReachable->Remove(availableNode);
			}
		}
	}

	bool IsEqualsCrossroads(List<Node^>^ newReachable, int vertice) {
		bool flag = false;
		for each (Node^ availableNode in newReachable) {
			if ((availableNode->vertice / 10) == (vertice / 10)) {
				flag = true;
				break;
			}
		}
		return flag;
	}

	void WayFind(Passenger^ passenger, array<array<Point^>^>^ Vertices, Label^ label) {
		Random^ rndGen = gcnew Random();

		// ������ ������ ���, ������� ������ ����� ����������� � ������ �����, � ������� �� ��� ����
		List<Node^>^ reachable = gcnew List<Node^>(0);
		List<int>^ explored = gcnew List<int>(0);

		// �� ��������� ����� ������ ������� �����, � ������� �� ������ ������ ���� ������
		int startIndexes = _npCrossroadIndex * 10 + _npVerticeIndex;
		Node^ startNode = gcnew Node(startIndexes);

		// ��������� � ������ ���������� ��� ��������� ����
		reachable->Add(startNode);

		// �� �� ����� � ������, ������� ������������ �� ���� ��������� ����� �����, ��� ��������� ��������
		int passIndexes = passenger->startCrossroadIndex::get() * 10 + passenger->startVerticeIndex::get();
		Node^ passNode = gcnew Node(passIndexes);

		bool a = false;
		bool b = false;

		// ���� ����� ��������, ���� ���� �����, ������� ����� �����������
		while (reachable->Count > 0) {
			// ����� ������� ����� (���� � �������� �� ������� ��������� �����)
			Node^ currentNode = reachable[rndGen->Next(0, reachable->Count)];
			int currentNodeCrossroad = currentNode->vertice / 10;
			int currentNodeVertice = currentNode->vertice % 10;

			// ������� ������ ��������������, ���� �� ����� �����, ������� ������������� ������ �����, ��� ��������� ��������
			if (currentNode->vertice == passNode->vertice) {
				_way = BuildPath(passNode);
				for (int i = 0; i < _way->Count; i++) {
					label->Text += Convert::ToString(_way[i]->vertice);
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
							if (a && nextNodeCrossroad > currentNodeCrossroad)
								newReachable->Add(nextNode);
							else if (b && nextNodeCrossroad < currentNodeCrossroad)
								newReachable->Add(nextNode);
						}
						else if (currentNodeVertice == 1 && nextNodeCrossroad < currentNodeCrossroad)
							newReachable->Add(nextNode);
						else if (currentNodeVertice == 2 && nextNodeCrossroad > currentNodeCrossroad)
							newReachable->Add(nextNode);
						else if (currentNodeVertice == 3) {
							if (a && nextNodeCrossroad < currentNodeCrossroad)
								newReachable->Add(nextNode);
							else if (b && nextNodeCrossroad > currentNodeCrossroad)
								newReachable->Add(nextNode);
						}
						/*if (a && (nextNodeCrossroad > currentNodeCrossroad)) { nextNode->direction == "down"; }
						else if (a && (nextNodeCrossroad < currentNodeCrossroad)) { nextNode->direction == "up"; }
						
						else if (b && (nextNodeCrossroad > currentNodeCrossroad)) { nextNode->direction == "right"; }
						else { nextNode->direction == "left"; }

						
						if (currentNode->direction == "") { newReachable->Add(nextNode); }
						else if (currentNode->direction == "up" && nextNode->direction != "down") { newReachable->Add(nextNode); }
						else if (currentNode->direction == "down" && nextNode->direction != "up") { newReachable->Add(nextNode); }
						else if (currentNode->direction == "right" && nextNode->direction != "left") { newReachable->Add(nextNode); }
						else if (currentNode->direction == "left" && nextNode->direction != "right") { newReachable->Add(nextNode); }*/

						
						

						// 12 13 30 32 50 52 82 83
						// -> 50 52 82 83



					}
				}
			}

			// ������������� ������ ������� ������� �����, � ������� ����� ������� �� ������� (current)
			for (int i = 0; i < newReachable->Count; i++) {
				for each (Node^ availableNode in newReachable->ToArray()) {
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

			for each (Node^ adjacent in newReachable) {
				if (!IsContainsVertice(reachable, adjacent)) {
					adjacent->previous = currentNode;
					reachable->Add(adjacent);
				}
			}
			newReachable = nullptr;
		}






		//// ������ � ������ ���� ��������� ���������� ����, ��������������� 3�� ����� ��� ���������, ���������� �� 10 (�������� ����� � ������� ������� ������ �� 10),
		//// � ����� ������� 3�� ����� (�������� ����� � ������� ������� �� ������ 10 (% 10)).
		//// �� ���� ����� ������������ �� ���� ����� (1-3�������), �������� 0�� ���������� 3�� ������� - 3, 1�� ���������� 2�� ������� - 12, 12�� ���������� 1�� ������� - 121.
		//
		//_way->Add((_npCrossroadIndex2 * 10) + _npVerticeIndex2);
		//List<int>^ reachable = gcnew List<int>(2);

		//bool a = false;
		//bool b = false;

		//// ����� ����� (�������) ���������� ��������� (������� ���� �� ��������� (������ ���������� ����������� �� ������� � ����������� �� ����������� ��������)
		//// + ���� �� ��� �� ����� ���������� �� ������ ������ ����)
		//bool finalEqualsPasLineStart = _way[_way->Count - 1] == (passenger->startCrossroadIndex::get() * 10) + passenger->startVerticeIndex::get();
		//bool finalEqualsPasLineEnd = _way[_way->Count - 1] == (passenger->endCrossroadIndex::get() * 10) + passenger->endCrossroadIndex::get();

		//// ���� ������ ���������� ���������� ������, ��� ������ ���������
		//if (_npCrossroadIndex2 < passenger->startCrossroadIndex::get()) {
		//	while (!finalEqualsPasLineStart) {
		//		// �������������� ������� �����
		//		int currentCrossroad = _way[0] / 10;
		//		int currentVertice = _way[0] % 10;
		//		Point^ currentPoint = Vertices[currentCrossroad][currentVertice];

		//		// verticeIndex = reachable[0] % 10; crossroadIndex = reachable[0] / 10
		//		// verticeIndex = 0 ����� a: i > crossroadIndex, b: i < crossroadIndex
		//		// verticeIndex = 1 ����� a ��� b: i < crossroadIndex
		//		// verticeIndex = 2 ����� a ��� b: i > crossroadIndex
		//		// verticeIndex = 3 ����� a: i < crossroadIndex, b: i > crosroadIndex

		//		List<int>^ verticeCompatible = gcnew List<int>(2); // 33, 32, 22, 20
		//		verticeCompatible->Add(33); verticeCompatible->Add(32); verticeCompatible->Add(22); verticeCompatible->Add(20);

		//		// ��������� ������� ����������� ����� �� ������� (vertice)
		//		// � ����� ��� �� ���� ����������� � �� �������� �� �������� ������� � ��������
		//		// 
		//		//for (int i = VERTEX_QUANTITY - 1; i >= currentCrossroad; i--) {
		//		//	int nextCrossroad = i;
		//		//	for each (int vertices in verticeCompatible) {
		//		//		if (currentVertice < 2) { nextVertice = (currentVertice % 2) + 2; }
		//		//		int nextVertice = vertices % 10;
		//		//	}
		//		//	if () { break; }

		//		//}


		//		/*if () {
		//			_way[wayLength] = Vertices[reachable[0] / 10][reachable[0] % 10];
		//			label->Text += Convert::ToString(String::Format(" {0}{1}", reachable[0] / 10, reachable[0] % 10));
		//			wayLength++;
		//		}
		//		else {
		//			_way[wayLength] = Vertices[passenger->endCrossroadIndex][passenger->endVerticeIndex];
		//			label->Text += Convert::ToString(_way[wayLength]);
		//			wayLength++;
		//			reachable->RemoveRange(0, reachable->Count);
		//		}*/
		//	}
		//}
	}

	void MoveToPassenger() {
		 // ����������� ������ move
	}

};