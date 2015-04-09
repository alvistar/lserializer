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

shared_ptr<WSObj> WSObjArray_JSON::objAtIndex(int const &index) {

    ostringstream indxString;

    indxString << "[" << index << "]";
    struct json_token * tok = find_json_token(token, indxString.str().c_str());

    if (tok == NULL)
        throw WSArrayOutOfIndexException();

    return WSObj_JSON::factory(tok);
}

string WSObjNumeric_JSON::toString() const {
    string s(token->ptr,(unsigned long) token->len);
    return s;
}

string WSObjArray_JSON::toString() const {
    string s(token->ptr,(unsigned long) token->len);
    return s;
}

string WSObjString_JSON::toString() const {
    string s(token->ptr,(unsigned long) token->len);
    return s;
}

string SerializerJson::serialize(shared_ptr<WSObj> o) {
    oss.str("");
    oss.clear();
    _serialize(o);
    return oss.str();
}

void SerializerJson::_serialize(shared_ptr<WSObjString> o) {
    oss << "\"" << o->toString() << "\"";
}

void SerializerJson::_serialize(shared_ptr<WSObjNumeric> o) {
    oss << o->toString();
}


void SerializerJson::_serialize(shared_ptr<WSObjArray> o) {
    shared_ptr<WSObj> cur;
    oss << "[";
    for (int i=0; i < o->size(); i++) {
        if (i>0)
            oss << ", ";
        cur = o->objAtIndex(i);
        _serialize(cur);
    }
    oss << "]";
 }

void SerializerJson::_serialize(shared_ptr<WSObjDict> o) {
    oss << *o;
}

int WSObjArray_JSON::size() {
    int count=0;

    for (int i=1; i < token->num_desc;) {
        count++;
        if (token[i].num_desc >0)
            i = i + token[i].num_desc;
        else
            i++;
    }

    return count;
}

void SerializerJson::_serialize(shared_ptr<WSObj> o) {
    switch (o->objType()) {
        case WS_TYPE_STRING:
            _serialize(dynamic_pointer_cast<WSObjString>(o));
            break;
        case WS_TYPE_ARRAY:
            _serialize(dynamic_pointer_cast<WSObjArray>(o));
            break;
        case WS_TYPE_NUMBER:
            _serialize(dynamic_pointer_cast<WSObjNumeric>(o));
            break;
        case WS_TYPE_DICT:
            _serialize(dynamic_pointer_cast<WSObjDict>(o));
    }
}



