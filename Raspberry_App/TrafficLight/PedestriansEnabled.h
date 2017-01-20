#ifndef TRAFFICLIGHTCONTROLLER_PEDESTRIANSENABLED_H
#define TRAFFICLIGHTCONTROLLER_PEDESTRIANSENABLED_H

#include <ctime>

#include "TrafficLightController.h"

struct PedestriansGreen;

struct PedestriansEnabled : ITimeLeft, sc::state<PedestriansEnabled, NormalOperation, PedestriansGreen> {
    PedestriansEnabled(my_context ctx);

    virtual ~PedestriansEnabled();

    int timeLeft() const;

private:
    std::chrono::high_resolution_clock::time_point end;
    friend class PedestriansGreen;
};

struct PedestriansGreen : sc::state<PedestriansGreen, PedestriansEnabled> {
    PedestriansGreen(my_context ctx);
    ~PedestriansGreen();

    typedef mpl::list <
    sc::custom_reaction<EvPedestriansGreen>,
    sc::custom_reaction<EvCarsDetected>
    > reactions;

    sc::result react(const EvPedestriansGreen&);

    sc::result react(const EvCarsDetected&);
};

struct PedestriansGreenShort : sc::simple_state<PedestriansGreenShort, PedestriansEnabled> {
    PedestriansGreenShort();
};

#endif //TRAFFICLIGHTCONTROLLER_PEDESTRIANSENABLED_H
