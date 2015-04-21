//
// Created by Alessandro Viganò on 19/04/15.
//

#ifndef CPPWAMP_WSDICT_H
#define CPPWAMP_WSDICT_H

class WSDict :public virtual WSObj {
public:
    virtual ~WSDict() { }

    virtual WSObj &at(string const &key) = 0;
    virtual enum wamp_serializer_type objType() const override {
        return WS_TYPE_DICT;
    }

    virtual WSDict &toDict() const override {
        return (WSDict&) *this;
    }
};

class WSDictM : public virtual WSDict {
private:
    unordered_map <string, WSObjM> data;
public:
    WSDictM() {};
    WSDictM(std::initializer_list<pair<string,WSObjM>> list) {
        for (auto&e: list) {
            data.insert(e);
        }
    }

    virtual WSObj * clone() const override {
        return new WSDictM(*this);
    }

    virtual WSObj *moveClone() override {
        return new WSDictM(move(*this));
    }

    void insert(pair<string,WSObjM> e)
    {
        data.insert(e);
    }

    void insert(std::initializer_list<pair<string,WSObjM>> list) {
        for (auto&e: list) {
            data.insert(e);
        }
    }

    virtual WSObj &at(string const &key) override {
        return data.at(key);
    }

    virtual string toString() const override {
        ostringstream oss;
        oss << "{";
        bool first {true};
        for (auto &kv: data) {
            if (!first)
                oss << ", ";
            oss << "\"" << kv.first << "\"" << ":";
            if (kv.second.objType() == WS_TYPE_STRING)
                oss << "\"";

            oss << kv.second;

            if (kv.second.objType() == WS_TYPE_STRING)
                oss << "\"";

            first = false;
        }

        oss << "}";
        return oss.str();
    }

};


//class WSDictM : public virtual WSDict {
//private:
//    unordered_map <string, unique_ptr<WSObj>> data;
//    void _insert(const WSPair &p) {
//        data[p.first] =  unique_ptr<WSObj> (p.second->clone());
//    }
//
//    void _insert(WSPair &&p) {
//        data[p.first] = move(p.second);
//    }
//
//public:
//
//    //Constructors
//    WSDictM() {};
//
//    //Copy Constructor
//    WSDictM(const WSDictM &o) {
//        CLOG(DEBUG, "wserializer") << "Dict deep copy:"<< o;
//
//        for (auto &kv: o.data) {
//            data[kv.first] = unique_ptr<WSObj> (kv.second->clone());
//        }
//    }
//    //Move Constructor
//    WSDictM(WSDictM &&o): data(move(o.data)) {};
//
//    //Constructor from pair
//    WSDictM(WSPair &&p) {
//        insert(move(p));
//    }
//
//    template <typename ... ARGS>
//    WSDictM(ARGS&& ... args) {
//        insert(forward<ARGS> (args) ...);
//    }
//
//    virtual WSObj *clone() const override {
//        return new WSDictM(*this);
//    }
//
//    virtual WSObj *moveClone() override {
//        return new WSDictM(move(*this));
//    }
//
////Methods
//    virtual WSObj & objForKey(string const &key) override {
//        return * data[key];
//    }
//
//    //To catch implicit casting
//    void insert(const WSPair &p) {_insert(p);};
//    void insert(WSPair &&p) {_insert(move(p));};
//
//    void insert(vector<reference_wrapper<WSPair>> vec) {
//        cout << "PRESO!" << endl;
//    }
//
//
//    template <typename T>
//    void insert(T&& e) {
//        _insert(forward<T> (e));
//    }
//
//    template <typename T, typename ... OTHERS>
//    void insert(T&& first, OTHERS&& ...others) {
//        insert(forward<T>(first));
//        insert(forward<OTHERS>(others) ...);
//    }
//
//    WSDictM & operator << (WSPair &&pair) {
//        insert(move(pair));
//        return *this;
//    }
//
//    virtual string toString() const override {
//        ostringstream oss;
//        oss << "{";
//        bool first {true};
//        for (auto &kv: data) {
//            if (!first)
//                oss << ", ";
//            oss << "\"" << kv.first << "\"" << ":";
//            if (kv.second->objType() == WS_TYPE_STRING)
//                oss << "\"";
//
//            oss << * kv.second;
//
//            if (kv.second->objType() == WS_TYPE_STRING)
//                oss << "\"";
//
//            first = false;
//        }
//
//        oss << "}";
//        return oss.str();
//    }
////
//};

#endif //CPPWAMP_WSDICT_H

