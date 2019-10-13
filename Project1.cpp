#include <iostream>
#include <chrono>
#include <ctime>
#include <cstdio>

#include "Project1.h"

using namespace std;

// This is the method which calls each test to be tested in the try block. It also keeps
// a count of number of tests passed. 
bool TestHarness::executor(vector<TestCase*> testCases) {
	std::cout << "execute method called" << endl;
	for (int i = 0; i < (int)testCases.size(); i++) {
		try {

			TestCase *test = testCases[i];
			
			if ((*testCases[i])()) {

				
				cout << "Test Passed " << endl;
			} else {
				cout << "Test Failed " << endl;
			}
		}
		catch (const char* msg) {
			cout << "Test Failed: " << msg << endl;
		}
	}
	return true;
}

// Just print out the results
/*void TestHarness::results()
{
	cout << "Total Tests Passed: " << passed << endl;
	cout << "Total Tests Run:    " << myTests.size() << endl;
}*/

class PassInfo : public TestCase {

public:
	PassInfo(LogLevel logLevel) {
		level = logLevel;
	}
	bool operator()() {
		messages.push_back("All good in here!");
		pass = true;
		return true;
	}
};

class test2 : public TestCase {
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

class test3 : public TestCase {
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
	class TestHarness th;

	vector<TestCase*> testCases;

	enum LogLevel level = INFO;
	testCases.push_back(new PassInfo(level));

	bool result = th.executor(testCases);

	cout << "General result: " << result << endl;

	/*int debug_level;

	debug_level = 0;
	th.addTest(new test1(3, 4, 5, debug_level));
	debug_level = 2;
	th.addTest(new test2(6, 7, debug_level));
	debug_level = 2;
	th.addTest(new test3(4, 0, debug_level));
	th.execute();
	th.results();*/
		
}