#include "test_base.h"

class EventsTestSuiteEmitter : public EventsTestSuite {
protected:
    void SetUp() override {
        testData = new EventsLib::EventData{{"first",  std::string("firstData")},
                                            {"second", std::string("secondData")}};
    }

    void TearDown() override {
        delete testData;
    }
};

TEST_F(EventsTestSuiteEmitter, Get_firtDefaultData) {
    ASSERT_EQ(std::any_cast<std::string>(testData->getData("first")), "firstData");
}

TEST_F(EventsTestSuiteEmitter, Get_secondDefaultData) {
    ASSERT_EQ(std::any_cast<std::string>(testData->getData("second")), "secondData");
}

TEST_F(EventsTestSuiteEmitter, Get_dataMapDefaultDataFirst) {
    ASSERT_EQ(std::any_cast<std::string>(testData->getData()["first"]), "firstData");
}

TEST_F(EventsTestSuiteEmitter, Get_dataMapDefaultDataSecond) {
    ASSERT_EQ(std::any_cast<std::string>(testData->getData()["second"]), "secondData");
}

TEST_F(EventsTestSuiteEmitter, Adding_data) {
    testData->addData("NewAdded", std::string("ValueOfNewAdded"));
    ASSERT_EQ(std::any_cast<std::string>(testData->getData("NewAdded")), "ValueOfNewAdded");
}

TEST_F(EventsTestSuiteEmitter, Removing_data) {
    testData->removeData("first");
    ASSERT_THROW(std::any_cast<std::string>(testData->getData("NewAdded")), std::bad_any_cast);
}
