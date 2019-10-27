#pragma once

#ifdef LIBRARY1_EXPORTS
#define LIBRARY1_API __declspec(dllexport)
#else
#define LIBRARY1_API __declspec(dllimport)
#endif

struct TestReturn {
	bool result;
	std::vector<std::string> applicationSpecificMessages;
	std::map<std::string, std::string> applicationState;
};

extern "C" LIBRARY1_API TestReturn ITest();

