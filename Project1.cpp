// Project1.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include "MsgPassingComm/Comm.h"
#include "UnitTests.h"
#include <iostream>
#include "TestHarness.h"
#include "TestPredicate.h"
#include <exception>
#include <iostream>

using namespace MsgPassingCommunication;
using namespace Sockets;
using std::cout;
using std::endl;
using std::exception;

int main()
{
    try {

        // Create Test Harness (RESULT Log Level) and supply unit tests
        TestHarness testHarness(TestHarness::LogLevel::RESULT, "tests.xml");

        testHarness.startServer();

        // leaving some time for the socket to start
        ::Sleep(1000);

        Comm comm(EndPoint("localhost", 9891), "client1Comm");
        comm.start();
        EndPoint serverEP("localhost", 9890);
        Message msg1;
        msg1.to(serverEP);
        msg1.command("tests.xml");
        msg1.name("Client #1");
        std::cout << "\n " + comm.name() + " posting:  " << msg1.name();
        comm.postMessage(msg1);

        ::Sleep(2000);

        // Run Unit Tests
        testHarness.runUnitTests("tests.xml");

        // Create Test Harness (INFO Log Level) and supply unit tests
        TestHarness testHarness2(TestHarness::LogLevel::INFO, "tests.xml");

        // Run Unit Tests
        testHarness2.runUnitTests("tests.xml");

        // Create Test Harness (DEBUG Log Level) and supply unit tests
        TestHarness testHarness3(TestHarness::LogLevel::DEBUG, "tests.xml");

        // Run Unit Tests
        testHarness3.runUnitTests("tests.xml");

        Message stop;
        stop.name("quit");
        stop.to(serverEP);
        stop.command("quit");
        comm.postMessage(stop);
        ::Sleep(2000);

        comm.stop();
    }
    catch (exception & e)
    {
        cout << "Error Occured while running tests: " << e.what() << endl;
    }
}


