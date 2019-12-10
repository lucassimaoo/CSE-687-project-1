#include "../Project1/TestHarness.h"
#include "TestHarness.h"

//CLI::TestHarness::TestHarness():_impl(new ::TestHarness(::TestHarness::LogLevel logLevel)) {
CLI::TestHarness::TestHarness(::TestHarness::LogLevel logLevel){
	_impl = new ::TestHarness(logLevel);
}

void CLI::TestHarness::Destroy() {
	if (_impl != nullptr)
	{
		delete _impl;
		_impl = nullptr;
	}
}

CLI::TestHarness::~TestHarness() {
	Destroy();
}

CLI::TestHarness::!TestHarness() {
	Destroy(); // Clean-up any native resources 
}

void CLI::TestHarness::runUnitTests(std::string file) {
	_impl->runUnitTests(file);
}

std::thread CLI::TestHarness::server() {
	return _impl->server();
}

void CLI::TestHarness::serverSocket() {
	_impl->serverSocket();
}


