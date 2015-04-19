//
// Created by Alessandro Viganò on 02/04/15.
//

#include "Serializer.h"
#include "WSArray.h"
#include "WSDict.h"

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
    virtual ~WSObj_JSON() {};

    static unique_ptr<WSObj> factory(const char* msg);
    static unique_ptr<WSObj> factory(struct json_token *token);
    virtual string toString() const override = 0;

};

class WSObjNumeric_JSON:virtual public WSObj_JSON, virtual public WSNumeric {

public:
    WSObjNumeric_JSON(json_token *token) : WSObj_JSON(token) { }
    virtual ~WSObjNumeric_JSON() { }

    WSObj *clone() const override;

    WSObj *moveClone() override;
    string toString() const override;

    operator int() const override;
    operator float() const override;
    operator unsigned long long int() const override;
};

class WSObjString_JSON :virtual public WSObj_JSON, virtual public WSString {

public:
    WSObjString_JSON(json_token *token) : WSObj_JSON(token) { }

    virtual ~WSObjString_JSON() { }

    virtual string toString() const override;

};

class WSObjArray_JSON:public virtual WSObj_JSON, virtual public WSArray {
private:
    unordered_map<int,unique_ptr<WSObj>> data;

public:
    //Constructors
    WSObjArray_JSON(json_token *token) : WSObj_JSON(token) { };
    WSObjArray_JSON(const WSObjArray_JSON & o):WSObj_JSON(o.token) {};
    virtual ~WSObjArray_JSON() { }

//Methods
    virtual string toString() const override;
    virtual WSObj & at(int const &index) override;
    virtual int size() override;

    virtual WSObj * clone() const override;
    virtual WSObj *moveClone() override;
};

class WSObjDict_JSON:public virtual WSObj_JSON, virtual public WSDict {
private:
    unordered_map<string,unique_ptr<WSObj>> data;
public:
    //Constructors
    WSObjDict_JSON(json_token *token) : WSObj_JSON(token) { }
    WSObjDict_JSON(const WSObjDict_JSON &o):WSObjDict_JSON(o.token) {};
    virtual ~WSObjDict_JSON() { }

    //Methods

    virtual string toString() const override;
    virtual WSObj& at(string const &key) override;

    virtual WSObj *clone() const override;

    virtual WSObj *moveClone() override;
};

//Deserializer

class Deserializer {
private:
    string input;
    unique_ptr<WSObj> root;
public:
    WSObj & deserialize(const string &input);
    WSObj & deserialize(string &&input);


    virtual ~Deserializer() { }
};

#endif //LSERIALIZER_SERIALIZERJSON_H



