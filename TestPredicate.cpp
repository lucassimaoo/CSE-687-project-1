/*
Name: Derrick Ward
Class/Section: CSE 687
Project: 1
Description: This file is the implementation for the Test Predicate interface. The Test Predicate is
             the data structure that hold all a test's information.
Date: 10/15/2019
*/

#include <chrono>
#include <ctime>
#include <map>
#include <string>
#include "TestPredicate.h"
#include <vector>

using std::map;
using std::string;
using std::time_t;
using std::vector;

TestPredicate::TestPredicate(bool result, vector<string> applicationSpecificMessages, map<string, string> applicationState, SYSTEMTIME startTime, SYSTEMTIME endTime) {
    this->result = result;
    this->applicationSpecificMessages = applicationSpecificMessages;
    this->applicationState = applicationState;
    this->startTime = startTime;
    this->endTime = endTime;
}

// Get the Test Result
bool TestPredicate::getResult()
{
    return this->result;
}

// Get Application Specific Messages
vector<string> TestPredicate::getApplicationSpecificMessages()
{
    return this->applicationSpecificMessages;
}

// Get Application State
map<string, string> TestPredicate::getApplicationState()
{
    return this->applicationState;
}

// Get Test Start Time
SYSTEMTIME TestPredicate::getStartTime()
{
    return this->startTime;
}

// Get Test End Time
SYSTEMTIME TestPredicate::getEndTime()
{
    return this->endTime;
}