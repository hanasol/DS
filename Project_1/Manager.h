#pragma once
#include "SubtitleQueue.h"
#include "SubtitleBST.h"
#include "SectionList.h"
#include <fstream>
using namespace std;

class Manager
{
private:
	ifstream fcmd; // Input file for reading commands
	ofstream flog; // Output file for logging
	SubtitleQueue que; // Queue to manage subtitles
	SubtitleBST bst; // Binary Search Tree for subtitle management
	SectionList list; // List to manage sections

public:
	Manager();
	~Manager();

	void Run(const char* command); // Function to execute the program
	void PrintSuccess(const char* cmd); // Function to log successful command execution
	void PrintErrorCode(int num); // Function to output an error message

	// LOAD
	void load(); // LOAD function to load subtitles from a file
	// QPOP
	void qpop(); // QPOP function to remove a subtitle from the queue
	// PRINT
	void print(string num); // PRINT function to print subtitles for a specific section
	// SECTION
	void section(int num, string first_time, string second_time); // SECTION function to manage subtitles within a time range
	// DELETE
	void delete_equal(string time); // DELETE_EQUAL function to delete a specific subtitle by time
	void delete_under(string time); // DELETE_UNDER function to delete subtitles under a specific time
	// OTHERS
	bool isNum(string num); // Function to check if a given string is a valid number
	bool isCont_time(string num); // Function to check if a given string follows the correct subtitle time format
};
