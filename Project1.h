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
	TestHarness() {}
	bool executor(std::vector<TestCase*> testCases);
private:
	void logInfo(TestContext ctx);
	void logDebug(TestContext ctx);
	void logTrace(TestContext ctx);
};