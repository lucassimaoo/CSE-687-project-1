#pragma once

#ifdef LIBRARY2_EXPORTS
#define LIBRARY2_API __declspec(dllexport)
#else
#define LIBRARY2_API __declspec(dllimport)
#endif

struct TestReturn {
	bool result;
	std::vector<std::string> applicationSpecificMessages;
	std::map<std::string, std::string> applicationState;
};

extern "C" LIBRARY2_API TestReturn ITest();