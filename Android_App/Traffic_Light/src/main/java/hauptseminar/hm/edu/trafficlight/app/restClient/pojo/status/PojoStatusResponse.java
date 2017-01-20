package hauptseminar.hm.edu.trafficlight.app.restClient.pojo.status;


import com.google.gson.annotations.Expose;
import com.google.gson.annotations.SerializedName;

/**
 * Created by shreaker on 11.12.16.
 */

public class PojoStatusResponse {

    @SerializedName("car_state")
    @Expose
    private String carState;
    @SerializedName("car_state_time_left")
    @Expose
    private Integer carStateTimeLeft;
    @SerializedName("debug_detected_cars")
    @Expose
    private Integer debugDetectedCars;
    @SerializedName("pedestrian_state")
    @Expose
    private String pedestrianState;
    @SerializedName("pedestrian_state_time_left")
    @Expose
    private Integer pedestrianStateTimeLeft;

    /**
     * @return The carState
     */
    public String getCarState() {
        return carState;
    }

    /**
     * @param carState The car_state
     */
    public void setCarState(String carState) {
        this.carState = carState;
    }

    /**
     * @return The carStateTimeLeft
     */
    public Integer getCarStateTimeLeft() {
        return carStateTimeLeft;
    }

    /**
     * @param carStateTimeLeft The car_state_time_left
     */
    public void setCarStateTimeLeft(Integer carStateTimeLeft) {
        this.carStateTimeLeft = carStateTimeLeft;
    }

    /**
     * @return The debugDetectedCars
     */
    public Integer getDebugDetectedCars() {
        return debugDetectedCars;
    }

    /**
     * @param debugDetectedCars The debug_detected_cars
     */
    public void setDebugDetectedCars(Integer debugDetectedCars) {
        this.debugDetectedCars = debugDetectedCars;
    }

    /**
     * @return The pedestrianState
     */
    public String getPedestrianState() {
        return pedestrianState;
    }

    /**
     * @param pedestrianState The pedestrian_state
     */
    public void setPedestrianState(String pedestrianState) {
        this.pedestrianState = pedestrianState;
    }

    /**
     * @return The pedestrianStateTimeLeft
     */
    public Integer getPedestrianStateTimeLeft() {
        return pedestrianStateTimeLeft;
    }

    /**
     * @param pedestrianStateTimeLeft The pedestrian_state_time_left
     */
    public void setPedestrianStateTimeLeft(Integer pedestrianStateTimeLeft) {
        this.pedestrianStateTimeLeft = pedestrianStateTimeLeft;
    }

    @Override
    public String toString() {
        return "PojoPedestrianTrafficLightResponse{" +
                ", carState='" + carState + '\'' +
                ", carStateTimeLeft=" + carStateTimeLeft +
                ", debugDetectedCars=" + debugDetectedCars +
                ", pedestrianState='" + pedestrianState + '\'' +
                ", pedestrianStateTimeLeft=" + pedestrianStateTimeLeft +
                '}';
    }
}