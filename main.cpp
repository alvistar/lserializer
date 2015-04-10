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
    char const* input = "[{\"merlo\":\"Felice\"},[2,3,4]]";
    cout << input << endl;

    auto obj = WSObj_JSON::factory(input);

    cout << "Len:" << obj->toArray()->size() << endl;
//
//
    auto ws = make_shared<WSObjString> ("gatto");
    auto mutableArray = make_shared<WSObjArrayMutable> ();
    auto ma2 =make_shared<WSObjArrayMutable>();
    WSObjDictMutable dict {};
//
    auto number = make_shared<WSObjNumericM<int>> (15);
//
//
    cout << "Dict:" << dict << endl;
//
    ma2->push_back("nested1");
    ma2->push_back("nested2");
    mutableArray->push_back(ws);
    mutableArray->push_back(ma2);
    mutableArray->push_back("ermegenilfi");
    mutableArray->push_back(number);
    mutableArray->push_back(WS_DICT_EMPTY);
//
    cout << "ws:" << *ws << endl;
    cout << "Array:" << *mutableArray << endl;
//
//    cout << "Serialized: " << *mutableArray << endl;
//
    cout << "NEW TESTS ---" << endl;
    WSObjArrayMutable na {"pino","mesiano","giamaicano",WSObjArrayMutable {26,42,32.5},24};
    //cout << na<< endl;

    //WSObjArrayMutable na2 {move(na), "cane", "gatto"};

    //cout << na2 << endl;
//
    WSObjDictMutable dict2 {};
    dict2.insert({"cane","gatto"});
    dict2.insert({"pino",12});
    cout << "Dict2 " << dict2 << endl;

    WSObjDictMutable dict3 {};

    //dict3.insert({"pino",1},{"cedro",2});

};
