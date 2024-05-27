#pragma once


using namespace System;
using namespace System::Threading;
using namespace System::Drawing;
using namespace System::Windows::Forms;
using namespace System::Collections::Generic;

#include "Defines.h"
#include "passengerClass.h"

#include <time.h>

// класс, используемый для поиска пути
public ref class Node {
public:
	int vertice;
	Node^ previous;
	String^ direction; // направление, куда двигались, чтобы попасть из previous в текущую ноду

	Node(int _vertice) {
		vertice = _vertice;
		previous = nullptr;
		direction = "";
	}

	Node(int _vertice, String^ _direction) {
		vertice = _vertice;
		previous = nullptr;
		direction = _direction;
	}

	~Node() {}
};

public ref class TaxiCar {
protected:
	int _xPos;
	int _yPos;

	int _maxVelocity;
	System::String^ _direction;
	System::String^ _color;
	int _state; // 0 - ожидание; 1 - в пути к пассажиру; 2 - перевозка пассажира; 3, 4  - ожидание пассажира на вход и выход из машины

	// поля для 2-ой точки
	Point^ _nextPoint;
	int _npCrossroadIndex;
	int _npVerticeIndex;

	// поля для 3-ей точки
	Point^ _nextPoint2;
	int _npCrossroadIndex2;
	int _npVerticeIndex2;

	List<Node^>^ _way;
	Passenger^ _currentClient;

	float _tripDuration; // в секундах

public:
	delegate void HandlerTakeOnPassenger(Passenger^ passenger, array<array<Point^>^>^ Vertices);
	delegate void HandlerDropOffPassenger(Passenger^ passenger);

	event HandlerTakeOnPassenger^ EventTakeOn;
	event HandlerDropOffPassenger^ EventDropOff;

	TaxiCar() {
		_xPos = 0;
		_yPos = 0;

		_state = 0; // 0 - хаотичное движение по дорогам; 1 - в пути до пассажира; 2 - перевозка пассажира;

		_way = gcnew List<Node^>(0);
		_tripDuration = 0;

		EventTakeOn += gcnew TaxiCar::HandlerTakeOnPassenger(this, &TaxiCar::onTakeOn);
		EventDropOff += gcnew TaxiCar::HandlerDropOffPassenger(this, &TaxiCar::onDropOff);
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
	
	String^ XPointReached(array<array<Point^>^>^ Vertices, bool isTurned, String^ _direction2) {
		Random^ rndGen = gcnew Random();

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

		return _direction2;
	}

	String^ YPointReached(array<array<Point^>^>^ Vertices, bool isTurned, String^ _direction2) {
		Random^ rndGen = gcnew Random();

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

		return _direction2;
	}
	
	// метод описания движения и поворота в 0 состоянии машины
	void Move(array<array<Point^>^>^ Vertices) {
		// в зависимости от направления (direction) меняются координаты x, y
		if (_direction == "left") { _xPos -= SPEED; }
		else if (_direction == "right") { _xPos += SPEED; }

		else if (_direction == "down") { _yPos += SPEED; }
		else if (_direction == "up") { _yPos -= SPEED; }

		bool isTurned = false;
		String^ _direction2 = _direction;


		// рассмотрим 2 ситуации: 1) машина движется по Y	2) машина движется по X
		// машина достигла Y (двигалась по вертикали)
		if (_direction == "up" || _direction == "down") {
			if ((Math::Abs(_yPos - (_nextPoint->Y - (TAXICAR_IMG_HEIGHT / 2))) < SPEED) && !isTurned) {
				_direction2 = YPointReached(Vertices, isTurned, _direction2);
				isTurned = true;
			}
		}
		// машина достигла X (двигалась по горизонтали)
		else if (_direction == "right" || _direction == "left") {
			if ((Math::Abs(_xPos - (_nextPoint->X - (TAXICAR_IMG_HEIGHT / 2))) < SPEED) && !isTurned) {
				_direction2 = XPointReached(Vertices, isTurned, _direction2);
				isTurned = true;
			}
		}

		if (isTurned) {
			_direction = _direction2;
			isTurned = false;
		}
	}

	// метод проверяет содержится ли в массиве нод (Nodes) индексы точки newNode
	bool IsContainsVertice(List<Node^>^ Nodes, Node^ newNode) {
		bool flag = false;
		for each (Node ^ node in Nodes)
			if (node->vertice == newNode->vertice) {
				flag = true;
				break;
			}
		return flag;
	}

	// метод возвращает первый индекс элемента newNode в массиве Nodes по соответствиям индексов точек (-1 если их нет)
	int IndexOfVertice(List<Node^>^ Nodes, Node^ newNode) {
		int index = -1;
		for each (Node ^ node in Nodes)
			if (node->vertice == newNode->vertice) {
				index = Nodes->IndexOf(node);
				break;
			}
		return index;
	}

	// метод проверяет, есть ли данный перекрёсток в массиве newReachable
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

	// метод строит путь от начальной точки до начала линии пассажира passNode путём обращения к ссылкам previous
	List<Node^>^ BuildPath(Node^ passNode, Node^ passEndNode) {
		List<Node^>^ path = gcnew List<Node^>(0);
		path->Add(passEndNode);
		while (passNode != nullptr) {
			path->Add(passNode);
			passNode = passNode->previous;
		}
		return path;
	}

	// метод поиска пути (заполнения ссылок previous для reachable точек)
	void WayFind(Passenger^ passenger, array<array<Point^>^>^ Vertices) {
		Random^ rndGen = gcnew Random();

		// создаём массив нод, которые сейчас можно рассмотреть и массив точек, в которых мы уже были
		List<Node^>^ reachable = gcnew List<Node^>(0);
		List<int>^ explored = gcnew List<int>(0);

		// за начальную точку возьмём индексы точки, в которую на данный момент едет машина
		int startIndexes = _npCrossroadIndex * 10 + _npVerticeIndex;
		Node^ startNode = gcnew Node(startIndexes);
		startNode->direction = _direction;

		// добавляем в массив допустимых нод стартовую ноду
		reachable->Add(startNode);

		// то же самое с точкой, которая представляет из себя начальную точку линии, где находится пассажир
		Node^ passStartNode = passenger->startNode::get();
		Node^ passEndNode = passenger->endNode::get();

		int passStartIndexes = passStartNode->vertice;
		int passEndIndexes = passEndNode->vertice;

		bool a = false;
		bool b = false;

		// цикл будет работать, пока есть точки, которые можно рассмотреть
		while (reachable->Count > 0) {
			// задаём текущую точку (берём её случайно из массива доступных точек)
			Node^ currentNode = reachable[rndGen->Next(0, reachable->Count)];
			int currentNodeCrossroad = currentNode->vertice / 10;
			int currentNodeVertice = currentNode->vertice % 10;

			// условие выхода осуществляется, если мы взяли точку, которая соответствует началу линии, где находится пассажир (в зависимости от направления вектора пассажира)
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
				break;
			}

			// так как мы взяли текущую точку, то удаляем её из массива доступных и заносим в массив пройденных
			reachable->Remove(currentNode);
			explored->Add(currentNode->vertice);

			// создаём массив точек, которых можно достичь ОТ текущей точки
			List<Node^>^ newReachable = gcnew List<Node^>(0);

			// проходим по всей системе дорог
			for (int i = 0; i < VERTEX_QUANTITY; i++) {
				for (int j = 0; j < 4; j++) {
					// задаём индексы точки, которую сейчас рассматриваем (потенциально следующая точка)
					int nextIndexes = i * 10 + j;
					int nextNodeCrossroad = nextIndexes / 10;
					int nextNodeVertice = nextIndexes % 10;


					a = Vertices[currentNodeCrossroad][currentNodeVertice]->X == Vertices[nextNodeCrossroad][nextNodeVertice]->X; // совпал X (вертикальное движение)
					b = Vertices[currentNodeCrossroad][currentNodeVertice]->Y == Vertices[nextNodeCrossroad][nextNodeVertice]->Y; // совпал Y (горизонтальное движение)

					Node^ nextNode = gcnew Node(nextIndexes);
					// если совпала лишь одна координата и мы пока не прошли потенциально следующую точку
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

			// рассматриваем каждый элемент массива точек, в которых можно попасть от текущей (current)
			for (int i = 0; i < newReachable->Count; i++) {
				for each (Node ^ availableNode in newReachable->ToArray()) {
					// availablePoint - точка, в которую можно поппасть от current (то есть путь current -> available уже существует)
					Point^ availablePoint = Vertices[availableNode->vertice / 10][availableNode->vertice % 10];
					Point^ currentPoint = Vertices[currentNodeCrossroad][currentNodeVertice];
					Point^ comparablePoint = Vertices[newReachable[i]->vertice / 10][newReachable[i]->vertice % 10];

					// если потенциально следующая точка (nextPoint) и точка, в которую можно попасть от current (available) находятся на одной линии
					// и расстояние от current до next меньше, чем расстояние от current до available
					// и перекрёсток nextPoint не равен перекрёсткам availablePoint и currentPoint
					bool sameX = (comparablePoint->X == availablePoint->X); bool sameY = (comparablePoint->Y == availablePoint->Y);
					bool IsnpYCloser = (Math::Abs(comparablePoint->Y - currentPoint->Y) < Math::Abs(availablePoint->Y - currentPoint->Y) - 60);
					bool IsnpXCloser = (Math::Abs(comparablePoint->X - currentPoint->X) < Math::Abs(availablePoint->X - currentPoint->X) - 60);
					if (((sameX && IsnpYCloser) || (sameY && IsnpXCloser)) && (newReachable[i]->vertice / 10 != availableNode->vertice / 10) && (newReachable[i]->vertice / 10 != currentNodeCrossroad)
						&& (newReachable[i]->vertice % 10 == availableNode->vertice % 10))
					{
						newReachable[newReachable->IndexOf(availableNode)] = newReachable[i];
					} // то меняем в массиве доступных точек значение рассматриваемого индекса на nextNode
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

	// метод для события перехода такси в состояние поездки (забрало пассажира)
	void onTakeOn(Passenger^ passenger, array<array<Point^>^>^ Vertices) {
		_state = 3; // через секунду перейдёт в 2
		passenger->state::set(2);

		Random^ rndGen = gcnew Random();
		int verticeIndex1 = passenger->startNode->vertice % 10;

		int crossroadIndex2 = _way[0]->vertice / 10;
		int verticeIndex2 = _way[0]->vertice % 10;

		int crossroadIndex3 = rndGen->Next(0, VERTEX_QUANTITY);
		int verticeIndex3 = rndGen->Next(0, 4);

		bool a1 = (Vertices[crossroadIndex2][verticeIndex2]->X == Vertices[crossroadIndex3][verticeIndex3]->X); // совпал X у 2-ой и 3-й точек
		bool b1 = (Vertices[crossroadIndex2][verticeIndex2]->Y == Vertices[crossroadIndex3][verticeIndex3]->Y); // совпал Y у 2-ой и 3-й точек

		// генерация следующей 3ей точки
		if (_way[0]->direction == "up" || _way[0]->direction == "down") { // машина изначально поедет по вертикали (корректировка 1-ой точки)
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
		else if (_way[0]->direction == "left" || _way[0]->direction == "right") { // машина изначально поедет по горизонтали
			while (!a1 || b1 || (crossroadIndex3 == crossroadIndex2)) { // пока не найден путь по вертикали
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

		//заполнение полей объекта
		_nextPoint = Vertices[crossroadIndex2][verticeIndex2];
		_npCrossroadIndex = crossroadIndex2;
		_npVerticeIndex = verticeIndex2;
		_nextPoint2 = Vertices[crossroadIndex3][verticeIndex3];
		_npCrossroadIndex2 = crossroadIndex3;
		_npVerticeIndex2 = verticeIndex3;
	}

	// триггер события
	void TakeOn(Passenger^ passenger, array<array<Point^>^>^ Vertices) {
		EventTakeOn(passenger, Vertices);
	}

	// транспорт приехал за пассажиром(-ами)
	void IfTransportIsHere(Passenger^ passenger, array<array<Point^>^>^ Vertices) {
		bool isTaxiHere = false;

		if (passenger->endNode->direction == "up") { isTaxiHere = (((passenger->xPos::get() - _xPos) < 60) && (Math::Abs(_yPos - (passenger->yPos::get() - (TAXICAR_IMG_HEIGHT / 2))) < SPEED)); }
		else if (passenger->endNode->direction == "down") { isTaxiHere = (((_xPos - passenger->xPos::get()) < 60) && (Math::Abs(_yPos - (passenger->yPos::get() - (TAXICAR_IMG_HEIGHT / 2))) < SPEED)); }
		else if (passenger->endNode->direction == "right") { isTaxiHere = ((Math::Abs(_xPos - (passenger->xPos::get() - (TAXICAR_IMG_HEIGHT / 2))) < SPEED) && ((passenger->yPos::get() - _yPos) < 120)); }
		else if (passenger->endNode->direction == "left") { isTaxiHere = ((Math::Abs(_xPos - (passenger->xPos::get() - (TAXICAR_IMG_HEIGHT / 2))) < SPEED) && ((_yPos - passenger->yPos::get()) < 120)); }

		if (isTaxiHere) {
			TakeOn(passenger, Vertices);
		}
	}

	// движение в 1-ом стейте
	void MoveToPassenger(array<array<Point^>^>^ Vertices, Passenger^ passenger) {
		_direction = _way[0]->direction;

		if (_direction == "left") { _xPos -= SPEED; }
		else if (_direction == "right") { _xPos += SPEED; }

		else if (_direction == "down") { _yPos += SPEED; }
		else if (_direction == "up") { _yPos -= SPEED; }

		Point^ nextWayPoint = Vertices[_way[0]->vertice / 10][_way[0]->vertice % 10];

		bool isTurned = false;


		if (_direction == "up" || _direction == "down") {
			// машина достигла Y (двигалась по вертикали)
			if ((Math::Abs(_yPos - (nextWayPoint->Y - (TAXICAR_IMG_HEIGHT / 2))) < SPEED) && !isTurned) {
				_yPos = nextWayPoint->Y - (TAXICAR_IMG_HEIGHT / 2); // корректировка Y
				_way->Remove(_way[0]);

				isTurned = true;
			}
		}
		else if (_direction == "right" || _direction == "left") {
			// машина достигла X (двигалась по горизонтали)
			if ((Math::Abs(_xPos - (nextWayPoint->X - (TAXICAR_IMG_HEIGHT / 2))) < SPEED) && !isTurned) {
				_xPos = nextWayPoint->X - (TAXICAR_IMG_HEIGHT / 2); // корректировка X
				_way->Remove(_way[0]);

				isTurned = true;
			}
		}

		if (_way->Count == 1 || ((_npCrossroadIndex * 10 + _npVerticeIndex) == passenger->endNode->vertice)) {
			IfTransportIsHere(passenger, Vertices);
		}
	}

	// событие такси доехало
	void onDropOff(Passenger^ passenger) {
		if (_tripDuration > (2000.0 / _maxVelocity)) {
			_state = 4; // в envClass через секунду перейдёт в 0
			passenger->state::set(3);
			passenger->serviceCarDirection::set(_direction);
			passenger->serviceCarX::set(_xPos);
			passenger->serviceCarY::set(_yPos);

			_tripDuration = 0;
		}
	}

	// триггер события такси доехало
	void DropOff(Passenger^ passenger) {
		EventDropOff(passenger);
	}
};








public ref class Bus : public TaxiCar {
private:
	array<bool>^ wasIn;
	array<Node^>^ nodeContainer;
	int _stopAt;

	List<Passenger^>^ _currentClient;

public:
	delegate void HandlerTakeOnPassenger(Passenger^ passenger);
	delegate void HandlerDropOffPassenger(Passenger^ passenger);

	event HandlerTakeOnPassenger^ EventTakeOn;
	event HandlerDropOffPassenger^ EventDropOff;

	delegate void HandlerStop(int _stopAt);
	static event HandlerStop^ EventStop;

	Bus() {
		_xPos = 151 - (BUS_WIDTH / 2);
		_yPos = 151;
		_direction = "down";

		_state = 0;

		_maxVelocity = BUS_SPEED;

		_currentClient = gcnew List<Passenger^>(0);
		wasIn = gcnew array<bool>(4) { false, false, false, false };
		nodeContainer = gcnew array<Node^>(6) { gcnew Node(22, "down"), gcnew Node(123, "right"), gcnew Node(101, "up"), gcnew Node(81, "left"), gcnew Node(71, "up"), gcnew Node(0, "left") };

		EventTakeOn += gcnew Bus::HandlerTakeOnPassenger(this, &Bus::onTakeOn);
		EventDropOff += gcnew Bus::HandlerDropOffPassenger(this, &Bus::onDropOff);
	}

	property int stopAt {
		int get() { return _stopAt; }
	}

	property List<Passenger^>^ currentClient {
		List<Passenger^>^ get() { return _currentClient; }
		void set(List<Passenger^>^ _value) { _currentClient = _value; }
	}

	void WayGenerator() {
		for each (Node ^ node in nodeContainer) { _way->Add(node); }
	}

	void IfTransportIsHere(array<Point^>^ busStops) {
		bool isBusHere = false;

		if (!wasIn[0] && (Math::Abs(_xPos - busStops[0]->X) < SPEED) && ((_yPos - busStops[0]->Y) < 50)) { isBusHere = true; wasIn[0] = true; _stopAt = 0; }
		if (!wasIn[1] && (Math::Abs((_xPos - BUS_WIDTH / 2) - busStops[1]->X) < SPEED) && ((busStops[0]->Y - _yPos) < 50)) { isBusHere = true; _stopAt = 1; wasIn[1] = true; wasIn[0] = false; }
		if (!wasIn[2] && (Math::Abs(_yPos - busStops[2]->Y) < SPEED) && ((busStops[0]->X - _xPos) < 80)) { isBusHere = true; wasIn[2] = true; _stopAt = 2; }
		if (!wasIn[3] && (Math::Abs(_yPos - busStops[3]->Y) < SPEED) && ((busStops[0]->X - _xPos) < 80)) { isBusHere = true; wasIn[3] = true; _stopAt = 3; }

		if (isBusHere) { 
			_state = 3; 
			EventStop(_stopAt); //вызов события
		}
		if (wasIn[0]) { for (int i = 1; i < 4; i++) { wasIn[i] = false; } }
	}

	// в  классе автобуса есть событие EventStop (класс автобус - это издатель события EventStop)
	// в классе пассажира осуществляется подписка на EventStop
	// как происходит реагирование пассажира:
	// 

	void Move(array<array<Point^>^>^ Vertices, array<Point^>^ busStops) {
		if (_direction == "left") { _xPos -= SPEED; }
		else if (_direction == "right") { _xPos += SPEED; }

		else if (_direction == "down") { _yPos += SPEED; }
		else if (_direction == "up") { _yPos -= SPEED; }

		if (_direction == "left" || _direction == "right") {
			if (Math::Abs(_xPos - (Vertices[_way[0]->vertice / 10][_way[0]->vertice % 10]->X - (BUS_HEIGHT / 2))) < SPEED) {
				_xPos = Vertices[_way[0]->vertice / 10][_way[0]->vertice % 10]->X - (BUS_WIDTH / 2);
				if (_direction == "right" || ((_direction == "left") && (_way[1]->direction == "up"))) { _yPos -= 32; }

				Node^ passedNode = _way[0];
				_way->Remove(_way[0]);
				_way->Add(passedNode);
			}
		}
		else if (_direction == "up" || _direction == "down") {
			if (Math::Abs(_yPos - (Vertices[_way[0]->vertice / 10][_way[0]->vertice % 10]->Y - (BUS_HEIGHT / 2))) < SPEED) {
				_yPos = Vertices[_way[0]->vertice / 10][_way[0]->vertice % 10]->Y - (BUS_WIDTH / 2);
				if (_direction == "up") { _xPos -= 32; }

				Node^ passedNode = _way[0];
				_way->Remove(_way[0]);
				_way->Add(passedNode);
			}
		}
		_direction = _way[0]->direction;

		IfTransportIsHere(busStops);
	}

	// метод для события перехода такси в состояние поездки (забрало пассажира)
	void onTakeOn(Passenger^ passenger) {
		passenger->state::set(2);
		passenger->goalbusStopIndex::set((_stopAt + 2) % 4);
		_currentClient->Add(passenger);
	}

	// триггер события
	void TakeOn(Passenger^ passenger) {
		EventTakeOn(passenger);
	}

	void onDropOff(Passenger^ passenger) {
		Random^ rndGen = gcnew Random();
		passenger->state::set(3);
		passenger->serviceCarDirection::set(_direction);
		passenger->serviceCarX::set(_xPos);
		passenger->serviceCarY::set(_yPos);
		currentClient->Remove(passenger);
	}

	void DropOff(Passenger^ passenger) {
		EventDropOff(passenger);
	}
};
