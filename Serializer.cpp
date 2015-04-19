#include "Serializer.h"
#include "WSArray.h"
#include "WSDict.h"

namespace wserializer {
    el::Logger * wslogger = el::Loggers::getLogger("wserializer");
}

WSArray &WSObj::toArray() {
    throw WSObjectIsNotArrayException();

}

WSDict &WSObj::toDict() {
    throw WSObjectIsNotDictException();
}

WSObj::operator WSArray() const {
    throw WSObjectIsNotArrayException();
}

WSObj::operator WSDict() const {
    throw WSObjectIsNotDictException();
}
