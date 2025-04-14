#pragma once
#include <array>
#include "Pixel.h"
//separate object that is a queue that pushes into the actual screen, for all intensive purposes this is the actual screen.

class Fifo{
private: 
	std::array<std::array<PIXEL, 160>, 144> display;
public:
	void insert(PIXEL);
	std::array<std::array<PIXEL, 160>, 144> getDisplay();
	Fifo();
};

