#include "Serializer.h"
#include "WSArray.h"
#include "WSDict.h"

namespace wserializer {
    el::Logger * wslogger = el::Loggers::getLogger("wserializer");
}

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
