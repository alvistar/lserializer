//
// Created by Alessandro Viganò on 23/04/15.
//
#include "Serializer.h"

#ifndef CPPWAMP_WSOBJ_H
#define CPPWAMP_WSOBJ_H

class WSArray;
class WSDict;

class WSObj  {
public:

    virtual ~WSObj() {};

    virtual enum wamp_serializer_type objType() const = 0;

    virtual WSArray & toArray() const;

    virtual WSDict & toDict() const;

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

    operator WSArray&() const;

    operator WSDict&() const;

    virtual operator bool() const {
        throw WSObjectIsNotBoolException();
    }

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


#endif //CPPWAMP_WSOBJ_H
