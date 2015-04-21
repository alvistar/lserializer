/**
 * Print a simple "Hello world!"
 *
 * @file main.cpp
 * @section LICENSE

    This code is under MIT License, http://opensource.org/licenses/MIT
 */

#include <iostream>
#include <assert.h>
#include "SerializerJSON.h"
#include "Serializer.h"
#include "alvistar/wamp/RegisteredProcedures.h"
INITIALIZE_EASYLOGGINGPP

int add (int a, int b) {
    return a+b;
}

int add2 (int a, int b, WSArray& arr) {
    cout << arr << endl;
    return a+b;
}

int main() {

    WSArrayM arr {5,6, WSArrayM {5,6}};

    auto rp = make_RegisteredProcedure(add);

    WSArrayM arr2 {4,"pino"};

    cout << rp->invoke(arr2);
};
