#pragma once

#include <Arduino.h>


#include <ArduinoLog.h>

#include "pump.h"

#include "message/messageService.h"

#include "pumpCommandService.h"

#include "pumpMessage.h"

#include "config.h"

#include "loramesh/loraMeshService.h"

class Pump: public MessageService {
public:
    /**
     * @brief Construct a new Pump Service object
     *
     */
    static Pump& getInstance() {
        static Pump instance;
        return instance;
    }


    PumpCommandService* pumpCommandService = new PumpCommandService();

    void init();

    String open();

    String close();

    String getState();

    String getJSON(DataMessage* message);

    DataMessage* getDataMessage(JsonObject data);

    void processReceivedMessage(messagePort port, DataMessage* message);

    void createAndSendAckMessage(PumpCommand ackCommand, uint8_t id);

    AckPumpMessage* getAckPumpMessage(PumpCommand ackCommand, uint8_t id);

private:
    Pump(): MessageService(PumpApp, "Pump") {
        commandService = pumpCommandService;
    };

    int pumpState;
};