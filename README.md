# **IoTSprinkler**
## Smart irrigation system with Esp32, Firebase RTDB and Android studio

### FIREBASE
For this project a real-time database hosted on firebase was used to store and read data from sensors and in some way it is used to store global variables that are used between the android application and the hardware installment made with the esp32.

### ESP32
The esp32 connects itself to the internet and sends periodically(evry 8 hours) readings of soil moisture from the capacitive soil moisture sensor and powers on a water pump that approximately supplies 1 liter of water in 50 seconds if the reading is inferior to the threshold value in the database or if it was manually started user-side.

### ANDROID 
The android application written in android studio permits the connection between your phone and the firebase real-time database. The application allows the user to change the threshold value for the automatic irrigation, permits manual activation of the watering and sensor reading processes and displays the real time values from the database of soil humidity and timestamp of the last sensor read.
