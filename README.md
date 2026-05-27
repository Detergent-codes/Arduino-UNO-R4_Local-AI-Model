# Arduino-UNO-R4_Local-AI-Model

This is a generative AI mode which uses the Markov chai for predicting the user interests and topics.

The promting to the AI can be done through the Serial monitor in ArduinoIDE if code is uploaded
and Arduino  is connected via a USB cable to the Computer.

Vocab.h - This file contains the dictionary for the words that the AI moden can use to reply to promts by the user.
          This dictionar contains 350 base words stored in flash memory and another 50 learnable words that can be added to SRAM if it detects that
          they are not in the base word database.

This model is designed to be used on the Arduino UNO R4 because of it high fedility and storage.
The Arduino UNO R4 has the Renesas RA4M1 (ARM Cortex-M4 @ 48 MHz) Microcontroller with 256 KB of flash memeory, 32 KB SRAM and 8KB EEPROM.
These specs are WAY higher than the previous generation of Arduino, the Arduino UNO R3. You can compare bothe below:

Feature          - Arduino UNO R3          - Arduino UNO R4 (Minima/WiFi)


Processor        - ATmega328P (8-bit AVR)  - Renesas RA4M1 (32-bit ARM Cortex-M4)

Clock Speed      - 16 MHz                  - 48 MHz

Flash Memory     - 32 KB                   - 256 KB

SRAM             - 2 KB                    - 32 KB

EEPROM           - 1 KB                    - 8 KB

USB Interface    - USB-B                   - USB-C



Code by Aalok
27 May, 2026
