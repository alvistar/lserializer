//
// Created by Alessandro Viganò on 02/04/15.
//

#include "SerializerJSON.h"
#include "Serializer.h"
#include <sstream>
#include <memory>

unique_ptr<WSObj> WSObj_JSON::factory(const char* msg) {
    struct json_token *token = parse_json2(msg, (int) strlen(msg));
    //WSObj_JSON::factory(token);

    return WSObj_JSON::factory(token);
}

unique_ptr<WSObj> WSObj_JSON::factory(struct json_token *token) {
    switch (token->type) {
        case JSON_TYPE_ARRAY:
            return unique_ptr<WSObj> (new WSObjArray_JSON(token));
        case JSON_TYPE_NUMBER:
            return unique_ptr<WSObj> (new WSObjNumeric_JSON(token));
        case JSON_TYPE_STRING:
            return unique_ptr<WSObj> (new WSObjString_JSON(token));
        case JSON_TYPE_OBJECT:
            return unique_ptr<WSObj> (new WSObjDict_JSON(token));
        case JSON_TYPE_TRUE:
            return unique_ptr<WSObj> (new WSBool(true));
        case JSON_TYPE_FALSE:
            return unique_ptr<WSObj> (new WSBool(false));
        case JSON_TYPE_NULL:
            return unique_ptr<WSObj> (new WSObj());
        default:
            throw (new WSException());
    };
}

//WSOBJDICT Methods
//-----------------

string WSObjDict_JSON::toString() const {
    string s(token->ptr,(unsigned long) token->len);
    return s;
}

WSObj & WSObjDict_JSON::at(string const &key) {
    try {
        return *data.at(key);
    }
    catch (std::out_of_range) {
        struct json_token * tok = find_json_token(token, key.c_str());

        if (tok == NULL)
            throw WSOutOfIndexException();

        auto p = WSObj_JSON::factory(tok);
        auto rawp = p.get();
        data[key] = move(p);
        return *rawp;
    }
}

WSObj *WSObjDict_JSON::clone() const {
    CLOG(DEBUG,"wserialize") << "Cloning " << *this << endl;
    return new WSObjDict_JSON (*this);
}

WSObj *WSObjDict_JSON::moveClone() {
    return new WSObjDict_JSON(move(*this));
}

//WSObjArray_JSON Methods
//-----------------------

WSObj& WSObjArray_JSON::at(int const &index) {
    try {
        return *data.at(index);
    }
    catch (std::out_of_range) {
        ostringstream indxString;

        indxString << "[" << index << "]";
        struct json_token *tok = find_json_token(token, indxString.str().c_str());

        if (tok == NULL)
            throw WSOutOfIndexException();

        auto p = WSObj_JSON::factory(tok);
        auto rawp = p.get();
        data[index] = move(p);
        return *rawp;
    }


}

string WSObjArray_JSON::toString() const {
    string s(token->ptr,(unsigned long) token->len);
    return s;
}

int WSObjArray_JSON::size() const {
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

WSObj* WSObjArray_JSON::clone() const  {
    CLOG(DEBUG,"wserialize") << "Cloning " << *this << endl;
    return new WSObjArray_JSON (*this);
}

WSObj* WSObjArray_JSON::moveClone()  {
    return new WSObjArray_JSON(move(*this));
}

string WSObjNumeric_JSON::toString() const {
    string s(token->ptr,(unsigned long) token->len);
    return s;
}

WSObj *WSObjNumeric_JSON::clone() const {
    CLOG(DEBUG,"wserialize") << "Cloning " << *this << endl;
    return new WSObjNumeric_JSON(*this);
}

WSObj *WSObjNumeric_JSON::moveClone() {
    return new WSObjNumeric_JSON(move(*this));
}

string WSObjString_JSON::toString() const {
    string s(token->ptr,(unsigned long) token->len);
    return s;
}

WSObj & Deserializer::deserialize(const string &input) {
    this->input = input;
    root= WSObj_JSON::factory(this->input.c_str());
    return * root;
};

WSObj & Deserializer::deserialize(string &&input) {
    this->input = move(input);
    root= WSObj_JSON::factory(this->input.c_str());
    return * root;
}

WSObjNumeric_JSON::operator int() const {
    string s(token->ptr,(unsigned long) token->len);
    return stoi(s);
}

WSObjNumeric_JSON::operator float() const {
    string s(token->ptr,(unsigned long) token->len);
    return stof(s);
}

WSObjNumeric_JSON::operator unsigned long long int() const {
    string s(token->ptr,(unsigned long) token->len);
    return stoull(s);
}
