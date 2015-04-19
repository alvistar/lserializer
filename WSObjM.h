//
// Created by Alessandro Viganò on 19/04/15.
//
#include "Serializer.h"

#ifndef CPPWAMP_WSOBJM_H
#define CPPWAMP_WSOBJM_H

class WSArrayM;

class WSObjM : public virtual WSObj {
    using PTR = shared_ptr<WSObj>;
private:
    PTR obj;
    enum wamp_serializer_type _objType;
public:
    template <typename T, typename enable_if <is_arithmetic<T>::value, int>::type = 0>
    WSObjM(const T &v) {
        obj = shared_ptr<WSObj> (new WSNumericM<T>(v));
        _objType = WS_TYPE_NUMBER;
    }

    WSObjM(const string &s) {
        obj = shared_ptr<WSObj> (new WSString(s));
        _objType = WS_TYPE_STRING;
    }

    WSObjM(const char* s) {
        obj = shared_ptr<WSObj> (new WSString(s));
        _objType = WS_TYPE_STRING;
    }

    WSObjM(const WSArrayM& arr);

    //Casting methods

    virtual operator WSArray() const override;

    virtual operator WSDict() const override;

    virtual operator int() const override;

    virtual operator float() const override;

    virtual operator unsigned long long int() const override;

    virtual enum wamp_serializer_type objType() const override {
        return _objType;
    }

    virtual string toString() const override {
        return obj->toString();
    }

    virtual WSObj *clone() const override {
        return new WSObjM(*this);
    }

    virtual WSObj *moveClone() override {
        return new WSObjM (move(*this));
    }
};


#endif //CPPWAMP_WSOBJM_H
