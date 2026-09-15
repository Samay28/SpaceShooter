#pragma once
#include "Event.h"

#include <cstdint>
#include <functional>
#include <memory>
#include <typeindex>
#include <unordered_map>
#include <vector>

class EventBus
{
public:
    // Every subscriber gets a unique ID.
    //this id can be used to unsubscribe from the event bus
    using SubscriptionID = uint64_t;

private:
    //subscriber
    struct Subscriber
    {
        SubscriptionID id;
        std::function<void(const Event&)> callback;
    };

public:
    //subscribe
    //register a callback for a specific event type

    template<typename T>
    SubscriptionID Subscribe(std::function<void(const T&)> callback)
    {
        //Every event type get its own subscriber list

        //type_index is a wrapper around type_info that allows it to be used as a key in associative containers like unordered_map. 
        // It provides a way to compare types at runtime.
        const std::type_index eventType = std::type_index(typeid(T));

        //Generate a unique ID for this subscription
        const SubscriptionID id = m_nextSubscriptionID++;

        //wrap the typed callback inside a generic callback
        Subscriber subscriber{};
        sunscriber.id = id;
    }
};