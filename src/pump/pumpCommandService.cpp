#include "pumpCommandService.h"
#include "pump.h"

PumpCommandService::PumpCommandService() {
    addCommand(Command("/pumpOpen", "Open de pump", PumpCommand::Open, 1,
        [this](String args) {
        return String(Pump::getInstance().open());
    }));

    addCommand(Command("/pumpClose", "Close the pump", PumpCommand::Close, 1,
        [this](String args) {
        return String(Pump::getInstance().close());
    }));

    
}