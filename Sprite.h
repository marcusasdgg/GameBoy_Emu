#pragma once
#include <cstdint>
class Sprite
{
public:
	uint8_t xpos;
	uint8_t ypos;
	uint8_t tindex;
	uint8_t flags;
	bool yflip;
	bool xflip;
	bool palletenumber;
	bool objtobgprio;
	Sprite(int);
	bool isVoid() const;
	Sprite();
};

