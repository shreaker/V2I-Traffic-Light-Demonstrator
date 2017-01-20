
package hauptseminar.hm.edu.trafficlight.app.restClient.pojo.pedestrian;

import com.google.gson.annotations.Expose;
import com.google.gson.annotations.SerializedName;

public class PojoPedestrianTrafficLight {

    @SerializedName("response")
    @Expose
    private PojoPedestrianTrafficLightResponse pojoPedestrianTrafficLightResponse;
    @SerializedName("status")
    @Expose
    private Integer status;

    /**
     * 
     * @return
     *     The pojoPedestrianTrafficLightResponse
     */
    public PojoPedestrianTrafficLightResponse getPojoPedestrianTrafficLightResponse() {
        return pojoPedestrianTrafficLightResponse;
    }

    /**
     * 
     * @param pojoPedestrianTrafficLightResponse
     *     The pojoPedestrianTrafficLightResponse
     */
    public void setPojoPedestrianTrafficLightResponse(PojoPedestrianTrafficLightResponse pojoPedestrianTrafficLightResponse) {
        this.pojoPedestrianTrafficLightResponse = pojoPedestrianTrafficLightResponse;
    }

    /**
     * 
     * @return
     *     The status
     */
    public Integer getStatus() {
        return status;
    }

    /**
     * 
     * @param status
     *     The status
     */
    public void setStatus(Integer status) {
        this.status = status;
    }

}
