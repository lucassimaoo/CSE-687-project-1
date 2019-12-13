/*
Name: Derrick Ward
Class/Section: CSE 687
Project: 2
Description: This file is the interface for the Test Logger
Date: 10/29/2019
*/

#include "TestPredicate.h"

#ifndef TESTLOGGER
#define TESTLOGGER

class TestLogger {
public:
    enum LogLevel { RESULT, INFO, DEBUG };
    TestLogger(TestLogger::LogLevel logLevel);
    ~TestLogger();
    std::string logTestPredicate(TestPredicate testPredicate);
	void log(std::string message);
private:
    LogLevel logLevel;
    void logTestPredicateApplicationMessages(TestPredicate testPredicate);
    void logTestPredicateApplicationState(TestPredicate testPredicate);
    string getLogLevel();
    string convertTimeToStr(SYSTEMTIME st);
};

#endif
