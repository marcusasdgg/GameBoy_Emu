#pragma once
#include <string>
#include "MBC.h"

class MBCFACTORY
{
public:
	static MBC* createMBC(std::string romPath, std::string savePath = "");
};

