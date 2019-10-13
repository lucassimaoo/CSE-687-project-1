// Project1.cpp : This file contains the 'main' function. Program execution begins and ends there.
// This is a test harness class that allows us to define and run many small tests. Each test has a
// debug level which is set when the test object is created. Each test when run returns a pass/fail 
// status. As the tests are all called by the test harness without arguments, a functor is used to
// setup any needed arguments. The debug levels are:
//     0: Pass/Fail only
//     1: Some extra debug information
//     2: Extra debug information as well as the time of day. 
//
// The test harness also calls each functor in a try block, so any exceptions can be logged
// and the test marked as "failed". 

#include <iostream>
#include <chrono>
#include <ctime>
#include <cstdio>

#include "Project1.h"

using namespace std;

// This is the method which calls each test to be tested in the try block. It also keeps
// a count of number of tests passed. 
bool TestHarness::execute()
{
	std::cout << "execute method called" << endl;
	for (int i = 0; i < (int)myTests.size(); i++) {
		try {
			if (!(*myTests[i])()) cout << "Test Failed " << endl;
			else {
				cout << "Test Passed" << endl;
				passed++;
			}
		}
		catch (const char* msg) {
			cout << "Test Failed: " << msg << endl;
		}
	}
	return true;
}

// This adds a test to the test vector
void TestHarness::addTest(theTest* newTest)
{
	myTests.push_back(newTest);
	std::cout << "Just pushed back a test " << endl;
}

// Just print out the results
void TestHarness::results()
{
	cout << "Total Tests Passed: " << passed << endl;
	cout << "Total Tests Run:    " << myTests.size() << endl;
}

class test1 : public theTest {
private:
	int a, b, c;
	int level;

public:
	test1(int x, int y, int z, int debug) : a(x), b(y), c(z), level(debug) {}
	bool operator()()
	{
		if ((a + b) > c) return true;
		return false;
	}
};

class test2 : public theTest {
private:
	int a, b;
	int level;

public:
	test2(int x, int y, int debug) : a(x), b(y), level(debug) {}
	bool operator()()
	{
		if ((a == b) && (level > 0)) cout << "operands close, but not quite" << endl;
		if (a > b) return true;
		if (level > 1) {
			const int max = 80;
			char str[max];
			time_t t = time(nullptr);
			struct tm timeinfo;
			//localtime_s(&timeinfo, &t);
			strftime(str, max, "%D, %H:%M (%I:%M%p)\n", (struct tm*)&timeinfo);
			printf(str);
		}
		return false;
	}
};

class test3 : public theTest {
private:
	int a, b;
	int level;

public:
	test3(int x, int y, int debug) : a(x), b(y), level(debug) {}
	bool operator()()
	{
		if (b == 0) throw ("divide by zero!");
		a = a / b;
		return true;
	}
};

int main()
{
	
    std::cout << "Hello World!\n";
		
	class TestHarness th;
	int debug_level;

	debug_level = 0;
	th.addTest(new test1(3, 4, 5, debug_level));
	debug_level = 2;
	th.addTest(new test2(6, 7, debug_level));
	debug_level = 2;
	th.addTest(new test3(4, 0, debug_level));
	th.execute();
	th.results();
		
}


