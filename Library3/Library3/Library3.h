#pragma once

#ifdef LIBRARY3_EXPORTS
#define LIBRARY3_API __declspec(dllexport)
#else
#define LIBRARY3_API __declspec(dllimport)
#endif

struct TestReturn {
	bool result;
	std::vector<std::string> applicationSpecificMessages;
	std::map<std::string, std::string> applicationState;
};

extern "C" LIBRARY3_API TestReturn ITest();