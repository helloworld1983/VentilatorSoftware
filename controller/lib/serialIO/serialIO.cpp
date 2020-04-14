/* Copyright 2020, Edwin Chiu

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include <Arduino.h>

#include "serialIO.h"

void serialIO_init() {
  // Initialize serial communications at 115200 bps.
  Serial.begin(115200, SERIAL_8N1);
}

bool serialIO_dataAvailable() { return Serial.available() > 0; }

void serialIO_readByte(char *buffer) {
  // NOTE: This assumes that a byte is ready in the buffer
  Serial.readBytes(buffer, 1);
}

void serialIO_send(uint8_t b) { Serial.write(b); }
