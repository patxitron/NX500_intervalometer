/**
 * @file
 * 
 * Program entry point.
 */

#include <string>
#include <vector>
#include <iostream>

typedef std::vector<std::string> stringlist;

int main(int argc, char* argv[])
{
    std::string holamundo_args;
    stringlist args;
    if (argc > 1) {
        args.insert(args.begin(), argv + 1, argv + argc);
        for (auto const& arg: args) {
            if (holamundo_args.empty()) holamundo_args += ": '";
            else holamundo_args += " '";
            holamundo_args += arg;
            holamundo_args += "'";
        }
    }
    std::cout << "Hola mundo" << holamundo_args << std::endl;
    return 0;
}
