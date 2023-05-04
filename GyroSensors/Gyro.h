class Gyro {
    private:
        int lasterror;
        int readAccValue;
        int readGyroValue;
        int readMagValue;
        int onStatus;
    public:
        Gyro();
        ~Gyro();
        int getLastError();
        bool getStatus();
        bool enableDefaultSensorSettings();
        int readAcc();
        int readGyro();
        int readMag();
        bool isDriving();
};
