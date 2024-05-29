#pragma once

constexpr auto VERTEX_QUANTITY = 13;

constexpr auto MAX_TAXICARS = 5;
constexpr auto TAXICAR_IMG_HEIGHT = 32;
constexpr auto TAXICAR_IMG_WIDTH = 32;
#define SPEED _maxVelocity / 30

constexpr auto PASSENGER_OFFSET = 30; // насколько далеко пассажир будет от дороги (20 - впритык по горизонтали; 35 - упирается в границу карты)
constexpr auto PASSENGER_HEIGHT = 16;
constexpr auto PASSENGER_SPEED = 1;

constexpr auto ANTISPAM_INTERVAL = 500;

constexpr auto BUS_WIDTH = 27;
constexpr auto BUS_HEIGHT = 58;
constexpr auto BUS_SPEED = 150;

constexpr auto MAX_BUS_PASSENGERS = 20;

constexpr auto BUSSTOPS_COUNT = 4;
constexpr auto BUSSTOP_WIDTH = 75;
constexpr auto BUSSTOP_HEIGHT = 25;