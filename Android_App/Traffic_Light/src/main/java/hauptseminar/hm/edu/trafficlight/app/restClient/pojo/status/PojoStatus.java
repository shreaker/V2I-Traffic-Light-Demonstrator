package hauptseminar.hm.edu.trafficlight.app.restClient.pojo.status;


import com.google.gson.annotations.Expose;
import com.google.gson.annotations.SerializedName;

/**
 * Created by shreaker on 11.12.16.
 */

public class PojoStatus {

    @SerializedName("response")
    @Expose
    private PojoStatusResponse response;
    @SerializedName("status")
    @Expose
    private Integer status;

    /**
     * @return The response
     */
    public PojoStatusResponse getResponse() {
        return response;
    }

    @Override
    public String toString() {
        return "PojoStatus{" +
                "response=" + response +
                ", status=" + status +
                '}';
    }

    /**
     * @param response The response
     */
    public void setResponse(PojoStatusResponse response) {
        this.response = response;
    }

    /**
     * @return The status
     */
    public Integer getStatus() {
        return status;
    }

    /**
     * @param status The status
     */
    public void setStatus(Integer status) {
        this.status = status;
    }

}