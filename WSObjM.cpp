//
// Created by Alessandro Viganò on 19/04/15.
//

#include "WSObjM.h"
#include "WSArray.h"
#include "WSDict.h"




WSObjM::WSObjM(const WSArray & arr) {
     obj = shared_ptr<WSObj> (arr.clone());
    _objType = WS_TYPE_ARRAY;
};


WSObjM::WSObjM(const WSDict &dict) {
    obj = shared_ptr<WSObj> (dict.clone());
    _objType = WS_TYPE_DICT;
}


WSObjM::operator int() const {
    return (int) *obj;
}

 WSObjM::operator float() const {
    return (float) *obj;
}

 WSObjM::operator unsigned long long int() const {
    return (unsigned long long int) *obj;
}


WSArray &WSObjM::toArray() const {
    return obj->toArray();
}

WSDict &WSObjM::toDict() const {
    return obj->toDict();
}

