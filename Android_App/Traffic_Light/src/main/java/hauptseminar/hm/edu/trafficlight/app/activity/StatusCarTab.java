package hauptseminar.hm.edu.trafficlight.app.activity;

/**
 * Created by shreaker on 27.11.16.
 */


import android.os.Bundle;
import android.os.Handler;
import android.support.v4.app.Fragment;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.TextView;

import hauptseminar.hm.edu.trafficlight.R;
import hauptseminar.hm.edu.trafficlight.app.model.AppData;
import hauptseminar.hm.edu.trafficlight.app.restClient.pojo.status.PojoStatus;
import hauptseminar.hm.edu.trafficlight.app.restClient.pojo.status.PojoStatusResponse;
import hauptseminar.hm.edu.trafficlight.app.restClient.service.*;
import retrofit2.Call;
import retrofit2.Callback;
import retrofit2.Response;

public class StatusCarTab extends Fragment {

    private static final int REQUEST_PERIOD_REST_SERVICE = 1000;

    private static final int HTTP_CODE_OK = 200;

    private View view;
    private ImageView trafficLightImageView;
    private Handler handler;

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        // Inflate the layout for this fragment
        view = inflater.inflate(R.layout.status_car_tab_view, container, false);

        trafficLightImageView = (ImageView) view.findViewById(R.id.tab1_trafficLight_ImageView);
        trafficLightImageView.setImageResource(R.drawable.red_light);

        // Create the Handler object (on the main thread by default)
        handler = new Handler();

        startPeriodicRequestFromRestService();

        return view;
    }

    private void startPeriodicRequestFromRestService() {
        // Define the code block to be executed
        final Runnable runnableCode = new Runnable() {
            @Override
            public void run() {
                if (AppData.getInstance().hasUserSetServerData()) {
                    getResponseFromRestService();
                } else {
                    final TextView textView = (TextView) view.findViewById(R.id.txtView_ErrorMsg);
                    textView.setText("set/apply REST server ip:port");
                }
                handler.postDelayed(this, REQUEST_PERIOD_REST_SERVICE);
            }
        };
        // Start the initial runnable task by posting through the handler
        handler.post(runnableCode);
    }


    private void getResponseFromRestService() {
        TrafficLightService trafficLightService = TrafficLightService.retrofit.create(TrafficLightService.class);

        final Call<PojoStatus> call =
                trafficLightService.status();

        call.enqueue(new Callback<PojoStatus>() {
            @Override
            public void onResponse(Call<PojoStatus> call, Response<PojoStatus> response) {
                if (response.body() != null && response.body().getStatus() == HTTP_CODE_OK) {
                    updateViewToRestResponse(response.body().getResponse());

                    final TextView textView = (TextView) view.findViewById(R.id.txtView_ErrorMsg);
                    textView.setText(""); //no error message

                } else {
                    final TextView textView = (TextView) view.findViewById(R.id.txtView_ErrorMsg);
                    textView.setText("Rest response is null");
                    Log.d("Rest response", "null");
                }
            }

            @Override
            public void onFailure(Call<PojoStatus> call, Throwable t) {
                final TextView textView = (TextView) view.findViewById(R.id.txtView_ErrorMsg);
                textView.setText("Something went wrong: " + t.getMessage());
            }
        });
    }

    private void updateViewToRestResponse(PojoStatusResponse response) {
        updateViewTrafficLightStatus(response.getCarState());
        updateViewTimeLeft(response.getCarStateTimeLeft());
        updateViewCarCrossingPossible(response.getCarStateTimeLeft() >= 2000);
    }

    private void updateViewTrafficLightStatus(String stateFromRestService) {

        switch (stateFromRestService) {
            case "red":
                trafficLightImageView.setImageResource(R.drawable.red_light);
                break;
            case "yellow":
                trafficLightImageView.setImageResource(R.drawable.yellow_light);
                break;
            case "green":
                trafficLightImageView.setImageResource(R.drawable.green_light);
                break;
        }
    }

    private void updateViewTimeLeft(int timeLeft) {
        final TextView textView = (TextView) view.findViewById(R.id.txtView_TimeLeft);
        textView.setText(Integer.toString((int) Math.ceil(timeLeft / 1000.0)));
    }

    private void updateViewCarCrossingPossible(boolean isCarCrossingPossible) {
        final TextView textView = (TextView) view.findViewById(R.id.txtView_CarCrossingPossible);
        if (isCarCrossingPossible) {
            textView.setText("Is possible.");
        } else {
            textView.setText("Is not possible");
        }
    }
}

