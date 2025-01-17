#include "led.h"

static const char* LED_TAG = "Led";

void Led::init() {
    pinMode(LED, OUTPUT);
}

String Led::ledOn() {
    digitalWrite(LED, LED_ON);
    ESP_LOGI(LED_TAG, "Led On");
    return "Led On";
}

String Led::ledOff() {
    digitalWrite(LED, LED_OFF);
    ESP_LOGI(LED_TAG, "Led Off");
    return "Led Off";
}

String Led::getJSON(DataMessage* message) {
    LedMessage* ledMessage = (LedMessage*) message;

    StaticJsonDocument<200> doc;

    JsonObject data = doc.createNestedObject("data");

    ledMessage->serialize(data);

    String json;
    serializeJson(doc, json);

    return json;
}

DataMessage* Led::getDataMessage(JsonObject data) {
    LedMessage* ledMessage = new LedMessage();

    ledMessage->deserialize(data);

    ledMessage->messageSize = sizeof(LedMessage) - sizeof(DataMessageGeneric);

    return ((DataMessage*) ledMessage);
}

void Led::processReceivedMessage(messagePort port, DataMessage* message) {
    LedMessage* ledMessage = (LedMessage*) message;

    switch (ledMessage->ledCommand) {
        case LedCommand::On:
            ledOn();
            break;
        case LedCommand::Off:
            ledOff();
            break;
        default:
            break;
    }
}
