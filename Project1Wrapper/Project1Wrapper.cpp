#pragma once

#include <vector>

#include "../Message/Message.h"
#include "../MsgPassingComm/Comm.h"
#include "../Project1/TestHarness.h"
#include "Project1Wrapper.h"
#include "TestHarness.h"


void Project1Wrapper::sendMessages(std::vector<std::string> paths, MsgPassingCommunication::EndPoint serverEP) {

	MsgPassingCommunication::Comm comm(MsgPassingCommunication::EndPoint("localhost", 9891), "client1Comm");
	comm.start();

	for (int i = 0; i < paths.size; i++) {
		MsgPassingCommunication::Message msg;
		msg.to(serverEP);
		msg.command("tests2.xml");
		msg.name("client #1 : msg # 2");
		//std::cout << "\n  " + comm.name() + " posting:  " << msg2.name();
		comm.postMessage(msg);
	}

	comm.stop();
}

void Project1Wrapper::main(std::vector<std::string> paths) {
	// Create Test Harness (RESULT Log Level) and supply unit tests
	CLI::TestHarness testHarness(::TestHarness::LogLevel::DEBUG);

	std::thread t = testHarness.server();

	// leaving some time for the socket to start
	::Sleep(1000);

	MsgPassingCommunication::EndPoint serverEP("localhost", 9890);

	sendMessages(paths, serverEP);

	t.join();
}
