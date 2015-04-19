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

//class WSArrayM :public virtual WSArray {
//private:
//    vector<unique_ptr<WSObj>> data;
//
//    template <typename T, typename enable_if <is_arithmetic<T>::value, int>::type = 0>
//    void pb(const T & num) {
//        unique_ptr<WSObj> o (new WSNumericM<T>(num));
//        data.push_back(move(o));
//    }
//
//    void pb(const WSObj &obj) {
//        unique_ptr<WSObj> o (obj.clone());
//        data.push_back(move(o));
//    }
//
//    void pb(WSObj &&obj) {
//        unique_ptr<WSObj> o (obj.moveClone());
//        data.push_back(move(o));
//    }
//
//    virtual void pb(const WSObjString &obj) {
//        unique_ptr<WSObj> o (obj.clone());
//        data.push_back(move(o));
//    }
//
//    virtual void pb(WSObjString &&obj) {
//        unique_ptr<WSObj> o (obj.moveClone());
//        data.push_back(move(o));
//    }
//
//public:
//    // Constructors
//    WSArrayM() {};
//
//    // Copy Constructor
//    WSArrayM(const WSArrayM & o) {
//        CLOG(DEBUG,"wserializer") << "Deep Copying:" << o << endl;
//
//        for (auto &e:o.data) {
//            push_back(*e);
//        }
//
//        CLOG(DEBUG,"wserializer") << "New log:" << *this;
//    };
//
//    WSArrayM(WSArrayM && o):data(move(o.data)) {
//
//        o.data.clear();
//    };
//
////    template <typename ... ARGS>
////    WSArrayM(ARGS&& ... args) {
////        LOG(DEBUG) << "Called normal constructor";
////        push_back(forward<ARGS> (args) ...);
////    }
//
//    WSArrayM(std::initializer_list<WSObjM> list) {
//        for (auto &e: list)
//            push_back(e);
//    }
//
//    virtual WSObj * clone() const override {
//        return new WSArrayM(*this);
//    }
//
//    virtual WSObj *moveClone() override {
//        return new WSArrayM(move(*this));
//    }
//
//    template <typename T>
//    void push_back(T&& obj) {
//        pb(forward<T> (obj));
//    }
//
//    template <typename T, typename ... OTHERS>
//    void push_back(T&& first, OTHERS&& ...others) {
//        push_back(forward<T>(first));
//        push_back(forward<OTHERS>(others) ...);
//    }
//
//    void clear() {
//        data.clear();
//    }
//
//    virtual WSObj& objAtIndex(int const &index) override {
//        return *data[index];
//    }
//
//    virtual int size() override {
//        return (int) data.size();
//    }
//
//    virtual string toString() const override {
//        ostringstream oss;
//        oss << "[";
//        bool first = true;
//        for (auto &e:data) {
//            if (!first)
//                oss << ", ";
//            if (e->objType() == WS_TYPE_STRING)
//                oss << "\"";
//
//            oss << e->toString();
//
//            if (e->objType() == WS_TYPE_STRING)
//                oss << "\"";
//
//            first = false;
//        }
//        oss << "]";
//        return oss.str();
//    }
//
//    //Test
//
//
//    template<std::size_t I = 0, typename... Tp>
//    inline typename std::enable_if<I == sizeof...(Tp), void>::type
//    convert_tuple(std::tuple<Tp...> &) // Unused arguments are given no names.
//    { }
//
//    template<std::size_t I = 0, typename... Tp>
//    inline typename std::enable_if<I < sizeof...(Tp), void>::type
//    convert_tuple(std::tuple<Tp...>& t)
//    {
//        using T = typename std::tuple_element<I, std::tuple<Tp...>>::type;
//
//        T &el = std::get<I>(t);
//        el = static_cast<T> (objAtIndex(I));
//
//        convert_tuple<I + 1, Tp...>(t);
//    }
//
//    template <typename ... Ts>
//    tuple <Ts ...> toTuple() {
//
//        tuple<Ts...> dest;
//
//        convert_tuple(dest);
//
//        return dest;
//
//    }
//
//};



//namespace wserializer {
//    extern el::Logger *wslogger;
//}

#endif //LSERIALIZER_SERIALIZER_H



