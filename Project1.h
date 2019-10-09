#pragma once
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Project1.h - Declares a test aggregator which processes a list of test functors and reports
//                 test pass/fail status.
//
#include <vector>
//#include <iostream>

class theTest {
protected:
	static const int count;

public:
	virtual bool operator() () = 0;
	virtual ~theTest() {}
};

class TestHarness {
private:
	std::vector<theTest*> myTests;
	unsigned passed;

public:
	TestHarness() : passed(0) {}
	bool execute();
	void addTest(theTest* newTest);
	void results();
};
