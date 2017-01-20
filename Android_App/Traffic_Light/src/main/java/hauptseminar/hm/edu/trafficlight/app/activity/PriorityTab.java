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
import android.widget.TextView;

import hauptseminar.hm.edu.trafficlight.R;
import hauptseminar.hm.edu.trafficlight.app.model.AppData;
import hauptseminar.hm.edu.trafficlight.app.restClient.pojo.priority.PojoPriority;
import hauptseminar.hm.edu.trafficlight.app.restClient.service.TrafficLightService;
import retrofit2.Call;
import retrofit2.Callback;
import retrofit2.Response;

import static hauptseminar.hm.edu.trafficlight.R.id.btnEmergency;
import static hauptseminar.hm.edu.trafficlight.R.id.btnPublicTransport;

public class PriorityTab extends Fragment implements View.OnClickListener {

    private static final int HTTP_CODE_OK = 200;

    View view;

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        // Inflate the layout for this fragment
        view = inflater.inflate(R.layout.priority_tab_view, container, false);
        ImageButton emergencyButton = (ImageButton) view.findViewById(R.id.btnEmergency);
        ImageButton publicTransportButton = (ImageButton) view.findViewById(R.id.btnPublicTransport);
        publicTransportButton.setOnClickListener(this);
        emergencyButton.setOnClickListener(this);

        return view;
    }

    @Override
    public void onClick(View view) {
        if(AppData.getInstance().hasUserSetServerData()){
            switch (view.getId()) {
                case btnEmergency:
                    sendCommandToRestService("emergency");
                    break;
                case btnPublicTransport:
                    sendCommandToRestService("public_transport");
                    break;
            }
        }else {
            final TextView textView = (TextView) this.view.findViewById(R.id.txtView_ErrorMsg);
            textView.setText("set/apply REST server ip:port");
        }
    }

    private void sendCommandToRestService(String priorityType) {
        TrafficLightService trafficLightService = TrafficLightService.retrofit.create(TrafficLightService.class);

        final Call<PojoPriority> call =
                trafficLightService.priorityCall(priorityType);

        call.enqueue(new Callback<PojoPriority>() {
            @Override
            public void onResponse(Call<PojoPriority> call, Response<PojoPriority> response) {
                if (response.body() != null && response.body().getStatus() == HTTP_CODE_OK) {
                    final TextView textView = (TextView) view.findViewById(R.id.txtView_ErrorMsg);
                    textView.setText(""); //no error message
                } else {
                    final TextView textView = (TextView) view.findViewById(R.id.txtView_ErrorMsg);
                    textView.setText("Rest response is null");
                    Log.d("Rest response", "null");
                }

            }

            @Override
            public void onFailure(Call<PojoPriority> call, Throwable t) {
                final TextView textView = (TextView) view.findViewById(R.id.txtView_ErrorMsg);
                textView.setText("Something went wrong: " + t.getMessage());
            }
        });
    }

}

