#include <thread>

#include "RestServer.h"
#include "Detection.h"

#include "TrafficLightController.h"

int main() {
    TrafficLightController trafficLightController;
    cv::CascadeClassifier cascadeClassifier("cascade.xml");
    Detection carDetection(trafficLightController, cascadeClassifier, CV_CAP_ANY);
    RestServer restServer("http://0.0.0.0:8080/json", trafficLightController, carDetection);

    restServer.start();
    carDetection.start();
    trafficLightController.initiate();

    std::clog << "Waiting for incoming connection..." << std::endl;

    while(true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10000));
    }

    return 0;
}
