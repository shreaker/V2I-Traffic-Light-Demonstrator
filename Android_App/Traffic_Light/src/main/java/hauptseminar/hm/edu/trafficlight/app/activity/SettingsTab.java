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
import android.widget.Button;
import android.widget.EditText;
import android.widget.TabHost;
import android.widget.TextView;


import hauptseminar.hm.edu.trafficlight.R;
import hauptseminar.hm.edu.trafficlight.app.model.AppData;
import hauptseminar.hm.edu.trafficlight.app.restClient.pojo.pedestrian.PojoPedestrianTrafficLight;
import hauptseminar.hm.edu.trafficlight.app.restClient.pojo.shutdown.PojoShutdown;
import hauptseminar.hm.edu.trafficlight.app.restClient.service.TrafficLightService;
import retrofit2.Call;
import retrofit2.Callback;
import retrofit2.Response;

import static hauptseminar.hm.edu.trafficlight.R.id.btnSettingsApply;
import static hauptseminar.hm.edu.trafficlight.R.id.btnShutdown;


public class SettingsTab extends Fragment implements View.OnClickListener {

    private View view;
    private AppData appData;
    private static final int HTTP_CODE_OK = 200;

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        // Inflate the layout for this fragment
        view = inflater.inflate(R.layout.settings_tab_view, container, false);
        Button applyButton = (Button) view.findViewById(btnSettingsApply);
        applyButton.setOnClickListener(this);
        Button shutdownButton = (Button) view.findViewById(btnShutdown);
        shutdownButton.setOnClickListener(this);

        appData = AppData.getInstance();

        //set value of editText fields
        setEditTxtInitialValue(appData.getLicensePlateCar(), R.id.editTxtLicensePlateCar);
        setEditTxtInitialValue(appData.getRestServerIp(), R.id.editTxtIP);
        setEditTxtInitialValue(String.valueOf(appData.getRestServerPort()), R.id.editTxtPort);

        //disable server input fields after first input.
        if (AppData.getInstance().hasUserSetServerData()) {
            disableInputField(R.id.editTxtIP);
            disableInputField(R.id.editTxtPort);
        }

        return view;
    }

    private void disableInputField(int fieldId) {
        EditText field = (EditText) view.findViewById(fieldId);
        field.setEnabled(false);
        field.setFocusable(false);
    }

    @Override
    public void onClick(View view) {
        switch (view.getId()) {
            case btnSettingsApply:
                AppData.getInstance().setHasUserSetServerData(true);
                readSettingsFromUserInput();
                switchToStatusCarTab();
                break;
            case btnShutdown:
                sendCommandToRestService();
                break;
        }
    }

    private void setEditTxtInitialValue(String valueToSet, int editTxtId) {
        EditText userInput = (EditText) view.findViewById(editTxtId);
        userInput.setText(valueToSet, TextView.BufferType.EDITABLE);
    }

    private void readSettingsFromUserInput() {
        EditText licensePlateCarInput = (EditText) view.findViewById(R.id.editTxtLicensePlateCar);
        String licensePlateCar = licensePlateCarInput.getText().toString().trim();
        appData.setLicensePlateCar(licensePlateCar);

        EditText serverIpInput = (EditText) view.findViewById(R.id.editTxtIP);
        String serverIp = serverIpInput.getText().toString().trim();
        appData.setRestServerIp(serverIp);

        EditText serverPortInput = (EditText) view.findViewById(R.id.editTxtPort);
        String serverPort = serverPortInput.getText().toString().trim();
        appData.setRestServerPort(Integer.parseInt(serverPort));
    }

    private void switchToStatusCarTab() {
        TabHost host = (TabHost) getActivity().findViewById(android.R.id.tabhost);
        host.setCurrentTab(0); //0 = StatusCarTab
    }

    private void sendCommandToRestService() {
        TrafficLightService trafficLightService = TrafficLightService.retrofit.create(TrafficLightService.class);

        final Call<PojoShutdown> call = trafficLightService.shutdownCall();

        call.enqueue(new Callback<PojoShutdown>() {
            @Override
            public void onResponse(Call<PojoShutdown> call, Response<PojoShutdown> response) {
            }

            @Override
            public void onFailure(Call<PojoShutdown> call, Throwable t) {
            }
        });
    }
}

