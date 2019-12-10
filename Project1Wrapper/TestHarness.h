#pragma once

#include <atomic>
#include <string>
#include "../Project1/ThreadPool.h"

//using namespace System;

class TestHarness;

namespace CLI {
	public ref class TestHarness {
	public:
		TestHarness(::TestHarness::LogLevel logLevel);
		~TestHarness();
		!TestHarness();
		void runUnitTests(std::string file);
		std::thread server();
		void serverSocket();

		void Destroy(); // Helper function
	private:
		// Pointer to our implementation
		::TestHarness* _impl;
	};
}





