#pragma once
using namespace System;
using namespace System::Drawing;
using namespace System::Windows::Forms;

#include "Defines.h"

public ref class TaxiCar {
private:
	int _xPos = 0;
	int _yPos = 0;
	int _maxVelocity; // ����� ���������� � ������� ���������, ��������� �
	int _currentVelocity = 0;
	System::String^ _direction;
	System::String^ _color;
	int _fuelWaste = 1;
	int _maxFuel = 100;
	int _currentFuel = 100;
	int _state = 0;

	Point^ _nextPoint;
	int _npCrossroadIndex;
	int _npVerticeIndex;

public:
	property int maxVelocity {
		int get() { return _maxVelocity; }
		void set(int _value) {
			if (_value < 30) { _maxVelocity = 30; }
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

	property int currentVelocity {
		int get() { return _currentVelocity; }
	};

	property int fuelWaste {
		int get() { return _fuelWaste; }
	};

	property int maxFuel {
		int get() { return _maxFuel; }
	};

	property int currentFuel {
		int get() { return _currentFuel; }
	};

	property int state {
		int get() { return _state; }
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

	// ����� �������� ��������
	void Move(array<array<Point^>^>^ Vertices) {
		// � ����������� �� ����������� (direction) �������� ���������� x, y
		if (_direction == "left") { _xPos -= SPEED; }
		else if (_direction == "right") { _xPos += SPEED; }

		else if (_direction == "down") { _yPos += SPEED; }
		else if (_direction == "up") { _yPos -= SPEED; }


		Random^ rndGen = gcnew Random();

		bool a = false; // ������� X
		bool b = false; // ������� Y
		int np2CrossroadIndex = 0;
		int np2VerticeIndex = 0;
		// ���� ������� Y-� � 1 � 2 �����
		if (_direction == "up" || _direction == "down") {
			if (Math::Abs(_yPos - (_nextPoint->Y - (TAXICAR_IMG_HEIGHT / 2))) < SPEED) {
				_yPos = _nextPoint->Y - (TAXICAR_IMG_HEIGHT / 2);
				_direction = "right";
			}
		}
		else if (_direction == "right" || _direction == "left") {
			if (Math::Abs(_xPos - (_nextPoint->X - (TAXICAR_IMG_HEIGHT / 2))) < SPEED) {
				_xPos = _nextPoint->X - (TAXICAR_IMG_HEIGHT / 2);
				_direction = "up";
			}
		}
		

			// ���� ��� ���������� ����� �� ��������� � 2 � 3 �����
			//while (!a) { //!((a + b) % 2)) {
			//	np2CrossroadIndex = rndGen->Next(0, VERTEX_QUANTITY);
			//	np2VerticeIndex = rndGen->Next(0, 4);
			//	a = (Math::Abs(_nextPoint->X - Vertices[np2CrossroadIndex][np2VerticeIndex]->X) < 0.1);
			//	b = (Math::Abs(_nextPoint->Y - Vertices[np2CrossroadIndex][np2VerticeIndex]->Y) < 0.1);
			//}
			//
			//if (a) {
			//	if (Math::Abs(_yPos - _nextPoint->Y) <= _maxVelocity / 30) {
			//		_xPos = _nextPoint->X - (TAXICAR_IMG_HEIGHT / 2);
			//		_nextPoint = Vertices[np2CrossroadIndex][np2VerticeIndex];
			//		if (np2VerticeIndex % 2 == 0) { _direction = "down"; } // ���� ����� �����, �� ������ ������� �������, � ������ ����. ����� ����� (������)
			//		else { _direction = "up"; } // ����������, ����. ����� ������ (��������)
			//	}
			//}

	}

		// ��������� ���������� ������ ���������, ����� �������� ������� � ��������� ���������� ��-�� ��������� �������
		//if (_direction == "up" || _direction == "down") {
		//	if (Math::Abs(nextPoint->Y - _yPos) <= 10) {
		//		while (b || (_npCrossroadIndex == npCrossroadIndex2)) {
		//			npCrossroadIndex2 = rndGen->Next(0, VERTEX_QUANTITY);
		//			nextPoint2 = Point(Vertices[npCrossroadIndex2][rndGen->Next(0, 4)]);
		//			b = Math::Abs(_nextPoint->Y - nextPoint2->Y) < 0.1;
		//		}

		//		_xPos = _nextPoint->X - (TAXICAR_IMG_HEIGHT / 2);
		//		if (_nextPoint->X < nextPoint2->X) { _direction = "right"; }
		//		else { _direction = "left"; }
		//	}
		//}
		//else {
		//	if (Math::Abs(nextPoint->X - _xPos) <= 10) {
		//		while (a || (_npCrossroadIndex == npCrossroadIndex2)) {
		//			npCrossroadIndex2 = rndGen->Next(0, VERTEX_QUANTITY);
		//			nextPoint2 = Point(Vertices[npCrossroadIndex2][rndGen->Next(0, 4)]);
		//			a = Math::Abs(_nextPoint->X - nextPoint2->X) < 0.1;
		//		}

		//		_yPos = _nextPoint->Y - (TAXICAR_IMG_HEIGHT / 2);
		//		if (_nextPoint->Y < nextPoint2->Y) { _direction = "down"; }
		//		else { _direction = "up"; }
		//	}
		//}

};


public ref class MyEnvironment {
private:
	int vertexQuantity;
	array<Point^>^ cordinates;
	array<array<Point^>^>^ Vertices;

public:
	System::Collections::Generic::List<TaxiCar^>^ TaxiCars;

	MyEnvironment() {
		TaxiCars = gcnew System::Collections::Generic::List<TaxiCar^>(0);
		vertexQuantity = VERTEX_QUANTITY;
		cordinates = gcnew array<Point^>(vertexQuantity) { Point(46, 419), Point(46, 575),  Point(151, 151), Point(402, 151),
			Point(402, 419), Point(447, 575), Point(447, 781), Point(524, 59), Point(524, 419), Point(803, 59), Point(803, 302), Point(803, 575) };
		Vertices = gcnew array<array<Point^>^>(vertexQuantity);
	}
	~MyEnvironment() {};

	// ������� ��������� ������� ������ �� ������� ���������
	Void VerticesGen() {
		for (int i = 0; i < vertexQuantity; i++) {
			Vertices[i] = gcnew array<Point^>(4);
		}

		for (int i = 0; i < vertexQuantity; i++) {
			for (int j = 0; j < 4; j++) {
				Vertices[i][j] = gcnew Point(cordinates[i]->X + ((j % 2) * 24), cordinates[i]->Y + ((j > 1) * 24));
			}
		}
	}

	// ������� ��������� ������� ������ �����
	void TaxiSpawn(Label^ label1, Label^ label2, Label^ label3, Label^ label4) {
		Random^ rndGen = gcnew Random();
		if (TaxiCars->Count < MAX_TAXICARS) {
			TaxiCars->Add(gcnew TaxiCar());

			// ��� ���������� ����� ��� ������ ����� ������ �����
			int crossroadIndex1 = rndGen->Next(0, vertexQuantity);
			int crossroadIndex2 = rndGen->Next(0, vertexQuantity);
			int crossroadIndex3 = rndGen->Next(0, vertexQuantity);

			// ��� ���������� ����� ��� ����������� ����������� ����������� ������
			int verticeIndex = rndGen->Next(0, 4);
			int verticeIndex2 = rndGen->Next(0, 4);
			int verticeIndex3 = rndGen->Next(0, 4);

			// ����������� ������������� �����, � �����, ������� ����� ������ ������ (spawnPoint2).
			Point^ spawnPoint = Vertices[crossroadIndex1][verticeIndex];

			bool a = (spawnPoint->X == Vertices[crossroadIndex2][verticeIndex2]->X); // ������ X
			bool b = (spawnPoint->Y == Vertices[crossroadIndex2][verticeIndex2]->Y); // ������ Y

			// !((a + b) % 2) - ���� ������ ���������� ����� �� ��� ���, ���� �� ��������� ������ ���� �� 2� ������� (���� a, ���� b)
			// || (crossroadIndex1 == crossroadIndex2) - ������� ����� ��� ����, ����� �������� �������� ������ ���� ����� ������ ����������
			while (!((a + b) % 2) || (crossroadIndex1 == crossroadIndex2)) {
				crossroadIndex2 = rndGen->Next(0, vertexQuantity);

				for (int i = 0; i < 4; i++) {
					a = (spawnPoint->X == Vertices[crossroadIndex2][i]->X);
					b = (spawnPoint->Y == Vertices[crossroadIndex2][i]->Y);
					if ((a + b) % 2) { break; }
				}
			}

			if (a && !b) { // ������ ������ �� ���������
				if (crossroadIndex2 > crossroadIndex1) {
					verticeIndex = 2 * rndGen->Next(0, 2);
					verticeIndex2 = 2 * rndGen->Next(0, 2);
				}
				else if (crossroadIndex2 < crossroadIndex1) {
					verticeIndex = 2 * rndGen->Next(0, 2) + 1;
					verticeIndex2 = 2 * rndGen->Next(0, 2) + 1;
				}
			}
			else if (b && !a) { // ������ ������ �� �����������
				if (crossroadIndex2 > crossroadIndex1) {
					verticeIndex = rndGen->Next(2, 4);
					verticeIndex2 = rndGen->Next(2, 4);
				}
				else if (crossroadIndex2 < crossroadIndex1) {
					verticeIndex = rndGen->Next(0, 2);
					verticeIndex2 = rndGen->Next(0, 2);
				}
			}

			spawnPoint = Vertices[crossroadIndex1][verticeIndex];
			Point^ nextPoint = Vertices[crossroadIndex2][verticeIndex2];

			//��� ������������
			label1->Text = Convert::ToString(crossroadIndex1);
			label2->Text = Convert::ToString(verticeIndex);

			label3->Text = Convert::ToString(crossroadIndex3);
			label4->Text = Convert::ToString(verticeIndex3);

			//���������� ����� ������� ����������
			TaxiCars[TaxiCars->Count - 1]->nextPoint::set(nextPoint);
			TaxiCars[TaxiCars->Count - 1]->npCrossroadIndex::set(crossroadIndex2);
			TaxiCars[TaxiCars->Count - 1]->npVerticeIndex::set(verticeIndex2);

			// ����������� �����������

			if (Math::Abs(spawnPoint->Y - nextPoint->Y) < 0.1) {
				// ������������� �������� ��������� ������� ����� ��������������� ��������
				TaxiCars[TaxiCars->Count - 1]->xPos::set(rndGen->Next(Math::Min(spawnPoint->X, nextPoint->X), Math::Max(spawnPoint->X, nextPoint->X)) - (TAXICAR_IMG_HEIGHT / 2));
				TaxiCars[TaxiCars->Count - 1]->yPos::set(spawnPoint->Y - (TAXICAR_IMG_HEIGHT / 2));
				// ���� verticeIndex > 1, �� ������ ��������� � ������ ����� ����������, �������������� � ����������� - ������
				if (crossroadIndex2 > crossroadIndex1) { // verticeIndex > 1) {
					TaxiCars[TaxiCars->Count - 1]->direction::set("right");
				}
				else {
					TaxiCars[TaxiCars->Count - 1]->direction::set("left");
				}
			}
			else { // ���� ������ X
				TaxiCars[TaxiCars->Count - 1]->xPos::set(spawnPoint->X - (TAXICAR_IMG_HEIGHT / 2));
				TaxiCars[TaxiCars->Count - 1]->yPos::set(rndGen->Next(Math::Min(spawnPoint->Y, nextPoint->Y), Math::Max(spawnPoint->Y, nextPoint->Y)) - (TAXICAR_IMG_HEIGHT / 2));
				// ���� verticeIndex % 2 ����� 0, �� ������ ��������� � ����� ����� ����������, � ������ � ����������� - ����
				if (crossroadIndex2 < crossroadIndex1) { // verticeIndex % 2 == 0) {
					TaxiCars[TaxiCars->Count - 1]->direction::set("up");
				}
				else { TaxiCars[TaxiCars->Count - 1]->direction::set("down"); }
			}


			// ��������� 3-�� �����. ������� �� ��������� 2-�� � ���, ��� ������ ����� ����� ���� �� ����� ����������
			// (������ ����� ���������� �� ������ ���� ���, ������ ������ ��� �������� ������)
			// ��� ����������, ��� ��� ����� ���������� �������� ����������, ������� ������� ������ � ����� ������ ����������� (������ � �������� ������)

			// !!�������� ����� ����������� � ����� Move ������ TaxiCar!!

			a = (nextPoint->X == Vertices[crossroadIndex3][verticeIndex3]->X);
			b = (Vertices[crossroadIndex2][verticeIndex2]->Y == Vertices[crossroadIndex3][verticeIndex3]->Y);

			while (!((a + b) % 2) || (crossroadIndex3 == crossroadIndex1)) {
				crossroadIndex3 = rndGen->Next(0, vertexQuantity);

				for (int i = 0; i < 4; i++) {
					a = (Vertices[crossroadIndex2][verticeIndex2]->X == Vertices[crossroadIndex3][i]->X);
					b = (Vertices[crossroadIndex2][verticeIndex2]->Y == Vertices[crossroadIndex3][i]->Y);
					if ((a + b) % 2) { break; }
				}
			}

			if (crossroadIndex3 != crossroadIndex2) {
				if (a && !b) { // ������ ������ �� ���������
					if (crossroadIndex3 > crossroadIndex2) { verticeIndex3 = 2 * rndGen->Next(0, 2); }
					else if (crossroadIndex3 < crossroadIndex2) { verticeIndex3 = 2 * rndGen->Next(0, 2) + 1; }
				}
				else if (b && !a) { // ������ ������ �� �����������
					if (crossroadIndex3 > crossroadIndex2) { verticeIndex3 = rndGen->Next(2, 4); }
					else if (crossroadIndex3 < crossroadIndex2) { verticeIndex3 = rndGen->Next(0, 2); }
				}
			}
			else {
				//����� �� ���������� ������ ������������� � ����������� �� �������� ����������� �������� ������
			}
		}
		// � ���������� ���������� ��� ������ ������ ������ ����� � ���������� ��� �������� ��������� �����
		// 1) ���������� (x; y)
		// 2) ����������� �������� (direction)
	}

	void TimerTickActions() {
		for (int i = 0; i < TaxiCars->Count; i++) TaxiCars[i]->Move(Vertices);
	}
};


public ref class RoadVertices {
private:
	array<Point^>^ _cordinate;
	int vertexId = 1;

public:
	property array<Point^>^ cordinate {
		array<Point^>^ get() { return _cordinate; }
		void set(array<Point^>^ _value) {
			_cordinate = _value;
		}
	}
};