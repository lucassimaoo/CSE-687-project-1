/*
Name: Derrick Ward
Class/Section: CSE 687
Project: 1
Description: This file is the implementation for the Test Harness interface
Date: 10/15/2019
*/

#include "../Message/Message.h"
#include "../Cpp11-BlockingQueue/Cpp11-BlockingQueue.h"
#include "../Sockets/Sockets.h"
#include "../MsgPassingComm/Comm.h"
#include "ThreadPool.h"
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
using std::exception;


typedef TestReturn(__cdecl* ITEST)();

TestLogger testLogger = TestLogger(TestLogger::LogLevel::RESULT);

TestHarness::TestHarness(TestHarness::LogLevel logLevel)
{
    this->logLevel = logLevel;
    this->passCount = 0;
    this->failCount = 0;

	switch (this->logLevel)
	{
	case TestHarness::LogLevel::RESULT:
	{
		testLogger = TestLogger(TestLogger::LogLevel::RESULT);
		break;
	}
	case TestHarness::LogLevel::INFO:
	{
		testLogger = TestLogger(TestLogger::LogLevel::INFO);
		break;
	}
	case TestHarness::LogLevel::DEBUG:
	{
		testLogger = TestLogger(TestLogger::LogLevel::DEBUG);
		break;
	}
	}

	testLogger.log("Starting Test Harness...");
}

// Desconstuctor
TestHarness::~TestHarness()
{
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

// Begins Running all unit Test
void TestHarness::runUnitTests(std::string file)
{
	testLogger.log("Running Unit Tests (With a Log Level of " + this->getLogLevel() + ")");

	pugi::xml_document doc;
	doc.load_string(file.c_str());
	
	for (pugi::xml_node child : doc.child("tests").children())
	{

		bool testResult = false;

		int testId = testCounter++;

		testLogger.log("--------------------------------------------");
		testLogger.log("Running Test " + std::to_string(testId) + "...");

		//use this to load the DLL
		std::string dll = child.attribute("library").value();

		HINSTANCE hinstLib = LoadLibraryA(dll.c_str());

		if (hinstLib != NULL)
		{
			ITEST itest = (ITEST) GetProcAddress(hinstLib, "ITest");

			// If the function address is valid, call the function.
			if (NULL != itest)
			{
				testResult = this->execute(itest, testId);
			}
			else {
				testLogger.log("could not find ITest method in DLL " + dll);
			}
			// Free the DLL module.
			FreeLibrary(hinstLib);
		}
		else {
			testLogger.log("could not load DLL " + dll);
		}

		testLogger.log("--------------------------------------------");
		testLogger.log("Test " + std::to_string(testId) + " Completed: Result -> " + (testResult == true ? "Pass" : "Fail"));

		if (testResult == true) {
			passCount++; // Update passed test counter
		}
		else {
			failCount++; // Update failed test counter
		}
    }

	testLogger.log("FINISHED RUNNING ALL UNIT TESTS...");
	testLogger.log("Pass: " + std::to_string(passCount) + " Fail: " + std::to_string(failCount) + "\n\n");
}

// Runs Unit Tests and Return Test Result
bool TestHarness::execute(TestReturn(*unitTest)(), int testId) {
    bool testResult = false;

    try
    {

        // Capture Test Start Time
        SYSTEMTIME startTime, endTime;
        GetLocalTime(&startTime);

        // Run Unit Test
        TestReturn testReturn = unitTest();
		TestPredicate testPredicate(testReturn.result, testReturn.applicationSpecificMessages, testReturn.applicationState, testId);


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
		std::string s = e.what();
		testLogger.log("Error Occured while running test: " + s);
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
		testLogger.log( comm.name() + " received message: " + msg.command());
		
		if (msg.command() == "stop")
		{
			// waiting on thread pool threads to finish
			ThreadPool<2>::CallObj exit = []() ->bool { return false; };
			trpl.workItem(exit);
			trpl.wait();
			break;
		}
		else 
		{
			ThreadPool<2>::CallObj co = [this, msg=msg.command()]() ->bool {
				runUnitTests(msg);
				return true;
			};

			trpl.workItem(co);
		}
	}
	comm.stop();
}

std::thread TestHarness::server() {
	std::thread t1([=] {serverSocket();});
	return t1;
}