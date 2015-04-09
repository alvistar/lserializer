//
// Created by Alessandro Viganò on 02/04/15.
//

#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <unordered_map>

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

class WSException: public exception {
public:
    virtual const char* what() const throw() {
        return "Wamp Serializer Exception";
    }
};

class WSArrayOutOfIndexException: public WSException{
    virtual const char* what() const throw() {
        return "Wamp Serializer Out of Index Array";
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
        return "Wamp Serializer Object Is Not Numeric";
    }
};

class WSDeserializeException : public WSException{
    virtual const char* what() const throw() {
        return "Wamp Serializer Error on Deserialization";
    }
};

class WSSerializeTokenUnexpected : public WSException{
    virtual const char* what() const throw() {
        return "Wamp Serializer Token Unexpected";
    }
};

class WSObjArray;
class WSObjNumeric;
class WSObjDict;

class WSObj: public std::enable_shared_from_this<WSObj> {
public:
    virtual enum wamp_serializer_type objType() = 0;
    shared_ptr<WSObjArray> toArray () {
        if (this->objType() != WS_TYPE_ARRAY)
            throw WSObjectIsNotArrayException();
        return dynamic_pointer_cast<WSObjArray> (shared_from_this());
    }
    shared_ptr<WSObjNumeric> toNumeric() {
        if (this->objType() != WS_TYPE_NUMBER)
            throw WSObjectIsNotNumException();
        return dynamic_pointer_cast<WSObjNumeric> (shared_from_this());
    }

    shared_ptr<WSObjDict> toDict() {
        if (this->objType() != WS_TYPE_DICT) {
            throw WSObjectIsNotDictException();
        }
        return dynamic_pointer_cast<WSObjDict> (shared_from_this());
    }

    virtual string toString() const= 0;

    friend std::ostream &operator << (std::ostream &os, const WSObj &o) {
        return os << string(o.toString());
    }
};

class WSObjString:public virtual WSObj{
private:
    string data;
public:
    WSObjString() {};
    WSObjString(string s):data(s) {};

    virtual enum wamp_serializer_type objType() override {
        return WS_TYPE_STRING;
    }

    virtual string toString() const override {
        return data;
    }
};

class WSObjNumeric:public virtual WSObj {

public:
    virtual enum wamp_serializer_type objType() override {
        return WS_TYPE_NUMBER;
    }
};


template <typename  T>
class WSObjNumericM:public virtual WSObjNumeric {
private:
    T data;
public:
    WSObjNumericM(T data) : data(data) { }

    T getValue() const {
        return data;
    }

    void setValue(T data) {
        WSObjNumericM::data = data;
    }

    virtual string toString() const override {
        ostringstream oss;
        oss << data;
        return oss.str();
    }
};

class WSObjDict:public virtual WSObj {
public:
    virtual shared_ptr<WSObj> objForKey(string const &key) = 0;
    virtual enum wamp_serializer_type objType() override {
        return WS_TYPE_DICT;
    }
    virtual string toString() const override {
        return "{}";
    }
};

class WSObjArray:public virtual WSObj {
public:
    virtual shared_ptr<WSObj> objAtIndex(int const &index) = 0;
    virtual int size() = 0;
    virtual enum wamp_serializer_type objType() override {
        return WS_TYPE_ARRAY;
    }
};

class WSObjArrayMutable:public virtual WSObjArray {
private:
    vector<shared_ptr<WSObj>> data;
public:
    vector<shared_ptr<WSObj>> const &getData() const {
        return data;
    }

    virtual void push_back(shared_ptr<WSObj> obj) {
        data.push_back(obj);
    }

    virtual void push_back(string const &s) {
        shared_ptr<WSObj> o (new WSObjString(s));
        data.push_back(o);
    }

    template <typename T, typename enable_if <is_arithmetic<T>::value, int>::type = 0>
    void push_back(T obj) {
        shared_ptr<WSObj> o (new WSObjNumericM<T>(obj));
        data.push_back(o);
    };

    virtual shared_ptr<WSObj> objAtIndex(int const &index) override {
        return data[index];
    }

    virtual int size() override {
        return (int) data.size();
    }

    virtual string toString() const override {
        ostringstream oss;
        oss << "[";
        bool first = true;
        for (auto e:data) {
            if (!first)
                oss << ", ";
            if (e->objType() == WS_TYPE_STRING)
                oss << "\"";

            oss << e->toString();

            if (e->objType() == WS_TYPE_STRING)
                oss << "\"";

            first = false;
        }
        oss << "]";
        return oss.str();
    }
};

class WSObjDictMutable: public virtual WSObjDict {
private:
    unordered_map <string, shared_ptr<WSObj>> data;
public:

    void insert (const string &key, const string &value) {
        shared_ptr<WSObj> o (new WSObjString(value));
        data[key] = o;
    }

    void insert (const string &key, shared_ptr<WSObj> obj) {
        data[key] = obj;
    }

    virtual shared_ptr<WSObj> objForKey(string const &key) override {
        return data[key];
    }

    virtual string toString() const override {
        ostringstream oss;
        oss << "{";
        bool first {true};
        for (auto kv: data) {
            if (!first)
                oss << ", ";
            oss << "\"" << kv.first << "\"" << ":";
            if (kv.second->objType() == WS_TYPE_STRING)
                oss << "\"";

            oss << * kv.second;

            if (kv.second->objType() == WS_TYPE_STRING)
                oss << "\"";

            first = false;
        }

        oss << "}";
        return oss.str();
    }
};


#define WS_DICT_EMPTY make_shared<WSObjDictMutable>()
#define WS_ARRAY_EMPTY make_shared<WSObjArrayMutable>()

#endif //LSERIALIZER_SERIALIZER_H



