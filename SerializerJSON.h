//
// Created by Alessandro Viganò on 02/04/15.
//

#include "Serializer.h"
#include <alvistar/frozen/frozen.h>
#include <vector>
#include <iostream>

#ifndef LSERIALIZER_SERIALIZERJSON_H
#define LSERIALIZER_SERIALIZERJSON_H


class WSObj_JSON:virtual public WSObj {
protected:
    struct json_token *token;
public:
    WSObj_JSON(struct json_token *token):token(token) {};
    static shared_ptr<WSObj> factory(char const *msg);
    static shared_ptr<WSObj> factory(struct json_token *token);
    //virtual enum wamp_serializer_type objType();
    virtual string toString() const override = 0;
};

class WSObjNumeric_JSON:virtual public WSObj_JSON, virtual public WSObjNumeric {

public:
    WSObjNumeric_JSON(json_token *token) : WSObj_JSON(token) { }

    virtual WSObj *clone() const override;

    virtual WSObj *moveClone() override;

    virtual string toString() const override;
};

class WSObjString_JSON :virtual public WSObj_JSON, virtual public WSObjString {

public:
    WSObjString_JSON(json_token *token) : WSObj_JSON(token) { }

    virtual string toString() const override;
};


class WSObjArray_JSON:public virtual WSObj_JSON, virtual public WSObjArray {

public:

    virtual string toString() const override;

    WSObjArray_JSON(json_token *token) : WSObj_JSON(token) { };
    virtual shared_ptr<WSObj> objAtIndex(int const &index);
    virtual int size() override;
    virtual WSObj * clone() const override {
        return new WSObjArray_JSON (*this);
    }

    virtual WSObj *moveClone() override {
        return new WSObjArray_JSON(move(*this));
    }
};

class WSObjDict_JSON:public virtual WSObj_JSON, virtual public WSObjDict {

public:
    WSObjDict_JSON(json_token *token) : WSObj_JSON(token) { }
    virtual string toString() const override;
    virtual shared_ptr<WSObj> objForKey(string const &key) override;

    virtual WSObj *clone() const override;

    virtual WSObj *moveClone() override;
};


#endif //LSERIALIZER_SERIALIZERJSON_H



