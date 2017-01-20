#ifndef TRAFFICLIGHTCONTROLLER_EVENTS_H
#define TRAFFICLIGHTCONTROLLER_EVENTS_H

#include <boost/statechart/event.hpp>
#include <string>

namespace sc = boost::statechart;

enum Pedestrian {
    PEDESTRIAN_NORMAL, PEDESTRIAN_EXTENDED, PEDESTRIAN_NONE
};

enum Priority {
    PRIORITY_EMERGENGY, PRIORITY_PUBLIC_TRANSPORT, PRIORITY_PUBLIC_TRANSPORT_DELETE
};

struct EvPedestriansDetected : sc::event<EvPedestriansDetected> {
public:
    EvPedestriansDetected(Pedestrian pedestrian = PEDESTRIAN_NONE) : pedestrian(pedestrian) {};

    Pedestrian getPedestrian() const {
        return pedestrian;
    }

private:
    Pedestrian pedestrian;
};

struct EvCarsDetected : sc::event<EvCarsDetected> {
public:
    EvCarsDetected(unsigned int carsNumber = 0) : carsNumber(carsNumber) {}

    unsigned int getCarsNumber() const {
        return carsNumber;
    }

private:
    unsigned int carsNumber;
};

struct EvPriority : sc::event<EvPriority> {
public:
    EvPriority(Priority priority) : priority(priority) {};

    Priority getPriority() const {
        return priority;
    }

private:
    Priority priority;
};

struct EvCharge : sc::event<EvCharge> {
public:
    EvCharge(std::string id) : id(id) {
    }

    std::string getId() {
        return id;
    }

private:
    std::string id;
};

struct EvCarGreen : sc::event<EvCarGreen> {
};

struct EvCarGreenExtended : sc::event<EvCarGreenExtended> {
};

struct EvCarGreenYellow : sc::event<EvCarGreenYellow> {
};

struct EvPedestriansGreen : sc::event<EvPedestriansGreen> {
};

struct EvIdle : sc::event<EvIdle> {
};

struct EvNormalOperation : sc::event<EvNormalOperation> {
};

struct EvPedestriansEnabled : sc::event<EvPedestriansEnabled> {

};

struct TrafficLightState {
    enum State {
        RED, GREEN, YELLOW
    };
    int vehiclesTimeLeft;
    State vehiclesState;
    int pedestrianTimeLeft;
    State pedestriansState;
    int detectedCars;
};

struct EvGetTrafficLightState : sc::event<EvGetTrafficLightState> {
public:
    EvGetTrafficLightState(TrafficLightState& state) : state(state) {}

    void assign(TrafficLightState state) const {
        this->state = state;
    }

private:
    TrafficLightState& state;
};

struct EvGetNextPedestrianPhaseTime : sc::event<EvGetNextPedestrianPhaseTime> {
public:
    EvGetNextPedestrianPhaseTime(int& nextPhaseTime) : nextPhaseTime(nextPhaseTime) {}

    void assign(int nextPhaseTime) const {
        this->nextPhaseTime = nextPhaseTime;
    }

private:
    int& nextPhaseTime;
};


#endif //TRAFFICLIGHTCONTROLLER_EVENTS_H
