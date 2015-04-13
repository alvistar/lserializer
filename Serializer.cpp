#include "Serializer.h"

namespace wserializer {
    el::Logger * wslogger = el::Loggers::getLogger("wserializer");
}

WSObjArray &WSObj::toArray() {
    if (this->objType() != WS_TYPE_ARRAY)
        throw WSObjectIsNotArrayException();

    return dynamic_cast<WSObjArray&> (*this);
}

WSObjDict &WSObj::toDict() {
    if (this->objType() != WS_TYPE_DICT)
        throw WSObjectIsNotDictException();
    return dynamic_cast<WSObjDict&> (*this);
}

WSObjNumeric &WSObj::toNumeric() const {
    if (this->objType() != WS_TYPE_NUMBER)
        throw WSObjectIsNotNumException();

    return (WSObjNumeric&) dynamic_cast<const WSObjNumeric&> (*this);
}

