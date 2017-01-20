#include "RestServer.h"

#include "Settings.h"

RestServer::RestServer(std::string url, TrafficLightController& trafficLightController, Detection& detection) :
    trafficLightController(trafficLightController) , detection(detection) {
    listener.reset(new web::http::experimental::listener::http_listener(url));
}

RestServer::~RestServer() {
    std::clog << U("Terminating JSON listener.") << std::endl;
    listener->close();
}

void RestServer::start() {
    std::clog << U("Starting listener.") << std::endl;
    listener->open().wait();
    std::clog << U("Setting up JSON listener.") << std::endl;
    listener->support(web::http::methods::GET, std::bind(&RestServer::handleRequests, this, std::placeholders::_1));
}

void RestServer::handleRequests(web::http::http_request request) {
    boost::optional<std::string> value_optional = getValueFromParameter(request, U("request"));

    if(!value_optional) {
        respond(request, web::http::status_codes::BadRequest, web::json::value::string(U("Invalid parameter: request")));
        return;
    }

    std::string value = value_optional.get();

    if(value != U("status")) {
        std::clog << U("Received request: ") << value << std::endl;
    }

    if(value == U("status")) {
        respond(request, web::http::status_codes::OK, handleStatus());
    } else if(value == U("pedestrian")) {
        value_optional = getValueFromParameter(request, U("duration"));

        if(value_optional) {
            respond(request, web::http::status_codes::OK, handlePedestrian(value_optional.get()));
        } else {
            respond(request, web::http::status_codes::BadRequest, web::json::value::string(U("Invalid parameter: pedestrian")));
        }
    } else if(value == "priority") {
        value_optional = getValueFromParameter(request, U("type"));

        if(value_optional) {
            respond(request, web::http::status_codes::OK, web::json::value::string(U("ok")));
            trafficLightController.process_event_synced(EvPriority(valToEnumPriority(value_optional.get())));
        } else {
            respond(request, web::http::status_codes::BadRequest, web::json::value::string(U("Invalid parameter: priority")));
        }

    } else if(value == "charge") {
        value_optional = getValueFromParameter(request, U("id"));

        if(value_optional) {
            respond(request, web::http::status_codes::OK, web::json::value::string(U("ok")));
            trafficLightController.process_event_synced(EvCharge(value_optional.get()));
        } else {
            respond(request, web::http::status_codes::BadRequest, web::json::value::string(U("Invalid parameter: charge")));
        }
    } else if(value == "car_debug") {
        value_optional = getValueFromParameter(request, U("detect"));

        if(value_optional) {
            respond(request, web::http::status_codes::OK, web::json::value::string(U("ok")));
            trafficLightController.process_event_synced(EvCarsDetected(1));;
        } else {
            value_optional = getValueFromParameter(request, U("enable"));

            if(value_optional) {
                if(value_optional.get() == "true") {
                    detection.enableDetection(true);
                } else if(value_optional.get() == "false") {
                    detection.enableDetection(false);
                }

                respond(request, web::http::status_codes::OK, web::json::value::string(U("ok")));
            } else {
                respond(request, web::http::status_codes::BadRequest, web::json::value::string(U("Invalid parameter: car_debug")));
            }
        }
    } else if(value == "shutdown") {
        respond(request, web::http::status_codes::OK, web::json::value::string(U("ok")));
        system("poweroff");
    } else {
        respond(request, web::http::status_codes::BadRequest, web::json::value::string(U("Unknown command.")));
    }
}

void RestServer::respond(const web::http::http_request& request, const web::http::status_code& status, const web::json::value& response) {
    web::json::value resp;
    resp[U("status")] = web::json::value::number(status);
    resp[U("response")] = response;
    request.reply(status, resp);
}

boost::optional<std::string> RestServer::getValueFromParameter(web::http::http_request request, const std::string parameter) {
    auto http_get_vars = web::http::uri::split_query(request.request_uri().query());
    auto found_name = http_get_vars.find(parameter);

    if(found_name == http_get_vars.end()) {
        return boost::optional<std::string>();
    } else {
        return found_name->second;
    }
}

web::json::value RestServer::handleStatus() {
    web::json::value response;
    TrafficLightState state;
    trafficLightController.process_event_synced(EvGetTrafficLightState(state));

    response[U("car_state_time_left")] = web::json::value::number(state.vehiclesTimeLeft);
    response[U("car_state")] = web::json::value::string(stateToStr(state.vehiclesState));
    response[U("pedestrian_state_time_left")] = web::json::value::number(state.pedestrianTimeLeft);
    response[U("pedestrian_state")] = web::json::value::string(stateToStr(state.pedestriansState));
    response[U("debug_detected_cars")] = web::json::value::number(state.detectedCars);

    return response;
}

web::json::value RestServer::handlePedestrian(const std::string& duration) {
    //todo: improve this by asking the statemachine
    web::json::value response;
    int durationResponse;
    trafficLightController.process_event_synced(EvPedestriansDetected(valToEnumPedestrian(duration)));
    trafficLightController.process_event_synced(EvGetNextPedestrianPhaseTime(durationResponse));

    response[U("pedestrian_green_signal_time")] = web::json::value::number(durationResponse);
    return response;
}

Priority RestServer::valToEnumPriority(const std::string& str) const {
    if(str == "emergency") {
        return PRIORITY_EMERGENGY;
    } else {
        return PRIORITY_PUBLIC_TRANSPORT;
    }
}

Pedestrian RestServer::valToEnumPedestrian(const std::string& str) const {
    if(str == "normal") {
        return PEDESTRIAN_NORMAL;
    } else if(str == "extended") {
        return PEDESTRIAN_EXTENDED;
    } else {
        return PEDESTRIAN_NONE;
    }
}

std::string RestServer::stateToStr(const TrafficLightState::State& state) const {
    std::string val;

    switch(state) {
        case TrafficLightState::State::RED: {
            val = "red";
            break;
        }

        case TrafficLightState::State::GREEN: {
            val = "green";
            break;
        }

        case TrafficLightState::State::YELLOW: {
            val = "yellow";
            break;
        }
    }

    return val;
}
