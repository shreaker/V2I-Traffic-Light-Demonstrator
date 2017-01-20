#include <iostream>
#include <wiringPi.h>

#include "TrafficLightController.h"
#include "PedestriansEnabled.h"
#include "VehiclesEnabled.h"
#include "Settings.h"

TrafficLightController::TrafficLightController() {
    pins.reset(new std::map<int, Pin>());
    pins->emplace(Pin::PIN_BUTTON, Pin(Pin::PIN_BUTTON, INPUT));
    pins->emplace(Pin::PIN_BUTTON_LED, Pin(Pin::PIN_BUTTON_LED, OUTPUT));
    pins->emplace(Pin::PIN_CAR_RED, Pin(Pin::PIN_CAR_RED, OUTPUT));
    pins->emplace(Pin::PIN_CAR_YELLOW, Pin(Pin::PIN_CAR_YELLOW, OUTPUT));
    pins->emplace(Pin::PIN_CAR_GREEN, Pin(Pin::PIN_CAR_GREEN, OUTPUT));
    pins->emplace(Pin::PIN_PEDESTRIAN_RED, Pin(Pin::PIN_PEDESTRIAN_RED, OUTPUT));
    pins->emplace(Pin::PIN_PEDESTRIAN_GREEN, Pin(Pin::PIN_PEDESTRIAN_GREEN, OUTPUT));
    Pin::setCallback(std::bind(&TrafficLightController::handleButton, this));
}

void TrafficLightController::process_event_synced(const sc::event_base& evt) {
    std::lock_guard<std::mutex> guard(mutex);
    process_event(evt);
}

NormalOperation::NormalOperation() {
    std::clog << "===NormalOperation entry===" << std::endl;
}

NormalOperation::~NormalOperation() {
    std::clog << "===NormalOperation exit===" << std::endl;
}

sc::result NormalOperation::react(const EvPriority& event) {
    if(event.getPriority() == PRIORITY_EMERGENGY) {
        return transit<EmergencyOperation>();
    } else if(event.getPriority() == PRIORITY_PUBLIC_TRANSPORT) {
        isPublicTransport = true;
        context<TrafficLightController>().process_event_delayed_synced<EvPriority>(PUBLIC_TRANSPORT, EvPriority(PRIORITY_PUBLIC_TRANSPORT_DELETE));
    } else if(event.getPriority() == PRIORITY_PUBLIC_TRANSPORT_DELETE) {
        isPublicTransport = false;
    }

    return discard_event();
}

boost::statechart::result NormalOperation::react(const EvGetTrafficLightState& event) {
    TrafficLightState state;
    const PedestriansEnabled* pedestriansState = state_downcast<const PedestriansEnabled*>();
    const VehiclesEnabled* vehiclesState = state_downcast<const VehiclesEnabled*>();

    if(pedestriansState == nullptr && vehiclesState == nullptr) {
        state.pedestriansState = TrafficLightState::State::RED;
        state.vehiclesState = TrafficLightState::State::RED;
        state.pedestrianTimeLeft = -1;
        state.vehiclesTimeLeft = -1;
    } else if(pedestriansState != nullptr) {
        state.pedestriansState = TrafficLightState::State::GREEN;
        state.pedestrianTimeLeft = pedestriansState->timeLeft();
        state.vehiclesState = TrafficLightState::State::RED;

        //in pedestrian state, but cars are waiting
        if(detectedCars > 0) {
            state.vehiclesTimeLeft = pedestriansState->timeLeft();
        } else {
            state.vehiclesTimeLeft = -1;
        }
    } else if(state_downcast<const VehiclesRedYellow*>() != nullptr) {
        // handle car yellow state
        state.pedestriansState = TrafficLightState::State::RED;
        state.vehiclesState = TrafficLightState::State::YELLOW;
        state.pedestrianTimeLeft = -1;
        state.vehiclesTimeLeft = vehiclesState->timeLeft();
    } else if(vehiclesState != nullptr) {
        state.vehiclesState = TrafficLightState::State::GREEN;
        state.vehiclesTimeLeft = vehiclesState->timeLeft();
        state.pedestriansState = TrafficLightState::State::RED;

        //in vehicles state, but pedestrians are waiting
        if(detectedPedestrian != PEDESTRIAN_NONE) {
            state.pedestrianTimeLeft = vehiclesState->timeLeft();
        } else {
            state.pedestrianTimeLeft = -1;
        }
    }

    state.detectedCars = detectedCars;

    event.assign(state);
    return discard_event();
}

boost::statechart::result NormalOperation::react(const EvGetNextPedestrianPhaseTime& event) {
    int phaseDuration;
    int waitingTimeDecrease = (isPublicTransport) ? PUBLIC_TRANSPOR_TIME_DECREASE : 0;

    if(detectedPedestrian == PEDESTRIAN_NORMAL) {
        phaseDuration = PEDESTRIAN_GREEN - waitingTimeDecrease;
    } else if(detectedPedestrian == PEDESTRIAN_EXTENDED) {
        phaseDuration = PEDESTRIAN_GREEN_EXTENDED - waitingTimeDecrease;
    } else if(detectedPedestrian == PEDESTRIAN_NONE) {
        phaseDuration = -1;
    }

    event.assign(phaseDuration);
    return discard_event();
}

sc::result NormalOperation::react(const EvCarsDetected& event) {
    detectedCars = event.getCarsNumber();
    return discard_event();
}

sc::result NormalOperation::react(const EvPedestriansDetected& event) {
    detectedPedestrian = event.getPedestrian();

    if(detectedPedestrian != PEDESTRIAN_NONE) {
        context<TrafficLightController>().switchLight(TrafficLightController::LIGHT_BUTTON, true);
    }

    return discard_event();
}

Idle::Idle(my_context ctx) : my_base(ctx) {
    std::clog << "===Idle entry===" << std::endl;
    context<TrafficLightController>().switchLight(TrafficLightController::LIGHT_CAR_RED, true);
    context<TrafficLightController>().switchLight(TrafficLightController::LIGHT_PEDESTRIAN_RED, true);
}

Idle::~Idle() {
    std::clog << "===Idle exit===" << std::endl;
}

sc::result Idle::react(const EvPedestriansDetected& event) {
    context<NormalOperation>().detectedPedestrian = event.getPedestrian();

    if(event.getPedestrian() != PEDESTRIAN_NONE) {
        context<TrafficLightController>().switchLight(TrafficLightController::LIGHT_BUTTON, true);
        return transit<PedestriansEnabled>();
    } else {
        return discard_event();
    }
}

sc::result Idle::react(const EvCarsDetected& event) {
    context<NormalOperation>().detectedCars = event.getCarsNumber();

    if(event.getCarsNumber() > 0) {
        return transit<VehiclesEnabled>();
    } else {
        return discard_event();
    }
}

EmergencyOperation::EmergencyOperation(my_context ctx) : my_base(ctx) {
    std::clog << "===EmergencyOperation entry===" << std::endl;
    context<TrafficLightController>().process_event_delayed_synced<EvNormalOperation>(EMERGENCY, EvNormalOperation());

}

EmergencyOperation::~EmergencyOperation() {
    std::clog << "===EmergencyOperation exit===" << std::endl;
}

boost::statechart::result EmergencyOperation::react(const EvGetTrafficLightState& event) {
    TrafficLightState state;
    state.pedestriansState = TrafficLightState::State::RED;
    state.vehiclesState = TrafficLightState::State::RED;
    state.pedestrianTimeLeft = -1;
    state.vehiclesTimeLeft = -1;
    event.assign(state);
    return discard_event();
}

void TrafficLightController::switchLight(TrafficLightController::Light light, bool on) {
    //low active
    if(light == LIGHT_CAR_GREEN) {
        pins->at(Pin::PIN_CAR_GREEN).write(!on);
    } else if(light == LIGHT_CAR_YELLOW) {
        pins->at(Pin::PIN_CAR_YELLOW).write(!on);
    } else if(light == LIGHT_CAR_RED) {
        pins->at(Pin::PIN_CAR_RED).write(!on);
    } else if(light == LIGHT_PEDESTRIAN_GREEN) {
        pins->at(Pin::PIN_PEDESTRIAN_GREEN).write(!on);
    } else if(light == LIGHT_PEDESTRIAN_RED) {
        pins->at(Pin::PIN_PEDESTRIAN_RED).write(!on);
    } else if(light == LIGHT_BUTTON) {
        pins->at(Pin::PIN_BUTTON_LED).write(!on);
    }
}

void TrafficLightController::handleButton() {
    process_event_synced(EvPedestriansDetected(PEDESTRIAN_NORMAL));
}
