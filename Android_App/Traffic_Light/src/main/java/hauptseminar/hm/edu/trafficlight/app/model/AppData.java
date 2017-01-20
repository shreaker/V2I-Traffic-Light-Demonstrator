package hauptseminar.hm.edu.trafficlight.app.model;

/**
 * Created by shreaker on 16.12.16.
 */

public class AppData {

////////////////////////////////////////////////////////////////////
// Member variables
////////////////////////////////////////////////////////////////////
    private static AppData instance;

    private boolean hasUserSetServerData = false;

    private String licensePlateCar = "M-UC-42";

    private String restServerIp = "10.5.0.1";
    private int restServerPort = 8080;

    private int pedestrianGreenSignalTime = 0;


////////////////////////////////////////////////////////////////////
// Singelton
////////////////////////////////////////////////////////////////////
    /**
     * Private constructor for singleton pattern.
     */
    private AppData(){

    }


    /**
     * AppData singleton instance.
     *
     * @return AppData
     */
    public static synchronized AppData getInstance() {
        if (AppData.instance == null) {
            AppData.instance = new AppData();
        }
        return AppData.instance;
    }

////////////////////////////////////////////////////////////////////
// Getter
////////////////////////////////////////////////////////////////////
    public boolean hasUserSetServerData() {
        return hasUserSetServerData;
    }

    public String getLicensePlateCar() {
        return licensePlateCar;
    }



    public String getRestServerIp() {
        return restServerIp;
    }



    public int getRestServerPort() {
        return restServerPort;
    }

    public int getPedestrianGreenSignalTime() {
        return pedestrianGreenSignalTime;
    }

////////////////////////////////////////////////////////////////////
// Setter
////////////////////////////////////////////////////////////////////
    public void setHasUserSetServerData(boolean hasUserSetServerData) {
        this.hasUserSetServerData = hasUserSetServerData;
    }

    public void setLicensePlateCar(String licensePlateCar) {
        this.licensePlateCar = licensePlateCar;
    }

    public void setRestServerIp(String restServerIp) {
        this.restServerIp = restServerIp;
    }

    public void setRestServerPort(int restServerPort) {
        this.restServerPort = restServerPort;
    }

    public void setPedestrianGreenSignalTime(int pedestrianGreenSignalTime) {
        this.pedestrianGreenSignalTime = pedestrianGreenSignalTime;
    }
}
