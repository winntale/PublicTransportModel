#pragma once


using namespace System;
using namespace System::Drawing;
using namespace System::Windows::Forms;
using namespace System::Collections::Generic;

#include "Defines.h"
#include "passengerClass.h"


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
	array<Point^>^ _way;

public:
	TaxiCar() {
		_xPos = 0;
		_yPos = 0;

		_fuelWaste = 1;
		_currentFuel = 100;
		_state = 0;

		_way = gcnew array<Point^>(VERTEX_QUANTITY);
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

	property array<Point^>^ way {
		array<Point^>^ get() { return _way; }
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
	void WayFind(Passenger^ passenger, array<array<Point^>^>^ Vertices, Label^ label) {
		Point^ pasPoint = Point(passenger->xPos::get(), passenger->yPos::get());

		int wayLength = 1;

		// ������ � ������ ����������� ����� ��������� ���������� ����, ��������������� 3�� ����� ��� ���������, ���������� �� 10 (�������� ����� � ������� ������� ������ �� 10),
		// � ����� ������� 3�� ����� (�������� ����� � ������� ������� �� ������ 10 (% 10)).
		// �� ���� ����� ������������ �� ���� ����� (1-3�������), �������� 0�� ���������� 3�� ������� - 3, 1�� ���������� 2�� ������� - 12, 12�� ���������� 1�� ������� - 121.
		List<int>^ reachable = gcnew List<int>(0);
		reachable->Add((_npCrossroadIndex2 * 10) + _npVerticeIndex2);
		way[0] = _nextPoint2;

		bool a = false;
		bool b = false;

		// ����� ����� (�������) ���������� ��������� (������� ���� �� ��������� (������ ���������� ����������� �� ������� � ����������� �� ����������� ��������)
		// + ���� �� ��� �� ����� ���������� �� ������ ������ ����)
		
		// ���� ������ ���������� ���������� ������, ��� ������ ���������
		if (_npCrossroadIndex2 < passenger->endCrossroadIndex::get()) {
			while (reachable->Count) {
				// �������������� ������� �����
				Point^ currentPoint = Vertices[reachable[0] / 10][reachable[0] % 10];
				int currentCrossroad = reachable[0] / 10;
				int currentVertice = reachable[0] % 10;

				// verticeIndex = reachable[0] % 10; crossroadIndex = reachable[0] / 10
				// verticeIndex = 0 ����� a: i > crossroadIndex, b: i < crossroadIndex
				// verticeIndex = 1 ����� a ��� b: i < crossroadIndex
				// verticeIndex = 2 ����� a ��� b: i > crossroadIndex
				// verticeIndex = 3 ����� a: i < crossroadIndex, b: i > crosroadIndex

				

				for (int i = Math::Max((VERTEX_QUANTITY - 1) * Math::Pow(-1, (reachable[0] % 10) % 2), ((reachable[0] / 10) - 1) * Math::Pow(-1, ((reachable[0] % 10) % 2) + 1)); i > -Math::Min((reachable[0] / 10) * Math::Pow(-1, ((reachable[0] % 10) % 2) + 1), 1 * Math::Pow(-1, (reachable[0] % 10) % 2)); i--) {
					// ��������� ���������� ������� �������
				}

				int iCur = 0;
				int iLimit = 0;
				if (currentVertice == 1) { iCur = currentCrossroad - 1; iLimit = -1; } // ���������� �������� ������, ��� �������
				else if (currentVertice == 2) { iCur = (VERTEX_QUANTITY - 1); iLimit = currentCrossroad; } //..������, ��� �������
				else {
					if (a && !b) {
						if (currentVertice == 0) { iCur = (VERTEX_QUANTITY - 1); iLimit = currentCrossroad; }
						else if (currentVertice == 3) { iCur = currentCrossroad - 1; iLimit = -1; }
					}
					else if (b && !a) {
						if (currentVertice == 0) { iCur = currentCrossroad - 1; iLimit = -1; }
						else if (currentVertice == 3) { iCur = (VERTEX_QUANTITY - 1); iLimit = currentCrossroad; }
					}
				}

				// ��������� ������� ����������� ����� �� ������� (vertice)
				// � ����� ��� �� ���� ����������� � �� �������� �� �������� ������� � ��������
				for (iCur; iCur > iLimit; iCur--) {
					for (int j = 3; j >= 0; j--) {
						Point^ nextPoint = Vertices[iCur][j];
						// ������� ���������� ����� �� ���������
						a = currentPoint->X == nextPoint->X;
						b = currentPoint->Y == nextPoint->Y;

						//if ((a && !b) && way


						List<int>^ reachableCrossroads = gcnew List<int>(reachable->Count);
						for each (int crossroad in reachable) { reachableCrossroads->Add(crossroad / 10); }

						// ���� ������� ������ ���� ���������� � � ������� ����������� ����� �� ���������� ����������
						if (((a + b) % 2) && (reachableCrossroads->IndexOf(iCur) == -1)) {
							// ��������� ��� ������� ���������� � ������� ����������� �����������
							List<int>^ reachableCopy = reachable;
							for each (int crossroad in reachable->ToArray()) {
								Point^ availablePoint = Vertices[crossroad / 10][crossroad % 10];
								// ���� ��������� ����� ����������� �� ����� ����� � ����� �� ��� ������������ ����� � ����������� � ����� ������� ���������� � ������� ������
								if (((nextPoint->X == availablePoint->X) && (Math::Abs(nextPoint->Y - currentPoint->Y) < Math::Abs(availablePoint->Y - currentPoint->Y) - 20))
									|| ((nextPoint->Y == availablePoint->Y) && (Math::Abs(nextPoint->X - currentPoint->X) < Math::Abs(availablePoint->X - currentPoint->X) - 20)))
								{ reachableCopy[reachableCopy->IndexOf(crossroad)] = (iCur * 10) + j; } // ������ ����� �� ���������
							}
							reachable = reachableCopy;

							// ���� ���� �� �������, ��������� ����, �� ������� ��� ������������ ����� �� ������ ��� ��������� - ��������� ��� �����
							// ������ �� ���������� ����� (�� j)
							if (reachable->IndexOf((iCur * 10) + j) == -1) { reachable->Add((iCur * 10) + j); break; }
						}
					}
				}
				reachable->Remove(reachable[0]);
				// ����� ������������� �������� �� �������� ���� ��������� �������-����� ����� vertice (���, � ������� �� ��������� ������)

				if (_way[wayLength] != Vertices[passenger->startCrossroadIndex::get()][passenger->startVerticeIndex::get()]) {
					_way[wayLength] = Vertices[reachable[0] / 10][reachable[0] % 10];
					label->Text += Convert::ToString(String::Format(" {0}{1}", reachable[0] / 10, reachable[0] % 10));
					wayLength++;
				}
				else {
					_way[wayLength] = Vertices[passenger->endCrossroadIndex][passenger->endVerticeIndex];
					label->Text += Convert::ToString(_way[wayLength]);
					wayLength++;
					reachable->RemoveRange(0, reachable->Count);
				}
			}
		}
	}

	void MoveToPassenger() {
		 // ����������� ������ move
	}

};