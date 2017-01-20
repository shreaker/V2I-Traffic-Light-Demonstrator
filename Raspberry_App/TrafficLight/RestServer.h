#ifndef RESTSERVER_H
#define RESTSERVER_H
#include <string>
#include <cpprest/http_listener.h>
#include <memory>
#include <boost/optional/optional.hpp>

#include "Detection.h"
#include "TrafficLightController.h"

class RestServer {
public:
    RestServer(std::string url, TrafficLightController& trafficLightController, Detection& detection);
    ~RestServer();
    void start();

private:
    std::unique_ptr<web::http::experimental::listener::http_listener> listener;
    TrafficLightController& trafficLightController;
    Detection& detection;

    void handleRequests(web::http::http_request request);
    void respond(const web::http::http_request& request, const web::http::status_code& status, const web::json::value& response);
    boost::optional<std::string> getValueFromParameter(web::http::http_request req, const std::string parameter);
    web::json::value handleStatus();
    web::json::value handlePedestrian(const std::string& duration);
    Priority valToEnumPriority(const std::string& str) const;
    Pedestrian valToEnumPedestrian(const std::string& str) const;
    std::string stateToStr(const TrafficLightState::State& state) const;
};

#endif // RESTSERVER_H
