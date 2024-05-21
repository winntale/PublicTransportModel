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
	array<array<Point^>^>^ Vertices;
	List<TaxiCar^>^ _TaxiCars;
	List<Passenger^>^ _Passengers;

public:
	MyEnvironment() {
		_TaxiCars = gcnew List<TaxiCar^>(0);
		_Passengers = gcnew List<Passenger^>(0);
		cordinates = gcnew array<Point^>(VERTEX_QUANTITY) { Point(151, 39), Point(151, 290), Point(151, 792),
			Point(402, 39), Point(402, 290), Point(402, 541), Point(402, 792), Point(653, 39), Point(653, 290), Point(653, 541), Point(904, 290), Point(904, 541), Point(904, 792) };
		Vertices = gcnew array<array<Point^>^>(VERTEX_QUANTITY);
	}
	~MyEnvironment() {};

	property List<TaxiCar^>^ TaxiCars {
		List<TaxiCar^>^ get() { return _TaxiCars; }
	}

	property List<Passenger^>^ Passengers {
		List<Passenger^>^ get() { return _Passengers; }
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

	void PassengerSpawn(Label^ label3, Label^ label4, Label^ label5, Label^ label6, Label^ label7, Label^ label8, Label^ label9, Label^ label10) {
		Random^ rndGen = gcnew Random();
		int randomNumber = rndGen->Next(0, 1001);

		if (TaxiCars->Count && (Passengers->Count < TaxiCars->Count) && (randomNumber > 100)) { // 994
			Passengers->Add(gcnew Passenger());

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


			if (a && !b) {
				Passengers[Passengers->Count - 1]->xPos::set(firstPoint->X - (Math::Pow(-1, (verticeIndex1 % 2)) * (PASSENGER_OFFSET + (PASSENGER_HEIGHT / 2))));
				Passengers[Passengers->Count - 1]->yPos::set(rndGen->Next(Math::Min(firstPoint->Y, secondPoint->Y) + 20, Math::Max(firstPoint->Y, secondPoint->Y) - 20));
			}
			else if (b && !a) {
				Passengers[Passengers->Count - 1]->xPos::set(rndGen->Next(Math::Min(firstPoint->X, secondPoint->X) + 20, Math::Max(firstPoint->X, secondPoint->X) - 20));
				Passengers[Passengers->Count - 1]->yPos::set(firstPoint->Y - (Math::Pow(-1, verticeIndex1 / 2)) * (PASSENGER_OFFSET + (PASSENGER_HEIGHT / 2)));
			}

			Passengers[Passengers->Count - 1]->endPoint::set(Vertices[crossroadIndex2][verticeIndex2]);
			Passengers[Passengers->Count - 1]->endCrossroadIndex::set(crossroadIndex2);
			Passengers[Passengers->Count - 1]->endVerticeIndex::set(verticeIndex2);

			Passengers[Passengers->Count - 1]->startCrossroadIndex::set(crossroadIndex1);
			Passengers[Passengers->Count - 1]->startVerticeIndex::set(verticeIndex1);

			label3->Text = Convert::ToString(String::Format("{0}, {1}", crossroadIndex1, verticeIndex1));
			label4->Text = Convert::ToString(String::Format("{0}, {1}", crossroadIndex2, verticeIndex2));

			label5->Text = Convert::ToString(String::Format("{0} {1}", Passengers[Passengers->Count - 1]->xPos::get(), Passengers[Passengers->Count - 1]->yPos::get()));

			label6->Text = Convert::ToString(String::Format("{0} {1}", Vertices[crossroadIndex1][verticeIndex1]->X, Vertices[crossroadIndex1][verticeIndex1]->Y));
			label7->Text = Convert::ToString(String::Format("{0} {1}", Vertices[crossroadIndex2][verticeIndex2]->X, Vertices[crossroadIndex2][verticeIndex2]->Y));

			label10->Text = Convert::ToString(Passengers->Count);
		}
	}

	void TaxiChoise(Label^ label) {
		Random^ rndGen = gcnew Random();
		for (int i = 0; i < Passengers->Count; i++) {
			int rndNumber = rndGen->Next(0, 101);
			if (rndNumber >= 80 && Passengers[i]->state::get() == 0) {
				TaxiCar^ serviceCar = TaxiCars[rndGen->Next(0, TaxiCars->Count)];
				while (serviceCar->state::get() == 1 || serviceCar->state::get() == 2) { serviceCar = TaxiCars[rndGen->Next(0, TaxiCars->Count)]; }
				serviceCar->state::set(1);
				Passengers[i]->state::set(1);
				serviceCar->WayFind(Passengers[i], Vertices, label);
			}
		}
	}

	void TimerTickActions(Label^ label3, Label^ label4, Label^ label5, Label^ label6, Label^ label7, Label^ label8, Label^ label9, Label^ label10, Label^ label11) {
		for (int i = 0; i < TaxiCars->Count; i++) {
			if (TaxiCars[i]->state::get() == 0) { TaxiCars[i]->Move(Vertices); }
			else if (TaxiCars[i]->state::get() == 1) { TaxiCars[i]->MoveToPassenger(); }
		}
		PassengerSpawn(label3, label4, label5, label6, label7, label8, label9, label10);
		TaxiChoise(label11);
		//IfTaxiIsGone();
	}
	
};