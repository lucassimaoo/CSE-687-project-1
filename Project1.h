#pragma once
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Project1.h - Declares a test aggregator which processes a list of test functors and reports
//                 test pass/fail status.
//
#include <vector>
//#include <iostream>

class theTest {
protected:
	int level;

public:
	virtual bool operator() () = 0;
	virtual ~theTest() {}
};

class TestHarness {
private:
	std::vector<theTest*> myTests;
	std::vector<int> levels;
	unsigned passed;

public:
	TestHarness() : passed(0) {}
	~TestHarness();
	void execute();
	void addTest(theTest* newTest, int debug_level);
	void results();
};
