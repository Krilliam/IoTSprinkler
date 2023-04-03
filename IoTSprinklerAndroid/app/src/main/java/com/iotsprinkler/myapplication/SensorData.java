package com.iotsprinkler.myapplication;

public class SensorData {
    private int humidity;
    private String timestamp;
    private int threshold;

    private boolean readingStatus;

    private boolean pumpStatus;

    public SensorData() {}

    public SensorData(int humidity, String timestamp, int threshold, boolean readingStatus, boolean pumpStatus) {
        this.humidity = humidity;
        this.timestamp = timestamp;
        this.threshold = threshold;
        this.pumpStatus = pumpStatus;
        this.readingStatus = readingStatus;
    }

    public int getHumidity() {
        return humidity;
    }

    public void setHumidity(int humidity) {
        this.humidity = humidity;
    }

    public String getTimestamp() {
        return timestamp;
    }

    public void setTimestamp(String timestamp) {
        this.timestamp = timestamp;
    }

    public int getThreshold() {
        return threshold;
    }

    public void setThreshold(int threshold) {
        this.threshold = threshold;
    }

    public boolean isReadingStatus() {
        return readingStatus;
    }

    public void setReadingStatus(boolean readingStatus) {
        this.readingStatus = readingStatus;
    }

    public boolean isPumpStatus() {
        return pumpStatus;
    }

    public void setPumpStatus(boolean pumpStatus) {
        this.pumpStatus = pumpStatus;
    }
}
