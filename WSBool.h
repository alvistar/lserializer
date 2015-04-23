//
// Created by Alessandro Viganò on 23/04/15.
//

#ifndef CPPWAMP_WSBOOL_H
#define CPPWAMP_WSBOOL_H
class WSBool: public virtual WSObj {
private:
    bool data;
public:
    WSBool(bool v):data(v) {};

    virtual operator bool() const {
        return data;
    }

    virtual enum wamp_serializer_type objType() const override {
        return WS_TYPE_BOOL;
    }

    virtual WSObj * clone() const override {
        CLOG(DEBUG,"wserializer") << "Cloning bool "<< *this;
        return new WSBool(*this);
    }

    virtual WSObj *moveClone() override {
        return new WSBool(move(*this));
    }

    virtual string toString() const override {
        if (data)
            return "true";
        else
            return "false";
    }


};
#endif //CPPWAMP_WSBOOL_H
