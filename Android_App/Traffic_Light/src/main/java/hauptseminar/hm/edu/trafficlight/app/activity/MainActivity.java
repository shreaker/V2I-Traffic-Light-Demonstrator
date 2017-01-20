package hauptseminar.hm.edu.trafficlight.app.activity;

/**
 * Created by shreaker on 27.11.16.
 */


import android.content.Context;
import android.os.Bundle;
import android.support.v4.app.FragmentActivity;
import android.support.v4.app.FragmentTabHost;
import android.util.AttributeSet;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.View;

import hauptseminar.hm.edu.trafficlight.R;

public class MainActivity extends FragmentActivity {
    // Fragment TabHost as mTabHost
    private FragmentTabHost mTabHost;


    @Override
    protected void onCreate(Bundle savedInstanceState) {

        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        mTabHost = (FragmentTabHost)findViewById(android.R.id.tabhost);
        mTabHost.setup(this, getSupportFragmentManager(), R.id.realtabcontent);

        mTabHost.addTab(mTabHost.newTabSpec("StatusCarTab").setIndicator("Status Car"),
                StatusCarTab.class, null);
        mTabHost.addTab(mTabHost.newTabSpec("StatusPedestrianTab").setIndicator("Status PED"),
                StatusPedestrianTab.class, null);
        mTabHost.addTab(mTabHost.newTabSpec("PedestrianTab").setIndicator("PED"),
                PedestrianTab.class, null);
        mTabHost.addTab(mTabHost.newTabSpec("PriorityTab").setIndicator("Prio"),
                PriorityTab.class, null);


        mTabHost.addTab(mTabHost.newTabSpec("SettingsTab").setIndicator("..."),
                SettingsTab.class, null);
    }

}