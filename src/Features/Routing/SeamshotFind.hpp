#pragma once
#include "Command.hpp"
#include "Features/Feature.hpp"
#include "Utils.hpp"
#include "Variable.hpp"

class SeamshotFind : public Feature {
public:
	SeamshotFind();
};

extern SeamshotFind *seamshotFind;

extern Variable p2fx_seamshot_finder;
