//
// Created by Alessandro Viganò on 23/04/15.
//

#include "WSObj.h"

WSArray &WSObj::toArray() const {
    throw WSObjectIsNotArrayException();
}

WSDict &WSObj::toDict() const {
    throw WSObjectIsNotDictException();
}

WSObj::operator WSArray&() const {
    return this->toArray();
}

WSObj::operator WSDict&() const {
    return this->toDict();
}