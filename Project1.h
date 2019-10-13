#pragma once
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Project1.h - Declares a test aggregator which processes a list of test functors and reports
//                 test pass/fail status.
//
#include <vector>

enum LogLevel {INFO, DEBUG, TRACE};

class TestCase {
// protected:

public:
	std::vector<std::string> messages;
	bool pass;
	LogLevel level;
	virtual bool operator() () = 0;
};

class TestHarness {
public:
	TestHarness() {}
	bool executor(std::vector<TestCase*> testCases);
};