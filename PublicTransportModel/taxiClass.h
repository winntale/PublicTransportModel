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

	property array<Point^>^ way {
		array<Point^>^ get() { return _way; }
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
	void WayFind(Passenger^ passenger, array<array<Point^>^>^ Vertices, Label^ label) {
		Point^ pasPoint = Point(passenger->xPos::get(), passenger->yPos::get());

		int wayLength = 1;

		// вносим в массив достигаемых точек начальный перекрёсток пути, соответствующий 3ей точке при генерации, умноженный на 10 (вытащить можно с помощью деления нацело на 10),
		// а также вершину 3ей точки (вытащить можно с помощью деления по модулю 10 (% 10)).
		// то есть точка представляет из себя число (1-3значное), например 0ой перекрёсток 3ья вершина - 3, 1ый перекрёсток 2ая вершина - 12, 12ый перекрёсток 1ая вершина - 121.
		List<int>^ reachable = gcnew List<int>(0);
		reachable->Add((_npCrossroadIndex2 * 10) + _npVerticeIndex2);
		way[0] = _nextPoint2;

		bool a = false;
		bool b = false;

		// нужен метод (событие) подбирания пассажира (совпала одна из координат (вторая координата варьируется по области в зависимости от направления движения)
		// + едет по той же линии независимо от метода поиска пути)
		
		// если индекс начального перекрёстка меньше, чем индекс конечного
		if (_npCrossroadIndex2 < passenger->endCrossroadIndex::get()) {
			while (reachable->Count) {
				// переназначение текущей точки
				Point^ currentPoint = Vertices[reachable[0] / 10][reachable[0] % 10];
				int currentCrossroad = reachable[0] / 10;
				int currentVertice = reachable[0] % 10;

				// verticeIndex = reachable[0] % 10; crossroadIndex = reachable[0] / 10
				// verticeIndex = 0 тогда a: i > crossroadIndex, b: i < crossroadIndex
				// verticeIndex = 1 тогда a или b: i < crossroadIndex
				// verticeIndex = 2 тогда a или b: i > crossroadIndex
				// verticeIndex = 3 тогда a: i < crossroadIndex, b: i > crosroadIndex

				

				for (int i = Math::Max((VERTEX_QUANTITY - 1) * Math::Pow(-1, (reachable[0] % 10) % 2), ((reachable[0] / 10) - 1) * Math::Pow(-1, ((reachable[0] % 10) % 2) + 1)); i > -Math::Min((reachable[0] / 10) * Math::Pow(-1, ((reachable[0] % 10) % 2) + 1), 1 * Math::Pow(-1, (reachable[0] % 10) % 2)); i--) {
					// генерация следующего индекса массива
				}

				int iCur = 0;
				int iLimit = 0;
				if (currentVertice == 1) { iCur = currentCrossroad - 1; iLimit = -1; } // перекрёстки индексом меньше, чем текущий
				else if (currentVertice == 2) { iCur = (VERTEX_QUANTITY - 1); iLimit = currentCrossroad; } //..больше, чем текущий
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

				// ГЕНЕРАЦИЯ МАССИВА ДОСТИГАЕМЫХ ТОЧЕК ОТ ТЕКУЩЕЙ (vertice)
				// в цикле идём по всем перекрёсткам и их вершинам от большего индекса к меньшему
				for (iCur; iCur > iLimit; iCur--) {
					for (int j = 3; j >= 0; j--) {
						Point^ nextPoint = Vertices[iCur][j];
						// условия совпадения одной из координат
						a = currentPoint->X == nextPoint->X;
						b = currentPoint->Y == nextPoint->Y;

						//if ((a && !b) && way


						List<int>^ reachableCrossroads = gcnew List<int>(reachable->Count);
						for each (int crossroad in reachable) { reachableCrossroads->Add(crossroad / 10); }

						// если совпала только одна координата И в массиве достигаемых точек не содержится перекрёсток
						if (((a + b) % 2) && (reachableCrossroads->IndexOf(iCur) == -1)) {
							// проверяем для каждого перекрёстка в массиве достигаемых перекрёстков
							List<int>^ reachableCopy = reachable;
							for each (int crossroad in reachable->ToArray()) {
								Point^ availablePoint = Vertices[crossroad / 10][crossroad % 10];
								// если найденная точка расположена на одной линии с одной из уже существующих точек в достигаемых и имеет меньшее расстояние с текущей точкой
								if (((nextPoint->X == availablePoint->X) && (Math::Abs(nextPoint->Y - currentPoint->Y) < Math::Abs(availablePoint->Y - currentPoint->Y) - 20))
									|| ((nextPoint->Y == availablePoint->Y) && (Math::Abs(nextPoint->X - currentPoint->X) < Math::Abs(availablePoint->X - currentPoint->X) - 20)))
								{ reachableCopy[reachableCopy->IndexOf(crossroad)] = (iCur * 10) + j; } // меняем точку на найденную
							}
							reachable = reachableCopy;

							// если цикл по массиву, описанный выше, не заменил уже существующую точку на только что найденную - добавляем как новую
							// выходя из вложенного цикла (по j)
							if (reachable->IndexOf((iCur * 10) + j) == -1) { reachable->Add((iCur * 10) + j); break; }
						}
					}
				}
				reachable->Remove(reachable[0]);
				// после вышеописанных действий мы получили всех ближайших соседей-точек точки vertice (той, в которой мы находимся сейчас)

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
		 // практически аналог move
	}

};