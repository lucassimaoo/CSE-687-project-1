// Project1.cpp : This file contains the 'main' function. Program execution begins and ends there.
#include "../Message/Message.h"
#include "../MsgPassingComm/Comm.h"
#include "UnitTests.h"
#include <iostream>
#include "TestHarness.h"
#include "TestPredicate.h"

using namespace MsgPassingCommunication;
void getResults();

int main()
{

    // Create Test Harness (RESULT Log Level) and supply unit tests
    //TestHarness testHarness(TestHarness::LogLevel::DEBUG);
	TestHarness testHarness(TestHarness::LogLevel::RESULT);

	std::thread t = testHarness.server();
	std::thread t2(getResults);

	// leaving some time for the socket to start
	::Sleep(1000);

	Comm comm(EndPoint("localhost", 9891), "client1Comm");
	comm.start();
	
	EndPoint serverEP("localhost", 9890);

	Message msg1;
	msg1.to(serverEP);
	msg1.command("tests1.xml");
	msg1.name("client #1 : msg # 1");
	std::cout << "\n  " + comm.name() + " posting:  " << msg1.name();
	comm.postMessage(msg1);

	Message msg2;
	msg2.to(serverEP);
	msg2.command("tests2.xml");
	msg2.name("client #1 : msg # 2");
	std::cout << "\n  " + comm.name() + " posting:  " << msg2.name();
	comm.postMessage(msg2);

	Message msg3;
	msg3.to(serverEP);
	msg3.command("tests3.xml");
	msg3.name("client #1 : msg # 3");
	std::cout << "\n  " + comm.name() + " posting:  " << msg3.name();
	comm.postMessage(msg3);

	Message stop;
	stop.name("stop");
	stop.to(serverEP);
	stop.command("stop");
	comm.postMessage(stop);

	t.join();
	t2.join();

	comm.stop();
	return 0;
}

void getResults()
{
	Comm comm(EndPoint("localhost", 9881), "clientComm");
	comm.start();
	
	std::cout << "getResults started\n";
	Message msg;
	while (true)
	{
		msg = comm.getMessage();
		std::cout << " getResults received message: " + msg.command() << std::endl;

		if (msg.command() == "stop")
		{
			std::cout << "Received stop\n";
			break;
		}
		else
		{
			std::cout << msg.command() << std::endl;
		}
	}
	comm.stop();
}