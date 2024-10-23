#ifndef MENU_H
#define MENU_H

#include<iostream>
#include<string>
#include<sstream>

class CommandLine {
    public:
        CommandLine();

        // Basic functionality
        void read_command();
        void clear();

        // Getters
        std::string get_command();
        std::string get_specifier(int);

    private:
        static const int MAX_SPECIFIERS = 5;

        std::string command;
        std::string specifiers[MAX_SPECIFIERS];
};

#endif