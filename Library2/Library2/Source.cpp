#include "pch.h"
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include "Library2.h"

TestReturn ITest() {

	std::vector<std::string> applicationSpecificMessages = { "Testing if (num1 + num2) > num3" };

	int num1 = 3;
	int num2 = 4;
	int num3 = 7;

	// Capture Application/Test State
	std::map<std::string, std::string> applicationState;
	applicationState.insert({ "num1", std::to_string(num1) });
	applicationState.insert({ "num2", std::to_string(num2) });
	applicationState.insert({ "num3", std::to_string(num3) });

	// Run Test and Capture Test Results
	bool result = ((num1 + num2) > num3);

	return { result, applicationSpecificMessages, applicationState};
}