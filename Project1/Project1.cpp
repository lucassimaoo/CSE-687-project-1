// Project1.cpp : This file contains the 'main' function. Program execution begins and ends there.
#include "../Message/Message.h"
#include "../MsgPassingComm/Comm.h"
#include "UnitTests.h"
#include <iostream>
#include "TestHarness.h"
#include "TestPredicate.h"

using namespace MsgPassingCommunication;

int main()
{

    // Create Test Harness (RESULT Log Level) and supply unit tests
    TestHarness testHarness(TestHarness::LogLevel::DEBUG);

	testHarness.server();

	// leaving some time for the socket to start
	::Sleep(1000);

	Comm comm(EndPoint("localhost", 9891), "client1Comm");
	comm.start();
	EndPoint serverEP("localhost", 9890);

	Message msg1;
	msg1.to(serverEP);
	msg1.command("tests.xml");
	msg1.name("client #1 : msg # 1");
	std::cout << "\n  " + comm.name() + " posting:  " << msg1.name();
	comm.postMessage(msg1);

	Message msg2;
	msg2.to(serverEP);
	msg2.command("tests.xml");
	msg2.name("client #1 : msg # 2");
	std::cout << "\n  " + comm.name() + " posting:  " << msg2.name();
	comm.postMessage(msg2);

	Message msg3;
	msg3.to(serverEP);
	msg3.command("tests.xml");
	msg3.name("client #1 : msg # 3");
	std::cout << "\n  " + comm.name() + " posting:  " << msg3.name();
	comm.postMessage(msg3);

	Message stop;
	stop.name("stop");
	stop.to(serverEP);
	stop.command("stop");
	comm.postMessage(stop);

	return 0;
}