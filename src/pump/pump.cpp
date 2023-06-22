#include "pump.h"

void Pump::init() {
    // TODO: Implement this correctly with the specific code.

    pinMode(PUMP_PIN_READ, INPUT);
    pinMode(PUMP_PIN_WRITE, OUTPUT);
}

String Pump::open() {
    // TODO: Implement this correctly with the specific code.

    digitalWrite(PUMP_PIN_WRITE, 1);
    Log.verboseln(F("Pump Open"));
    return "Pump Open";
}

String Pump::close() {
    // TODO: Implement this correctly with the specific code.

    digitalWrite(PUMP_PIN_WRITE, 0);
    Log.verboseln(F("Pump Close"));
    return "Pump Close";
}

String Pump::getState() {
    // TODO: Implement this correctly with the specific code.
    // Get the state of a pump, if it is open or closed
    pumpState = digitalRead(PUMP_PIN_READ);

    return String(pumpState);
}

String Pump::getJSON(DataMessage* message) {
    PumpMessage* pumpMessage = (PumpMessage*) message;

    StaticJsonDocument<200> doc;

    JsonObject data = doc.createNestedObject("data");

    if (pumpMessage->pumpCommand == AckMessage) {
        AckPumpMessage* ackMessage = (AckPumpMessage*) message;

        ackMessage->serialize(data);
    }
    else
        pumpMessage->serialize(data);

    String json;
    serializeJson(doc, json);

    return json;
}

DataMessage* Pump::getDataMessage(JsonObject data) {
    PumpMessage* pumpMessage = new PumpMessage();

    pumpMessage->deserialize(data);

    pumpMessage->messageSize = sizeof(PumpMessage) - sizeof(DataMessageGeneric);

    return ((DataMessage*) pumpMessage);
}

void Pump::processReceivedMessage(messagePort port, DataMessage* message) {
    PumpMessage* pumpMessage = (PumpMessage*) message;
    bool sendAck = true;

    switch (pumpMessage->pumpCommand) {
        case PumpCommand::Open:
            open();
            break;
        case PumpCommand::Close:
            close();
            break;
        case PumpCommand::GetState:
            getState();
        default:
            sendAck = false;
            break;
    }

    if (sendAck)
        createAndSendAckMessage(pumpMessage->pumpCommand, pumpMessage->messageId);
}

void Pump::createAndSendAckMessage(PumpCommand ackCommand, uint8_t id) {
    AckPumpMessage* message = getAckPumpMessage(ackCommand, id);
    MessageManager::getInstance().sendMessage(messagePort::MqttPort, (DataMessage*) message);
    delete message;
}

AckPumpMessage* Pump::getAckPumpMessage(PumpCommand ackCommand, uint8_t id) {
    AckPumpMessage* ackPumpMessage = new AckPumpMessage();

    ackPumpMessage->appPortDst = appPort::MQTTApp;
    ackPumpMessage->appPortSrc = appPort::PumpApp;
    ackPumpMessage->addrSrc = LoRaMeshService::getInstance().getLocalAddress();
    ackPumpMessage->addrDst = 0;
    ackPumpMessage->messageId = id;

    ackPumpMessage->messageSize = sizeof(AckPumpMessage) - sizeof(DataMessageGeneric);

    return ackPumpMessage;
}
