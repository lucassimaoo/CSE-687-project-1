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

// Begins Running all unit Test
void TestHarness::runUnitTests(std::string file)
{
    cout << "Running Unit Tests (With a Log Level of " << this->getLogLevel() << ")" << "\n\n" << endl;

    int testCounter = 1;
	int failCounter = 0;
	int passCounter = 0;

	pugi::xml_document doc;
	doc.load_file(file.c_str());
	
	for (pugi::xml_node child : doc.child("tests").children())
	{

		bool testResult = false;

		cout << "Running Test " << std::to_string(testCounter) << "..." << endl;
		cout << "--------------------------------------------" << endl;
		cout << "Test Details: " << endl;

		//use this to load the DLL
		std::string dll = child.attribute("library").value();

		HINSTANCE hinstLib = LoadLibraryA(dll.c_str());

		if (hinstLib != NULL)
		{
			ITEST itest = (ITEST) GetProcAddress(hinstLib, "ITest");

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

        cout << "--------------------------------------------" << endl;
        cout << "Test " << std::to_string(testCounter) << " Completed: Result -> " << (testResult == true ? "Pass" : "Fail") << endl << endl;
        testCounter++; // Update Test Counter

		if (testResult == true) {
			passCounter++; // Update passed test counter
		}
		else {
			failCounter++; // Update failed test counter
		}
    }

	this->failCount = failCounter;
	this->passCount = passCounter;

    cout << "FINISHED RUNNING ALL UNIT TESTS..." << endl;
	cout << "Pass: " << std::to_string(passCount) << " Fail: " << std::to_string(failCount) << "\n\n" << endl;
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
			break;
		}
		else 
		{
			runUnitTests(msg.command());
		}
	}
	comm.stop();
}

void TestHarness::server() {
	std::thread t1([=] {serverSocket();});
	t1.detach();
}