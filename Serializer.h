//
// Created by Alessandro Viganò on 02/04/15.
//

#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <unordered_map>
#include "halfvector/easylogging/easylogging++.h"
#include "in.h"

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

class WSObjArray;
class WSObjNumeric;
class WSObjDict;

class WSObj  {
public:

    virtual ~WSObj() {};

    virtual enum wamp_serializer_type objType() const = 0;
    WSObjArray& toArray ();
    WSObjDict & toDict();
    WSObjNumeric & toNumeric () const;

    virtual string toString() const= 0;

    friend std::ostream &operator << (std::ostream &os, const WSObj &o) {
        return os << string(o.toString());
    }

    virtual WSObj * clone() const = 0;
    virtual WSObj *moveClone() = 0;

    //Casting operator
    virtual operator int() const {
        throw WSObjectIsNotNumException();
    }

    virtual operator float() const {
        throw WSObjectIsNotNumException();
    }
};

class WSObjString:public virtual WSObj{
private:
    string data;
public:
    WSObjString() {};
    WSObjString(string s):data(s) {};
    WSObjString(const char* s):data(s) {};
    WSObjString(const WSObjString& o):data(o.data) {};
    WSObjString(WSObjString&& o):data(move(o.data)) {};

    virtual ~WSObjString() {};

    virtual enum wamp_serializer_type objType() const override {
        return WS_TYPE_STRING;
    }

    virtual string toString() const override {
        return data;
    }

    virtual WSObj * clone() const override {
        CLOG(DEBUG,"wserializer") << "Cloning string "<< *this;
        return new WSObjString(*this);
    }

    virtual WSObj *moveClone() override {
        return new WSObjString(move(*this));
    }
};

class WSObjNumeric:public virtual WSObj {

public:
    virtual ~WSObjNumeric() { }

    virtual enum wamp_serializer_type objType() const override {
        return WS_TYPE_NUMBER;
    }
};


template <typename  T>
class WSObjNumericM:public virtual WSObjNumeric {
private:
    T data;
public:
    WSObjNumericM(T data) : data(data) { }

    virtual WSObj *clone() const override {
        CLOG(DEBUG, "wserializer") << "Cloning number " << (*this) << endl;
        return new WSObjNumericM<T> (*this);
    }

    virtual WSObj *moveClone() override {
        return new WSObjNumericM<T> (move(*this));
    }

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

    operator int() { return data;};
    operator float() {return data;};
};

class WSObjDict:public virtual WSObj {
public:
    virtual ~WSObjDict() { }

    virtual WSObj & objForKey(string const &key) = 0;
    virtual enum wamp_serializer_type objType() const override {
        return WS_TYPE_DICT;
    }
    virtual string toString() const override {
        return "{}";
    }
};

class WSObjArray :public virtual WSObj {
public:
    virtual ~WSObjArray() { }
    virtual WSObj & objAtIndex(int const &index) = 0;
    virtual int size() = 0;
    virtual enum wamp_serializer_type objType() const override {
        return WS_TYPE_ARRAY;
    }

    WSObjDict & toDict() = delete;

};

class WSObjArrayMutable:public virtual WSObjArray {
private:
    vector<unique_ptr<WSObj>> data;

    template <typename T, typename enable_if <is_arithmetic<T>::value, int>::type = 0>
    void pb(const T & num) {
        unique_ptr<WSObj> o (new WSObjNumericM<T>(num));
        data.push_back(move(o));
    }

    void pb(const WSObj &obj) {
        unique_ptr<WSObj> o (obj.clone());
        data.push_back(move(o));
    }

    void pb(WSObj &&obj) {
        unique_ptr<WSObj> o (obj.moveClone());
        data.push_back(move(o));
    }

    virtual void pb(const WSObjString &obj) {
        unique_ptr<WSObj> o (obj.clone());
        data.push_back(move(o));
    }

    virtual void pb(WSObjString &&obj) {
        unique_ptr<WSObj> o (obj.moveClone());
        data.push_back(move(o));
    }

public:
    WSObjArrayMutable() {};
    WSObjArrayMutable(const WSObjArrayMutable& o) {
        CLOG(DEBUG,"wserializer") << "Deep Copying..." << endl;
        for (auto &e:o.data) {
            push_back(*e);
        }
    };

    WSObjArrayMutable(WSObjArrayMutable&& o):data(move(o.data)) {
        o.data.clear();
    };


    template <typename ... ARGS>
    WSObjArrayMutable(ARGS&& ... args) {
        push_back(forward<ARGS> (args) ...);
    }

    virtual WSObj * clone() const override {
        return new WSObjArrayMutable(*this);
    }

    virtual WSObj *moveClone() override {
        return new WSObjArrayMutable(move(*this));
    }

    template <typename T>
    void push_back(T&& obj) {
        pb(forward<T> (obj));
    }

    template <typename T, typename ... OTHERS>
    void push_back(T&& first, OTHERS&& ...others) {
        push_back(forward<T>(first));
        push_back(forward<OTHERS>(others) ...);
    }

    virtual WSObj& objAtIndex(int const &index) override {
        return *data[index];
    }

    virtual int size() override {
        return (int) data.size();
    }

    virtual string toString() const override {
        ostringstream oss;
        oss << "[";
        bool first = true;
        for (auto &e:data) {
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

class WSPair
{
public:
    string first;
    unique_ptr<WSObj> second;

    WSPair(const string& first, const WSObj & sec):first(first) {
        second = unique_ptr<WSObj> (sec.clone());
    }

    WSPair(const string& first, WSObj && sec):first(first) {
        second = unique_ptr<WSObj> (sec.moveClone());
    }

    WSPair(const string& first, WSObjString&& sec):first(first) {
        second = unique_ptr<WSObj>(new WSObjString(forward<WSObjString>(sec)));
    }

    WSPair(const string& first, const WSObjDict& sec):first(first) {
        second = unique_ptr<WSObj> (sec.clone());;
    }

    WSPair(const string& first, WSObjDict&& sec):first(first) {
        second = unique_ptr<WSObj> (sec.moveClone());;
    }

    template <typename T, typename enable_if <is_arithmetic<T>::value, int>::type = 0>
    WSPair(const string& first, T&& sec):first(first) {
        second = unique_ptr<WSObj>(new WSObjNumericM<T>(sec));
    }
};


class WSObjDictMutable: public virtual WSObjDict {
private:
    unordered_map <string, unique_ptr<WSObj>> data;


public:
    virtual WSObj *clone() const override {
        CLOG(DEBUG, "wserializer") << "Dict deep copy...";
        auto o = new WSObjDictMutable ();
        for (auto &kv: data) {
            o->data[kv.first] = unique_ptr<WSObj> (kv.second->clone());
        }
        return o;
    }

    virtual WSObj *moveClone() override {
        return new WSObjDictMutable(move(*this));
    }

    void insert(const WSPair &p) {
        data[p.first] =  unique_ptr<WSObj> (p.second->clone());
    }

    void insert(WSPair &&p) {
        data[p.first] = move(p.second);
    }

    virtual WSObj & objForKey(string const &key) override {
        return * data[key];
    }

    template <typename T, typename ... OTHERS>
    void insert(T&& first, OTHERS&& ...others) {
        insert(forward<T>(first));
        insert(forward<OTHERS>(others) ...);
    }

    WSObjDictMutable& operator << (WSPair &&pair) {
        insert(move(pair));
        return *this;
    }

    virtual string toString() const override {
        ostringstream oss;
        oss << "{";
        bool first {true};
        for (auto &kv: data) {
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
//
};

namespace wserializer {
    extern el::Logger *wslogger;
}

#endif //LSERIALIZER_SERIALIZER_H



