
// #include <WiFiManager.h> // https://github.com/tzapu/WiFiManager
#include <Arduino.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
// #include <time.h>
// #include <unistd.h>

// extern "C"{
#include "dwin_lib.h"
// };


void setup()
{
    Serial.begin(115200);
}

void loop()
{

    int err;
    int i, cmd;
    int count = 4096;

    // write reset
    cmd = 0;

    // dgus_system_reset(0);
}
