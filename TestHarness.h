/*
Name: Derrick Ward
Class/Section: CSE 687
Project: 1
Description: This file is the interface for the Test Harness
Date: 10/15/2019
*/

#include <ctime>
#include "UnitTests.h"
#include <string>
#include "TestPredicate.h"
#include <vector>

using std::vector;

#ifndef TESTHARNESS
#define TESTHARNESS

struct TestReturn {
	bool result;
	std::vector<std::string> applicationSpecificMessages;
	std::map<std::string, std::string> applicationState;
};

class TestHarness {
public:
    enum LogLevel { RESULT, INFO, DEBUG };
    TestHarness(TestHarness::LogLevel logLevel, string file);
    ~TestHarness();
    void runUnitTests();
private:
    LogLevel logLevel;
	string file;
	int failCount;
	int passCount;
    bool execute(TestReturn(*)());
    void logTestPredicate(TestPredicate testPredicate);
    string getLogLevel();
};

#endif 
