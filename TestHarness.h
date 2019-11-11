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
#include <condition_variable>
#include <mutex>
#include <thread>
#include <queue>
#include "Cpp11-BlockingQueue.h"

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
    TestHarness(TestHarness::LogLevel logLevel, string file, int numChildren);
    ~TestHarness();
    void runUnitTests();
	
private:
    LogLevel logLevel;
	string file;
	int failCount;
	int passCount;
	int testCount;
	int numChildren;
	int work[2];
	//string dlls[4];
	string dlls[2];
	BlockingQueue<int> readyQueue;
	BlockingQueue<string> testQueue;
	//std::condition_variable cv;
	std::mutex m_counts;
	std::mutex iolock;
	
    bool execute(TestReturn(*)());
    void logTestPredicate(TestPredicate testPredicate);
    string getLogLevel();
	void getWork(int i);
	void processQueues();
	//void processQueues(int count);
};

#endif 
