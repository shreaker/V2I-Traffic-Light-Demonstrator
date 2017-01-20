package hauptseminar.hm.edu.trafficlight.app.activity;

/**
 * Created by shreaker on 27.11.16.
 */


import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageButton;
import android.widget.TabHost;
import android.widget.TextView;

import hauptseminar.hm.edu.trafficlight.R;
import hauptseminar.hm.edu.trafficlight.app.model.AppData;
import hauptseminar.hm.edu.trafficlight.app.restClient.pojo.pedestrian.PojoPedestrianTrafficLight;
import hauptseminar.hm.edu.trafficlight.app.restClient.service.TrafficLightService;
import retrofit2.Call;
import retrofit2.Callback;
import retrofit2.Response;

import static hauptseminar.hm.edu.trafficlight.R.id.btnRequestGreenLong;
import static hauptseminar.hm.edu.trafficlight.R.id.btnEmergency;

public class PedestrianTab extends Fragment implements View.OnClickListener {

    private static final int HTTP_CODE_OK = 200;

    private View view;
    private AppData appData;

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        // Inflate the layout for this fragment
        view = inflater.inflate(R.layout.pedestrian_tab_view, container, false);
        ImageButton greenNormalButton = (ImageButton) view.findViewById(R.id.btnEmergency);
        ImageButton btnRequestGreenLong = (ImageButton) view.findViewById(R.id.btnRequestGreenLong);
        greenNormalButton.setOnClickListener(this);
        btnRequestGreenLong.setOnClickListener(this);

        appData = AppData.getInstance();

        return view;
    }

    @Override
    public void onClick(View view) {
        if (AppData.getInstance().hasUserSetServerData()) {
            switch (view.getId()) {
                case btnEmergency:
                    sendCommandToRestService("normal");
                    break;
                case btnRequestGreenLong:
                    sendCommandToRestService("extended");
                    break;
            }
            TabHost host = (TabHost) getActivity().findViewById(android.R.id.tabhost);
            host.setCurrentTab(1); //1 = PedestrianTab
        } else {
            final TextView textView = (TextView) this.view.findViewById(R.id.txtView_ErrorMsg);
            textView.setText("set/apply REST server ip:port");
        }
    }


    private void sendCommandToRestService(String greenphasePeriod) {
        TrafficLightService trafficLightService = TrafficLightService.retrofit.create(TrafficLightService.class);

        final Call<PojoPedestrianTrafficLight> call = trafficLightService.pedestrianTrafficLightCall(greenphasePeriod);

        call.enqueue(new Callback<PojoPedestrianTrafficLight>() {
            @Override
            public void onResponse(Call<PojoPedestrianTrafficLight> call, Response<PojoPedestrianTrafficLight> response) {
                if (response.body() != null && response.body().getStatus() == HTTP_CODE_OK) {

                    appData.setPedestrianGreenSignalTime(response.body().getPojoPedestrianTrafficLightResponse().getPedestrianGreenSignalTime());


                    final TextView textView = (TextView) view.findViewById(R.id.txtView_ErrorMsg);
                    textView.setText(""); //no error message
                } else {
                    final TextView textView = (TextView) view.findViewById(R.id.txtView_ErrorMsg);
                    textView.setText("Rest response is null");
                    Log.d("Rest response", "null");
                }

            }

            @Override
            public void onFailure(Call<PojoPedestrianTrafficLight> call, Throwable t) {
                final TextView textView = (TextView) view.findViewById(R.id.txtView_ErrorMsg);
                textView.setText("Something went wrong: " + t.getMessage());
            }
        });
    }
}

