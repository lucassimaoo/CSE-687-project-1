#include "pch.h"
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include "Library1.h"

TestReturn ITest() {

	int num1 = 3;
	int num2 = 4;
	int sum = 7;

	// Run Test and Capture Test Results
	bool result = (sum == (num1 + num2));

	std::map<std::string, std::string> applicationState;
	applicationState.insert({ "num1", std::to_string(num1) });
	applicationState.insert({ "num2", std::to_string(num2) });
	applicationState.insert({ "sum", std::to_string(sum) });

	return { result, { "Testing if (num1 + num2) == sum" }, applicationState};
}