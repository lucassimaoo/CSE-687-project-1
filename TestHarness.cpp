/*
Name: Derrick Ward
Class/Section: CSE 687
Project: 1
Description: This file is the implementation for the Test Harness interface
Date: 10/15/2019
*/

#include <ctime>

#include <exception>
#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
#include <windows.h>
#include "TestHarness.h"
#include "TestLogger.h"
#include "pugixml.hpp"

using namespace pugi;
using std::cout;
using std::endl;
using std::exception;

typedef TestReturn(__cdecl* ITEST)();

TestHarness::TestHarness(TestHarness::LogLevel logLevel, std::string file, int numChildren)
{
    this->logLevel = logLevel;
    this->file = file;
    this->passCount = 0;
    this->failCount = 0;
	this->testCount = 1;
	this->numChildren = numChildren;
	//work = new int(numChildren);
	for (int i = 0; i < numChildren; i++) work[i] = -1;    // initialize
	cout << "Starting Test Harness..." << endl;
}

// Desconstuctor
TestHarness::~TestHarness()
{
    //this->unitTests.clear();
}

// Get Log Level as a string
string TestHarness::getLogLevel()
{
    string logLevel = "Unknown";

    switch (this->logLevel)
    {
    case TestHarness::LogLevel::RESULT:
        logLevel = "RESULT";
        break;
    case TestHarness::LogLevel::INFO:
        logLevel = "INFO";
        break;
    case TestHarness::LogLevel::DEBUG:
        logLevel = "DEBUG";
        break;
    }

    return logLevel;
}

//Child threads
void TestHarness::getWork(int i)
{
	int myThread;

	myThread = i;
	while (true) {
		//enqueue i
		readyQueue.enQ(myThread);
		{
			std::lock_guard<std::mutex> l(iolock);
			cout << "Enqueued " << myThread << endl;
		}
		//wait on appropriate work entry
		while (work[myThread] < 0) Sleep(1);
		if (work[myThread] == 8) break;        // work for this thread is done

		//process
		bool testResult = false;

		//HINSTANCE hinstLib = LoadLibraryA(dlls[work[myThread]].c_str());
		HINSTANCE hinstLib = LoadLibraryA(dlls[myThread].c_str());
		
		if (hinstLib != NULL)
		{
			ITEST itest = (ITEST)GetProcAddress(hinstLib, "ITest");

			// If the function address is valid, call the function.
			if (NULL != itest)
			{
				testResult = this->execute(itest);
			}
			else {
				cout << "could not find ITest method in DLL " << dlls[i] << endl;
			}
			// Free the DLL module.
			FreeLibrary(hinstLib);
		}
		else {
			cout << "could not load DLL " << dlls[i] << endl;
		}
		{
			std::lock_guard<std::mutex> l(iolock);
			cout << "--------------------------------------------" << endl;
			cout << "Test " << std::to_string(testCount) << " Completed: Result -> " << (testResult == true ? "Pass" : "Fail") << endl << endl;
		}
		{
			std::lock_guard<std::mutex> mc(m_counts);
			testCount++;  // Update test counter
		}
		
		if (testResult == true) {
			std::lock_guard<std::mutex> mc(m_counts);
			passCount++; // Update passed test counter
		}
		else {
			std::lock_guard<std::mutex> mc(m_counts);
			failCount++; // Update failed test counter
		} 
		work[myThread] = -1;   // reset
	}
	{
		std::lock_guard<std::mutex> l(iolock);
		cout << "thread " << myThread << " is returning\n";
	}
	return;
}

// Process queues
//void testHarness::processQueues(int count)
void TestHarness::processQueues()
{
	int threadNum;
	string dll;

	//for (int i = 0; i <= count; i++) {
	while (true) {
		//find next test
		dll = testQueue.deQ();
		if (dll == "stop") break;             // all done
		threadNum = readyQueue.deQ();
		//set appropriate work value
		{
			std::lock_guard<std::mutex> l(iolock);
			cout << "processQueues: just dequeued thread " << threadNum << ", using dll: " << dll << endl;
		}
		dlls[threadNum] = dll;               // setup dll
		work[threadNum] = 1;                 // indicate work is available
	}
	{
		std::lock_guard<std::mutex> l(iolock);
		cout << "All done assigning work" <<  endl;
	}
	for (int i = 0; i < numChildren; i++) {
		while (work[i] >= 0) Sleep(1);               // make sure they are finished
		work[i] = 8;    // signal to all threads to stop
	}
}

// Begins Running all unit Test
void TestHarness::runUnitTests()
{
    cout << "Running Unit Tests (With a Log Level of " << this->getLogLevel() << ")" << "\n\n" << endl;

    //int count = -1;

	std::thread t1([=] {getWork(0); });
	std::thread t2([=] {getWork(1); });
	std::thread t3([=] {processQueues(); });

	pugi::xml_document doc;
	doc.load_file(file.c_str());
	
	for (pugi::xml_node child : doc.child("tests").children())
	{

		//count++;
		
		//use this to load the DLL
		std::string dll = child.attribute("library").value();
		testQueue.enQ(dll);
		//dlls[count] = dll;
	}
	testQueue.enQ("stop");
	//processQueues(count);
	
	t1.join();
	t2.join();
	t3.join();
	{
		std::lock_guard<std::mutex> l(iolock);
		cout << "FINISHED RUNNING ALL UNIT TESTS...\n";
		cout << "Pass: " << std::to_string(passCount) << " Fail: " << std::to_string(failCount) << "\n\n" << endl;
	}
}

// Runs Unit Tests and Return Test Result
bool TestHarness::execute(TestReturn(*unitTest)()) {
    bool testResult = false;

    try
    {

        // Capture Test Start Time
        SYSTEMTIME startTime, endTime;
        GetLocalTime(&startTime);

        // Run Unit Test
        TestReturn testReturn = unitTest();
		TestPredicate testPredicate(testReturn.result, testReturn.applicationSpecificMessages, testReturn.applicationState);


        // Capture Test End Time
        GetLocalTime(&endTime);
        testPredicate.setStartTime(startTime);
        testPredicate.setEndTime(endTime);

        // Save Test Result
        testResult = testPredicate.getResult();

        // Log Test Predicate Information
        this->logTestPredicate(testPredicate);
    }
    catch (exception & e)
    {
        cout << "Error Occured while running test: " << e.what() << endl;
    }

    return testResult;
}

// Logs Test Predicate Information, based on test harness' log level
void TestHarness::logTestPredicate(TestPredicate testPredicate)
{
    switch (this->logLevel)
    {
    case TestHarness::LogLevel::RESULT:
    {
        TestLogger testLogger(TestLogger::LogLevel::RESULT);
        testLogger.logTestPredicate(testPredicate);
        break;
    }
    case TestHarness::LogLevel::INFO:
    {
        TestLogger testLogger(TestLogger::LogLevel::INFO);
        testLogger.logTestPredicate(testPredicate);
        break;
    }
    case TestHarness::LogLevel::DEBUG:
    {
        TestLogger testLogger(TestLogger::LogLevel::DEBUG);
        testLogger.logTestPredicate(testPredicate);
        break;
    }
    }
}