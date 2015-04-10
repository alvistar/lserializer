#include "google/gtest/include/gtest/gtest.h"
#include "../SerializerJSON.h"

class DeserializeTest : public ::testing::Test {
protected:
    virtual void SetUp() {

    }
};

TEST(DeserializeJSON, array) {
    auto obj = WSObj_JSON::factory("[\"pino\",\"cedro\",1,2,3]");
    ASSERT_STREQ("pino",obj->toArray()->objAtIndex(0)->toString().c_str());
    ASSERT_STREQ("cedro",obj->toArray()->objAtIndex(1)->toString().c_str());
    ASSERT_STREQ("3",obj->toArray()->objAtIndex(4)->toString().c_str());
}

TEST(DeserializeJSON, dict) {
    auto obj = WSObj_JSON::factory("{\"pino\":1,\"cedro\":2}");
    ASSERT_STREQ("1",obj->toDict()->objForKey("pino")->toString().c_str());
    ASSERT_STREQ("2",obj->toDict()->objForKey("cedro")->toString().c_str());
}

TEST(DeserializeJSON, size) {
    auto obj = WSObj_JSON::factory("[]");
    ASSERT_EQ (0,obj->toArray()->size());
    char const* s = "[1,\"pino\",2,3,\"cedro\"]";
    obj = WSObj_JSON::factory(s);
    ASSERT_EQ (5,obj->toArray()->size());
    obj = WSObj_JSON::factory("[1,2,[4,5,6],[8,[9,10,11]]]");
    ASSERT_EQ(4, obj->toArray()->size());
}

TEST(SerializeJSON, dict) {
    WSObjDictMutable dict {};
    dict.insert({"pino",1});
    dict.insert({"cedro",2});
    dict.insert({"erba","verde"});
    ASSERT_STREQ("{\"erba\":\"verde\", \"cedro\":2, \"pino\":1}", dict.toString().c_str());
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}