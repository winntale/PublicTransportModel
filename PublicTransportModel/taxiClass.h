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
	int _state; // 0 - ожидание; 1 - выполнение заказа; 2 - низкий уровень топлива

	// поля для 2-ой точки
	Point^ _nextPoint;
	int _npCrossroadIndex;
	int _npVerticeIndex;

	// поля для 3-ей точки
	Point^ _nextPoint2;
	int _npCrossroadIndex2;
	int _npVerticeIndex2;
	List<int>^ _wayX;
	List<int>^ _wayY;

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

	property List<int>^ wayX {
		List<int>^ get() { return _wayX; }
	}

	property List<int>^ wayY {
		List<int>^ get() { return _wayY; }
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
	void WayFind(Passenger^ passenger, array<array<Point^>^>^ Vertices) {
		Point^ pasPoint = Point(passenger->xPos::get(), passenger->yPos::get());

		//Point^ nextVertice = _nextPoint2;
		//_wayX->Add(nextVertice->X);
		//_wayY->Add(nextVertice->Y);
		Point^ wayEndPoint = Point(_wayX[_wayX->Count - 1], _wayY[_wayY->Count - 1]);

		List<int>^ reachableX = {};
		List<int>^ reachableY = {};
		reachableX->Add(_nextPoint2->X);
		reachableY->Add(_nextPoint2->Y);

		List<int>^ exploredX = {};
		List<int>^ exploredY = {};

		List<int>^ reachableCrossroads = {};
		reachableCrossroads->Add(_npCrossroadIndex2);

		array<Point^>^ reachable = gcnew array<Point^>(0);
		int reachableCount = 1;

		bool a = false;
		bool b = false;

		// нужен метод (событие) подбирания пассажира (совпала одна из координат (вторая координата варьируется по области в зависимости от направления движения)
		// + едет по той же линии независимо от метода поиска пути)
		
		if (_npCrossroadIndex2 < passenger->endCrossroadIndex::get()) {
			while (reachableCrossroads[0]) {
				Point^ vertice = Vertices[reachableCrossroads[0]][_npVerticeIndex2];
				for (int i = VERTEX_QUANTITY - 1; i >= 0; i--) {
					for (int j = 3; j >= 0; j--) {
						Point^ tempVertice = Vertices[i][j];
						a = vertice->X == tempVertice->X;
						b = vertice->Y == tempVertice->Y;

						if ((a + b) % 2) {
							for each (int crossroad in reachableCrossroads) {
								if (((Vertices[i][0]->X == Vertices[crossroad][0]->X) && (Vertices[i][0]->Y < Vertices[crossroad][0]->Y))
									|| ((Vertices[i][0]->Y == Vertices[crossroad][0]->Y) && (Vertices[i][0]->X < Vertices[crossroad][0]->X)))
								{ reachableCrossroads[reachableCrossroads->IndexOf(crossroad)] = i; }
							}
							reachableCrossroads->Add(i);
							break;
						}
					}
				}

				for (int i = 0; i < reachableCount - 1; i++) {
					for (int j = i + 1; j < reachableCount; j++) {
						if (((reachable[j]->X < reachable[i]->X) && (reachable[j]->Y == reachable[i]->Y))
							|| ((reachable[j]->Y < reachable[i]->Y) && (reachable[j]->X == reachable[i]->X)))
					}
				}


			}
		}

		
			
			Point^ vertice = Point(reachableX[0], reachableY[0]);
			for (int i = 0; i < VERTEX_QUANTITY; i++) {
				for (int j = 0; j < 4; j++) {
					Point^ tempVertice = Vertices[i][j];
					a = vertice->X == tempVertice->X;
					b = vertice->Y == tempVertice->Y;

					if ((a + b) % 2) {
						reachableX->Add(tempVertice->X);
						reachableY->Add(tempVertice->Y);
					}
				}
			}

			exploredX->Add(vertice->X);
			exploredY->Add(vertice->Y);

			reachableX->Remove(vertice->X);
			reachableY->Remove(vertice->Y);
		}

		while (wayEndPoint != passenger->endPoint::get()) {

			// добавить внутрь вайла проверку на самую маленькую разность совпадающих координат перекрёстков путём генерации массива ВСЕХ перекрёстков одной координаты

			// цикл определяет, есть ли путь до 2-го перекрёстка

		}

		//логика поиска пути
	}

	void MoveToPassenger() {
		 // практически аналог move
	}

};