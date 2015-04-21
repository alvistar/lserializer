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
    WS_TYPE_NULL
};

template<std::size_t I = 0, typename FuncT, typename... Tp>
inline typename std::enable_if<I == sizeof...(Tp), void>::type
for_each(std::tuple<Tp...> &, FuncT) // Unused arguments are given no names.
{ }

template<std::size_t I = 0, typename FuncT, typename... Tp>
inline typename std::enable_if<I < sizeof...(Tp), void>::type
for_each(std::tuple<Tp...>& t, FuncT f)
{
    f(std::get<I>(t),I);

    for_each<I + 1, FuncT, Tp...>(t, f);
}

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

class WSDeserializeException : public WSException{
    virtual const char* what() const throw() {
        return "Wamp Serializer Error on Deserialization";
    }
};

class WSArray;
class WSNumeric;
class WSDict;

class WSObj  {
public:

    virtual ~WSObj() {};

    virtual enum wamp_serializer_type objType() const = 0;

    virtual WSArray & toArray ();

    virtual WSDict & toDict();

    virtual string toString() const= 0;

    friend std::ostream &operator << (std::ostream &os, const WSObj &o) {
        return os << string(o.toString());
    }

    virtual WSObj * clone() const = 0;
    virtual WSObj *moveClone() = 0;

    //Casting operator
    virtual operator string() const  {
        return toString();
    }

    virtual operator WSArray() const;

    virtual operator WSDict() const;

    virtual operator int() const {
        throw WSObjectIsNotNumException();
    }

    virtual operator float() const {
        throw WSObjectIsNotNumException();
    }

    virtual operator unsigned long long int() const {
        throw WSObjectIsNotNumException();
    }
};

class WSString :public virtual WSObj{
private:
    string data;
public:
    WSString() {};
    WSString(string s):data(s) {};
    WSString(const char* s):data(s) {};
    WSString(const WSString & o):data(o.data) {};
    WSString(WSString && o):data(move(o.data)) {};

    virtual ~WSString() {};

    virtual enum wamp_serializer_type objType() const override {
        return WS_TYPE_STRING;
    }

    virtual string toString() const override {
        return data;
    }

    virtual WSObj * clone() const override {
        CLOG(DEBUG,"wserializer") << "Cloning string "<< *this;
        return new WSString(*this);
    }

    virtual WSObj *moveClone() override {
        return new WSString(move(*this));
    }
};

class WSNumeric :public virtual WSObj {

public:
    virtual ~WSNumeric() { }

    virtual enum wamp_serializer_type objType() const override {
        return WS_TYPE_NUMBER;
    }
};

template <typename  T>
class WSNumericM :public virtual WSNumeric {
private:
    T data;
public:
    WSNumericM(T data) : data(data) { }

    virtual WSObj *clone() const override {
        CLOG(DEBUG, "wserializer") << "Cloning number " << (*this) << endl;
        return new WSNumericM<T> (*this);
    }

    virtual WSObj *moveClone() override {
        return new WSNumericM<T> (move(*this));
    }

    T getValue() const {
        return data;
    }

    void setValue(T data) {
        WSNumericM::data = data;
    }

    virtual string toString() const override {
        ostringstream oss;
        oss << data;
        return oss.str();
    }

    operator int() const override {return data;};

    operator float() const override {return data;};

    operator unsigned long long int() const override {return data;};
};

//namespace wserializer {
//    extern el::Logger *wslogger;
//}

#endif //LSERIALIZER_SERIALIZER_H



