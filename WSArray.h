#include "Serializer.h"
#include "WSObjM.h"

#ifndef LSERIALIZER_WSARRAY_H
#define LSERIALIZER_WSARRAY_H

class WSArray :public virtual WSObj {
public:
    virtual ~WSArray() { }
    virtual WSObj &at(int const &index) = 0; //Cannot be const for Json Implementation
    virtual int size() const = 0;
    virtual enum wamp_serializer_type objType() const override {
        return WS_TYPE_ARRAY;
    }

    virtual WSArray &toArray() const override {
        return (WSArray&) *this;
    }
};

class WSArrayM: public virtual WSArray {
private:
    vector<WSObjM> data;
public:
    WSArrayM() {};

    WSArrayM(std::initializer_list<WSObjM> list) {
        for (auto& e:list) {
            data.push_back(e);
        }
    }

    virtual WSObj *clone() const override {
        return new WSArrayM(*this);
    }

    virtual WSObj *moveClone() override {
        return new WSArrayM (move(*this));
    }

    virtual WSObj&at(int const &index) override {
        return (WSObj&) data[index];
    }

    void push_back(WSObjM const& e) {
        data.push_back(e);
    }

    virtual int size() const override {
        return (int) data.size();
    }

    void clear() {
        data.clear();
    }

    virtual string toString() const override {
        ostringstream oss;
        oss << "[";
        bool first = true;
        for (auto &e:data) {
            if (!first)
                oss << ", ";
            if (e.objType() == WS_TYPE_STRING)
                oss << "\"";

            oss << e.toString();

            if (e.objType() == WS_TYPE_STRING)
                oss << "\"";

            first = false;
        }
        oss << "]";
        return oss.str();
    }

    template<std::size_t I = 0, typename... Tp>
    inline typename std::enable_if<I == sizeof...(Tp), void>::type
    convert_tuple(std::tuple<Tp...> &) // Unused arguments are given no names.
    { }

    template<std::size_t I = 0, typename... Tp>
    inline typename std::enable_if<I < sizeof...(Tp), void>::type
    convert_tuple(std::tuple<Tp...>& t)
    {
        using T = typename std::tuple_element<I, std::tuple<Tp...>>::type;

        T &el = std::get<I>(t);
        el = static_cast<T> (at(I));

        convert_tuple<I + 1, Tp...>(t);
    }

    template <typename ... Ts>
    tuple <Ts ...> toTuple() {

        tuple<Ts...> dest;

        convert_tuple(dest);

        return dest;

    }

};

#endif //LSERIALIZER_WSARRAY_HWSArray& WSArray::toArray(){


