#include "Manager.h"

Manager::Manager()
{

}
Manager::~Manager()
{

}

void Manager::Run(const char* command)
{
    // Open command & log file
    fcmd.open(command); // ifstream, command(input) file
    flog.open("log.txt"); // ofstream, log(output) file
    if (!fcmd)
    {
        flog << "Fail to open command file" << endl;
        exit(-1);
    }

    // Read and Run command
    string temp; // command
    while (getline(fcmd, temp)) {
        if (temp == "LOAD") { // LOAD command
            load();
        }
        else if (temp == "QPOP") // QPOP command
            qpop();
        else if (temp[0] == 'P') {
            if (temp == "PRINT") { // Print bst
                print("");
            }
            else { // Print list or error
                string temp_print;
                for (int i = 0; i < 5; i++) { // Parse up to index 0~4
                    if (i > temp.length()) // If 'i' is greater than the string length, it's an invalid command
                        break;
                    temp_print += temp[i];
                }
                if (temp_print == "PRINT") { // If the command is PRINT
                    string num;
                    for (int i = 6; i < temp.length(); i++)
                        num += temp[i]; // Store the argument after PRINT
                    bool flag = isNum(num); // Check if it's a number
                    if (flag) // If it's a number
                        print(num);
                    else // If it's not a number, it's an invalid command
                        PrintErrorCode(300);
                }
                else
                    PrintErrorCode(300); // Otherwise, it's an invalid command
            }
        }
        else if (temp[0] == 'D') { // DELETE command
            string temp_delete_1, temp_delete_2;
            for (int i = 0; i < 6; i++) { // Parse up to index 0~5
                if (i > temp.length()) // If 'i' is greater than the string length, it's an invalid command
                    break;
                temp_delete_1 += temp[i];
            }
            if (temp_delete_1 == "DELETE") { // If the command is DELETE
                for (int i = 7; i < 12; i++) {
                    if (i > temp.length()) // If 'i' is greater than the string length, it's an invalid command
                        break;
                    temp_delete_2 += temp[i];
                }
                int i = 13;
                string time;
                if (temp_delete_2 == "EQUAL") { // If the argument after DELETE is EQUAL
                    while (i < temp.length()) {
                        time += temp[i];
                        i++;
                    } // Parse until the end of the string
                    if (isCont_time(time)) // If the time format matches the subtitle time
                        delete_equal(time);
                    else // Otherwise, it's an invalid command
                        PrintErrorCode(500);
                }
                else if (temp_delete_2 == "UNDER") { // If the argument after DELETE is UNDER
                    while (i < temp.length()) {
                        time += temp[i];
                        i++;
                    } // Parse until the end of the string
                    if (isCont_time(time)) // If the time format matches the subtitle time
                        delete_under(time);
                    else // Otherwise, it's an invalid command
                        PrintErrorCode(500);
                }
                else // Otherwise, it's an invalid command
                    PrintErrorCode(500);
            }
            else // Otherwise, it's an invalid command
                PrintErrorCode(500);
        }
        else if (temp[0] == 'S') { // SECTION command
            string temp_section;
            for (int i = 0; i < 7; i++) { // Parse up to index 0~6
                if (i > temp.length()) // If 'i' is greater than the string length, it's an invalid command
                    break;
                temp_section += temp[i];
            }
            if (temp_section == "SECTION") { // If the command is SECTION
                string num;
                int i = 8;
                while (i < temp.length() && temp[i] != ' ') { // As long as the index doesn't exceed string length and the character isn't a space, parse
                    num += temp[i];
                    i++;
                } // Parse until a space
                if (isNum(num)) { // If the string is a number
                    string fir_time, sec_time;
                    i++; // Skip the space
                    while (i < temp.length() && temp[i] != ' ') { // As long as the index doesn't exceed string length and the character isn't a space, parse
                        fir_time += temp[i]; // Parse until a space
                        i++;
                    }
                    if (isCont_time(fir_time)) { // If the first string matches the subtitle time format
                        i++; // Skip the space
                        while (i < temp.length()) {
                            sec_time += temp[i]; // Parse until the end of the string
                            i++;
                        }
                        if (isCont_time(sec_time)) // If the second string matches the subtitle time format
                            section(stoi(num), fir_time, sec_time); // Execute the SECTION command
                        else // Otherwise, it's an invalid command
                            PrintErrorCode(400);
                    }
                    else // Otherwise, it's an invalid command
                        PrintErrorCode(400);
                }
                else // Otherwise, it's an invalid command
                    PrintErrorCode(400);
            }
            else // Otherwise, it's an invalid command
                PrintErrorCode(400);
        }
        else if (temp == "EXIT") { // EXIT command
            PrintSuccess("EXIT");
            break;
        }
        else { // Any other invalid command
            PrintErrorCode(1000);
        }
    }

    fcmd.close();
    flog.close();
    return;
}

void Manager::PrintSuccess(const char* cmd)
{
    flog << "===== " << cmd << " =====" << endl;
    flog << "Success" << endl;
    flog << "===============" << endl;
}
void Manager::PrintErrorCode(int num)
{
    flog << "===== ERROR =====" << endl;
    flog << num << endl;
    flog << "===============" << endl;
}

// LOAD
void Manager::load() {
    ifstream fsub;
    fsub.open("subtitle.txt");
    if (fsub.is_open() && que.IsEmpty() && bst.getRoot() == nullptr && list.getHead() == nullptr) { // If file exists and queue, bst, list are empty, execute LOAD
        string content;
        while (getline(fsub, content)) {
            string time, cont; // Strings to store subtitle time and content
            for (int i = 0; i < 8; i++) {
                time += content[i];
            }
            for (int i = 9; i < content.length(); i++)
                cont += content[i];
            que.Push(time, cont, flog);
        }
        flog << "===== LOAD =====\n";
        que.print(flog);
        flog << "==============\n";
    }
    else {
        PrintErrorCode(100);
    }
}
// QPOP
void Manager::qpop() {
    if (que.IsEmpty()) { // SubtitleQueue is empty
        PrintErrorCode(200);
        flog << "Subtitle queue is empty\n";
        exit(-1);
    }
    else { // SubtitleQueue is not empty
        while (!que.IsEmpty()) { // Repeat until queue is empty
            SubtitleQueueNode* temp = que.Pop(flog); // Save the first node of the queue
            bst.insert(temp->getTime(), temp->getCont()); // Insert into bst
            delete temp; // Free memory
        }
        PrintSuccess("QPOP");
    }
}
// PRINT
void Manager::print(string num) {
    if (num == "") { // print bst
        if (bst.getRoot() == nullptr) {
            PrintErrorCode(300);
            return;
        }
        flog << "===== PRINT =====\n";
        bst.print(bst.getRoot(), flog);
        flog << "===============\n";
    }
    else { // print list
        bool flag = true;
        list.print(stoi(num), flag, flog);
        if (!flag) {
            PrintErrorCode(300);
            return;
        }
    }
}
// SECTION
void Manager::section(int num, string first_time, string second_time) {
    bool flag = false; // Variable to check if nodes are found
    bst.search(bst.getRoot(), num, first_time, second_time, list, flag);
    if (!flag) // If no nodes are found
        PrintErrorCode(400);
    else
        PrintSuccess("SECTION");
}
// DELETE
void Manager::delete_equal(string time) {
    bool flag = true;
    bst.delete_equal(time, flag); // Delete node with the same subtitle time as 'time'
    if (flag) // Not found
        PrintErrorCode(500);
    else
        PrintSuccess("DELETE");
}

void Manager::delete_under(string time) {
    bool flag = true;
    stack<SubtitleBSTNode*> stk; // Stack to store nodes earlier than the specified time
    bst.delete_under(bst.getRoot(), time, flag, stk);
    if (flag) // Not found
        PrintErrorCode(500);
    else {
        while (!stk.empty()) { // While there are elements in the stack
            bst.delete_equal(stk.top()->getTime(), flag); // Delete the node matching the top's time
            stk.pop(); // Perform pop operation
        } // Delete all nodes earlier than the specified time
        PrintSuccess("DELETE");
    }
}

// OTHERS
bool Manager::isNum(string num) {
    for (int i = 0; i < num.length(); i++) { // Iterate over the input string
        if (num[i] < 48 || 57 < num[i]) // If the character is not a number
            return false;
    }
    return true; // If all are numbers
}

bool Manager::isCont_time(string num) {
    if (num.length() != 8) // If the string length is not 8, the format is incorrect
        return false;
    for (int i = 0; i < 8; i++) {
        if (i == 2 || i == 5) { // Index 2, 5
            if (num[i] != ':') // If the character is not ':', the format is incorrect
                return false;
        }
        else { // For other indices
            if (num[i] < 48 || 57 < num[i]) // If the character is not a number, the format is incorrect
                return false;
        }
    }
    return true; // If all format checks pass
}
