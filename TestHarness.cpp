/*
Name: Derrick Ward
Class/Section: CSE 687
Project: 1
Description: This file is the implementation for the Test Harness interface
Date: 10/15/2019
*/

#include <ctime>

#include <exception>
#include <iostream>
#include <sstream>
#include <string>
#include "TestHarness.h"

using std::cout;
using std::endl;
using std::exception;

TestHarness::TestHarness(TestHarness::LogLevel logLevel, vector<TestPredicate(*)()> unitTests)
{
    this->logLevel = logLevel;
    this->unitTests = unitTests;
    cout << "Starting Test Harness..." << endl;
}

// Desconstuctor
TestHarness::~TestHarness()
{
    this->unitTests.clear();
}

// Get Log Level as a string
string TestHarness::getLogLevel()
{
    string logLevel = "Unknown";

    switch (this->logLevel)
    {
    case TestHarness::LogLevel::RESULT:
        logLevel = "RESULT";
        break;
    case TestHarness::LogLevel::INFO:
        logLevel = "INFO";
        break;
    case TestHarness::LogLevel::DEBUG:
        logLevel = "DEBUG";
        break;
    }

    return logLevel;
}

// Get the string version of the time, test
string TestHarness::convertTimeToStr(time_t time) {
    struct tm localTime;
    localtime_s(&localTime, &time);
    std::ostringstream strStream;

    strStream << (localTime.tm_year + 1900) << "-" << (localTime.tm_mon + 1) << "-" << (localTime.tm_mday) << " " << (localTime.tm_hour > 12 ? localTime.tm_hour - 12 : localTime.tm_hour) << ":" << (localTime.tm_min) << ":" << (localTime.tm_sec) << " " << (localTime.tm_hour >= 12 ? "PM" : "AM");

    string timeStr = strStream.str();

    return timeStr;
}

// Begins Running all unit Test
void TestHarness::runUnitTests()
{
    cout << "Running Unit Tests (With a Log Level of " << this->getLogLevel() << ")" << "\n\n" << endl;

    int testCounter = 1;
    for (auto unitTest : unitTests)
    {
        cout << "Running Test " << std::to_string(testCounter) << "..." << endl;
        cout << "--------------------------------------------" << endl;
        cout << "Test Details: " << endl;

        bool testResult = this->execute(unitTest);

        cout << "--------------------------------------------" << endl;
        cout << "Test " << std::to_string(testCounter) << " Completed: Result -> " << (testResult == true ? "Pass" : "Fail") << endl << endl;
        testCounter++; // Update Test Counter
    }

    cout << "FINISHED RUNNING ALL UNIT TESTS...\n\n" << endl;
}

// Runs Unit Tests and Return Test Result
bool TestHarness::execute(TestPredicate(*unitTest)()) {
    bool testResult = false;

    try
    {
        // Run Unit Test
        TestPredicate testPredicate = unitTest();

        // Save Test Result
        testResult = testPredicate.getResult();

        // Log Test Predicate Information
        this->logTestPredicate(testPredicate);
    }
    catch (exception & e)
    {
        cout << "Error Occured while running test: " << e.what() << endl;
    }

    return testResult;
}

// Logs Test Predicate Information, based on test harness' log level
void TestHarness::logTestPredicate(TestPredicate testPredicate)
{
    switch (this->logLevel)
    {
    case TestHarness::LogLevel::RESULT:
        cout << "Test Result: " << (testPredicate.getResult() == true ? "Pass" : "Fail") << endl;
        break;
    case TestHarness::LogLevel::INFO:
        cout << "Test Result: " << (testPredicate.getResult() == true ? "Pass" : "Fail") << endl << endl;
        this->logTestPredicateApplicationMessages(testPredicate);
        break;
    case TestHarness::LogLevel::DEBUG:
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
void TestHarness::logTestPredicateApplicationMessages(TestPredicate testPredicate)
{
    cout << "Test Application Specific Messages: " << endl;

    for (auto message : testPredicate.getApplicationSpecificMessages())
    {
        cout << "- " << message << endl;
    }

    cout << endl;
}

// Logs Test Predicate's Application State
void TestHarness::logTestPredicateApplicationState(TestPredicate testPredicate)
{
    cout << "Test Application State: " << endl;

    for (auto stateElement : testPredicate.getApplicationState())
    {
        cout << "-> Variable Name: " << stateElement.first << ", Value: " << stateElement.second << endl;
    }

    cout << endl;
}