// The main.cpp you can use for testing will replace this file soon.

#include "provided.h"
#include "MyMap.h"
#include <iostream>
#include <string>
using namespace std;

void foo()
{
	MyMap<string,double> nameToGPA;  // maps student name to GPA
	// add new items to the binary search tree-based map
	nameToGPA.associate("Carey", 3.5);  // Carey has a 3.5 GPA
	nameToGPA.associate("David", 3.99); // David beat Carey
	nameToGPA.associate("Abe", 3.2);

	// Abe has a 3.2 GPA
	double* davidsGPA = nameToGPA.find("David");
	if (davidsGPA != nullptr)
		*davidsGPA = 1.5;
	// after a re-grade of David’s exam
	nameToGPA.associate("Carey", 4.0);
	// Carey deserves a 4.0
	// replaces old 3.5 GPA
	double* lindasGPA = nameToGPA.find("Linda");
	if (lindasGPA == nullptr)
		cout << "Linda is not in the roster!" << endl;
	else
		cout << "Linda’s GPA is: " << *lindasGPA << endl;
}


int main()
{
	foo();
}

