#include "param_storage.h"

void params_begin(){
    preferences.begin("lf-robot", false); // open a namespace with RW mode
    //Print out stored parameters to serial: 
    Serial.printf("Stored params: P1: %.2f, P2: %.2f, P3 %.2f, P4: %.2f", preferences.getInt("P1"), preferences.getInt("P2"), preferences.getInt("P3"), preferences.getFloat("P4"));
}

void params_store(int P1, int P2, int P3, float P4){
    preferences.putInt("P1", P1);
    preferences.putInt("P2", P2);
    preferences.putInt("P3", P3);
    preferences.putFloat("P4", P4);
}