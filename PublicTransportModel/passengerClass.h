#pragma once

#include "envClass.h"

public ref class Passenger {
private:
	int _xPos;
	int _yPos;

	int _state; // 0 - выбор машины; 1 - ожидание машины; 2 - в пункте прибытия
	MyEnvironment^ env;

public:
	Passenger(MyEnvironment^ _env) {
		_xPos = 0;
		_yPos = 0;
		_state = 0;
		env = _env;
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

	void TaxiChoise() {
		Random^ rndGen = gcnew Random();
		int rndNumber = rndGen->Next(0, 101);
		if (env->Passengers->Count && rndNumber >= 80) {
			int serviceCarIndex = rndGen->Next(0, env->TaxiCars->Count);
			TaxiCar^ ServiceCar = env->TaxiCars[serviceCarIndex];
			while (ServiceCar->state::get() == 2 || ServiceCar->state::get() == 1) {
				serviceCarIndex = rndGen->Next(0, env->TaxiCars->Count);
				ServiceCar = env->TaxiCars[serviceCarIndex];
			}

			ServiceCar->state::set(1);
		}
	}
};