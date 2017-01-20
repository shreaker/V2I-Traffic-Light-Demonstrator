#ifndef TRAFFICLIGHTCONTROLLER_VEHICLESENABLED_H
#define TRAFFICLIGHTCONTROLLER_VEHICLESENABLED_H

#include "TrafficLightController.h"

struct VehiclesRedYellow;

struct VehiclesEnabled : ITimeLeft, sc::state<VehiclesEnabled, NormalOperation, VehiclesRedYellow> {
    VehiclesEnabled(my_context ctx);
    virtual ~VehiclesEnabled();

    int timeLeft() const;

private:
    std::chrono::high_resolution_clock::time_point end;
    friend class VehiclesGreenYellow;
    friend class VehiclesGreenExtended;
    friend class VehiclesGreen;
    friend class VehiclesRedYellow;
};

struct VehiclesGreen;
struct VehiclesRedYellow : sc::state<VehiclesRedYellow, VehiclesEnabled> {
    VehiclesRedYellow(my_context ctx);
    virtual ~VehiclesRedYellow();

    typedef sc::transition <EvCarGreen, VehiclesGreen> reactions;
};

struct VehiclesGreenExtended;
struct VehiclesGreen : sc::state<VehiclesGreen, VehiclesEnabled> {
    VehiclesGreen(my_context ctx);
    virtual ~VehiclesGreen();

    typedef sc::transition <EvCarGreenExtended, VehiclesGreenExtended> reactions;
};

struct VehiclesGreenYellow;
struct VehiclesGreenExtended : sc::state<VehiclesGreenExtended, VehiclesEnabled> {
    VehiclesGreenExtended(my_context ctx);
    virtual ~VehiclesGreenExtended();

    typedef mpl::list <
    sc::custom_reaction<EvCarGreenExtended>,
    sc::transition<EvCarGreenYellow, VehiclesGreenYellow>
    > reactions;

    sc::result react(const EvCarGreenExtended&);
};

struct VehiclesGreenYellow : sc::state<VehiclesGreenYellow, VehiclesEnabled> {
    VehiclesGreenYellow(my_context ctx);
    virtual ~VehiclesGreenYellow();

    typedef mpl::list <
    sc::custom_reaction<EvCarGreenYellow>,
    sc::custom_reaction<EvPedestriansEnabled>,
    sc::custom_reaction<EvIdle>
    > reactions;

    sc::result react(const EvCarGreenYellow&);

    sc::result react(const EvPedestriansEnabled&);

    sc::result react(const EvIdle&);
};

#endif //TRAFFICLIGHTCONTROLLER_VEHICLESENABLED_H
