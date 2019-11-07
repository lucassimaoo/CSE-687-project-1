// Project1.cpp : This file contains the 'main' function. Program execution begins and ends there.
#include "UnitTests.h"
#include <iostream>
#include "TestHarness.h"
#include "TestPredicate.h"
//#include "./MsgPassingComm/Comm.h"

int main()
{
    // Create Test Harness (RESULT Log Level) and supply unit tests
    TestHarness testHarness(TestHarness::LogLevel::RESULT, "tests.xml");

    // Run Unit Tests
    testHarness.runUnitTests();

    // Create Test Harness (INFO Log Level) and supply unit tests
    TestHarness testHarness2(TestHarness::LogLevel::INFO, "tests.xml");

    // Run Unit Tests
    testHarness2.runUnitTests();

    // Create Test Harness (DEBUG Log Level) and supply unit tests
    TestHarness testHarness3(TestHarness::LogLevel::DEBUG, "tests.xml");

    // Run Unit Tests
    testHarness3.runUnitTests();
}


