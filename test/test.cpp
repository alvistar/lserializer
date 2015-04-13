#include "google/gtest/include/gtest/gtest.h"
#include "../SerializerJSON.h"

INITIALIZE_EASYLOGGINGPP

class DeserializeTest : public ::testing::Test {
protected:
    virtual void SetUp() {

    }
};

TEST(DeserializeJSON, array) {
    Deserializer d;
    auto &obj = d.deserialize({"[\"pino\",\"cedro\",1,2,3]"});
    ASSERT_STREQ("pino",obj.toArray().objAtIndex(0).toString().c_str());
    ASSERT_STREQ("cedro",obj.toArray().objAtIndex(1).toString().c_str());
    ASSERT_STREQ("3",obj.toArray().objAtIndex(4).toString().c_str());
}

TEST(DeserializeJSON, dict) {
    Deserializer d;
    auto &obj = d.deserialize("{\"pino\":1,\"cedro\":2}");
    ASSERT_STREQ("1",obj.toDict().objForKey("pino").toString().c_str());
    ASSERT_STREQ("2",obj.toDict().objForKey("cedro").toString().c_str());
}

TEST(DeserializeJSON, size) {
    Deserializer d;
    auto &obj = d.deserialize("[]");
    ASSERT_EQ (0,obj.toArray().size());
    char const* s = "[1,\"pino\",2,3,\"cedro\"]";
    auto &obj2 = d.deserialize(s);
    ASSERT_EQ (5,obj2.toArray().size());
    auto &obj3 = d.deserialize("[1,2,[4,5,6],[8,[9,10,11]]]");
    ASSERT_EQ(4, obj3.toArray().size());
}

TEST(DeserializeJSON, conversions) {
    Deserializer d;
    auto &obj = d.deserialize("[7,2.5,3]");
    auto &a = obj.toArray();
    int i = a.objAtIndex(0);
    float f = a.objAtIndex(1);
    ASSERT_EQ(7,i);
    ASSERT_EQ(2.5,f);

}

TEST(SerializeJSON, dict) {
    WSObjDictMutable dict {};
    dict.insert({"pino",1});
    dict.insert({"cedro",2});
    dict.insert({"erba","verde"});
    ASSERT_STREQ("{\"erba\":\"verde\", \"cedro\":2, \"pino\":1}", dict.toString().c_str());
}

int main(int argc, char **argv) {
//    el::Configurations defaultConf;
//    defaultConf.setToDefault();
//    defaultConf.set(el::Level::Global, el::ConfigurationType::Enabled, "false");
//    el::Loggers::reconfigureAllLoggers(defaultConf);

    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}