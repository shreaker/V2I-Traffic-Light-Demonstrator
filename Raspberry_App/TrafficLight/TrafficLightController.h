#ifndef TRAFFICLIGHTCONTROLLER_TRAFFICLIGHTCONTROLLER_H
#define TRAFFICLIGHTCONTROLLER_TRAFFICLIGHTCONTROLLER_H

#include <boost/statechart/state_machine.hpp>
#include <boost/statechart/simple_state.hpp>
#include <boost/statechart/state.hpp>
#include <boost/statechart/event.hpp>
#include <boost/statechart/transition.hpp>
#include <boost/statechart/custom_reaction.hpp>
#include <boost/mpl/list.hpp>

#include <thread>
#include <mutex>
#include <memory>

#include "Events.h"

#include "Pin.h"

namespace sc = boost::statechart;
namespace mpl = boost::mpl;

struct NormalOperation;

struct TrafficLightController : sc::state_machine<TrafficLightController, NormalOperation> {
    TrafficLightController();

    void process_event_synced(const sc::event_base& evt);

    template<class Event>
    void process_event_delayed_synced(int ms, const Event& evt) {
        const Event* ptr = new Event(evt);
        std::thread([ms, ptr, this] {
            std::this_thread::sleep_for(std::chrono::milliseconds(ms));
            process_event_synced(*ptr);
            delete ptr;
        }).detach();

    }

private:
    enum Light {
        LIGHT_CAR_RED, LIGHT_CAR_YELLOW, LIGHT_CAR_GREEN, LIGHT_PEDESTRIAN_RED, LIGHT_PEDESTRIAN_GREEN, LIGHT_BUTTON
    };
    void switchLight(Light light, bool on);

    std::mutex mutex;
    std::unique_ptr<std::map<int, Pin>> pins;

    void handleButton();

    friend class NormalOperation;
    friend class Idle;
    friend class VehiclesEnabled;
    friend class VehiclesRedYellow;
    friend class VehiclesGreen;
    friend class VehiclesGreenYellow;
    friend class PedestriansEnabled;
    friend class PedestriansGreen;
    friend class VehiclesGreenExtended;
};

struct Idle;

struct NormalOperation : sc::simple_state<NormalOperation, TrafficLightController, Idle> {
    NormalOperation();

    virtual ~NormalOperation();

    typedef mpl::list <
    sc::custom_reaction<EvCarsDetected>,
    sc::custom_reaction<EvPedestriansDetected>,
    sc::custom_reaction<EvPriority>,
    sc::custom_reaction<EvGetTrafficLightState>,
    sc::custom_reaction<EvGetNextPedestrianPhaseTime>
    > reactions;

    sc::result react(const EvCarsDetected&);

    sc::result react(const EvPedestriansDetected&);

    sc::result react(const EvPriority&);

    sc::result react(const EvGetTrafficLightState&);

    sc::result react(const EvGetNextPedestrianPhaseTime&);

    unsigned int detectedCars = 0;
    Pedestrian detectedPedestrian = PEDESTRIAN_NONE;
    bool isPublicTransport = false;
private:
    friend class Idle;
};

struct EmergencyOperation : sc::state<EmergencyOperation, TrafficLightController> {
    EmergencyOperation(my_context ctx);

    virtual ~EmergencyOperation();

    typedef mpl::list <
    sc::transition<EvNormalOperation, NormalOperation>,
    sc::custom_reaction<EvGetTrafficLightState>
    >
    reactions;

    sc::result react(const EvGetTrafficLightState&);
};

struct Idle : sc::state<Idle, NormalOperation> {
    Idle(my_context ctx);

    virtual ~Idle();

    typedef mpl::list <
    sc::custom_reaction<EvPedestriansDetected>,
    sc::custom_reaction<EvCarsDetected>
    > reactions;

    sc::result react(const EvPedestriansDetected&);

    sc::result react(const EvCarsDetected&);
};


struct ITimeLeft {
    virtual int timeLeft() const = 0;
};

#endif //TRAFFICLIGHTCONTROLLER_TRAFFICLIGHTCONTROLLER_H
