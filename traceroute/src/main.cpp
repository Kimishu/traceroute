//
// Created by Kimihito on 03.10.2024.
//

#include "Tracerouter/Tracerouter.h"

#include <iostream>
#include <algorithm>

using namespace std;

void Help() {
    cout << "Usage:\n";
    cout << "\t--help\tPrints help info about CLI\n";
    cout << "\t<link>\tPrints path of provided resource\n";
}

bool ParseCommandLine(uint8_t count, char** args) {
    vector<string> arguments;
    //Skip first element equals program name =)
    for (int i = 1; i < count; i++)
        arguments.emplace_back(args[i]);

    //If command line contains '--help' or more than 2 args -> Help()
    if(std::find(arguments.begin(), arguments.end(),"--help") != arguments.end() || count >= 3){
        Help();
        return false;
    }
    return true;
}

int main(int argc, char** argv) {

    if (argc != 2) {
        Help();
        exit(0);
    }

    if(!ParseCommandLine(argc, argv))
        exit(0);

    Tracerouter tracerouter(argv[1]);

    tracerouter.PrintDestinationInfo();
    tracerouter.Trace();

    return 0;
}
