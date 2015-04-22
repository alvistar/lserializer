#include "google/gtest/include/gtest/gtest.h"
#include "../SerializerJSON.h"
#include "../WSDict.h"

INITIALIZE_EASYLOGGINGPP



TEST(DeserializeJSON, array) {
    Deserializer d;
    auto &obj = d.deserialize({"[\"pino\",\"cedro\",1,2,3]"});
    ASSERT_STREQ("pino", obj.toArray().at(0).toString().c_str());
    ASSERT_STREQ("cedro", obj.toArray().at(1).toString().c_str());
    ASSERT_STREQ("3", obj.toArray().at(4).toString().c_str());
}

TEST(DeserializeJSON, bool) {
    Deserializer d;
    auto &obj = d.deserialize({"[true, false, true]"});
    ASSERT_EQ(true, (bool) obj.toArray().at(0));
    ASSERT_EQ(false, (bool) obj.toArray().at(1));
    ASSERT_EQ(true, (bool) obj.toArray().at(2));
}

TEST(DeserializeJSON, dict) {
    Deserializer d;
    auto &obj = d.deserialize("{\"pino\":1,\"cedro\":2}");
    ASSERT_STREQ("1",obj.toDict().at("pino").toString().c_str());
    ASSERT_STREQ("2",obj.toDict().at("cedro").toString().c_str());
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
    int i = a.at(0);
    float f = a.at(1);
    ASSERT_EQ(7,i);
    ASSERT_EQ(2.5,f);

}

TEST(SerializeJSON, dict) {
    Deserializer d;

    WSDictM dict {};
    dict.insert({"pino",1});
    dict.insert({"cedro",2});
    dict.insert({"erba","verde"});
    dict.insert({{"Color","Red"},{"Size","Large"}});

    auto &obj = d.deserialize(dict.toString());
    ASSERT_STREQ("Large", obj.toDict().at("Size").toString().c_str());
    ASSERT_STREQ("Red", obj.toDict().at("Color").toString().c_str());

    WSDictM dict2 {{"Color","Red"}};
    ASSERT_STREQ("{\"Color\":\"Red\"}",dict2.toString().c_str());

    WSDictM dict3 {{"Color","Red"}};
    ASSERT_STREQ("{\"Color\":\"Red\"}",dict3.toString().c_str());

    WSDictM dict4 {{"Color","Red"}, {"Size","Large"}};
    auto &obj4 = d.deserialize(dict4.toString());
    ASSERT_STREQ("Large", obj4.toDict().at("Size").toString().c_str());
    ASSERT_STREQ("Red", obj4.toDict().at("Color").toString().c_str());

};

//
TEST(SerializeJSON, casting) {
    WSArrayM arr {1,2.5, "Pino"};
    int first = arr.at(0);
    string s = arr.at(2);
    ASSERT_EQ(1, first);
    ASSERT_STREQ("Pino",s.c_str());

    float second = arr.at(1);
    ASSERT_EQ(2.5, second);

    auto tup = arr.toTuple<int,int,string>();
    ASSERT_EQ(1, std::get<0>(tup));
    ASSERT_EQ(2, std::get<1>(tup));
    ASSERT_STREQ("Pino", std::get<2>(tup).c_str());

}

TEST(WArrayM, copying) {

    WSArrayM arr {1,2,3};
    //WSArrayM arr {1,2.5, "Pino"};

    WSArrayM  arr2 (arr);

    ASSERT_STREQ(arr.toString().c_str(), arr2.toString().c_str());
}

TEST(WDictM, addingIntegers) {
    Deserializer d;

    WSDictM dict {{"height","25"}};
    dict.insert({"width","32"});

    auto &obj = d.deserialize(dict);

    ASSERT_STREQ(obj.toString().c_str(), dict.toString().c_str());
}

TEST(WDictM, addingDict) {
    Deserializer d;

    WSDictM dict {{"height","25"}};
    WSDictM dict2 {{"apple","25"}};

    dict.insert({"fruits",dict2});

    auto &obj = d.deserialize(dict);

    ASSERT_STREQ(obj.toString().c_str(), dict.toString().c_str());
}

TEST(WArrayM, addingIntegers) {
    WSArrayM arr {1,2,3};
    ASSERT_STREQ("[1, 2, 3]", arr.toString().c_str());

    arr.push_back(4);
    arr.push_back(5);

    ASSERT_STREQ("[1, 2, 3, 4, 5]", arr.toString().c_str());
}

TEST(WArrayM, addingStrings) {
    WSArrayM arr {"red","blue","green"};
    ASSERT_STREQ("[\"red\", \"blue\", \"green\"]", arr.toString().c_str());

    arr.push_back("white");
    arr.push_back("black");
    ASSERT_STREQ("[\"red\", \"blue\", \"green\", \"white\", \"black\"]", arr.toString().c_str());
}

TEST(WArrayM, addingDict) {
    WSArrayM arr {1,2,3};
    WSArrayM arr2 {4,5,6};
    arr.push_back(arr2);
    ASSERT_STREQ("[1, 2, 3, [4, 5, 6]]", arr.toString().c_str());
}

TEST(WArrayM, addingMixed) {
    WSArrayM arr {1,2,3, "red", "blue", WSArrayM {4,5,6}};
    ASSERT_STREQ("[1, 2, 3, \"red\", \"blue\", [4, 5, 6]]", arr.toString().c_str());
    arr.push_back("white");
    arr.push_back("black");

    ASSERT_STREQ("[1, 2, 3, \"red\", \"blue\", [4, 5, 6], \"white\", \"black\"]", arr.toString().c_str());
}


int main(int argc, char **argv) {
//    el::Configurations defaultConf;
//    defaultConf.setToDefault();
//    defaultConf.set(el::Level::Global, el::ConfigurationType::Enabled, "false");
//    el::Loggers::reconfigureAllLoggers(defaultConf);

    testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}