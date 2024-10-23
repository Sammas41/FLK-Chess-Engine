#include "commandLine.h"

// Constructor
CommandLine::CommandLine() : command() {

}

// Read function
void CommandLine::read_command() {

    std::string line, line_token;
    std::getline(std::cin, line);

    std::stringstream ss(line);

    bool first_token = true;
    int cont = 0;
    
    while(ss >> line_token) {
        // The first string is the command
        if(first_token) {
            command = line_token;
            first_token = false;
        }
        else {
            if(cont < MAX_SPECIFIERS) {
                specifiers[cont] = line_token;
                cont++;
            }
            else {
                std::cout << "Too many specifiers\n";
                clear();
                return;
            }
        }
    }
}

// Clear function
void CommandLine::clear() {
    command = "";
    for(std::string& s : specifiers)
        s = "";
}

// Getters
std::string CommandLine::get_command() {
    return command;
}

std::string CommandLine::get_specifier(int index) {

    if(index < 0 || index >= MAX_SPECIFIERS)
        return "";
    
    return specifiers[index];
}