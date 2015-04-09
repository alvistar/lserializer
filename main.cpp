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
    //char const* input = "[1,[],2,\"cavallo\",[4,5,6,[7,8,9]]]";
    char const* input = "[{\"merlo\":\"Felice\"},[2,3,4]]";
    cout << input << endl;

    auto obj = WSObj_JSON::factory(input);

    cout << "Len:" << obj->toArray()->size() << endl;




    auto ws = make_shared<WSObjString> ("gatto");
    auto mutableArray = make_shared<WSObjArrayMutable> ();
    auto ma2 =make_shared<WSObjArrayMutable>();
    auto dict = WS_DICT_EMPTY;

    auto number = make_shared<WSObjNumericM<int>> (15);

    dict->insert("Renna","Babbo Natale");

    cout << "Dict:" << * dict << endl;

    ma2->push_back("nested1");
    ma2->push_back("nested2");
    mutableArray->push_back(ws);
    mutableArray->push_back(ma2);
    mutableArray->push_back("ermegenilfi");
    mutableArray->push_back(number);
    mutableArray->push_back(WS_DICT_EMPTY);

    cout << "ws:" << *ws << endl;
    cout << "Array:" << *mutableArray << endl;

    SerializerJson s {};
    cout << "Serialized: " << s.serialize(mutableArray) << endl;


}
