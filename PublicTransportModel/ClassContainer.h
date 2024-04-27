#pragma once
using namespace System;
using namespace System::Drawing;
using namespace System::Windows::Forms;

#include "Defines.h"

public ref class TaxiCar {
private:
	int _xPos = 0;
	int _yPos = 0;
	int _maxVelocity; // можно объединить с текущей скоростью, ограничив её
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
			if (System::String::IsNullOrEmpty(_value)) { MessageBox::Show("Вы не указали цвет машины такси", "Ошибка",
				MessageBoxButtons::OK, MessageBoxIcon::Error); }
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

	// метод описания движения
	void Move(array<array<Point^>^>^ Vertices) {
		Random^ rndGen = gcnew Random();
		int npCrossroadIndex2 = rndGen->Next(0, VERTEX_QUANTITY);
		Point^ nextPoint2 = Point(Vertices[npCrossroadIndex2][rndGen->Next(0, 4)]);
		bool a = true;
		bool b = true;

		// в зависимости от направления (direction) меняются координаты x, y
		if (_direction == "left") { _xPos -= (_maxVelocity / 30); }
		else if (_direction == "right") { _xPos += (_maxVelocity / 30); }

		else if (_direction == "down") { _yPos += (_maxVelocity / 30); }
		else if (_direction == "up") { _yPos -= (_maxVelocity / 30); }

		if (_xPos == _nextPoint->X) {
			if (Math::Abs(nextPoint->Y - _yPos - (_maxVelocity / 30)) <= 10) {
				while (b || (_npCrossroadIndex == npCrossroadIndex2)) {
					npCrossroadIndex2 = rndGen->Next(0, VERTEX_QUANTITY);
					nextPoint2 = Point(Vertices[npCrossroadIndex2][rndGen->Next(0, 4)]);
					b = (_nextPoint->Y == nextPoint2->Y);
				}

				_xPos = _nextPoint->X - (TAXICAR_IMG_HEIGHT / 2);
				if (_nextPoint->X < nextPoint2->X) { _direction = "right"; }
				else { _direction = "left"; }
			}
		}
		else {
			if (Math::Abs(nextPoint->X - _xPos - (_maxVelocity / 30)) <= 10) {
				while (a || (_npCrossroadIndex == npCrossroadIndex2)) {
					npCrossroadIndex2 = rndGen->Next(0, VERTEX_QUANTITY);
					nextPoint2 = Point(Vertices[npCrossroadIndex2][rndGen->Next(0, 4)]);
					a = (_nextPoint->X == nextPoint2->X);
				}

				_yPos = _nextPoint->Y - (TAXICAR_IMG_HEIGHT / 2);
				if (_nextPoint->Y < nextPoint2->Y) { _direction = "down"; }
				else { _direction = "up"; }
			}
		}
	}

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

	// функция генерации массива вершин по массиву координат
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

	// функция генерации объекта машины такси
	void TaxiSpawn() {
		Random^ rndGen = gcnew Random();
		if (TaxiCars->Count < MAX_TAXICARS) {
			TaxiCars->Add(gcnew TaxiCar());

			// эти переменные нужны для отбора далее второй точки
			int crossroadIndex1 = rndGen->Next(0, vertexQuantity);
			int crossroadIndex2 = rndGen->Next(0, vertexQuantity);

			// эта переменная нужна для дальнейшего определения направления машины
			int verticeIndex = rndGen->Next(0, 4);

			// определение фиксированной точки, и точки, которую будем менять дальше (spawnPoint2). сейчас же определяем её, как пустую (корд. -1; -1)
			Point^ spawnPoint = Point(Vertices[crossroadIndex1][verticeIndex]);
			Point^ nextPoint = Point(Vertices[crossroadIndex2][rndGen->Next(0, 4)]);

			// !((a + b) % 2) - цикл меняет координаты точки до тех пор, пока не произойдёт ТОЛЬКО одно из 2х событий (либо a, либо b)
			// || (crossroadIndex1 == crossroadIndex2) - условие нужно для того, чтобы избежать ситуации взятия двух точек одного перекрёстка
			while (!(((spawnPoint->X == nextPoint->X) + (spawnPoint->Y == nextPoint->Y)) % 2) || (crossroadIndex1 == crossroadIndex2)) {
				crossroadIndex2 = rndGen->Next(0, vertexQuantity);
				nextPoint = Point(Vertices[crossroadIndex2][rndGen->Next(0, 4)]);
			}

			TaxiCars[TaxiCars->Count - 1]->nextPoint::set(nextPoint);
			TaxiCars[TaxiCars->Count - 1]->npCrossroadIndex::set(crossroadIndex2);

			if (spawnPoint->Y == nextPoint->Y) {
				// устанавливаем значения координат объекту через соответствующие свойства
				TaxiCars[TaxiCars->Count - 1]->xPos::set(rndGen->Next(Math::Min(spawnPoint->X, nextPoint->X), Math::Max(spawnPoint->X, nextPoint->X)) - (TAXICAR_IMG_HEIGHT / 2));
				TaxiCars[TaxiCars->Count - 1]->yPos::set(spawnPoint->Y - (TAXICAR_IMG_HEIGHT / 2));
				// если verticeIndex > 1, то машина находится в нижней части перекрёстка, соответственно её направление - вправо
				if (verticeIndex > 1) {
					TaxiCars[TaxiCars->Count - 1]->direction::set("right");
				}
				else {
					TaxiCars[TaxiCars->Count - 1]->direction::set("left");
				}
			}
			else { // если совпал X
				TaxiCars[TaxiCars->Count - 1]->xPos::set(spawnPoint->X - (TAXICAR_IMG_HEIGHT / 2));
				TaxiCars[TaxiCars->Count - 1]->yPos::set(rndGen->Next(Math::Min(spawnPoint->Y, nextPoint->Y), Math::Max(spawnPoint->Y, nextPoint->Y)) - (TAXICAR_IMG_HEIGHT / 2));
				// если verticeIndex % 2 равен 0, то машина находится в левой части перекрёстка, а значит её направление - вниз
				if (verticeIndex % 2 == 0) {
					TaxiCars[TaxiCars->Count - 1]->direction::set("down");
				}
				else { TaxiCars[TaxiCars->Count - 1]->direction::set("up"); }
			}
		}
		// в результате приведённый код создаёт объект машины такси и генерирует ему значения следующих полей
		// 1) координаты (x; y)
		// 2) направление движения (direction)
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