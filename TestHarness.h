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

class TestHarness {
public:
    enum LogLevel { RESULT, INFO, DEBUG };
    TestHarness(TestHarness::LogLevel logLevel, vector<TestPredicate(*)()> unitTests);
    ~TestHarness();
    void runUnitTests();
private:
    LogLevel logLevel;
    vector<TestPredicate(*)()> unitTests;
    bool execute(TestPredicate(*)());
    void logTestPredicate(TestPredicate testPredicate);
    void logTestPredicateApplicationMessages(TestPredicate testPredicate);
    void logTestPredicateApplicationState(TestPredicate testPredicate);
    string getLogLevel();
    string convertTimeToStr(time_t time);
};

#endif 
