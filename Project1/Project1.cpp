// Project1.cpp : This file contains the 'main' function. Program execution begins and ends there.
#include "../Message/Message.h"
#include "../MsgPassingComm/Comm.h"
#include "UnitTests.h"
#include <iostream>
#include "TestHarness.h"
#include "TestPredicate.h"
#include "pugixml.hpp"

using namespace MsgPassingCommunication;
using namespace pugi;

struct xml_string_writer : pugi::xml_writer
{
	std::string result;

	virtual void write(const void* data, size_t size)
	{
		result.append(static_cast<const char*>(data), size);
	}
};

int main()
{

	std::vector<std::string> dlls;
	dlls.push_back("..\\Library1\\Debug\\Library1.dll");
	dlls.push_back("..\\Library2\\Debug\\Library2.dll");
	dlls.push_back("..\\Library3\\Debug\\Library3.dll");

	pugi::xml_document doc;
	pugi::xml_node tests = doc.append_child("tests");
	xml_string_writer writer;

	for (const auto& dll : dlls) {
		tests.append_child("driver").append_attribute("library").set_value(dll.c_str());
	}

	doc.save(writer, nullptr, pugi::format_raw);

    // Create Test Harness (RESULT Log Level) and supply unit tests
    TestHarness testHarness(TestHarness::LogLevel::DEBUG);

	std::thread t = testHarness.server();

	// leaving some time for the socket to start
	::Sleep(1000);

	Comm comm(EndPoint("localhost", 9891), "client1Comm");
	comm.start();
	EndPoint serverEP("localhost", 9890);

	Message msg1;
	msg1.to(serverEP);
	msg1.command(writer.result);
	msg1.name("client #1 : msg # 1");
	std::cout << "\n  " + comm.name() + " posting:  " << msg1.name();
	comm.postMessage(msg1);

	/*Message msg2;
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
	comm.postMessage(msg3);*/

	Message stop;
	stop.name("stop");
	stop.to(serverEP);
	stop.command("stop");
	comm.postMessage(stop);

	comm.stop();

	t.join();

	return 0;
}