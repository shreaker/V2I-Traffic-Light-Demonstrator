
package hauptseminar.hm.edu.trafficlight.app.restClient.pojo.id;

import com.google.gson.annotations.Expose;
import com.google.gson.annotations.SerializedName;

public class PojoCarId {

    @SerializedName("response")
    @Expose
    private String response;
    @SerializedName("status")
    @Expose
    private Integer status;

    /**
     * 
     * @return
     *     The response
     */
    public String getResponse() {
        return response;
    }

    /**
     * 
     * @param response
     *     The response
     */
    public void setResponse(String response) {
        this.response = response;
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
