// Define the data-type of a measurement
#define MEASURE_T double
// Define the data-type of a measurement
#define MEASURE_P unsigned short
// Define a function to parse a measurement (i.e. to convert a string to a MEASURE_T)
#define TO_MEASURE_T toDouble

// Uncomment this line to enable diagnostics log on serial for lora operations
#define LORA_DEBUG

// Uncomment this line to enable diagnostics log on serial for SD operations
#define SD_DEBUG

#include "../shared/Lora.hpp"
#include "../shared/Low_Power.cpp"
#include "../shared/Pressure_Sensor.hpp"
#include "../shared/Temp_Sensor.hpp"
#include "../shared/Time.cpp"
#include "../shared/SD_Initializer.cpp"
#include "../shared/Writer.hpp"
#include "../shared/Waiter.hpp"
#include <ArduinoLowPower.h>