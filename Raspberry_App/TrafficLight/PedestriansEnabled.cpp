#include "PedestriansEnabled.h"

#include <iostream>

#include "Settings.h"
#include "VehiclesEnabled.h"

PedestriansEnabled::PedestriansEnabled(my_context ctx) : my_base(ctx) {
    std::clog << "===PedestriansEnabled entry===" << std::endl;
    context<TrafficLightController>().switchLight(TrafficLightController::LIGHT_PEDESTRIAN_RED, false);
}

PedestriansEnabled::~PedestriansEnabled() {
    std::clog << "===PedestriansEnabled exit===" << std::endl;
    std::clog << "PEDESTRIANS: RED" << std::endl;

    context<TrafficLightController>().switchLight(TrafficLightController::LIGHT_PEDESTRIAN_RED, true);
    context<TrafficLightController>().switchLight(TrafficLightController::LIGHT_BUTTON, false);
}

int PedestriansEnabled::timeLeft() const {
    return std::chrono::duration_cast<std::chrono::milliseconds>(end - std::chrono::high_resolution_clock::now()).count();
}

PedestriansGreen::PedestriansGreen(my_context ctx) : my_base(ctx) {
    std::clog << "PEDESTRIANS: GREEN" << std::endl;
    context<TrafficLightController>().switchLight(TrafficLightController::LIGHT_PEDESTRIAN_GREEN, true);

    int waitingTimeDecrease = (context<NormalOperation>().isPublicTransport) ? PUBLIC_TRANSPOR_TIME_DECREASE : 0;
    int endTime;

    if(context<NormalOperation>().detectedPedestrian == PEDESTRIAN_NORMAL) {
        endTime = PEDESTRIAN_GREEN - waitingTimeDecrease;
        context<TrafficLightController>().process_event_delayed_synced<EvPedestriansGreen>(endTime, EvPedestriansGreen());
        context<PedestriansEnabled>().end = std::chrono::high_resolution_clock::now() + std::chrono::milliseconds(endTime);
    } else if(context<NormalOperation>().detectedPedestrian == PEDESTRIAN_EXTENDED) {
        endTime = PEDESTRIAN_GREEN_EXTENDED - waitingTimeDecrease;
        context<TrafficLightController>().process_event_delayed_synced<EvPedestriansGreen>(endTime, EvPedestriansGreen());
        context<PedestriansEnabled>().end = std::chrono::high_resolution_clock::now() + std::chrono::milliseconds(endTime);
    }
}

PedestriansGreen::~PedestriansGreen() {
    context<TrafficLightController>().switchLight(TrafficLightController::LIGHT_PEDESTRIAN_GREEN, false);
}

sc::result PedestriansGreen::react(const EvPedestriansGreen&) {
    post_event(EvPedestriansDetected(PEDESTRIAN_NONE));

    if(context<NormalOperation>().detectedCars > 0) {
        return transit<VehiclesEnabled>();
    } else {
        return transit<Idle>();
    }
}


sc::result PedestriansGreen::react(const EvCarsDetected& event) {
    context<NormalOperation>().detectedCars = event.getCarsNumber();
    return discard_event();
}

PedestriansGreenShort::PedestriansGreenShort() {

}
