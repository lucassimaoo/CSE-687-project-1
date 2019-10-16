// DWardProject1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "DemoUnitTests.h"
#include <iostream>
#include "TestHarness.h"
#include "TestPredicate.h"

int main()
{
    // Create collection of unit tests we want to run
    vector<TestPredicate(*)()> unitTests = {};
    unitTests.insert(unitTests.end(), unitTest1);
    unitTests.insert(unitTests.end(), unitTest2);
    unitTests.insert(unitTests.end(), unitTest3);

    // Create Test Harness (RESULT Log Level) and supply unit tests
    TestHarness testHarness(TestHarness::LogLevel::RESULT, unitTests);

    // Run Unit Tests
    testHarness.runUnitTests();

    // Create Test Harness (INFO Log Level) and supply unit tests
    TestHarness testHarness2(TestHarness::LogLevel::INFO, unitTests);

    // Run Unit Tests
    testHarness2.runUnitTests();

    // Create Test Harness (DEBUG Log Level) and supply unit tests
    TestHarness testHarness3(TestHarness::LogLevel::DEBUG, unitTests);

    // Run Unit Tests
    testHarness3.runUnitTests();
}


