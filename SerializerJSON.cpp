//
// Created by Alessandro Viganò on 02/04/15.
//

#include "SerializerJSON.h"
#include "Serializer.h"
#include <sstream>
#include <memory>


shared_ptr<WSObj> WSObj_JSON::factory(char const *msg) {
    struct json_token *token = parse_json2(msg, (int) strlen(msg));

    return WSObj_JSON::factory(token);
}

shared_ptr<WSObj> WSObj_JSON::factory(struct json_token *token) {
    switch (token->type) {
        case JSON_TYPE_ARRAY:
            return shared_ptr<WSObj> (new WSObjArray_JSON(token));
        case JSON_TYPE_NUMBER:
            return shared_ptr<WSObj> (new WSObjNumeric_JSON(token));
        case JSON_TYPE_STRING:
            return shared_ptr<WSObj> (new WSObjString_JSON(token));
        case JSON_TYPE_OBJECT:
            return shared_ptr<WSObj> (new WSObjDict_JSON(token));
    };
}

string WSObjDict_JSON::toString() const {
    string s(token->ptr,(unsigned long) token->len);
    return s;
}

shared_ptr<WSObj> WSObjDict_JSON::objForKey(string const &key) {
    struct json_token * tok = find_json_token(token, key.c_str());
    return WSObj_JSON::factory(tok);
}

WSObj *WSObjDict_JSON::clone() const {
    //Todo to be implemented
    return nullptr;
}

WSObj *WSObjDict_JSON::moveClone() {
    //Todo to be implemented
    return nullptr;
}

shared_ptr<WSObj> WSObjArray_JSON::objAtIndex(int const &index) {

    ostringstream indxString;

    indxString << "[" << index << "]";
    struct json_token * tok = find_json_token(token, indxString.str().c_str());

    if (tok == NULL)
        throw WSArrayOutOfIndexException();

    return WSObj_JSON::factory(tok);
}

string WSObjArray_JSON::toString() const {
    string s(token->ptr,(unsigned long) token->len);
    return s;
}

int WSObjArray_JSON::size() {
    int count= token->num_desc;

    for (int i=1; i < token->num_desc;) {
        if (token[i].num_desc >0) {
            count -= token[i].num_desc;
            i = i + token[i].num_desc;
        }
        else
            i++;
    }

    return count;
}
string WSObjNumeric_JSON::toString() const {
    string s(token->ptr,(unsigned long) token->len);
    return s;
}

WSObj *WSObjNumeric_JSON::clone() const {
    //Todo to be implemented
    return nullptr;
}

WSObj *WSObjNumeric_JSON::moveClone() {
    //Todo to be implemented
    return nullptr;
}

string WSObjString_JSON::toString() const {
    string s(token->ptr,(unsigned long) token->len);
    return s;
}
