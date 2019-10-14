#include <iostream>
#include <chrono>
#include <ctime>
#include <cstdio>

#include "Project1.h"

using namespace std;

bool TestHarness::executor(vector<TestCase*> testCases) {

	// indicates if all tests have passed
	bool result = true;

	for (int i = 0; i < (int)testCases.size(); i++) {
		try {
			TestContext ctx = (*testCases[i])();
			cout << "Running test " << ctx.name << endl;

			switch (ctx.level) {
				case INFO:
					logInfo(ctx);
					break;

				case DEBUG:
					logDebug(ctx);
					break;

				case TRACE:
					logTrace(ctx);
					break;
			}

			if (!ctx.pass) {
				result = false;
			}

		} catch (const char* msg) {
			result = false;
			cout << "Test Failed " << msg << endl;
		} 

		cout << "-------------------------------------------" << endl;
	}
	return result;
}

void TestHarness::logInfo(TestContext ctx) {
	if (ctx.pass) {	
		cout << "Test Passed " << ctx.name << endl;
	} else {
		cout << "Test Failed " << ctx.name << endl;
	}
}

void TestHarness::logDebug(TestContext ctx) {
	logInfo(ctx);

	if (ctx.messages.empty()) {
		cout << "No messages for test " << ctx.name << endl;
	} else {
		cout << "Messages for test " << ctx.name << endl;
		for (int i = 0; i < ctx.messages.size(); i++) {
			cout << ctx.messages[i] << endl;
		}
	}
}

void TestHarness::logTrace(TestContext ctx) {
	logDebug(ctx);
	//TODO print variable map
	auto timenow = chrono::system_clock::to_time_t(chrono::system_clock::now());
	cout << ctime(&timenow) << endl;
}

class PassTest : public TestCase {

public:
	PassTest(LogLevel logLevel, string name) {
		context.level = logLevel;
		context.name = name;
	}
	TestContext operator()() {
		context.messages.push_back("All good in here!");
		context.pass = true;
		return context;
	}
};

class FailTest : public TestCase {

public:
	FailTest(LogLevel logLevel, string name) {
		context.level = logLevel;
		context.name = name;
	}
	TestContext operator()() {
		context.messages.push_back("Sometimes stuff just goes wrong!");
		context.pass = false;
		return context;
	}
};

class ExceptionTest : public TestCase {

public:
	ExceptionTest(LogLevel logLevel, string name) {
		context.level = logLevel;
		context.name = name;
	}
	TestContext operator()() {
		throw ("Oh God something really bad happened!");
	}
};

int main() {
	class TestHarness th;

	vector<TestCase*> testCases;

	testCases.push_back(new PassTest(INFO, "PassInfo"));
	testCases.push_back(new PassTest(DEBUG, "PassDebug"));
	testCases.push_back(new PassTest(TRACE, "PassTrace"));

	testCases.push_back(new FailTest(INFO, "FailInfo"));
	testCases.push_back(new FailTest(DEBUG, "FailDebug"));
	testCases.push_back(new FailTest(TRACE, "FailTrace"));

	testCases.push_back(new ExceptionTest(TRACE, "FailTrace"));

	bool result = th.executor(testCases);

	cout << "General result: " << result << endl;
		
}