// This file defines the Measure and Sensor class 

#ifndef MEASURE_CLASS
#define MEASURE_CLASS

#include <Arduino.h>
#include <string>
#include <vector>
typedef double MESURE;

class Sensor {
    public :
        Sensor(); // Default constructor
        // Initialise the sensor for the first time. 
        // dataPin -> Analog input to read data from (Analog pin)
        // enablePin -> Digital output to enable/disable the sensor 
        Sensor(int dataPin, int enablePin, float offset, float scale, std::string type_capteur);
        
        // Measure the pressure
        MESURE Measure();

    private :
        // Pin to read data from (Analog pin)
        const int dataPin;
        // Pin to enable/disable the sensor
        const int enablePin;
        const float offset;  
        const float scale;
        const std::string type_capteur;   
};

class Measure {
  public:
    // Unique ID for each measurement
    unsigned int id;

    String date;
    String time;
    unsigned long time_in_second;

    int ncapteur;

    std::vector<double> channel;
 
    String oneLine();
    String ToString();
};

#endif
