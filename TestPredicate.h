/*
Name: Derrick Ward
Class/Section: CSE 687
Project: 1
Description: This file is the interface for the Test Predicate. The Test Predicate is
             the data structure that hold all a test's information.
Date: 10/15/2019
*/

#include <chrono>
#include <ctime>
#include <map>
#include <string>
#include <vector>

using std::map;
using std::string;
using std::time_t;
using std::vector;

#ifndef TESTPREDICATE
#define TESTPREDICATE

class TestPredicate {
public:
    TestPredicate(bool result, vector<string> applicationSpecificMessages, map<string, string> applicationState, time_t startTime, time_t endTime);
    bool getResult();
    vector<string> getApplicationSpecificMessages();
    map<string, string> getApplicationState();
    time_t getStartTime();
    time_t getEndTime();
private:
    bool result; // Holds Test Result
    vector<string> applicationSpecificMessages; // Holds Application Specific Messages
    map <string, string> applicationState; // Holds Object State, during test execution
    time_t startTime; // Holds Test Start Time
    time_t endTime; // Holds Test End Time
};

#endif
