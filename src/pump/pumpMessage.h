#pragma once

#include <Arduino.h>

#include <ArduinoLog.h>

#include "message/dataMessage.h"

#pragma pack(1)

enum PumpCommand: uint8_t {
    Close = 0,
    Open = 1,
    GetState = 2,
    AckMessage = 3,
};

class PumpMessage: public DataMessageGeneric {
public:
    PumpCommand pumpCommand;

    void serialize(JsonObject& doc) {
        // Call the base class serialize function
        ((DataMessageGeneric*) (this))->serialize(doc);

        // Add the derived class data to the JSON object
        doc["pumpCommand"] = pumpCommand;
    }

    void deserialize(JsonObject& doc) {
        // Call the base class deserialize function
        ((DataMessageGeneric*) (this))->deserialize(doc);

        // Add the derived class data to the JSON object
        pumpCommand = (PumpCommand) doc["pumpCommand"];
    }
};

class AckPumpMessage: public PumpMessage {
public:
    PumpCommand ackCommand; // The command that is acknowledged
    int pumpState = Close; // The pump state

    void serialize(JsonObject& doc) {
        // Call the base class serialize function
        ((PumpMessage*) (this))->serialize(doc);

        // Add the derived class data to the JSON object
        doc["ackCommand"] = ackCommand;
        doc["pumpState"] = pumpState;
    }

    void deserialize(JsonObject& doc) {
        Log.errorln("Deserialize ACKPumpMessage not implemented.");
    }
};
#pragma pack()