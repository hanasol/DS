#include "Manager.h"
#include <iostream>

int main()
{
	Manager manager(3); // B+ tree has order of 3
	manager.run("command.txt");
	return 0;
}

