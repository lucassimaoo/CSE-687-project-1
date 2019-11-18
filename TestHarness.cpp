/*
Name: Derrick Ward
Class/Section: CSE 687
Project: 1
Description: This file is the implementation for the Test Harness interface
Date: 10/15/2019
*/

#include "Message/Message.h"
// #include "Cpp11-BlockingQueue/Cpp11-BlockingQueue.h"
#include "MsgPassingComm/Comm.h"
// #include "Sockets/Sockets.h"

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
using namespace MsgPassingCommunication;
using namespace Sockets;

typedef TestReturn(__cdecl* ITEST)();

TestHarness::TestHarness(TestHarness::LogLevel logLevel, std::string file)
{
    this->logLevel = logLevel;
    this->file = file;
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
void TestHarness::runUnitTests(string fileName)
{
    cout << "Running Unit Tests (With a Log Level of " << this->getLogLevel() << ")" << "\n\n" << endl;

    int testCounter = 1;
	int failCounter = 0;
	int passCounter = 0;

	pugi::xml_document doc;
	doc.load_file(fileName.c_str());
	
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

// Start Processing Network Messages
void TestHarness::startProcessingNetworkMessages() {
    // Start Socket System
    SocketSystem ss;

    // Set Host and Port for listening
    EndPoint endPoint("localhost", 9890);

    // Bind to Endpoint
    Comm communicationPoint(endPoint, "TestHarnessCommunicationPoint");

    // Start listening for Messages on the communication point
    communicationPoint.start();

    // Create Logger
    TestLogger testLogger(TestLogger::LogLevel::INFO);

    Message message;

    while (true)
    {
        message = communicationPoint.getMessage();
        if (message.command() == "quit")
        {
            communicationPoint.stop();
        }
        else
        {
            string command = message.command();
            testLogger.logMessage("\n" + communicationPoint.name() + " recevied message: " + command);
            this->runUnitTests(command);
        }
    }

    communicationPoint.stop();
}

// Starts Test Harness Server
void TestHarness::startServer() {
    // Create Thread
    std::thread thread([=] {startProcessingNetworkMessages();});

    //Start Thread
    thread.detach();    
}