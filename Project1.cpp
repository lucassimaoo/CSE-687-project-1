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
#include <exception>
#include <Windows.h>

#include "Project1.h"

using namespace std;
using namespace std::chrono;

// This is the method which calls each test in myTests and runs it in a try block. If the test fails,
// messages are printed out depending on the value of the debug_level associated with that test.
void TestHarness::execute()
{
	std::cout << "execute method called" << endl;
	
	SYSTEMTIME st;
	char start_time[84];
	
	for (int i = 0; i < (int)myTests.size(); i++) {
		
		GetLocalTime(&st);

		sprintf_s(start_time,(size_t)84,"%d/%d/%d  %d:%02d:%02d %d", st.wDay, st.wMonth, st.wYear, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
		
		try {
			if (!(*myTests[i])()) {
				cout << "Test Failed " << endl;
			}
			else {
				cout << "Test Passed" << endl;
				passed++;
				continue;
			}
		}
		catch (const char* msg) {
			cout << "Test Failed: " << msg << endl;
		}
		catch (exception e) {
			cout << "Exception detected: " << e.what() << endl;
		}
		catch (...) {
			cout << "Unexpected Exception" << endl;
		}
		if (levels[i] >= 1) cout << "level one detected" << endl;
		if (levels[i] == 2) {
			cout << "level two detected" << endl;
			printf("Start Time: %s msec\n", start_time);
			GetLocalTime(&st);
			printf("End Time:   %d/%d/%d  %d:%02d:%02d %d msec\n", st.wDay, st.wMonth, st.wYear, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
		}
	}
	return;
}

// This adds a test to the test vector
void TestHarness::addTest(theTest* newTest, int level)
{
	myTests.push_back(newTest);
	levels.push_back(level);
	std::cout << "Just pushed back a test " << endl;
}

// Just print out the results
void TestHarness::results()
{
	cout << "Total Tests Passed: " << passed << endl;
	cout << "Total Tests Run:    " << myTests.size() << endl;
}

// Destructor, free up the two vectors
TestHarness::~TestHarness()
{
	myTests.clear();
	levels.clear();
}

class test1 : public theTest {
private:
	int a, b, c;
	
public:
	test1(int x, int y, int z) : a(x), b(y), c(z) {}
	bool operator()()
	{
		Sleep(1);
		if ((a + b) > c) return true;
		return false;
	}
};

class test2 : public theTest {
private:
	int a, b;
	
public:
	test2(int x, int y) : a(x), b(y) {}
	bool operator()()
	{
		Sleep(2);
		if (a > b) return true;
		return false;
	}
};

class test3 : public theTest {
private:
	int a, b;
	
public:
	test3(int x, int y) : a(x), b(y) {}
	bool operator()()
	{
		Sleep(3);
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
	th.addTest(new test1(3, 4, 5), debug_level);
	debug_level = 2;
	th.addTest(new test2(6, 7), debug_level);
	debug_level = 2;
	th.addTest(new test3(4, 0), debug_level);
	th.execute();
	th.results();
}


