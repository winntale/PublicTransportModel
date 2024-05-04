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
			if (System::String::IsNullOrEmpty(_value)) {
				MessageBox::Show("Вы не указали цвет машины такси", "Ошибка",
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

	// метод описания движения
	void Move(array<array<Point^>^>^ Vertices) {
		// в зависимости от направления (direction) меняются координаты x, y
		if (_direction == "left") { _xPos -= SPEED; }
		else if (_direction == "right") { _xPos += SPEED; }

		else if (_direction == "down") { _yPos += SPEED; }
		else if (_direction == "up") { _yPos -= SPEED; }


		Random^ rndGen = gcnew Random();

		bool a = false; // совпали X
		bool b = false; // совпали Y
		int np2CrossroadIndex = 0;
		int np2VerticeIndex = 0;
		// если совпали Y-и у 1 и 2 точки
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
		

			// пока нет совпадения одной из координат у 2 и 3 точки
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
			//		if (np2VerticeIndex % 2 == 0) { _direction = "down"; } // если точка слева, то машина повернёт направо, а значит след. точка слева (чётная)
			//		else { _direction = "up"; } // аналогично, след. точка справа (нечётная)
			//	}
			//}

	}

		// проверяем расстояние вместо равенства, чтобы избежать проблем с точностью вычислений из-за плавающей запятой
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
			Point(402, 419), Point(447, 575), Point(447, 781), Point(524, 59), Point(524, 419), Point(803, 59), Point(803, 575) };
		Vertices = gcnew array<array<Point^>^>(vertexQuantity);
	}
	~MyEnvironment() {};

	// метод генерации массива вершин по массиву координат
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

	// метод генерации объекта машины такси
	void TaxiSpawn(Label^ label1, Label^ label2, Label^ label3, Label^ label4) {
		Random^ rndGen = gcnew Random();
		if (TaxiCars->Count < MAX_TAXICARS) {
			TaxiCars->Add(gcnew TaxiCar());

			// следующие переменные определяют
			// 1) 2 точки на 2-х разных перекрёстках, образующие линию, на произвольной точке которой сгенерируется машина
			// 2) точку на 3-м перекрёстке (не равном 1-ому), куда будет поворачивать машина
			// фиксированная точка определена элементами с индексами 1, следующие две точки - с индексами 2 и 3
			int crossroadIndex1 = rndGen->Next(0, vertexQuantity);
			int crossroadIndex2 = rndGen->Next(0, vertexQuantity);
			int crossroadIndex3 = rndGen->Next(0, vertexQuantity);

			int verticeIndex1 = rndGen->Next(0, 4);
			int verticeIndex2 = rndGen->Next(0, 4);
			int verticeIndex3 = rndGen->Next(0, 4);


			bool a = (Vertices[crossroadIndex1][verticeIndex1]->X == Vertices[crossroadIndex2][verticeIndex2]->X); // совпал X у 2-х точек (1-ая и 2-ая)
			bool b = (Vertices[crossroadIndex1][verticeIndex1]->Y == Vertices[crossroadIndex2][verticeIndex2]->Y); // совпал Y у 2-х точек (1-ая и 2-ая)

			// !((a + b) % 2) - цикл меняет координаты точки до тех пор, пока не произойдёт ТОЛЬКО одно из 2х событий (либо a, либо b)
			// || (crossroadIndex1 == crossroadIndex2) - условие нужно для того, чтобы избежать ситуации взятия двух точек одного перекрёстка
 			while (!((a + b) % 2) || (crossroadIndex1 == crossroadIndex2)) {
				crossroadIndex2 = rndGen->Next(0, vertexQuantity);

				// цикл определяет, есть ли путь до 2-го перекрёстка
				for (int i = 0; i < 4; i++) {
					a = (Vertices[crossroadIndex1][verticeIndex1]->X == Vertices[crossroadIndex2][i]->X);
					b = (Vertices[crossroadIndex1][verticeIndex1]->Y == Vertices[crossroadIndex2][i]->Y);
					if ((a + b) % 2) { break; }
				}
			}

			bool a1 = (Vertices[crossroadIndex2][verticeIndex2]->X == Vertices[crossroadIndex3][verticeIndex3]->X); // совпал X у 2-ой и 3-й точек
			bool b1 = (Vertices[crossroadIndex2][verticeIndex2]->Y == Vertices[crossroadIndex3][verticeIndex3]->Y); // совпал Y у 2-ой и 3-й точек

			if (a && !b) { // машина поедет по вертикали
				if (crossroadIndex2 > crossroadIndex1) { verticeIndex1 = 2 * rndGen->Next(0, 2); } // вниз
				else if (crossroadIndex2 < crossroadIndex1) { verticeIndex1 = 2 * rndGen->Next(0, 2) + 1; } // вверх

				while (!b1 || a1) {
					crossroadIndex3 = rndGen->Next(0, vertexQuantity);

					for (int i = 0; i < 4; i++) {
						a1 = (Vertices[crossroadIndex2][verticeIndex2]->X == Vertices[crossroadIndex3][verticeIndex3]->X);
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
				else {
					verticeIndex2 = verticeIndex1;
					verticeIndex3 = verticeIndex2 + Math::Pow(-1, (verticeIndex2 % 2));
				}
			}
			else if (b && !a) { // машина поедет по горизонтали
				if (crossroadIndex2 > crossroadIndex1) { verticeIndex1 = rndGen->Next(2, 4); } // направо
				else if (crossroadIndex2 < crossroadIndex1) { verticeIndex1 = rndGen->Next(0, 2); } // налево

				while (!a1 || b1) {
					crossroadIndex3 = rndGen->Next(0, vertexQuantity);

					for (int i = 0; i < 4; i++) {
						a1 = (Vertices[crossroadIndex2][verticeIndex2]->X == Vertices[crossroadIndex3][i]->X);
						b1 = (Vertices[crossroadIndex2][verticeIndex2]->Y == Vertices[crossroadIndex3][i]->Y);
						if (a1 && !b1) { break; }
					}
				}

				if (crossroadIndex3 > crossroadIndex2) {
					verticeIndex2 = Math::Floor(verticeIndex1 / 2) * 2;
					verticeIndex3 = 2 * rndGen->Next(0, 2);
				}
				else if (crossroadIndex3 < crossroadIndex2) {
					verticeIndex2 = Math::Floor(verticeIndex1 / 2) * 2 + 1;
					verticeIndex3 = 2 * rndGen->Next(0, 2) + 1;
				}
				else {
					verticeIndex2 = verticeIndex1;
					verticeIndex3 = (verticeIndex2 + 2) % 4;
				}
			}

			//для тестирования
			label1->Text = Convert::ToString(crossroadIndex1);
			label2->Text = Convert::ToString(verticeIndex1);

			// ГЕНЕРАЦИЯ 3-ЕЙ ТОЧКИ. отличие от генерации 2-ой в том, что теперь точки могут быть на одном перекрёстке
			// (однако новый перекрёсток не должен быть тем, откуда только что приехала машина)
			// это необходимо, так как может возникнуть ситуация перекрёстка, который соединён только с одним другим перекрёстком (откуда и приехала машина)

			// !!ВОЗМОЖНО СТОИТ ПЕРЕМЕСТИТЬ В МЕТОД Move КЛАССА TaxiCar!!

			//для тестирования
			label3->Text = Convert::ToString(crossroadIndex3);
			label4->Text = Convert::ToString(verticeIndex3);

			Point^ spawnPoint = Vertices[crossroadIndex1][verticeIndex1];
			Point^ nextPoint = Vertices[crossroadIndex2][verticeIndex2];
			Point^ nextPoint2 = Vertices[crossroadIndex3][verticeIndex3];


			//заполнение полей объекта свойствами
			TaxiCars[TaxiCars->Count - 1]->nextPoint::set(nextPoint);
			TaxiCars[TaxiCars->Count - 1]->npCrossroadIndex::set(crossroadIndex2);
			TaxiCars[TaxiCars->Count - 1]->npVerticeIndex::set(verticeIndex2);

			// ОПРЕДЕЛЕНИЕ НАПРАВЛЕНИЯ

			if (Math::Abs(spawnPoint->Y - nextPoint->Y) < 0.1) {
				// устанавливаем значения координат объекту через соответствующие свойства
				TaxiCars[TaxiCars->Count - 1]->xPos::set(rndGen->Next(Math::Min(spawnPoint->X, nextPoint->X), Math::Max(spawnPoint->X, nextPoint->X)) - (TAXICAR_IMG_HEIGHT / 2));
				TaxiCars[TaxiCars->Count - 1]->yPos::set(spawnPoint->Y - (TAXICAR_IMG_HEIGHT / 2));
				// если verticeIndex > 1, то машина находится в нижней части перекрёстка, соответственно её направление - вправо
				if (crossroadIndex2 > crossroadIndex1) { // verticeIndex > 1) {
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
				if (crossroadIndex2 < crossroadIndex1) { // verticeIndex % 2 == 0) {
					TaxiCars[TaxiCars->Count - 1]->direction::set("up");
				}
				else { TaxiCars[TaxiCars->Count - 1]->direction::set("down"); }
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