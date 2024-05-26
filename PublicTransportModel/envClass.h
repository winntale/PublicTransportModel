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
	List<Passenger^>^ _BusPassengers;

	float _localTimer;

public:
	MyEnvironment() {
		_Bus = gcnew Bus();
		_TaxiCars = gcnew List<TaxiCar^>(0);
		_Passengers = gcnew List<Passenger^>(0);
		_BusPassengers = gcnew List<Passenger^>(0);
		cordinates = gcnew array<Point^>(VERTEX_QUANTITY) { Point(151, 75), Point(151, 326), Point(151, 828),
			Point(402, 75), Point(402, 326), Point(402, 577), Point(402, 828), Point(653, 75), Point(653, 326), Point(653, 577), Point(904, 326), Point(904, 577), Point(904, 828) };
		Vertices = gcnew array<array<Point^>^>(VERTEX_QUANTITY);
		
		busStops = gcnew array<Point^>(BUSSTOPS_COUNT) { Point(227, 35), Point(541, 867), Point(692, 159), Point(945, 399) }; // ������ 2 ����������� �� �����������, ��������� 2 - �� ���������
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

	property List<Passenger^>^ BusPassengers {
		List<Passenger^>^ get() { return _BusPassengers; }
	}

	// ����� ��������� ������� ������ �� ������� ���������
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

	// ����� ��������� ������� ������ ����� (��������)
	void TaxiSpawn() {
		Random^ rndGen = gcnew Random();
		if (TaxiCars->Count < MAX_TAXICARS) {
			TaxiCars->Add(gcnew TaxiCar());

			// ��������� ���������� ����������
			// 1) 2 ����� �� 2-� ������ �����������, ���������� �����, �� ������������ ����� ������� ������������� ������
			// 2) ����� �� 3-� ���������� (�� ������ 1-���), ���� ����� ������������ ������
			// ������������� ����� ���������� ���������� � ��������� 1, ��������� ��� ����� - � ��������� 2 � 3
			int crossroadIndex1 = rndGen->Next(0, VERTEX_QUANTITY);
			int crossroadIndex2 = rndGen->Next(0, VERTEX_QUANTITY);
			int crossroadIndex3 = rndGen->Next(0, VERTEX_QUANTITY);

			int verticeIndex1 = rndGen->Next(0, 4);
			int verticeIndex2 = rndGen->Next(0, 4);
			int verticeIndex3 = rndGen->Next(0, 4);


			bool a = (Vertices[crossroadIndex1][verticeIndex1]->X == Vertices[crossroadIndex2][verticeIndex2]->X); // ������ X � 2-� ����� (1-�� � 2-��)
			bool b = (Vertices[crossroadIndex1][verticeIndex1]->Y == Vertices[crossroadIndex2][verticeIndex2]->Y); // ������ Y � 2-� ����� (1-�� � 2-��)

			// !((a + b) % 2) - ���� ������ ���������� ����� �� ��� ���, ���� �� ��������� ������ ���� �� 2� ������� (���� a, ���� b)
			// || (crossroadIndex1 == crossroadIndex2) - ������� ����� ��� ����, ����� �������� �������� ������ ���� ����� ������ ����������
 			while (!((a + b) % 2) || (crossroadIndex1 == crossroadIndex2)) {
				crossroadIndex2 = rndGen->Next(0, VERTEX_QUANTITY);

				// ���� ����������, ���� �� ���� �� 2-�� ����������
				for (int i = 0; i < 4; i++) {
					a = (Vertices[crossroadIndex1][verticeIndex1]->X == Vertices[crossroadIndex2][i]->X);
					b = (Vertices[crossroadIndex1][verticeIndex1]->Y == Vertices[crossroadIndex2][i]->Y);
					if ((a + b) % 2) { break; }
				}
			}

			// ��������� 3-�� �����. ������� �� ��������� 2-�� � ���, ��� ������ ����� ����� ���� �� ����� ����������
			// (������ ����� ���������� �� ������ ���� ���, ������ ������ ��� �������� ������)
			// ��� ����������, ��� ��� ����� ���������� �������� ����������, ������� ������� ������ � ����� ������ ����������� (������ � �������� ������)

			bool a1 = (Vertices[crossroadIndex2][verticeIndex2]->X == Vertices[crossroadIndex3][verticeIndex3]->X); // ������ X � 2-�� � 3-� �����
			bool b1 = (Vertices[crossroadIndex2][verticeIndex2]->Y == Vertices[crossroadIndex3][verticeIndex3]->Y); // ������ Y � 2-�� � 3-� �����

			if (a && !b) { // ������ ���������� ������ �� ��������� (������������� 1-�� �����)
				if (crossroadIndex2 > crossroadIndex1) { verticeIndex1 = 2 * rndGen->Next(0, 2); } // ����
				else if (crossroadIndex2 < crossroadIndex1) { verticeIndex1 = 2 * rndGen->Next(0, 2) + 1; } // �����

				while (!b1 || a1 || (crossroadIndex3 == crossroadIndex2)) { // ���� �� ������ ���� �� �����������
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
			else if (b && !a) { // ������ ���������� ������ �� �����������
				if (crossroadIndex2 > crossroadIndex1) { verticeIndex1 = rndGen->Next(2, 4); } // �������
				else if (crossroadIndex2 < crossroadIndex1) { verticeIndex1 = rndGen->Next(0, 2); } // ������

				while (!a1 || b1 || (crossroadIndex3 == crossroadIndex2)) { // ���� �� ������ ���� �� ���������
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

			//���������� ����� ������� ����������
			TaxiCars[TaxiCars->Count - 1]->nextPoint::set(nextPoint);
			TaxiCars[TaxiCars->Count - 1]->npCrossroadIndex::set(crossroadIndex2);
			TaxiCars[TaxiCars->Count - 1]->npVerticeIndex::set(verticeIndex2);

			TaxiCars[TaxiCars->Count - 1]->nextPoint2::set(nextPoint2);
			TaxiCars[TaxiCars->Count - 1]->npCrossroadIndex2::set(crossroadIndex3);
			TaxiCars[TaxiCars->Count - 1]->npVerticeIndex2::set(verticeIndex3);

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
		}
		// � ���������� ���������� ��� ������ ������ ������ ����� � ���������� ��� �������� ��������� �����
		// 1) ���������� (x; y)
		// 2) ����������� �������� (direction)
	}

	void PassengerSpawn() {
		Random^ rndGen = gcnew Random();
		int randomNumber = rndGen->Next(0, 1001);
		
		// �������� ��������� �����
		if (TaxiCars->Count && (Passengers->Count < TaxiCars->Count) && (randomNumber > 100)) { // 994
			Passengers->Add(gcnew Passenger(0)); // �������� ������� � 0-�� ����������

			int crossroadIndex1 = rndGen->Next(0, VERTEX_QUANTITY);
			int crossroadIndex2 = 0;

			List<int>^ verticeCompatibleFirstHalf = gcnew List<int>(2); // 20, 32
			verticeCompatibleFirstHalf->Add(20); verticeCompatibleFirstHalf->Add(32);

			List<int>^ verticeCompatibleSecondHalf = gcnew List<int>(2); // 01, 13
			verticeCompatibleSecondHalf->Add(1); verticeCompatibleSecondHalf->Add(13);

			int verticeIndex1 = 0;
			int verticeIndex2 = 0;

			bool a = (Vertices[crossroadIndex1][verticeIndex1]->X == Vertices[crossroadIndex2][verticeIndex2]->X); // ������ X � 2-� ����� (1-�� � 2-��)
			bool b = (Vertices[crossroadIndex1][verticeIndex1]->Y == Vertices[crossroadIndex2][verticeIndex2]->Y); // ������ Y � 2-� ����� (1-�� � 2-��)

			if (crossroadIndex1 < Convert::ToInt16(VERTEX_QUANTITY / 2)) {
				for each (int verticeIndexes in verticeCompatibleFirstHalf) {
					verticeIndex1 = verticeIndexes / 10;
					verticeIndex2 = verticeIndexes % 10;

					for (int i = crossroadIndex1 + 1; i < VERTEX_QUANTITY - 1; i++) {
						crossroadIndex2 = i;

						a = (Vertices[crossroadIndex1][verticeIndex1]->X == Vertices[crossroadIndex2][verticeIndex2]->X); // ������ X � 2-� ����� (1-�� � 2-��)
						b = (Vertices[crossroadIndex1][verticeIndex1]->Y == Vertices[crossroadIndex2][verticeIndex2]->Y); // ������ Y � 2-� ����� (1-�� � 2-��)

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

						a = (Vertices[crossroadIndex1][verticeIndex1]->X == Vertices[crossroadIndex2][verticeIndex2]->X); // ������ X � 2-� ����� (1-�� � 2-��)
						b = (Vertices[crossroadIndex1][verticeIndex1]->Y == Vertices[crossroadIndex2][verticeIndex2]->Y); // ������ Y � 2-� ����� (1-�� � 2-��)

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
		// �������� ��������� ��������
		else if ((BusPassengers->Count < 20) && (randomNumber > 980)) {
			BusPassengers->Add(gcnew Passenger(4)); // �������� ������� � 4-�� ����������

			int BusStopIndex = rndGen->Next(0, BUSSTOPS_COUNT);
			if (BusStopIndex < 2) {
				if (BusStopIndex == 0) { BusPassengers[BusPassengers->Count - 1]->direction::set("left"); }
				else { BusPassengers[BusPassengers->Count - 1]->direction::set("right"); }

				BusPassengers[BusPassengers->Count - 1]->xPos::set(busStops[BusStopIndex]->X + rndGen->Next(PASSENGER_HEIGHT / 2, BUSSTOP_WIDTH - PASSENGER_HEIGHT / 2));
				BusPassengers[BusPassengers->Count - 1]->yPos::set(busStops[BusStopIndex]->Y + rndGen->Next(PASSENGER_HEIGHT / 2, BUSSTOP_HEIGHT - PASSENGER_HEIGHT / 2));
			}
			else if (BusStopIndex >= 2) {
				BusPassengers[BusPassengers->Count - 1]->direction::set("up");

				BusPassengers[BusPassengers->Count - 1]->xPos::set(busStops[BusStopIndex]->X + rndGen->Next(PASSENGER_HEIGHT / 2, BUSSTOP_HEIGHT - PASSENGER_HEIGHT / 2));
				BusPassengers[BusPassengers->Count - 1]->yPos::set(busStops[BusStopIndex]->Y + rndGen->Next(PASSENGER_HEIGHT / 2, BUSSTOP_WIDTH - PASSENGER_HEIGHT / 2));
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



	void TimerTickActions(Label^ label4, Label^ label5) {

		_Bus->Move(Vertices, label4, label5);

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
		PassengerSpawn();
		TaxiChoise();

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
	
};