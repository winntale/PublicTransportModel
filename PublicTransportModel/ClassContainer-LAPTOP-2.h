#pragma once
using namespace System;
using namespace System::Drawing;
using namespace System::Windows::Forms;

#include "Defines.h"

public ref class TaxiCar {
private:
	int _xPos;
	int _yPos;

	int _maxVelocity;
	System::String^ _direction;
	System::String^ _color;
	int _fuelWaste;
	int _currentFuel;
	int _state; // 0 - ожидание; 1 - выполнение заказа; 2 - низкий уровень топлива

	// поля для 2-ой точки
	Point^ _nextPoint;
	int _npCrossroadIndex;
	int _npVerticeIndex;

	// поля для 3-ей точки
	Point^ _nextPoint2;
	int _npCrossroadIndex2;
	int _npVerticeIndex2;

public:
	TaxiCar() {
		_xPos = 0;
		_yPos = 0;

		_fuelWaste = 1;
		_currentFuel = 100;
		_state = 0;
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
				MessageBox::Show("Вы не указали цвет машины такси", "Ошибка",
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

	// метод описания движения и поворота в 0 состоянии машины (завершён)
	void Move(array<array<Point^>^>^ Vertices) {
		// в зависимости от направления (direction) меняются координаты x, y
		if (_direction == "left") { _xPos -= SPEED; }
		else if (_direction == "right") { _xPos += SPEED; }

		else if (_direction == "down") { _yPos += SPEED; }
		else if (_direction == "up") { _yPos -= SPEED; }


		Random^ rndGen = gcnew Random();

		bool isTurned = false;
		String^ _direction2 = _direction;


		// рассмотрим 2 ситуации: 1) машина движется по Y	2) машина движется по X
		// машина достигла Y (двигалась по вертикали)
		if (_direction == "up" || _direction == "down") {
			if ((Math::Abs(_yPos - (_nextPoint->Y - (TAXICAR_IMG_HEIGHT / 2))) < SPEED) && !isTurned) {
				_yPos = _nextPoint->Y - (TAXICAR_IMG_HEIGHT / 2); // корректировка Y

				bool a = (Vertices[_npCrossroadIndex][_npVerticeIndex]->X == Vertices[_npCrossroadIndex2][_npVerticeIndex2]->X); // совпал X у 1-й и 2-й точки
				bool b = (Vertices[_npCrossroadIndex][_npVerticeIndex]->Y == Vertices[_npCrossroadIndex2][_npVerticeIndex2]->Y); // совпал Y у 1-й и 2-й точки

				// (2) -> (1) (то есть (2) становится startPoint'ом)
				int crossroadIndex1 = _npCrossroadIndex;
				int verticeIndex1 = _npVerticeIndex;

				// (3) -> (2) (то есть (3) становится nextPoint'ом)
				_nextPoint = _nextPoint2;
				_npCrossroadIndex = _npCrossroadIndex2;
				_npVerticeIndex = _npVerticeIndex2;

				// задаём горизонтальное движение до текущей (2)
				if (_npCrossroadIndex > crossroadIndex1) { _direction2 = "right"; } // направо
				else if (_npCrossroadIndex < crossroadIndex1) { _direction2 = "left"; } // налево

				// (*) -> (3). создание новой точки, которая станет nextPoint2'ом
				// 1) генерируем перекрёсток (*), пока не достигнем горизонтального движения
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
		// машина достигла X (двигалась по горизонтали)
		else if (_direction == "right" || _direction == "left") {
			if ((Math::Abs(_xPos - (_nextPoint->X - (TAXICAR_IMG_HEIGHT / 2))) < SPEED) && !isTurned) {
				_xPos = _nextPoint->X - (TAXICAR_IMG_HEIGHT / 2); // корректировка X

				bool a = (Vertices[_npCrossroadIndex][_npVerticeIndex]->X == Vertices[_npCrossroadIndex2][_npVerticeIndex2]->X); // совпал X у 1-й и 2-й точки
				bool b = (Vertices[_npCrossroadIndex][_npVerticeIndex]->Y == Vertices[_npCrossroadIndex2][_npVerticeIndex2]->Y); // совпал Y у 1-й и 2-й точки

				// (2) -> (1) (то есть (2) становится startPoint'ом)
				int crossroadIndex1 = _npCrossroadIndex;
				int verticeIndex1 = _npVerticeIndex;

				// (3) -> (2) (то есть (3) становится nextPoint'ом)
				_nextPoint = _nextPoint2;
				_npCrossroadIndex = _npCrossroadIndex2;
				_npVerticeIndex = _npVerticeIndex2;

				// задаём вертикальное движение до текущей (2)
				if (_npCrossroadIndex > crossroadIndex1) { _direction2 = "down"; } // вниз
				else if (_npCrossroadIndex < crossroadIndex1) { _direction2 = "up"; } // вверх

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

	// метод поиска пути
	//void WayFind(Passenger^ passenger) {
	//	Point^ pasPoint = Point(passenger->xPos::get(), passenger->yPos::get());

	//	//логика поиска пути
	//}

};

public ref class Passenger {
private:
	int _xPos;
	int _yPos;

	int _state; // 0 - выбор машины; 1 - ожидание машины; 2 - в пункте прибытия
//	MyEnvironment^ env;

public:
	Passenger() {
		_xPos = 0;
		_yPos = 0;
		_state = 0;
	}
	~Passenger() {};

public:
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

	//void TaxiChoise() {
	//	if (env->Passengers->Count) {
	//		Random^ rndGen = gcnew Random();

	//		TaxiCar^ ServiceCar = env->TaxiCars[rndGen->Next(0, env->TaxiCars->Count)];

	//		if (ServiceCar->state::get() != 2) { ServiceCar->state::set(1); }
	//	}
	//}
};


public ref class MyEnvironment {
private:
	array<Point^>^ cordinates;
	array<array<Point^>^>^ Vertices;
	System::Collections::Generic::List<TaxiCar^>^ _TaxiCars;
	System::Collections::Generic::List<Passenger^>^ _Passengers;

public:
	MyEnvironment() {
		_TaxiCars = gcnew System::Collections::Generic::List<TaxiCar^>(0);
		_Passengers = gcnew System::Collections::Generic::List<Passenger^>(0);
		cordinates = gcnew array<Point^>(VERTEX_QUANTITY) { Point(151, 39), Point(151, 290), Point(151, 792),
			Point(402, 39), Point(402, 290), Point(402, 541), Point(402, 792), Point(653, 39), Point(653, 290), Point(653, 541), Point(904, 290), Point(904, 541), Point(904, 792) };
		Vertices = gcnew array<array<Point^>^>(VERTEX_QUANTITY);
	}
	~MyEnvironment() {};

	property System::Collections::Generic::List<TaxiCar^>^ TaxiCars {
		System::Collections::Generic::List<TaxiCar^>^ get() { return _TaxiCars; }
	}

	property System::Collections::Generic::List<Passenger^>^ Passengers {
		System::Collections::Generic::List<Passenger^>^ get() { return _Passengers; }
	}

	// метод генерации массива вершин по массиву координат
	void VerticesGen() {
		for (int i = 0; i < VERTEX_QUANTITY; i++) {
			Vertices[i] = gcnew array<Point^>(4);
		}

		for (int i = 0; i < VERTEX_QUANTITY; i++) {
			for (int j = 0; j < 4; j++) {
				Vertices[i][j] = gcnew Point(cordinates[i]->X + ((j % 2) * 24), cordinates[i]->Y + ((j > 1) * 24));
			}
		}
	}

	// метод генерации объекта машины такси (завершён)
	void TaxiSpawn() {
		Random^ rndGen = gcnew Random();
		if (TaxiCars->Count < MAX_TAXICARS) {
			TaxiCars->Add(gcnew TaxiCar());

			// следующие переменные определяют
			// 1) 2 точки на 2-х разных перекрёстках, образующие линию, на произвольной точке которой сгенерируется машина
			// 2) точку на 3-м перекрёстке (не равном 1-ому), куда будет поворачивать машина
			// фиксированная точка определена элементами с индексами 1, следующие две точки - с индексами 2 и 3
			int crossroadIndex1 = rndGen->Next(0, VERTEX_QUANTITY);
			int crossroadIndex2 = rndGen->Next(0, VERTEX_QUANTITY);
			int crossroadIndex3 = rndGen->Next(0, VERTEX_QUANTITY);

			int verticeIndex1 = rndGen->Next(0, 4);
			int verticeIndex2 = rndGen->Next(0, 4);
			int verticeIndex3 = rndGen->Next(0, 4);


			bool a = (Vertices[crossroadIndex1][verticeIndex1]->X == Vertices[crossroadIndex2][verticeIndex2]->X); // совпал X у 2-х точек (1-ая и 2-ая)
			bool b = (Vertices[crossroadIndex1][verticeIndex1]->Y == Vertices[crossroadIndex2][verticeIndex2]->Y); // совпал Y у 2-х точек (1-ая и 2-ая)

			// !((a + b) % 2) - цикл меняет координаты точки до тех пор, пока не произойдёт ТОЛЬКО одно из 2х событий (либо a, либо b)
			// || (crossroadIndex1 == crossroadIndex2) - условие нужно для того, чтобы избежать ситуации взятия двух точек одного перекрёстка
 			while (!((a + b) % 2) || (crossroadIndex1 == crossroadIndex2)) {
				crossroadIndex2 = rndGen->Next(0, VERTEX_QUANTITY);

				// цикл определяет, есть ли путь до 2-го перекрёстка
				for (int i = 0; i < 4; i++) {
					a = (Vertices[crossroadIndex1][verticeIndex1]->X == Vertices[crossroadIndex2][i]->X);
					b = (Vertices[crossroadIndex1][verticeIndex1]->Y == Vertices[crossroadIndex2][i]->Y);
					if ((a + b) % 2) { break; }
				}
			}

			// ГЕНЕРАЦИЯ 3-ЕЙ ТОЧКИ. отличие от генерации 2-ой в том, что теперь точки могут быть на одном перекрёстке
			// (однако новый перекрёсток не должен быть тем, откуда только что приехала машина)
			// это необходимо, так как может возникнуть ситуация перекрёстка, который соединён только с одним другим перекрёстком (откуда и приехала машина)

			bool a1 = (Vertices[crossroadIndex2][verticeIndex2]->X == Vertices[crossroadIndex3][verticeIndex3]->X); // совпал X у 2-ой и 3-й точек
			bool b1 = (Vertices[crossroadIndex2][verticeIndex2]->Y == Vertices[crossroadIndex3][verticeIndex3]->Y); // совпал Y у 2-ой и 3-й точек

			if (a && !b) { // машина изначально поедет по вертикали (корректировка 1-ой точки)
				if (crossroadIndex2 > crossroadIndex1) { verticeIndex1 = 2 * rndGen->Next(0, 2); } // вниз
				else if (crossroadIndex2 < crossroadIndex1) { verticeIndex1 = 2 * rndGen->Next(0, 2) + 1; } // вверх

				while (!b1 || a1 || (crossroadIndex3 == crossroadIndex2)) { // пока не найден путь по горизонтали
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
			else if (b && !a) { // машина изначально поедет по горизонтали
				if (crossroadIndex2 > crossroadIndex1) { verticeIndex1 = rndGen->Next(2, 4); } // направо
				else if (crossroadIndex2 < crossroadIndex1) { verticeIndex1 = rndGen->Next(0, 2); } // налево

				while (!a1 || b1 || (crossroadIndex3 == crossroadIndex2)) { // пока не найден путь по вертикали
					crossroadIndex3 = rndGen->Next(0, VERTEX_QUANTITY);

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
			}

			Point^ spawnPoint = Vertices[crossroadIndex1][verticeIndex1];
			Point^ nextPoint = Vertices[crossroadIndex2][verticeIndex2];
			Point^ nextPoint2 = Vertices[crossroadIndex3][verticeIndex3];

			//заполнение полей объекта свойствами
			TaxiCars[TaxiCars->Count - 1]->nextPoint::set(nextPoint);
			TaxiCars[TaxiCars->Count - 1]->npCrossroadIndex::set(crossroadIndex2);
			TaxiCars[TaxiCars->Count - 1]->npVerticeIndex::set(verticeIndex2);

			TaxiCars[TaxiCars->Count - 1]->nextPoint2::set(nextPoint2);
			TaxiCars[TaxiCars->Count - 1]->npCrossroadIndex2::set(crossroadIndex3);
			TaxiCars[TaxiCars->Count - 1]->npVerticeIndex2::set(verticeIndex3);

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

	void PassengerSpawn(Label^ label3, Label^ label4, Label^ label5, Label^ label6, Label^ label7, Label^ label8, Label^ label9) {
		Random^ rndGen = gcnew Random();
		int randomNumber = rndGen->Next(0, 101);

		if (TaxiCars->Count && (Passengers->Count < TaxiCars->Count) && (randomNumber > 49)) {
			Passengers->Add(gcnew Passenger());

			int crossroadIndex1 = rndGen->Next(0, VERTEX_QUANTITY);
			int crossroadIndex2 = rndGen->Next(0, VERTEX_QUANTITY);

			int verticeIndex1 = rndGen->Next(0, 4);
			int verticeIndex2 = rndGen->Next(0, 4);


			bool a = (Vertices[crossroadIndex1][verticeIndex1]->X == Vertices[crossroadIndex2][verticeIndex2]->X); // совпал X у 2-х точек (1-ая и 2-ая)
			bool b = (Vertices[crossroadIndex1][verticeIndex1]->Y == Vertices[crossroadIndex2][verticeIndex2]->Y); // совпал Y у 2-х точек (1-ая и 2-ая)

			// !((a + b) % 2) - цикл меняет координаты точки до тех пор, пока не произойдёт ТОЛЬКО одно из 2х событий (либо a, либо b)
			// || (crossroadIndex1 == crossroadIndex2) - условие нужно для того, чтобы избежать ситуации взятия двух точек одного перекрёстка
			while (!((a + b) % 2) || (crossroadIndex1 == crossroadIndex2)) {
				crossroadIndex2 = rndGen->Next(0, VERTEX_QUANTITY);

				// добавить внутрь вайла проверку на самую маленькую разность совпадающих координат перекрёстков путём генерации массива ВСЕХ перекрёстков одной координаты

				// цикл определяет, есть ли путь до 2-го перекрёстка
				for (int i = 0; i < 4; i++) {
					a = (Vertices[crossroadIndex1][verticeIndex1]->X == Vertices[crossroadIndex2][i]->X);
					b = (Vertices[crossroadIndex1][verticeIndex1]->Y == Vertices[crossroadIndex2][i]->Y);
					if ((a + b) % 2) { break; }
				}
			}

			if (a && !b) { // пассажир появится на вертикальной линии
				if (crossroadIndex2 > crossroadIndex1) { verticeIndex1 = 2 * rndGen->Next(0, 2); } // вниз
				else if (crossroadIndex2 < crossroadIndex1) { verticeIndex1 = 2 * rndGen->Next(0, 2) + 1; } // вверх
				verticeIndex2 = verticeIndex1 % 2;
				Point^ firstPoint = Vertices[crossroadIndex1][verticeIndex1];
				Point^ secondPoint = Vertices[crossroadIndex2][verticeIndex2];

				array<array<int>^>^ ignoredIntervals = gcnew array<array<int>^>(VERTEX_QUANTITY);
				for (int i = 0; i < VERTEX_QUANTITY; i++) {
					ignoredIntervals[i] = gcnew array<int>(2);
				}
				
				for (int i = 0; i < VERTEX_QUANTITY; i++) {
					for (int j = 0; j < 2; j++) {
						ignoredIntervals[i][j] = 0;
					}
				}

				int countIgnored = 0;
				label9->Text = "ignoredIntervals ";
				for (int i = 0; i < VERTEX_QUANTITY; i++) {
					Point^ thirdPoint = Vertices[i][0];
					bool isExist = false;

					if ((thirdPoint->Y >= Math::Min(Vertices[crossroadIndex1][0]->Y, Vertices[crossroadIndex2][0]->Y))
						&& (thirdPoint->Y <= Math::Max(Vertices[crossroadIndex1][2]->Y, Vertices[crossroadIndex2][2]->Y))) {
						for (int j = 0; j < countIgnored; j++) {
							if ((thirdPoint->Y - 10 == ignoredIntervals[j][0]) && (thirdPoint->Y + 40 == ignoredIntervals[j][1])) {
								isExist = true;
								break;
							}
						}

						if (!isExist) {
							ignoredIntervals[countIgnored][0] = thirdPoint->Y - 10;
							ignoredIntervals[countIgnored][1] = thirdPoint->Y + 40;

							countIgnored++;
						}
					}
				}
					
				for (int i = 1; i < countIgnored; ++i) {
					array<int>^ key = gcnew array<int>(2);
					for (int j = 0; j < 2; j++) {
						key[j] = ignoredIntervals[i][j];
					}

					int t = i - 1;
					while (t >= 0 && ignoredIntervals[t][0] > key[0]) {
						ignoredIntervals[t + 1] = ignoredIntervals[t];
						t = t - 1;
					}

					for (int j = 0; j < 2; j++)
					ignoredIntervals[t + 1] = key;
				}

				for (int i = 0; i < countIgnored; i++) {
					label9->Text += Convert::ToString(String::Format("[{0}, {1}]", ignoredIntervals[countIgnored][0], ignoredIntervals[countIgnored][1]));
				}

				array<int>^ localRndY = gcnew array<int>(countIgnored - 1);
				label8->Text = "localRnd ";
				for (int i = 0; i < countIgnored - 1; i++) {
				//	localRndY[i] = rndGen->Next(Math::Min(ignoredIntervals[i][1], ignoredIntervals[i + 1][0]), Math::Max(ignoredIntervals[i][1], ignoredIntervals[i + 1][0]));
					localRndY[i] = ignoredIntervals[i][1];
					label8->Text += Convert::ToString(localRndY[i] + " ");
				}

				Passengers[Passengers->Count - 1]->xPos::set(firstPoint->X - (Math::Pow(-1, (verticeIndex1 % 2)) * (PASSENGER_OFFSET + (PASSENGER_HEIGHT / 2))));
				Passengers[Passengers->Count - 1]->yPos::set(localRndY[rndGen->Next(0, countIgnored - 2)]);
				//Passengers[Passengers->Count - 1]->yPos::set(rndGen->Next((Math::Min(firstPoint->Y, secondPoint->Y) + 50), (Math::Max(firstPoint->Y, secondPoint->Y) - 50)));
			}
			else if (b && !a) { // // пассажир появится на горизонтальной линии
				if (crossroadIndex2 > crossroadIndex1) { verticeIndex1 = rndGen->Next(2, 4); } // направо
				else if (crossroadIndex2 < crossroadIndex1) { verticeIndex1 = rndGen->Next(0, 2); } // налево
				verticeIndex2 = Math::Floor(verticeIndex1 / 2) * 2;
				Point^ firstPoint = Vertices[crossroadIndex1][verticeIndex1];
				Point^ secondPoint = Vertices[crossroadIndex2][verticeIndex2];

				array<array<int>^>^ ignoredIntervals = gcnew array<array<int>^>(VERTEX_QUANTITY);
				for (int i = 0; i < VERTEX_QUANTITY; i++) {
					ignoredIntervals[i] = gcnew array<int>(2);
				}

				for (int i = 0; i < VERTEX_QUANTITY; i++) {
					for (int j = 0; j < 2; j++) {
						ignoredIntervals[i][j] = 0;
					}
				}

				int countIgnored = 0;
				label9->Text = "ignoredIntervals ";
				for (int i = 0; i < VERTEX_QUANTITY; i++) {
					Point^ thirdPoint = Vertices[i][0];
					bool isExist = false;

					if ((thirdPoint->X >= Math::Min(Vertices[crossroadIndex1][0]->X, Vertices[crossroadIndex2][0]->X))
						&& (thirdPoint->X <= Math::Max(Vertices[crossroadIndex1][2]->X, Vertices[crossroadIndex2][2]->X))) {
						for (int j = 0; j < countIgnored; j++) {
							if ((thirdPoint->X - 10 == ignoredIntervals[j][0]) && (thirdPoint->X + 40 == ignoredIntervals[j][1])) {
								isExist = true;
								break;
							}
						}
						
						if (!isExist) {
							ignoredIntervals[countIgnored][0] = thirdPoint->X - 10;
							ignoredIntervals[countIgnored][1] = thirdPoint->X + 40;

							label9->Text += Convert::ToString(String::Format("[{0}, {1}]", ignoredIntervals[countIgnored][0], ignoredIntervals[countIgnored][1]));

							countIgnored++;
						}
					}
				}
				

				array<int>^ localRndY = gcnew array<int>(countIgnored - 1);
				label8->Text = "localRnd ";
				for (int i = 0; i < countIgnored - 1; i++) {
					//localRndY[i] = rndGen->Next(ignoredIntervals[i][1], ignoredIntervals[i + 1][0]);
					localRndY[i] = ignoredIntervals[i][1];
					label8->Text += Convert::ToString(localRndY[i] + " ");
				}

				Passengers[Passengers->Count - 1]->xPos::set(localRndY[rndGen->Next(0, countIgnored - 2)]);
				Passengers[Passengers->Count - 1]->yPos::set(firstPoint->Y - (Math::Pow(-1, Math::Floor(verticeIndex1 / 2)) * PASSENGER_OFFSET));

				
			}

			label3->Text = Convert::ToString(String::Format("{0}, {1}", crossroadIndex1, verticeIndex1));
			label4->Text = Convert::ToString(String::Format("{0}, {1}", crossroadIndex2, verticeIndex2));

			label5->Text = Convert::ToString(String::Format("{0} {1}", Passengers[Passengers->Count - 1]->xPos::get(), Passengers[Passengers->Count - 1]->yPos::get()));

			label6->Text = Convert::ToString(String::Format("{0} {1}", Vertices[crossroadIndex1][verticeIndex1]->X, Vertices[crossroadIndex1][verticeIndex1]->Y));
			label7->Text = Convert::ToString(String::Format("{0} {1}", Vertices[crossroadIndex2][verticeIndex2]->X, Vertices[crossroadIndex2][verticeIndex2]->Y));

			
		}
	}

	void TimerTickActions(Label^ label3, Label^ label4, Label^ label5, Label^ label6, Label^ label7, Label^ label8, Label^ label9) {
		for (int i = 0; i < TaxiCars->Count; i++) TaxiCars[i]->Move(Vertices);
		PassengerSpawn(label3, label4, label5, label6, label7, label8, label9);
	}
	
};


