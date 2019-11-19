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
#include "ThreadPool.h"
#include <atomic>

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
    TestHarness(TestHarness::LogLevel logLevel);
    ~TestHarness();
    void runUnitTests(std::string file);
	std::thread server();
	void serverSocket();
private:
    LogLevel logLevel;
	std::atomic<int> failCount = 0;
	std::atomic<int>  passCount = 0;
	std::atomic<int>  testCounter = 1;
    bool execute(TestReturn(*)(), int testId);
    void logTestPredicate(TestPredicate testPredicate);
    string getLogLevel();
	ThreadPool<2> trpl;
};

#endif 
