
package hauptseminar.hm.edu.trafficlight.app.restClient.pojo.pedestrian;

import com.google.gson.annotations.Expose;
import com.google.gson.annotations.SerializedName;

public class PojoPedestrianTrafficLightResponse {

    @SerializedName("pedestrian_green_signal_time")
    @Expose
    private Integer pedestrianGreenSignalTime;

    /**
     * 
     * @return
     *     The pedestrianGreenSignalTime
     */
    public Integer getPedestrianGreenSignalTime() {
        return pedestrianGreenSignalTime;
    }

    /**
     * 
     * @param pedestrianGreenSignalTime
     *     The pedestrian_green_signal_time
     */
    public void setPedestrianGreenSignalTime(Integer pedestrianGreenSignalTime) {
        this.pedestrianGreenSignalTime = pedestrianGreenSignalTime;
    }

}
