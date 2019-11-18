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
#include <mutex>
#include <thread>
#include <queue>
#include "../Cpp11-BlockingQueue/Cpp11-BlockingQueue.h"

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
    void runUnitTests(int threadNum);
	void server();
	void serverSocket();
	void processQueues();
	bool stillGoing() { return (running[0] | running[1]); }
	void printStats();

private:
	LogLevel logLevel;
	int failCount;
	int passCount;
	int testCount;
    bool execute(TestReturn(*)());
    void logTestPredicate(TestPredicate testPredicate);
    string getLogLevel();
	bool running[2];
	int work[2];
	string files[2];
	BlockingQueue<int> readyQueue;
	BlockingQueue<string> testQueue;
	std::mutex m_counts;
	std::mutex iolock;
};

#endif 
