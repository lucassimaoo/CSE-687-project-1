/*
Name: Derrick Ward
Class/Section: CSE 687
Project: 1
Description: This file is the implementation for the Test Harness interface
Date: 10/15/2019
*/

#include "../Message/Message.h"
//#include "../Cpp11-BlockingQueue/Cpp11-BlockingQueue.h"
#include "../Sockets/Sockets.h"
#include "../MsgPassingComm/Comm.h"
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

using namespace MsgPassingCommunication;
using namespace Sockets;
using namespace pugi;
using std::cout;
using std::endl;
using std::exception;


typedef TestReturn(__cdecl* ITEST)();

TestHarness::TestHarness(TestHarness::LogLevel logLevel)
{
    this->logLevel = logLevel;
    this->passCount = 0;
    this->failCount = 0;
	this->testCount = 0;
	for (int i = 0; i < 2; i++) running[i] = true;  // initialize
	for (int i = 0; i < 2; i++) work[i] = -1;    // initialize
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
// Process queues
//void testHarness::processQueues(int count)
void TestHarness::processQueues()
{
	int threadNum;
	string file;

	//for (int i = 0; i <= count; i++) {
	while (true) {
		//find next test
		file = testQueue.deQ();
		
		if (file == "stop") break;             // all done
		threadNum = readyQueue.deQ();
		files[threadNum] = file;               // setup file
		work[threadNum] = 1;                 // indicate work is available
	}
	{
		std::lock_guard<std::mutex> l(iolock);
		cout << "All done assigning work" << endl;
	}
	for (int i = 0; i < 2; i++) {
		while (work[i] >= 0) Sleep(1);               // make sure they are finished
		work[i] = 8;    // signal to all threads to stop
	}
}

// Begins Running all unit Test
void TestHarness::runUnitTests(int myThread)
{
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

		
		pugi::xml_document doc;
		doc.load_file(files[myThread].c_str());
		
		for (pugi::xml_node child : doc.child("tests").children())
		{

			bool testResult = false;
			{
				std::lock_guard<std::mutex> l(iolock);
				cout << "Running Test " << ++testCount << endl;
				cout << "--------------------------------------------" << endl;
				cout << "Test Details: " << endl;
			}

			//use this to load the DLL
			std::string dll = child.attribute("library").value();

			HINSTANCE hinstLib = LoadLibraryA(dll.c_str());

			if (hinstLib != NULL)
			{
				ITEST itest = (ITEST)GetProcAddress(hinstLib, "ITest");

				// If the function address is valid, call the function.
				if (NULL != itest)
				{
					testResult = this->execute(itest);
				}
				else {
					cout << "could not find ITest method in DLL " << dll << endl;
				}
				// Free the DLL module.
				FreeLibrary(hinstLib);
			}
			else {
				cout << "could not load DLL " << dll << endl;
			}

			if (testResult == true) {
				std::lock_guard<std::mutex> mc(m_counts);
				passCount++; // Update passed test counter
			}
			else {
				std::lock_guard<std::mutex> mc(m_counts);
				failCount++; // Update failed test counter
			}
		}    // end for loop
		work[myThread] = -1;                // reset to indicate ready for more work
	}    // end while(true)
	{
		std::lock_guard<std::mutex> l(iolock);
		cout << "thread " << myThread << " is finished\n";
	}
	running[myThread] = false;
}

// Runs Unit Tests and Return Test Result
bool TestHarness::execute(TestReturn(*unitTest)()) {
    bool testResult = false;

    try
    {

        // Capture Test Start Time
        SYSTEMTIME startTime, endTime;
        GetLocalTime(&startTime);
		Sleep(2);

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

void TestHarness::printStats()
{
	cout << "FINISHED RUNNING ALL UNIT TESTS..." << endl;
	cout << "Tests Run: " << std::to_string(testCount) << endl;
	cout << "Pass: " << std::to_string(passCount) << " Fail: " << std::to_string(failCount) << "\n\n" << endl;
}

void TestHarness::serverSocket() {
	SocketSystem ss;

	//starting the server
	EndPoint serverEP("localhost", 9890);
	Comm comm(serverEP, "serverComm");
	comm.start();

	Message msg;
	while (true)
	{
		msg = comm.getMessage();
		std::cout << "\n  " + comm.name() + " received message: " << msg.command() << endl;
		
		if (msg.command() == "stop")
		{
			testQueue.enQ(msg.command());
			break;
		}
		else 
		{
			//runUnitTests(msg.command());
			testQueue.enQ(msg.command());
		}
	}
	comm.stop();
		
}

void TestHarness::server() {
	std::thread t1([=] {serverSocket();});
	std::thread t2([=] {runUnitTests(0); });
	std::thread t3([=] {runUnitTests(1); });
	std::thread t4([=] {processQueues(); });

	t1.detach();
	t2.detach();
	t3.detach();
	t4.detach();
}