//
// Created by Alessandro Viganò on 19/04/15.
//
#include "Serializer.h"
#include "WSObj.h"
#include "WSBool.h"
#include "WSNumeric.h"
#include "WSString.h"

#ifndef CPPWAMP_WSOBJM_H
#define CPPWAMP_WSOBJM_H

class WSArrayM;
class WSDictM;

class WSObjM : public virtual WSObj {
    using PTR = shared_ptr<WSObj>;
private:
    PTR obj;
    enum wamp_serializer_type _objType;
public:
    WSObjM() {
        _objType = WS_TYPE_NULL;
    }

    WSObjM(bool b) {
        obj = shared_ptr<WSObj> (new WSBool(b));
        _objType = WS_TYPE_BOOL;
    }

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

    WSObjM(const WSArray& arr);

    WSObjM(const WSDict& arr);

    //Casting methods

    virtual WSArray &toArray() const override;

    virtual WSDict &toDict() const override;


    virtual operator bool() const override;

    virtual operator int() const override;

    virtual operator float() const override;

    virtual operator unsigned long long int() const override;

    virtual enum wamp_serializer_type objType() const override {
        return _objType;
    }

    virtual string toString() const override {
        if (_objType == WS_TYPE_NULL) {
            return "null";
        }
        else
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
