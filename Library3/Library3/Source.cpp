#include "pch.h"
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include "Library3.h"

TestReturn ITest() {

	std::vector<std::string> applicationSpecificMessages = { "Testing what happens if an exception is thrown" };

	// Capture Application/Test State
	std::map<std::string, std::string> applicationState;

	throw std::exception("Can't divide by zero");
}