#include "VehiclesEnabled.h"

#include <iostream>

#include "Settings.h"
#include "PedestriansEnabled.h"

VehiclesEnabled::VehiclesEnabled(my_context ctx) : my_base(ctx) {
    std::clog << "===VehiclesEnabled entry===" << std::endl;
    context<TrafficLightController>().switchLight(TrafficLightController::LIGHT_CAR_RED, false);
}

VehiclesEnabled::~VehiclesEnabled() {
    std::clog << "===VehiclesEnabled exit===" << std::endl;
    std::clog << "CARS: RED" << std::endl;
    context<TrafficLightController>().switchLight(TrafficLightController::LIGHT_CAR_RED, true);
}

int VehiclesEnabled::timeLeft() const {
    return std::chrono::duration_cast<std::chrono::milliseconds>(end - std::chrono::high_resolution_clock::now()).count();
}

VehiclesRedYellow::VehiclesRedYellow(my_context ctx) : my_base(ctx) {
    std::clog << "CARS: RED / YELLOW" << std::endl;
    context<TrafficLightController>().switchLight(TrafficLightController::LIGHT_CAR_RED, true);
    context<TrafficLightController>().switchLight(TrafficLightController::LIGHT_CAR_YELLOW, true);

    context<TrafficLightController>().process_event_delayed_synced<EvCarGreen>(CAR_RED_YELLOW, EvCarGreen());
    context<VehiclesEnabled>().end = std::chrono::high_resolution_clock::now() + std::chrono::milliseconds(CAR_RED_YELLOW);
}

VehiclesRedYellow::~VehiclesRedYellow() {
    context<TrafficLightController>().switchLight(TrafficLightController::LIGHT_CAR_RED, false);
    context<TrafficLightController>().switchLight(TrafficLightController::LIGHT_CAR_YELLOW, false);
}

VehiclesGreen::VehiclesGreen(my_context ctx) : my_base(ctx) {
    std::clog << "CARS: GREEN" << std::endl;
    context<TrafficLightController>().switchLight(TrafficLightController::LIGHT_CAR_GREEN, true);

    context<TrafficLightController>().process_event_delayed_synced<EvCarGreenExtended>(CAR_GREEN, EvCarGreenExtended());
    context<VehiclesEnabled>().end = std::chrono::high_resolution_clock::now() + std::chrono::milliseconds(CAR_GREEN + CAR_GREEN_EXTENDED_LOOP);
}

VehiclesGreen::~VehiclesGreen() {
    context<TrafficLightController>().switchLight(TrafficLightController::LIGHT_CAR_GREEN, false);
}

VehiclesGreenExtended::VehiclesGreenExtended(my_context ctx) : my_base(ctx) {
    std::clog << "CARS: GREEN EXTENDED" << std::endl;
    context<TrafficLightController>().switchLight(TrafficLightController::LIGHT_CAR_GREEN, true);

    context<TrafficLightController>().process_event_delayed_synced<EvCarGreenExtended>(CAR_GREEN_EXTENDED_LOOP, EvCarGreenExtended());
    context<VehiclesEnabled>().end = std::chrono::high_resolution_clock::now() + std::chrono::milliseconds(CAR_GREEN_EXTENDED_LOOP + CAR_GREEN_YELLOW);
}

VehiclesGreenExtended::~VehiclesGreenExtended() {
    context<TrafficLightController>().switchLight(TrafficLightController::LIGHT_CAR_GREEN, false);
}

sc::result VehiclesGreenExtended::react(const EvCarGreenExtended&) {
    if(context<NormalOperation>().detectedCars > 0 && context<NormalOperation>().detectedPedestrian == PEDESTRIAN_NONE) {
        context<TrafficLightController>().process_event_delayed_synced<EvCarGreenExtended>(CAR_GREEN_EXTENDED_LOOP, EvCarGreenExtended());
        context<VehiclesEnabled>().end = std::chrono::high_resolution_clock::now() + std::chrono::milliseconds(CAR_GREEN_EXTENDED_LOOP + CAR_GREEN_YELLOW);
    } else {
        post_event(EvCarGreenYellow());
    }

    return discard_event();
}

VehiclesGreenYellow::VehiclesGreenYellow(my_context ctx) : my_base(ctx) {
    std::clog << "CARS: GREEN YELLOW" << std::endl;
    context<TrafficLightController>().switchLight(TrafficLightController::LIGHT_CAR_YELLOW, true);
    context<TrafficLightController>().switchLight(TrafficLightController::LIGHT_CAR_GREEN, true);
    post_event(EvCarGreenYellow());
}

VehiclesGreenYellow::~VehiclesGreenYellow() {
    context<TrafficLightController>().switchLight(TrafficLightController::LIGHT_CAR_YELLOW, false);
    context<TrafficLightController>().switchLight(TrafficLightController::LIGHT_CAR_GREEN, false);
}

sc::result VehiclesGreenYellow::react(const EvCarGreenYellow&) {
    if(context<NormalOperation>().detectedPedestrian != PEDESTRIAN_NONE) {
        context<TrafficLightController>().process_event_delayed_synced<EvPedestriansEnabled>(CAR_GREEN_YELLOW, EvPedestriansEnabled());
    } else {
        context<TrafficLightController>().process_event_delayed_synced<EvIdle>(CAR_GREEN_YELLOW, EvIdle());
    }

    return discard_event();
}

sc::result VehiclesGreenYellow::react(const EvPedestriansEnabled&) {
    return transit<PedestriansEnabled>();
}

sc::result VehiclesGreenYellow::react(const EvIdle&) {
    return transit<Idle>();
}
