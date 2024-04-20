#include <utility>

#include "test_base.h"

/**
 * This test suite contains tests for the EventsLib library.
 */
class EventsTestSuiteEvents : public EventsTestSuite {
protected:
    void SetUp() override {
        testData = new EventsLib::EventData{{"first",  std::string("firstData")},
                                            {"second", std::string("secondData")}};
        testEmitter = std::make_shared<EventsLib::EventEmitter>("TestEmitter");
    }

    void TearDown() override {
        delete testData;
    }
};

/**
 * This test checks if an emitter can be registered and retrieved correctly.
 */
TEST_F(EventsTestSuiteEvents, Add_Emitter) {
    // Register an emitter
    EventsLib::registerEmitter(testEmitter);

    // Get the emitter
    ASSERT_EQ(EventsLib::getEmitter("TestEmitter"), testEmitter);
}

/**
 * This test checks if a listener can be registered and triggered correctly.
 */
TEST_F(EventsTestSuiteEvents, Register_And_Trigger_Listener) {
    // Register a listener
    testEmitter->On("testEvent", [&](EventsLib::EventData data) {
        testEventHandler(std::move(data));
    });
    // Emit an event
    testEmitter->Emit("testEvent", *testData);

    // Check if the listener was called
    ASSERT_TRUE(testEventHandlerDataOk);
}

/**
 * This test checks the functionality of the global emitter.
 */
TEST_F(EventsTestSuiteEvents, Global_Emitter_Functionality) {
    // Set the global emitter
    EventsLib::setGlobalEmitter(testEmitter);
    // Register a listener to the global emitter
    EventsLib::globalOn("globalEvent", [&](EventsLib::EventData data) {
        testEventHandler(std::move(data));
    });
    // Emit an event using the global emitter
    EventsLib::globalEmit("globalEvent", *testData);

    // Check if the listener was called
    ASSERT_TRUE(testEventHandlerDataOk);
}

/**
 * This test checks if an emitter can be removed correctly.
 */
TEST_F(EventsTestSuiteEvents, Remove_Emitter) {
    // Register an emitter
    EventsLib::registerEmitter(testEmitter);
    // Remove the emitter
    EventsLib::removeEmitter("TestEmitter");

    // Attempt to get the removed emitter, the pointer should be equal to nullptr
    const std::shared_ptr<EventsLib::EventEmitter> &ptr = EventsLib::getEmitter("TestEmitter");
    ASSERT_EQ(ptr.get(), nullptr);
}

TEST_F(EventsTestSuiteEvents, Set_And_Get_Global_Emitter) {
    EventsLib::removeEmitter("TestEmitter");
    // Set the global emitter
    EventsLib::setGlobalEmitter(testEmitter);
    // Get the global emitter
    auto globalEmitter = EventsLib::getGlobalEmitter();
    // Check if the retrieved emitter is the same as the set one
    ASSERT_EQ(globalEmitter, testEmitter);
}

TEST_F(EventsTestSuiteEvents, Global_Emitter_Creation) {
    EventsLib::removeEmitter("TestEmitter");
    // Attempt to get the global emitter, which should create a default one
    auto globalEmitter = EventsLib::getGlobalEmitter();
    // Check if the global emitter is not null
    ASSERT_NE(globalEmitter, nullptr);
    // Check if the global emitter's name is "Global"
    ASSERT_EQ(globalEmitter->getName(), "Global");
}

TEST_F(EventsTestSuiteEvents, Global_Event_Emission) {
    // Set the global emitter
    EventsLib::setGlobalEmitter(testEmitter);
    // Register a listener to the global emitter
    EventsLib::globalOn("globalEvent", [&](EventsLib::EventData data) {
        testEventHandlerCalled = true;
    });
    // Emit a global event
    EventsLib::globalEmit("globalEvent", *testData);
    // Check if the listener was called
    ASSERT_TRUE(testEventHandlerCalled);
}

TEST_F(EventsTestSuiteEvents, Global_Event_Emission_Without_Data) {
    // Set the global emitter
    EventsLib::setGlobalEmitter(testEmitter);
    // Register a listener to the global emitter
    EventsLib::globalOn("globalEventWithoutData", [&](EventsLib::EventData data) {
        testEventHandlerCalled = true;
    });
    // Emit a global event without data
    EventsLib::globalEmit("globalEventWithoutData");
    // Check if the listener was called
    ASSERT_TRUE(testEventHandlerCalled);
}



