// Project1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <chrono>
#include <ctime>

#include "Project1.h"

using namespace std;

bool TestHarness::execute()
{
	std::cout << "execute method called" << endl;
	for (int i = 0; i < (int)myTests.size(); i++) {
		if (!(*myTests[i])()) cout << "Test Failed " << endl;
		else cout << "Test Passed" << endl;
	}
	return true;
}

void TestHarness::addTest(theTest* newTest)
{
	myTests.push_back(newTest);
	std::cout << "Just pushed back a test " << endl;
}

class test1 : public theTest {
private:
	int a, b, c;

public:
	test1(int x, int y, int z) : a(x), b(y), c(z) {}
	bool operator()()
	{
		if ((a + b) > c) return true;
		return false;
	}
};

class test2 : public theTest {
private:
	int a, b;

public:
	test2(int x, int y) : a(x), b(y) {}
	bool operator()()
	{
		if (a > b) return true;
		return false;
	}
};

int main()
{
	time_t seconds;
	time(&seconds);

    std::cout << "Hello World!\n";
	cout << "time is " << seconds << endl;
	
	class TestHarness th;
	th.addTest(new test1(3, 4, 5));
	th.addTest(new test2(6, 7));
	th.execute();
		
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
