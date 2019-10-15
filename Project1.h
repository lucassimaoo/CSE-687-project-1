#pragma once
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Project1.h - Declares a test aggregator which processes a list of test functors and reports
//                 test pass/fail status.
//
#include <vector>

enum LogLevel {INFO, DEBUG, TRACE};

class TestContext {
public:
	std::vector<std::string> messages;
	bool pass;
	LogLevel level;
	std::string name;
};

class TestCase {
public:
	TestContext context;
	virtual TestContext operator() () = 0;
};

class TestHarness {
public:
	TestHarness(LogLevel defaultLevel) {}
	bool executor(std::vector<TestCase*> testCases);
	LogLevel defaultLevel;
private:
	void logInfo(TestContext ctx);
	void logDebug(TestContext ctx);
	void logTrace(TestContext ctx);
	void log(TestContext ctx);
};