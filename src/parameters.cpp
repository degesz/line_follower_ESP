#include "parameters.h"


Preferences prefs_storage;

params_t savedParams = {0};

void paramsBegin() {
    if (prefs_storage.begin("line-follower", false)) {  // Ensure successful initialization
        prefs_storage.getBytes("params", &savedParams, sizeof(params_t));  // Read out params from storage
         Serial.printf("Loaded parameters from storage: \n P0: %d   P1: %d   P2: %d  P3: %d  P4: %d\n", savedParams.P0, savedParams.P1, savedParams.P2, savedParams.P3, savedParams.P4);
        prefs_storage.end();  // End the preferences
    } else {
        Serial.println("Failed to initialize preferences storage.");
    }
}

void updateParams(params_t params) {
    if (prefs_storage.begin("line-follower", false)) {  // Ensure successful initialization
        prefs_storage.putBytes("params", &params, sizeof(params_t));
         Serial.printf("Updated parameters in storage: \n P0: %d   P1: %d   P2: %d  P3: %d   P4: %d \n", params.P0 , params.P1, params.P2, params.P3, params.P4);
        savedParams = params;
        prefs_storage.end();  // End the preferences
    } else {
        Serial.println("Failed to initialize preferences storage.");
    }
}

params_t readParams() {
    if (prefs_storage.begin("line-follower", false)) {  // Ensure successful initialization
        
        prefs_storage.getBytes("params", &savedParams, sizeof(params_t));  // Read out params from storage
        //savedParams = {0};
        Serial.printf("Loaded parameters from storage: \n P0: %d   P1: %d   P2: %d  P3: %d  P4: %d\n", savedParams.P0, savedParams.P1, savedParams.P2, savedParams.P3, savedParams.P4);
        prefs_storage.end();  // End the preferences
    } else {
        Serial.println("Failed to initialize preferences storage.");
    }
    return savedParams;
}