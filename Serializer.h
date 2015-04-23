//
// Created by Alessandro Viganò on 02/04/15.
//

#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <unordered_map>
#include "halfvector/easylogging/easylogging++.h"

#ifndef LSERIALIZER_SERIALIZER_H
#define LSERIALIZER_SERIALIZER_H

using namespace std;

enum wamp_serializer_type {
    WS_TYPE_STRING,
    WS_TYPE_NUMBER,
    WS_TYPE_DICT,
    WS_TYPE_ARRAY,
    WS_TYPE_NULL,
    WS_TYPE_BOOL
};

class WSException: public exception {
public:
    virtual const char* what() const throw() {
        return "Wamp Serializer Exception";
    }
};

class WSOutOfIndexException : public WSException{
    virtual const char* what() const throw() {
        return "Wamp Serializer Out of Index";
    }
};

class WSObjectIsNotArrayException: public WSException{
    virtual const char* what() const throw() {
        return "Wamp Serializer Object Is Not Array";
    }
};

class WSObjectIsNotDictException: public WSException{
    virtual const char* what() const throw() {
        return "Wamp Serializer Object Is Not Dictionary";
    }
};

class WSObjectIsNotStringException : public WSException{
    virtual const char* what() const throw() {
        return "Wamp Serializer Object Is Not string";
    }
};

class WSObjectIsNotNumException : public WSException{
    virtual const char* what() const throw() {
        return "Wamp Serializer Cannot Convert To Numeric";
    }
};

class WSObjectIsNotBoolException : public WSException{
    virtual const char* what() const throw() {
        return "Wamp Serializer Cannot Convert To Bool";
    }
};

class WSDeserializeException : public WSException{
    virtual const char* what() const throw() {
        return "Wamp Serializer Error on Deserialization";
    }
};

#endif //LSERIALIZER_SERIALIZER_H



