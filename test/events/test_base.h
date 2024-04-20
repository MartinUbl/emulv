#pragma once

#include <gtest/gtest.h>

#include "Events.h"

class EventsTestSuite : public ::testing::Test {
protected:
    EventsLib::EventData* testData;
    std::shared_ptr<EventsLib::EventEmitter> testEmitter;

    bool testEventHandlerCalled;
    bool testEventHandlerDataOk;

    void testEventHandler(EventsLib::EventData data) {
        testEventHandlerCalled = true;
        if(std::any_cast<std::string>(data.getData("first")) == "firstData")
            testEventHandlerDataOk = true;
    }
};
