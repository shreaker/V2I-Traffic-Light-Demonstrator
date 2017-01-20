#include "Detection.h"

#include <opencv2/highgui/highgui.hpp>
#include <functional>
#include <vector>

#include "Settings.h"

Detection::Detection(TrafficLightController& trafficLightController, cv::CascadeClassifier carCascade,
                     int webcam) : trafficLightController(trafficLightController), cascadeClassifier(carCascade),
    webcam(webcam) { }

Detection::~Detection() {
    cv::destroyAllWindows();
}

void Detection::start() {
    if(started) {
        return;
    }

    started = true;
    webcam.set(CV_CAP_PROP_FRAME_WIDTH, CAMERA_WIDTH);
    webcam.set(CV_CAP_PROP_FRAME_HEIGHT, CAMERA_HEIGHT);
    cv::startWindowThread();

    std::function<void()> detectionLoop = [this]() {
        cv::Mat frame;
        int detectedObjects;
        bool isDetected;
        cv::namedWindow("Detection", cv::WINDOW_AUTOSIZE); // Create a window for display.
        cv::moveWindow("Detection", 450, 200);
        webcam.read(frame); //first image is garbage for some reason -> throw away


        //only send an empty detected cars event if 3 images in a row are empty
        while(true) {
            if(!enabled) {
                trafficLightController.process_event_synced(EvCarsDetected(0));
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
                continue;
            }

            isDetected = false;

            for(int i = 0; i < 3; i++) {
                webcam.read(frame);
                detectedObjects = detectObjects(frame);

                if(detectedObjects > 0) {
                    trafficLightController.process_event_synced(EvCarsDetected(detectedObjects));
                    std::clog << "detected cars: " << detectedObjects << std::endl;
                    isDetected = true;
                }

                cv::imshow("Detection", frame);  // Display frame
                std::this_thread::sleep_for(std::chrono::milliseconds(200));
            }

            if(!isDetected) {
                trafficLightController.process_event_synced(EvCarsDetected(0));
            }
        }
    };

    detectionThread.reset(new std::thread(detectionLoop));
}

void Detection::enableDetection(bool enabled) {
    this->enabled = enabled;
}


int Detection::detectObjects(cv::Mat& frame) {
    std::vector<cv::Rect> objects;
    cv::Mat frameGray;
    cv::cvtColor(frame, frameGray, cv::COLOR_BGR2GRAY);  // Convert to gray scale
    cv::equalizeHist(frameGray, frameGray);     // Equalize histogram

    // Detect objects
    cascadeClassifier.detectMultiScale(frameGray, objects, 1.1, 1, 0 | cv::CASCADE_SCALE_IMAGE, cv::Size(100, 100), cv::Size(140, 140));

    // Iterate over all of the objects
    for(const auto& object : objects) {

        // Find center of object
        cv::Point center(object.x + object.width / 2, object.y + object.height / 2);

        // Draw ellipse around object
        cv::ellipse(frame, center, cv::Size(object.width / 2, object.height / 2), 0, 0, 360, cv::Scalar(255, 0, 255), 4, 8, 0);
    }

    return objects.size();
}
