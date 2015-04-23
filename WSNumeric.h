//
// Created by Alessandro Viganò on 23/04/15.
//

#ifndef CPPWAMP_WSNUMERIC_H
#define CPPWAMP_WSNUMERIC_H

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


#endif //CPPWAMP_WSNUMERIC_H
