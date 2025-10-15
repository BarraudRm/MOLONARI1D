// il faut faire attention à l'utilisation de double potentiellement utiliser des 'new' et 'delete' pour ne pas avoir de souci
// This file defines the Measure class, which stores readings from all 4 sensors at a specific time.  

// fusionner mesure pression et température 
// fusionner temp_sensor, pressure_sensor et measure en un seul fichier measure
// measure prend en argument le pin du port, et si c'est pression ou température, et comment tu transformes les tensions en mesure
// les arguments à prendre sont : int _dataPin, int _enablePin, float _offset, float _scale comme dans temp sensor

// renvoyer la valeur de la pression et celle de la température (dans temp sensor on fait ces calculs)
// regarder ce que fait analog read 

#ifndef MEASURE_CLASS
#define MEASURE_CLASS


class Measure {
  public:
    // Unique ID for each measurement
    unsigned int id;

    // Date in the format "dd/mm/yyyy"
    char date[11];

    // Time in the format "hh:mm:ss"
    char time[9];

// A PARTIR DE LA TOUT EST DIFFERENT MAIS J'AI L'IMPRESSION QUE CA FAIT LA MEME CHOSE EN PLUS 'SECURISE'

    int npressure; // number of sensors
    int ntemp; // number of sensors

    double *chanelP; // pointer to the value of all the analogical sensors 
    double *chanelT; // pointer to the value of all the analogical sensors 



 
    String oneLine() {
      int i;
      String str = String(id);                        // Add ID
      str += " (" + String(date) + " " + String(time) + ") : ";      // Add timestamp
      for (i = 0; i < npressure ; i++) {
        str += String(chanelP[i]) + ", ";                             // Add sensor i data
      }  
      for (i = 0; i < ntemp-1 ; i++) {
        str += String(chanelT[i]) + ", ";                             // Add sensor i data
      }                             // Add sensor 3 data
    str += String(chanelT[i]);                                        // Add last sensor data
    
    return str; // Return the constructed string
    }

    /**
     * Convert the measurement details into a string representation.
     * @return A string summarizing the measurement details.
     */
    String ToString() {
      int i;
      String str = "Measure n°" + oneLine();                                      // Add last sensor data
    
    return str; // Return the constructed string
    }  
};

#endif
