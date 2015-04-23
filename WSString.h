//
// Created by Alessandro Viganò on 23/04/15.
//

#ifndef CPPWAMP_WSSTRING_H
#define CPPWAMP_WSSTRING_H
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
#endif //CPPWAMP_WSSTRING_H
