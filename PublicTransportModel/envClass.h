#pragma once
using namespace System;
using namespace System::Drawing;
using namespace System::Windows::Forms;
using namespace System::Collections::Generic;

#include "Defines.h"
#include "taxiClass.h"
#include "passengerClass.h"

public ref class MyEnvironment {
private:
	array<Point^>^ cordinates;
	array<Point^>^ busStops;
	array<array<Point^>^>^ Vertices;
	Bus^ _Bus;
	List<TaxiCar^>^ _TaxiCars;
	List<Passenger^>^ _Passengers;
	List<List<Passenger^>^>^ _BusPassengers;

	float _localTimer;
	float _localTimer2;
	float _localTimer3;

public:
	MyEnvironment() {
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
	~MyEnvironment() {};

	property Bus^ pBus {
		Bus^ get() { return _Bus; }
	}

	property List<TaxiCar^>^ TaxiCars {
		List<TaxiCar^>^ get() { return _TaxiCars; }
	}

	property List<Passenger^>^ Passengers {
		List<Passenger^>^ get() { return _Passengers; }
	}

	property List<List<Passenger^>^>^ BusPassengers {
		List<List<Passenger^>^>^ get() { return _BusPassengers; }
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

	// ОПРЕДЕЛЕНИЕ НАПРАВЛЕНИЯ
	void DirectionSet(Point^ spawnPoint, Point^ nextPoint, int crossroadIndex1, int crossroadIndex2) {
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

			DirectionSet(spawnPoint, nextPoint, crossroadIndex1, crossroadIndex2);
			
		}
		// в результате приведённый код создаёт объект машины такси и генерирует ему значения следующих полей
		// 1) координаты (x; y)
		// 2) направление движения (direction)
	}

	void PassengerSpawn() {
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

	void TaxiChoise() {
		Random^ rndGen = gcnew Random();
		for (int i = 0; i < Passengers->Count; i++) {
			int rndNumber = rndGen->Next(0, 101);
			if (rndNumber >= 80 && Passengers[i]->state::get() == 0) {
				int serviceCarIndex = rndGen->Next(0, TaxiCars->Count);
				TaxiCar^ serviceCar = TaxiCars[serviceCarIndex];
				while (serviceCar->state::get() == 1 || serviceCar->state::get() == 2) { serviceCarIndex = rndGen->Next(0, TaxiCars->Count); serviceCar = TaxiCars[serviceCarIndex]; }

				serviceCar->state::set(1);
				serviceCar->currentClient::set(Passengers[i]);

				Passengers[i]->state::set(1);
				Passengers[i]->serviceCarIndex::set(serviceCarIndex);

				serviceCar->WayFind(Passengers[i], Vertices);
			}
		}
	}

	// действия для автобуса
	void BusActions() {
		if (_Bus->state::get() == 0) {
			_Bus->Move(Vertices, busStops);
		}
		else if (_Bus->state::get() == 3) {
			_localTimer += 0.05;
			if (_localTimer > 2) { _Bus->state::set(0); _localTimer = 0; }
		}
	}

	// действия для такси
	void TaxiActions() {
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
	void BusPassengersMove() {
		for (int i = 0; i < BusPassengers->Count; i++) {
			for (int j = 0; j < BusPassengers[i]->Count; j++) {
				if ((BusPassengers[i]->ToArray()[j]->state::get() == 3) && BusPassengers[i]->ToArray()[j]->moveActions::get() < 10) { BusPassengers[i]->ToArray()[j]->MoveAway(); }
				else if ((BusPassengers[i]->ToArray()[j]->state::get() == 3) && BusPassengers[i]->ToArray()[j]->moveActions::get() >= 10) { BusPassengers[i]->Remove(BusPassengers[i]->ToArray()[j]); }
			}
		}
	}

	// действия пассажиров
	void PassengersAction() {
		for (int i = 0; i < Passengers->Count; i++) {
			if (Passengers[i]->state::get() == 2) {
				if (TaxiCars[Passengers[i]->serviceCarIndex::get()]->tripDuration::get() > 10) {
					TaxiCars[Passengers[i]->serviceCarIndex::get()]->DropOff(Passengers[i]);
				}
			}
			else if ((Passengers[i]->state::get() == 3) && (Passengers[i]->moveActions::get() < 10)) { Passengers[i]->MoveAway(); }
			else if ((Passengers[i]->state::get() == 3) && (Passengers[i]->moveActions::get() >= 10)) { Passengers->Remove(Passengers[i]); }
		} 
	}

	void TimerTickActions() {
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
};