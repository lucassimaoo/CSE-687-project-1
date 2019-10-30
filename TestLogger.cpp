/*
Name: Derrick Ward
Class/Section: CSE 687
Project: 2
Description: This file is the implementation for the Test Logger
Date: 10/29/2019
*/

#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include "TestLogger.h"
#include "TestPredicate.h"

using std::cout;
using std::endl;

TestLogger::TestLogger(TestLogger::LogLevel logLevel)
{
    this->logLevel = logLevel;
}

// Deconstructor
TestLogger::~TestLogger()
{

}

// Get Log Level as a string
string TestLogger::getLogLevel()
{
    string logLevel = "Unknown";

    switch (this->logLevel)
    {
    case TestLogger::LogLevel::RESULT:
        logLevel = "RESULT";
        break;
    case TestLogger::LogLevel::INFO:
        logLevel = "INFO";
        break;
    case TestLogger::LogLevel::DEBUG:
        logLevel = "DEBUG";
        break;
    }

    return logLevel;
}

// Get the string version of the time, test
string TestLogger::convertTimeToStr(SYSTEMTIME st) {

    GetLocalTime(&st);

    std::ostringstream strStream;

    strStream << st.wDay << "/" << st.wMonth << "/" << st.wYear << "  " << st.wHour << ":" << std::setfill('0') << std::setw(2) << std::right;
    strStream << st.wMinute << ":" << std::setfill('0') << std::setw(2) << std::right << st.wSecond << "  " << st.wMilliseconds << " msecs" << endl;

    string timeStr = strStream.str();

    return timeStr;
}

// Logs Test Predicate Information, based on logger's log level
void TestLogger::logTestPredicate(TestPredicate testPredicate)
{
    switch (this->logLevel)
    {
    case TestLogger::LogLevel::RESULT:
        cout << "Test Result: " << (testPredicate.getResult() == true ? "Pass" : "Fail") << endl;
        break;
    case TestLogger::LogLevel::INFO:
        cout << "Test Result: " << (testPredicate.getResult() == true ? "Pass" : "Fail") << endl << endl;
        this->logTestPredicateApplicationMessages(testPredicate);
        break;
    case TestLogger::LogLevel::DEBUG:
        cout << "Test Result: " << (testPredicate.getResult() == true ? "Pass" : "Fail") << endl << endl;
        this->logTestPredicateApplicationMessages(testPredicate);
        cout << endl;
        this->logTestPredicateApplicationState(testPredicate);
        cout << endl;
        cout << "Test Start Time: " << this->convertTimeToStr(testPredicate.getStartTime()) << endl;
        cout << "Test End Time: " << this->convertTimeToStr(testPredicate.getEndTime()) << endl;
        break;
    }
}

// Logs Test Predicate's Application Specific Messages
void TestLogger::logTestPredicateApplicationMessages(TestPredicate testPredicate)
{
    cout << "Test Application Specific Messages: " << endl;

    for (auto message : testPredicate.getApplicationSpecificMessages())
    {
        cout << "- " << message << endl;
    }

    cout << endl;
}

// Logs Test Predicate's Application State
void TestLogger::logTestPredicateApplicationState(TestPredicate testPredicate)
{
    cout << "Test Application State: " << endl;

    for (auto stateElement : testPredicate.getApplicationState())
    {
        cout << "-> Variable Name: " << stateElement.first << ", Value: " << stateElement.second << endl;
    }

    cout << endl;
}