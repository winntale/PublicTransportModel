#include <cstdlib>
#include "classes.h"

// Определение класса машины такси

TaxiCar::TaxiCar() {
	_xPos = 0;
	_yPos = 0;

	_state = 0; // 0 - хаотичное движение по дорогам; 1 - в пути до пассажира; 2 - перевозка пассажира;

	_way = gcnew List<Node^>(0);
	_tripDuration = 0;
}

String^ TaxiCar::XPointReached(array<array<Point^>^>^ Vertices, bool isTurned, String^ _direction2) {
	Random^ rndGen = gcnew Random();

	_xPos = _nextPoint->X - (TAXICAR_IMG_HEIGHT / 2); // корректировка X

	bool a = (Vertices[_npCrossroadIndex][_npVerticeIndex]->X == Vertices[_npCrossroadIndex2][_npVerticeIndex2]->X); // совпал X у 1-й и 2-й точки
	bool b = (Vertices[_npCrossroadIndex][_npVerticeIndex]->Y == Vertices[_npCrossroadIndex2][_npVerticeIndex2]->Y); // совпал Y у 1-й и 2-й точки

	// (2) -> (1) (то есть (2) становится startPoint'ом)
	Int16 crossroadIndex1 = _npCrossroadIndex;
	Int16 verticeIndex1 = _npVerticeIndex;

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
String^ TaxiCar::YPointReached(array<array<Point^>^>^ Vertices, bool isTurned, String^ _direction2) {
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
void TaxiCar::Move(array<array<Point^>^>^ Vertices) {
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
bool TaxiCar::IsContainsVertice(List<Node^>^ Nodes, Node^ newNode) {
	bool flag = false;
	for each (Node ^ node in Nodes)
		if (node->vertice == newNode->vertice) {
			flag = true;
			break;
		}
	return flag;
}

// метод возвращает первый индекс элемента newNode в массиве Nodes по соответствиям индексов точек (-1 если их нет)
int TaxiCar::IndexOfVertice(List<Node^>^ Nodes, Node^ newNode) {
	int index = -1;
	for each (Node ^ node in Nodes)
		if (node->vertice == newNode->vertice) {
			index = Nodes->IndexOf(node);
			break;
		}
	return index;
}

// метод проверяет, есть ли данный перекрёсток в массиве newReachable
bool TaxiCar::IsEqualsCrossroads(List<Node^>^ newReachable, int vertice) {
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
List<Node^>^ TaxiCar::BuildPath(Node^ passNode, Node^ passEndNode) {
	List<Node^>^ path = gcnew List<Node^>(0);
	path->Add(passEndNode);
	while (passNode != nullptr) {
		path->Add(passNode);
		passNode = passNode->previous;
	}
	return path;
}

// метод поиска пути (заполнения ссылок previous для reachable точек)
void TaxiCar::WayFind(Passenger^ passenger, array<array<Point^>^>^ Vertices) {
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
void TaxiCar::onTakeOn(Passenger^ passenger, array<array<Point^>^>^ Vertices) {
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

// транспорт приехал за пассажиром(-ами)
void TaxiCar::IfTransportIsHere(Passenger^ passenger, array<array<Point^>^>^ Vertices) {
	bool isTaxiHere = false;

	if (passenger->endNode->direction == "up") { isTaxiHere = (((passenger->xPos::get() - _xPos) < 60) && (Math::Abs(_yPos - (passenger->yPos::get() - (TAXICAR_IMG_HEIGHT / 2))) < SPEED)); }
	else if (passenger->endNode->direction == "down") { isTaxiHere = (((_xPos - passenger->xPos::get()) < 60) && (Math::Abs(_yPos - (passenger->yPos::get() - (TAXICAR_IMG_HEIGHT / 2))) < SPEED)); }
	else if (passenger->endNode->direction == "right") { isTaxiHere = ((Math::Abs(_xPos - (passenger->xPos::get() - (TAXICAR_IMG_HEIGHT / 2))) < SPEED) && ((passenger->yPos::get() - _yPos) < 120)); }
	else if (passenger->endNode->direction == "left") { isTaxiHere = ((Math::Abs(_xPos - (passenger->xPos::get() - (TAXICAR_IMG_HEIGHT / 2))) < SPEED) && ((_yPos - passenger->yPos::get()) < 120)); }

	if (isTaxiHere) {
		onTakeOn(passenger, Vertices);
	}
}

// движение в 1-ом стейте
void TaxiCar::MoveToPassenger(array<array<Point^>^>^ Vertices, Passenger^ passenger) {
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
void TaxiCar::onDropOff(Passenger^ passenger) {
	if (_tripDuration > (2000.0 / _maxVelocity)) {
		_state = 4; // в envClass через секунду перейдёт в 0
		passenger->state::set(3);
		passenger->serviceCarDirection::set(_direction);
		passenger->serviceCarX::set(_xPos);
		passenger->serviceCarY::set(_yPos);

		_tripDuration = 0;
	}
}


// Определение Bus

Bus::Bus() {
	_xPos = 151 - (BUS_WIDTH / 2);
	_yPos = 151;
	_direction = "down";

	_state = 0;

	_maxVelocity = BUS_SPEED;

	_currentClient = gcnew List<Passenger^>(0);
	wasIn = gcnew array<bool>(4) { false, false, false, false };
	nodeContainer = gcnew array<Node^>(6) { gcnew Node(22, "down"), gcnew Node(123, "right"), gcnew Node(101, "up"), gcnew Node(81, "left"), gcnew Node(71, "up"), gcnew Node(0, "left") };
	/*EventTakeOn += gcnew Bus::HandlerTakeOnPassenger(this, &Bus::onTakeOn);
	EventDropOff += gcnew Bus::HandlerDropOffPassenger(this, &Bus::onDropOff);*/
}

void Bus::WayGenerator() {
	for each (Node ^ node in nodeContainer) { _way->Add(node); }
}

void Bus::IfTransportIsHere(array<Point^>^ busStops) {
	bool isBusHere = false;

	if (!wasIn[0] && (Math::Abs(_xPos - busStops[0]->X) < SPEED) && ((_yPos - busStops[0]->Y) < 50)) { isBusHere = true; wasIn[0] = true; _stopAt = 0; }
	if (!wasIn[1] && (Math::Abs((_xPos - BUS_WIDTH / 2) - busStops[1]->X) < SPEED) && ((busStops[0]->Y - _yPos) < 50)) { isBusHere = true; _stopAt = 1; wasIn[1] = true; wasIn[0] = false; }
	if (!wasIn[2] && (Math::Abs(_yPos - busStops[2]->Y) < SPEED) && ((busStops[0]->X - _xPos) < 80)) { isBusHere = true; wasIn[2] = true; _stopAt = 2; }
	if (!wasIn[3] && (Math::Abs(_yPos - busStops[3]->Y) < SPEED) && ((busStops[0]->X - _xPos) < 80)) { isBusHere = true; wasIn[3] = true; _stopAt = 3; }

	if (isBusHere) {
		_state = 3;
		EventStop(_stopAt, _xPos, _yPos, _direction); //вызов события
	}
	if (wasIn[0]) { for (int i = 1; i < 4; i++) { wasIn[i] = false; } }
}

// в  классе автобуса есть событие EventStop (класс автобус - это издатель события EventStop)
// в классе пассажира осуществляется подписка на EventStop
// как происходит реагирование пассажира:
// 

void Bus::Move(array<array<Point^>^>^ Vertices, array<Point^>^ busStops) {
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


void Bus::onTakeOn(Passenger^ passenger) {
	passenger->state::set(2);
	passenger->goalBusStopIndex::set((_stopAt + 2) % 4);
	_currentClient->Add(passenger);
}

// триггер события
//void Bus::TakeOn(Passenger^ passenger) {
//	EventTakeOn(passenger);
//}

void Bus::onDropOff(Passenger^ passenger) {
	Random^ rndGen = gcnew Random();
	passenger->state::set(3);
	passenger->serviceCarDirection::set(_direction);
	passenger->serviceCarX::set(_xPos);
	passenger->serviceCarY::set(_yPos);
	currentClient->Remove(passenger);
}

// Определение Passanger

Passenger::Passenger() {
	Random^ rndGen = gcnew Random();
	_color = rndGen->Next(1, 4);

	_xPos = 0;
	_yPos = 0;
	_direction = "";

	_isMovingAway = false;
	_moveActions = 0;

	_state = 0;
	_serviceCarIndex = -1;

	_busStopIndex = -1;

	MyEnvironment::EventPassengerTaxiChoise += gcnew MyEnvironment::HandlerPassengerTaxiChoise(this, &Passenger::onTaxiChoised);
	/*TaxiCar::EventTakeOn += gcnew TaxiCar::HandlerTakeOnPassenger(this, &Passenger::OnEventTakeOn);*/
}
Passenger::Passenger(int _stateVal, int _BusStopIndex) {
	Random^ rndGen = gcnew Random();
	_color = rndGen->Next(1, 4);

	_xPos = 0;
	_yPos = 0;
	_direction = "";

	_isMovingAway = false;
	_moveActions = 0;

	_state = _stateVal;

	_busStopIndex = _BusStopIndex;

	Bus::EventStop += gcnew Bus::HandlerStop(this, &Passenger::OnEventStop);
}


void Passenger::OnEventStop(int _stopAt, int _servXPos, int _servYPos, String^ _servDirection) {
	Random^ rndGen = gcnew Random();
	if ((_state == 4) && (_stopAt == _busStopIndex)) {
		_state = 2;
		_goalBusStopIndex = (_stopAt + 2) % 4;
	}
	else if ((_state == 2) && (_stopAt == _goalBusStopIndex)) {
		_state = 3;
		_serviceCarDirection = _servDirection;
		_serviceCarX = _servXPos;
		_serviceCarY = _servYPos;
	}
	//	_currentClient->Add(passenger);
}

void Passenger::MoveAway() {
	if (!_isMovingAway) {
		if (_serviceCarDirection == "down") { _xPos = _serviceCarX - 10; _yPos = _serviceCarY; _direction = "left"; }
		else if (_serviceCarDirection == "up") { _xPos = _serviceCarX + 10; _yPos = _serviceCarY; _direction = "right"; }
		else if (_serviceCarDirection == "right") { _xPos = _serviceCarX; _yPos = _serviceCarY + 10; _direction = "down"; }
		else if (_serviceCarDirection == "left") { _xPos = _serviceCarX; _yPos = _serviceCarY - 10; _direction = "up"; }

		_isMovingAway = true;
	}
	else {
		if (_direction == "left") { _xPos -= PASSENGER_SPEED; }
		else if (_direction == "right") { _xPos += PASSENGER_SPEED; }
		else if (_direction == "down") { _yPos += PASSENGER_SPEED; }
		else if (_direction == "up") { _yPos -= PASSENGER_SPEED; }

		_moveActions++;
	}
}

void Passenger::onTaxiChoised(int serviceCarIndex) {
	if (_state == 0 && (_serviceCarIndex == -1)) {
		_state = 1;
		_serviceCarIndex = serviceCarIndex;
	}
}


// Определение MyEnvironment

MyEnvironment::MyEnvironment() {
	_Bus = gcnew Bus();
	_TaxiCars = gcnew List<TaxiCar^>(0);
	_Passengers = gcnew List<Passenger^>(0);
	_BusPassengers = gcnew List<List<Passenger^>^>(4);
	for (int i = 0; i < 4; i++) { _BusPassengers->Add(gcnew List<Passenger^>()); }
	cordinates = gcnew array<Point^>(VERTEX_QUANTITY) {
		Point(151, 75), Point(151, 326), Point(151, 828),
			Point(402, 75), Point(402, 326), Point(402, 577), Point(402, 828), Point(653, 75), Point(653, 326), Point(653, 577), Point(904, 326), Point(904, 577), Point(904, 828)
	};
	Vertices = gcnew array<array<Point^>^>(VERTEX_QUANTITY);

	busStops = gcnew array<Point^>(BUSSTOPS_COUNT) { Point(227, 35), Point(541, 867), Point(692, 159), Point(945, 399) }; // первые 2 расположены на горизонтали, последние 2 - на вертикали
}

// метод генерации массива вершин по массиву координат
void MyEnvironment::VerticesGen() {
	for (int i = 0; i < VERTEX_QUANTITY; i++) {
		Vertices[i] = gcnew array<Point^>(4);
	}

	for (int i = 0; i < VERTEX_QUANTITY; i++) {
		for (int j = 0; j < 4; j++) {
			Vertices[i][j] = gcnew Point(cordinates[i]->X + ((j % 2) * 24), cordinates[i]->Y + ((j > 1) * 24));
		}
	}
}

// ОПРЕДЕЛЕНИЕ НАПРАВЛЕНИЯ
void MyEnvironment::DirectionSet(Point^ spawnPoint, Point^ nextPoint, int crossroadIndex1, int crossroadIndex2) {
	Random^ rndGen = gcnew Random();

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

// метод генерации объекта машины такси (завершён)
void MyEnvironment::TaxiSpawn() {
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

		DirectionSet(spawnPoint, nextPoint, crossroadIndex1, crossroadIndex2);

	}
	// в результате приведённый код создаёт объект машины такси и генерирует ему значения следующих полей
	// 1) координаты (x; y)
	// 2) направление движения (direction)
}

void MyEnvironment::PassengerSpawn() {
	Random^ rndGen = gcnew Random();
	int randomNumber = rndGen->Next(0, 1001);

	int numOfPassengers = 0;
	for each (List<Passenger^> ^ passengerList in BusPassengers) {
		numOfPassengers += passengerList->Count;
	}

	// создание пассажира такси
	if (TaxiCars->Count && (Passengers->Count < TaxiCars->Count) && (randomNumber > 950)) {
		Passengers->Add(gcnew Passenger()); // создание объекта с 0-ым состоянием

		int crossroadIndex1 = rndGen->Next(0, VERTEX_QUANTITY);
		int crossroadIndex2 = 0;

		List<int>^ verticeCompatibleFirstHalf = gcnew List<int>(2); // 20, 32
		verticeCompatibleFirstHalf->Add(20); verticeCompatibleFirstHalf->Add(32);

		List<int>^ verticeCompatibleSecondHalf = gcnew List<int>(2); // 01, 13
		verticeCompatibleSecondHalf->Add(1); verticeCompatibleSecondHalf->Add(13);

		int verticeIndex1 = 0;
		int verticeIndex2 = 0;

		bool a = (Vertices[crossroadIndex1][verticeIndex1]->X == Vertices[crossroadIndex2][verticeIndex2]->X); // совпал X у 2-х точек (1-ая и 2-ая)
		bool b = (Vertices[crossroadIndex1][verticeIndex1]->Y == Vertices[crossroadIndex2][verticeIndex2]->Y); // совпал Y у 2-х точек (1-ая и 2-ая)

		if (crossroadIndex1 < Convert::ToInt16(VERTEX_QUANTITY / 2)) {
			for each (int verticeIndexes in verticeCompatibleFirstHalf) {
				verticeIndex1 = verticeIndexes / 10;
				verticeIndex2 = verticeIndexes % 10;

				for (int i = crossroadIndex1 + 1; i < VERTEX_QUANTITY - 1; i++) {
					crossroadIndex2 = i;

					a = (Vertices[crossroadIndex1][verticeIndex1]->X == Vertices[crossroadIndex2][verticeIndex2]->X); // совпал X у 2-х точек (1-ая и 2-ая)
					b = (Vertices[crossroadIndex1][verticeIndex1]->Y == Vertices[crossroadIndex2][verticeIndex2]->Y); // совпал Y у 2-х точек (1-ая и 2-ая)

					if (((a + b) % 2) && (verticeCompatibleFirstHalf->Contains(verticeIndex1 * 10 + verticeIndex2))) { break; }
				}
				if (((a + b) % 2) && verticeCompatibleFirstHalf->Contains(verticeIndex1 * 10 + verticeIndex2)) { break; }
			}
		}
		else if (crossroadIndex1 >= Convert::ToInt16(VERTEX_QUANTITY / 2)) {
			for each (int verticeIndexes in verticeCompatibleSecondHalf) {
				verticeIndex1 = verticeIndexes / 10;
				verticeIndex2 = verticeIndexes % 10;

				for (int i = crossroadIndex1 - 1; i >= 0; i--) {
					crossroadIndex2 = i;

					a = (Vertices[crossroadIndex1][verticeIndex1]->X == Vertices[crossroadIndex2][verticeIndex2]->X); // совпал X у 2-х точек (1-ая и 2-ая)
					b = (Vertices[crossroadIndex1][verticeIndex1]->Y == Vertices[crossroadIndex2][verticeIndex2]->Y); // совпал Y у 2-х точек (1-ая и 2-ая)

					if (((a + b) % 2) && (verticeCompatibleSecondHalf->Contains(verticeIndex1 * 10 + verticeIndex2))) { break; }
				}
				if (((a + b) % 2) && verticeCompatibleSecondHalf->Contains(verticeIndex1 * 10 + verticeIndex2)) { break; }
			}
		}

		Point^ firstPoint = Vertices[crossroadIndex1][verticeIndex1];
		Point^ secondPoint = Vertices[crossroadIndex2][verticeIndex2];

		Passengers[Passengers->Count - 1]->startNode::set(gcnew Node(crossroadIndex1 * 10 + verticeIndex1));
		Node^ passEndNode = gcnew Node(crossroadIndex2 * 10 + verticeIndex2);
		passEndNode->previous = Passengers[Passengers->Count - 1]->startNode::get();

		if (a && !b) {
			if (crossroadIndex2 > crossroadIndex1) { passEndNode->direction = "down"; }
			else if (crossroadIndex2 < crossroadIndex1) { passEndNode->direction = "up"; }
			Passengers[Passengers->Count - 1]->xPos::set(firstPoint->X - (Math::Pow(-1, (verticeIndex1 % 2)) * (PASSENGER_OFFSET + (PASSENGER_HEIGHT / 2))));
			Passengers[Passengers->Count - 1]->yPos::set(rndGen->Next(Math::Min(firstPoint->Y, secondPoint->Y) + 70, Math::Max(firstPoint->Y, secondPoint->Y) - 50));
		}
		else if (b && !a) {
			if (crossroadIndex2 > crossroadIndex1) { passEndNode->direction = "right"; Passengers[Passengers->Count - 1]->yPos::set(firstPoint->Y + PASSENGER_OFFSET - (PASSENGER_HEIGHT / 2)); }
			else if (crossroadIndex2 < crossroadIndex1) { passEndNode->direction = "left"; Passengers[Passengers->Count - 1]->yPos::set(firstPoint->Y - PASSENGER_OFFSET - (PASSENGER_HEIGHT / 2)); }
			Passengers[Passengers->Count - 1]->xPos::set(rndGen->Next(Math::Min(firstPoint->X, secondPoint->X) + 70, Math::Max(firstPoint->X, secondPoint->X) - 50));
		}

		Passengers[Passengers->Count - 1]->endNode::set(passEndNode);

	}
	// создание пассажира автобуса
	else if ((numOfPassengers < 20) && (randomNumber > 950)) {
		int BusStopIndex = rndGen->Next(0, BUSSTOPS_COUNT);
		BusPassengers[BusStopIndex]->Add(gcnew Passenger(4, BusStopIndex));

		if (BusStopIndex < 2) {
			if (BusStopIndex == 0) { BusPassengers[BusStopIndex]->ToArray()[BusPassengers[BusStopIndex]->Count - 1]->direction::set("left"); }
			else { BusPassengers[BusStopIndex]->ToArray()[BusPassengers[BusStopIndex]->Count - 1]->direction::set("right"); }

			BusPassengers[BusStopIndex]->ToArray()[BusPassengers[BusStopIndex]->Count - 1]->xPos::set(busStops[BusStopIndex]->X + rndGen->Next(PASSENGER_HEIGHT / 2, BUSSTOP_WIDTH - PASSENGER_HEIGHT / 2));
			BusPassengers[BusStopIndex]->ToArray()[BusPassengers[BusStopIndex]->Count - 1]->yPos::set(busStops[BusStopIndex]->Y + rndGen->Next(PASSENGER_HEIGHT / 2, BUSSTOP_HEIGHT - PASSENGER_HEIGHT / 2));
		}
		else if (BusStopIndex >= 2) {
			BusPassengers[BusStopIndex]->ToArray()[BusPassengers[BusStopIndex]->Count - 1]->direction::set("up");

			BusPassengers[BusStopIndex]->ToArray()[BusPassengers[BusStopIndex]->Count - 1]->xPos::set(busStops[BusStopIndex]->X + rndGen->Next(PASSENGER_HEIGHT / 2, BUSSTOP_HEIGHT - PASSENGER_HEIGHT / 2));
			BusPassengers[BusStopIndex]->ToArray()[BusPassengers[BusStopIndex]->Count - 1]->yPos::set(busStops[BusStopIndex]->Y + rndGen->Next(PASSENGER_HEIGHT / 2, BUSSTOP_WIDTH - PASSENGER_HEIGHT / 2));
		}
	}
}

void MyEnvironment::TaxiChoise() {
	Random^ rndGen = gcnew Random();
	for (int i = 0; i < Passengers->Count; i++) {
		int rndNumber = rndGen->Next(0, 101);
		if (rndNumber >= 80 && Passengers[i]->state::get() == 0) {
			int serviceCarIndex = rndGen->Next(0, TaxiCars->Count);
			TaxiCar^ serviceCar = TaxiCars[serviceCarIndex];
			while (serviceCar->state::get() == 1 || serviceCar->state::get() == 2) { serviceCarIndex = rndGen->Next(0, TaxiCars->Count); serviceCar = TaxiCars[serviceCarIndex]; }

			serviceCar->state::set(1);
			serviceCar->currentClient::set(Passengers[i]);

			EventPassengerTaxiChoise(serviceCarIndex);
			/*Passengers[i]->state::set(1);
			Passengers[i]->serviceCarIndex::set(serviceCarIndex);*/

			serviceCar->WayFind(Passengers[i], Vertices);
		}
	}
}

// действия для автобуса
void MyEnvironment::BusActions() {
	if (_Bus->state::get() == 0) {
		_Bus->Move(Vertices, busStops);
	}
	else if (_Bus->state::get() == 3) {
		_localTimer += 0.05;
		if (_localTimer > 2) { _Bus->state::set(0); _localTimer = 0; }
	}
}

// действия для такси
void MyEnvironment::TaxiActions() {
	for (int i = 0; i < TaxiCars->Count; i++) {
		if (TaxiCars[i]->state::get() == 0) { TaxiCars[i]->Move(Vertices); }
		else if (TaxiCars[i]->state::get() == 1) { TaxiCars[i]->MoveToPassenger(Vertices, TaxiCars[i]->currentClient::get()); }
		else if (TaxiCars[i]->state::get() == 2) {
			TaxiCars[i]->tripDuration::set(TaxiCars[i]->tripDuration::get() + 0.05);
			TaxiCars[i]->Move(Vertices);
		}
		else if (TaxiCars[i]->state::get() == 3) {
			_localTimer += 0.05;
			if (_localTimer > 1) { TaxiCars[i]->state::set(2); _localTimer = 0; }
		}
		else if (TaxiCars[i]->state::get() == 4) {
			_localTimer += 0.05;
			if (_localTimer > 1) { TaxiCars[i]->state::set(0); _localTimer = 0; }
		}
	}
}

// действия для пассажиров автобуса
	// посадка
	//void BusPassengersTakeOn(List<Passenger^>^ waitingPassengers) {
	//	for (int i = 0; i < waitingPassengers->Count; i++) {
	//		if (_Bus->state::get() == 3) {
	//			_localTimer2 += 0.5;
	//			if (_localTimer2 > 2) {
	//				_Bus->TakeOn(waitingPassengers->ToArray()[i]);
	//				_localTimer2 = 0;
	//			}
	//		}
	//	}
	//}
	//// высадка
	//void BusPassengersDropOff() {
	//	for each (Passenger ^ tripingPassenger in _Bus->currentClient::get()->ToArray()) {
	//		if ((_Bus->state::get() == 3) && (tripingPassenger->goalbusStopIndex::get() == _Bus->stopAt::get())) {
	//			_localTimer3 += 0.5;
	//			if (_localTimer3 > 5) {
	//				Random^ rndGen = gcnew Random();
	//				_Bus->DropOff(tripingPassenger);
	//				_localTimer3 = 0;
	//			}
	//		}
	//	}
	//}
	// движение по прибытии
void MyEnvironment::BusPassengersMove() {
	for (int i = 0; i < BusPassengers->Count; i++) {
		for (int j = 0; j < BusPassengers[i]->Count; j++) {
			if ((BusPassengers[i]->ToArray()[j]->state::get() == 3) && BusPassengers[i]->ToArray()[j]->moveActions::get() < 10) { BusPassengers[i]->ToArray()[j]->MoveAway(); }
			else if ((BusPassengers[i]->ToArray()[j]->state::get() == 3) && BusPassengers[i]->ToArray()[j]->moveActions::get() >= 10) { BusPassengers[i]->Remove(BusPassengers[i]->ToArray()[j]); }
		}
	}
}

// действия пассажиров
void MyEnvironment::PassengersAction() {
	for (int i = 0; i < Passengers->Count; i++) {
		if (Passengers[i]->state::get() == 2) {
			if (TaxiCars[Passengers[i]->serviceCarIndex::get()]->tripDuration::get() > 10) {
				TaxiCars[Passengers[i]->serviceCarIndex::get()]->onDropOff(Passengers[i]);
			}
		}
		else if ((Passengers[i]->state::get() == 3) && (Passengers[i]->moveActions::get() < 10)) { Passengers[i]->MoveAway(); }
		else if ((Passengers[i]->state::get() == 3) && (Passengers[i]->moveActions::get() >= 10)) { Passengers->Remove(Passengers[i]); }
	}
}

void MyEnvironment::TimerTickActions() {
	// услуги сервиса (появление пассажира в системе + выбор им одной из свободных машин такси)
	PassengerSpawn();
	TaxiChoise();

	BusActions();

	// массив пассажиров в 4 состоянии (ждут автобуса)
	List<Passenger^>^ waitingPassengers = gcnew List<Passenger^>(0);
	for each (Passenger ^ passenger in BusPassengers[_Bus->stopAt::get()]) { if (passenger->state::get() == 4) { waitingPassengers->Add(passenger); } }

	//BusPassengersTakeOn(waitingPassengers);
	//BusPassengersDropOff();
	BusPassengersMove();

	TaxiActions();

	PassengersAction();

}

Node::Node(int _vertice) {
	vertice = _vertice;
	previous = nullptr;
	direction = "";
}
Node::Node(int _vertice, String^ _direction) {
	vertice = _vertice;
	previous = nullptr;
	direction = _direction;
}

