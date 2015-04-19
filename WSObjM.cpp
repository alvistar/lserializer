//
// Created by Alessandro Viganò on 19/04/15.
//

#include "WSObjM.h"
#include "WSArray.h"
#include "WSDict.h"

WSObjM::WSObjM(const WSArrayM & arr) {
    obj = shared_ptr<WSObj> (new WSArrayM(arr));
    _objType = WS_TYPE_ARRAY;
};

WSObjM::operator WSArray() const {
    return obj->toArray();
}

WSObjM::operator WSDict() const {
    return obj->toDict();
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
