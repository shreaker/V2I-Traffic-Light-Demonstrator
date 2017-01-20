

package hauptseminar.hm.edu.trafficlight.app.restClient.service;

import hauptseminar.hm.edu.trafficlight.app.model.AppData;
import hauptseminar.hm.edu.trafficlight.app.restClient.pojo.id.PojoCarId;
import hauptseminar.hm.edu.trafficlight.app.restClient.pojo.pedestrian.PojoPedestrianTrafficLight;
import hauptseminar.hm.edu.trafficlight.app.restClient.pojo.priority.PojoPriority;
import hauptseminar.hm.edu.trafficlight.app.restClient.pojo.shutdown.PojoShutdown;
import hauptseminar.hm.edu.trafficlight.app.restClient.pojo.status.PojoStatus;
import retrofit2.Call;
import retrofit2.Retrofit;
import retrofit2.converter.gson.GsonConverterFactory;
import retrofit2.http.GET;
import retrofit2.http.Query;
import retrofit2.http.Url;

/**
 * Created by shreaker on 11.12.16.
 */


public interface TrafficLightService {

    public static final Retrofit retrofit = new Retrofit.Builder()
            .baseUrl("http://" + AppData.getInstance().getRestServerIp() + ":" + AppData.getInstance().getRestServerPort())
            .addConverterFactory(GsonConverterFactory.create())
            .build();

    @GET("json?request=status")
    Call<PojoStatus> status();


    @GET("json?request=pedestrian")
        Call<PojoPedestrianTrafficLight> pedestrianTrafficLightCall(@Query("duration") String greenphasePeriod);

    @GET("json?request=priority")
    Call<PojoPriority> priorityCall(@Query("type") String priorityType);

    @GET("json?request=charge")
    Call<PojoCarId> carIdCall(@Query("id") String carId);

    @GET("json?request=shutdown")
    Call<PojoShutdown> shutdownCall();

}

