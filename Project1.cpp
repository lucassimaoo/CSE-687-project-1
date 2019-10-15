#include <iostream>
#include <chrono>
#include <ctime>
#include <cstdio>

#include "Project1.h"

using namespace std;

bool TestHarness::executor(vector<TestCase*> testCases) {

	bool haveAllTestsPassed = true;

	for (int i = 0; i < (int)testCases.size(); i++) {
		try {
			
			TestContext ctx = (*testCases[i])();
			cout << "Running test " << ctx.name << endl;

			log(ctx);

			if (!ctx.pass) {
				haveAllTestsPassed = false;
			}

		} catch (const char* msg) {

			TestContext ctx;
			ctx.level = defaultLevel;
			ctx.pass = false;
			ctx.messages.push_back(msg);

			log(ctx);

			haveAllTestsPassed = false;
		} 

		cout << "-------------------------------------------" << endl;
	}
	return haveAllTestsPassed;
}

void TestHarness::log(TestContext ctx) {
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
	class TestHarness th = TestHarness(TRACE);

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