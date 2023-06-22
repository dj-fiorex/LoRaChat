#pragma once

#include "Arduino.h"

#include "commands/commandService.h"

#include "pumpMessage.h"

class PumpCommandService: public CommandService {
public:
    PumpCommandService();
};