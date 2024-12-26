#include "Manager.h"
Manager::Manager(int bpOrder) {
	bp = new BpTree(&fout, bpOrder);
	avl = new AVLTree();
}

Manager::~Manager() {
	delete bp;
	delete avl;
	fin.close();
	fout.close();
}

void Manager::run(const char* command_txt) {
	fin.open(command_txt);
	fout.open("log.txt");
	if (!fin)
	{
		fout << "Fail to open command file" << endl;
		exit(-1);
	}
	if (fin.peek() == ifstream::traits_type::eof()) // exit if command.txt is empty
		return;
	string command;
	while (getline(fin, command)) {
		if (command == "")
			continue;
		if (command == "LOAD") {
			if (LOAD()) {
				PrintSuccess("LOAD");
			}
			else {
				printErrorCode(100);
			}
		}
		else if (command == "VLOAD") {
			if (VLOAD()) {
				PrintSuccess("VLOAD");
			}
			else {
				printErrorCode(200);
			}
		}
		else if (command[0] == 'A') {
			string airline[4], air_info;
			istringstream temp(command); // store command in temp
			int i = 0;
			bool factor_over = false;
			while (getline(temp, air_info, '\t')) { // parse command by '\t'
				if (i >= 4) { // more than 4 arguments
					printErrorCode(300);
					factor_over = true;
					break;
				}
				if (air_info == "ADD") // skip command keyword
					continue;
				airline[i] = air_info;
				i++;
			}
			if (factor_over)
				continue; // if more than 4 arguments, output error code and move to next command
			if (i <= 3) // insufficient number of arguments
				printErrorCode(300);
			else { // exactly 4 arguments
				if (airline[0] == "JEJU" && airline[2] != "CJU") { // Jeju airline with a non-Jeju destination
					printErrorCode(300);
					continue;
				}

				if (!ADD(airline[0], airline[1], airline[2], airline[3])) {
					printErrorCode(300);
				}
			}
		}
		else if (command[0] == 'S') {
			bool factor_over = false;
			istringstream temp(command); // store command in temp
			string search;
			int i = 0;
			while (command[i] != '\t' && i < command.length()) {
				search += command[i];
				i++;
			}
			i = 0;
			if (search == "SEARCH_BP") {
				string fac[2];
				string factor;
				while (getline(temp, factor, '\t')) { // parse command by '\t'
					if (i >= 2) {
						printErrorCode(400);
						factor_over = true;
						break;
					}
					if (factor == "SEARCH_BP")
						continue;
					fac[i] = factor;
					i++;
				}
				if (factor_over) // more than 2 arguments
					continue;
				if (i == 0) // zero arguments
					printErrorCode(400);
				else if (i == 1) { // one argument -> output search result for that specific flight
					if (!SEARCH_BP(fac[0])) { // if the information does not exist in the B+ tree or B+ tree is empty
						printErrorCode(400);
					}
				}
				else { // two arguments -> output results for all flights between the two arguments
					if (!SEARCH_BP(fac[0], fac[1])) { // if the information does not exist in the B+ tree or B+ tree is empty
						printErrorCode(400);
					}
				}
			}
			else if (search == "SEARCH_AVL") {
				string factor;
				while (getline(temp, factor, '\t')) { // parse command by '\t'
					if (i >= 2) {
						printErrorCode(500);
						factor_over = true;
						break;
					}
					if (factor == "SEARCH_AVL") // skip command keyword
						continue;
					i++;
				}
				if (factor_over) // more than 1 argument
					continue;
				if (i == 0) // zero arguments
					printErrorCode(500);
				else {
					if (!SEARCH_AVL(factor))
						printErrorCode(500);
				}
			}
		}
		else if (command[0] == 'V') {
			int i = 0;
			string Vprint;
			while (i < command.length() && command[i] != '\t') { // parse until '\t' or end of string
				Vprint += command[i];
				i++;
			}
			if (Vprint == "VPRINT") { // if command is VPRINT
				bool factor_over = false;
				istringstream temp(command); // store command in temp
				string cond; // condition for printing Vector
				i = 0;
				while (getline(temp, cond, '\t')) { // parse command by '\t'
					if (i >= 2) {
						printErrorCode(600);
						factor_over = true;
						break;
					}
					if (cond == "VPRINT") // skip command keyword
						continue;
					i++;
				}
				if (factor_over) // more than 1 argument
					continue;
				if (i == 0) // zero arguments
					printErrorCode(600);
				else {
					if (!VPRINT(cond))
						printErrorCode(600);
				}
			}
			else // invalid command
				printErrorCode(800);
		}
		else if (command == "PRINT_BP") {
			if (bp->getRoot() == nullptr)
				printErrorCode(700);
			else {
				fout << "======== PRINT_BP ========\n";
				bp->Print(fout);
				fout << "=======================\n\n";
			}
		}
		else if (command == "EXIT") {
			PrintSuccess("EXIT");
			return;
		}
		else // invalid command
			printErrorCode(800);
	}
}

bool Manager::LOAD() {
	ifstream fdata;

	fdata.open("input_data.txt");
	if (fdata.is_open() && bp->getRoot() == nullptr) { // File opens properly and B+ tree is empty
		if (fdata.peek() == ifstream::traits_type::eof()) { // If the first character of the file is eof, the file is empty
			return false;
		}
		string AirlineName // Airline name
			, FlightNumber // Flight number
			, Destination // Destination
			, Status; // State
		int Seats; // Number of seats
		while (fdata >> AirlineName >> FlightNumber >> Destination >> Seats >> Status) { // Read information from file, create FlightData object, and insert into B+ tree
			FlightData* F_data = new FlightData; // Create a new object
			F_data->SetAirlineName(AirlineName);
			F_data->SetFlightNumber(FlightNumber);
			F_data->SetDestination(Destination);
			F_data->SetNumberofSeats(Seats);
			F_data->SetStatus(Status);
			bp->Insert(F_data);
		}
		return true;
	}
	else {
		return false;
	}
}

bool Manager::VLOAD() {
	if (avl->GetRoot() == nullptr) // If AVL tree is empty, print error
		return false;
	avl->GetVector(Print_vector);
	return true;
}

bool Manager::ADD(string AirlineName, string FlightNumber, string Destination, string Status) {
	if (bp->SearchModel(FlightNumber)) { // If it exists in the B+ tree
		bool flag = true;
		bp->add_bp(AirlineName, FlightNumber, Destination, Status, fout, flag, avl);
		return flag;
	}
	else { // If it does not exist in the B+ tree
		FlightData* F_data = new FlightData;
		F_data->SetAirlineName(AirlineName);
		F_data->SetFlightNumber(FlightNumber);
		F_data->SetDestination(Destination);
		F_data->SetStatus(Status);
		fout << "=========== ADD ===========\n";
		fout << FlightNumber << " | " << AirlineName << " | " << Destination << " | ";
		if (AirlineName == "KoreanAir" || AirlineName == "ASIANA") {
			F_data->SetNumberofSeats(7);
			fout << "7 | ";
		}
		else if (AirlineName == "JEJU" || AirlineName == "JeanAir") {
			F_data->SetNumberofSeats(5);
			fout << "5 | ";
		}
		else {
			F_data->SetNumberofSeats(6);
			fout << "6 | ";
		}
		fout << Status << endl;
		fout << "===========================\n\n";
		bp->Insert(F_data);
		return true;
	}
}

bool Manager::SEARCH_BP(string name) {
	if (bp->getRoot() == nullptr) // If B+ tree is empty
		return false;
	if (bp->SearchModel(name)) { // If it exists in the B+ tree
		BpTreeNode* node = bp->searchDataNode(name);
		map <string, FlightData*>::iterator iter; // Iterator pointing to the element with the same flight name as 'name'
		for (iter = node->getDataMap()->begin(); iter != node->getDataMap()->end(); iter++) { // Traverse elements in map stored in node
			if (iter->second->GetFlightNumber() == name) // If the flight name matches 'model_name'
				break;
		}
		fout << "======== SEARCH_BP ========\n";
		fout << iter->second->GetFlightNumber() << " | " << iter->second->GetAirlineName() << " | " << iter->second->GetDestination() << " | " << iter->second->GetNumberofSeats() << " | " << iter->second->GetStatus() << endl;
		fout << "========================\n\n";
		return true;
	}
	else { // If it does not exist in the B+ tree
		return false;
	}
}

bool Manager::SEARCH_BP(string start, string end) {
	if (bp->getRoot() == nullptr) // If B+ tree is empty
		return false;
	return bp->SearchRange(start, end, fout);
}

bool Manager::SEARCH_AVL(string name) {
	if (avl->GetRoot() == nullptr) // If AVL tree is empty
		return false;
	FlightData* node = avl->Search(name); // Ticket with the same flight as 'name'
	if (node == nullptr) // If not found
		return false;
	fout << "======== SEARCH_AVL ========\n";
	fout << node->GetFlightNumber() << " | " << node->GetAirlineName() << " | " << node->GetDestination() << " | " << node->GetNumberofSeats() << " | " << node->GetStatus() << endl;
	fout << "===========================\n\n";
	return true;
}

bool Compare_A(FlightData* flight1, FlightData* flight2) {
	if (flight1->GetAirlineName() == flight2->GetAirlineName()) { // If airline names are the same
		if (flight1->GetDestination() == flight2->GetDestination()) { // If destination names are also the same
			return flight1->GetStatus() > flight2->GetStatus(); // Sort by status in descending order
		}
		else
			return flight1->GetDestination() < flight2->GetDestination(); // Sort by destination name in ascending order
	}
	else
		return flight1->GetAirlineName() < flight2->GetAirlineName(); // Sort by airline name in ascending order
}

bool Compare_B(FlightData* flight1, FlightData* flight2) {
	if (flight1->GetDestination() == flight2->GetDestination()) { // If destination names are the same
		if (flight1->GetStatus() == flight2->GetStatus()) { // If status information is also the same
			return flight1->GetAirlineName() > flight2->GetAirlineName(); // Sort by airline name in descending order
		}
		else
			return flight1->GetStatus() < flight2->GetStatus(); // Sort by status information in ascending order
	}
	else
		return flight1->GetDestination() < flight2->GetDestination(); // Sort by destination name in ascending order
}

bool Manager::VPRINT(string type_) {
	if (avl->GetRoot() == nullptr) // If AVL tree is empty
		return false;
	if (Print_vector.size() == 0) // If Print vector is empty
		return false;
	vector<FlightData*>::iterator iter;
	if (type_ == "A") { // Print vector with condition A
		sort(Print_vector.begin(), Print_vector.end(), Compare_A); // Sort vector by condition A
		fout << "========= VPRINT A =========\n";
	}
	else if (type_ == "B") { // Print vector with condition B
		sort(Print_vector.begin(), Print_vector.end(), Compare_B); // Sort vector by condition B
		fout << "========= VPRINT B =========\n";
	}
	else // Invalid argument
		return false;

	for (iter = Print_vector.begin(); iter != Print_vector.end(); iter++) {
		fout << (*iter)->GetAirlineName() << " | " << (*iter)->GetFlightNumber() << " | " << (*iter)->GetDestination() << " | " << (*iter)->GetStatus() << endl; // Output elements of sorted vector
	}
	fout << "===========================\n\n";
	return true;
}

void Manager::printErrorCode(int n) { // Error
	fout << "========== ERROR ==========" << endl;
	fout << n << endl;
	fout << "===========================" << endl << endl;
}

void Manager::PrintSuccess(string command) { // Success
	fout << "=========== " << command << " ===========\n";
	fout << "Success\n";
	fout << "============================\n\n";
}

