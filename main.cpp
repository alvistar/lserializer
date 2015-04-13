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


int main() {
    std::cout << "Hello world!\n";
    //string input {"[{\"merlo\":\"Felice\"},[2,3,4]]"};
    string input {"[1,2,\"Enzo\",4]"};
    cout << input << endl;
    Deserializer ds;

    auto& obj = ds.deserialize(input);
    //WSObjArray& a = dynamic_cast<WSObjArray&> (obj);
    auto& a = obj.toArray().toDict();
    //cout << a.objAtIndex(0) << "," << a.objAtIndex(2) << endl;

    //cout << ((WSObjArray) obj).objAtIndex(0) << endl;
    //WSObjArray& a = obj.toArray();
    //cout << "Element 0:" << a.objAtIndex(0) << endl;

    cout << obj << endl;

    cout << "fine" << endl;
//    cout << "NEW TESTS ---" << endl;
//    //WSObjArrayMutable na {"pino","mesiano","giamaicano",WSObjArrayMutable {26,42,32.5},24};
//
//    WSObjArrayMutable a {};
//    WSObjString s ("pino");
//    a.push_back(s);
//    a.push_back("renna");
//
//    cout << "Result A: "<< a << endl;
//
//    WSObjArrayMutable b {"gino","bramieri",22,23.5};
//    b.push_back(move(a));
//    cout << "Result B: "<< b << endl;
//
//    WSObjDictMutable d {};
//    d.insert (WSPair{"gatto","cassa"}, WSPair{"pino",2});
//    cout << "Result C:" << d << endl;



};
