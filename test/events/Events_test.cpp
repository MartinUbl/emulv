//
// Created by xPC on 13.09.2023.
//

#include "test_base.h"

class EventsTestSuiteEvents : public EventsTestSuite {
protected:
    void SetUp() override {
        testData = new EventsLib::EventData{{"first",  std::string("firstData")},
                                            {"second", std::string("secondData")}};
    }

    void TearDown() override {
        delete testData;
    }
};

TEST_F(EventsTestSuiteEvents, Add_emitter) {
    auto add = std::make_shared<EventsLib::EventEmitter>("FirstAdded");
    EventsLib::registerEmitter(add);

    ASSERT_EQ(EventsLib::getEmitter("FirstAdded"), add);
}

