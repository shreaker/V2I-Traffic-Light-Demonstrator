#ifndef CARDETECTION_H
#define CARDETECTION_H

#include <opencv2/opencv.hpp>
#include <memory>
#include <thread>
#include "TrafficLightController.h"

class Detection {
public:
    Detection(TrafficLightController& trafficLightController, cv::CascadeClassifier cascadeClassifier, int webcam);
    ~Detection();
    void start();
    void enableDetection(bool);

private:
    TrafficLightController& trafficLightController;
    cv::CascadeClassifier cascadeClassifier;
    cv::VideoCapture webcam;
    bool started = false;
    bool enabled = true;
    std::unique_ptr<std::thread> detectionThread;
    int detectObjects(cv::Mat& frame);
};

#endif // CARDETECTION_H
