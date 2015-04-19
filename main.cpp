/**
 * Print a simple "Hello world!"
 *
 * @file main.cpp
 * @section LICENSE

    This code is under MIT License, http://opensource.org/licenses/MIT
 */

#include <iostream>
#include "SerializerJSON.h"
#include "Serializer.h"

INITIALIZE_EASYLOGGINGPP
int main() {
    std::cout << "Hello world!\n";

    WSObjM o {1};

    cout << o << endl;

    WSArrayM arr {{1,2.5,3,"pino",WSArrayM{3,4,5}}};

    WSArrayM arr2 {4,5,6};
    cout << "Array: " << arr << endl;

};
