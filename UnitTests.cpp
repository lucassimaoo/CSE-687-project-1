/*
Name: Derrick Ward
Class/Section: CSE 687
Project: 1
Description: This file is the implementation for all of the unit tests
Date: 10/15/2019
*/

#include "UnitTests.h"
#include "TestHarness.h"
#include "TestPredicate.h"

// Unit Test #1
TestPredicate unitTest1()
{
    vector<string> applicationSpecificMessages = { "Testing if (num1 + num2) == sum" };

    int num1 = 3;
    int num2 = 4;
    int sum = 7;

    // Capture Application/Test State
    map<string, string> applicationState;
    applicationState.insert({ "num1", std::to_string(num1) });
    applicationState.insert({ "num2", std::to_string(num2) });
    applicationState.insert({ "sum", std::to_string(sum) });

    // Run Test and Capture Test Results
    bool result = (sum == (num1 + num2));

    // Generate Test Predicate
    TestPredicate testPredicate(result, applicationSpecificMessages, applicationState);

    return testPredicate;
}

// Unit Test #2
TestPredicate unitTest2()
{
    vector<string> applicationSpecificMessages = { "Testing if (num1 + num2) > num3" };

    int num1 = 3;
    int num2 = 4;
    int num3 = 7;

    // Capture Application/Test State
    map<string, string> applicationState;
    applicationState.insert({ "num1", std::to_string(num1) });
    applicationState.insert({ "num2", std::to_string(num2) });
    applicationState.insert({ "num3", std::to_string(num3) });

    // Run Test and Capture Test Results
    bool result = ((num1 + num2) > num3);

    // Generate Test Predicate
    TestPredicate testPredicate(result, applicationSpecificMessages, applicationState);

    return testPredicate;
}

// Unit Test #3
TestPredicate unitTest3()
{
    vector<string> applicationSpecificMessages = { "Testing what happens if an exception is thrown" };

    // Capture Application/Test State
    map<string, string> applicationState;

    throw std::exception("Can't divide by zero");

    // Run Test and Capture Test Results
    bool result = true;

    // Generate Test Predicate
    TestPredicate testPredicate(result, applicationSpecificMessages, applicationState);

    return testPredicate;
}