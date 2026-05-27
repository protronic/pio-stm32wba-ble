/*
  BLE LED Control — STM32WBA5MMG / B-WBA5M-WPAN
  -----------------------------------------------
  Advertises a BLE LED service. A central (phone / PC) can
  connect and write 0x01 / 0x00 to toggle the built-in LED.

  Service UUID : 19B10000-E8F2-537E-4F6C-D104768A1214
  Characteristic: 19B10001-E8F2-537E-4F6C-D104768A1214  (Read | Write)

  Prerequisites
  -------------
  - Arduino_Core_STM32 with B-WBA5M-WPAN / STM32WBA5MMG support
  - STM32CubeWBA BLE middleware (ble_stack.h) bundled with the core
  - STM32duinoBLE fork: protronic/STM32duinoBLE@feature/stm32wba-transport
*/

#include <Arduino.h>
#include <STM32duinoBLE.h>

// ── BLE objects ──────────────────────────────────────────────────────────────
BLEService        ledService("19B10000-E8F2-537E-4F6C-D104768A1214");
BLEByteCharacteristic ledCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214",
                                        BLERead | BLEWrite);

// ── helpers ──────────────────────────────────────────────────────────────────
void onLedWrite(BLEDevice /*central*/, BLECharacteristic characteristic) {
  uint8_t val = (uint8_t)characteristic.value();
  digitalWrite(LED_BUILTIN, val ? HIGH : LOW);
  Serial.print("LED -> ");
  Serial.println(val ? "ON" : "OFF");
}

// ── setup ────────────────────────────────────────────────────────────────────
void setup() {
  Serial.begin(115200);
  while (!Serial);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  Serial.println("STM32WBA5MMG BLE LED Control");

  if (!BLE.begin()) {
    Serial.println("ERROR: BLE init failed.");
    Serial.println("  -> Check that STM32CubeWBA middleware is available");
    Serial.println("     and that the board variant is correct.");
    while (1);
  }

  Serial.print("BLE address: ");
  Serial.println(BLE.address());

  BLE.setLocalName("WBA5-LED");
  BLE.setAdvertisedService(ledService);

  ledService.addCharacteristic(ledCharacteristic);
  BLE.addService(ledService);

  ledCharacteristic.writeValue(0);
  ledCharacteristic.setEventHandler(BLEWritten, onLedWrite);

  BLE.advertise();
  Serial.println("Advertising… connect with LightBlue, nRF Connect, etc.");
}

// ── loop ─────────────────────────────────────────────────────────────────────
void loop() {
  BLEDevice central = BLE.central();

  if (central) {
    Serial.print("Connected: ");
    Serial.println(central.address());

    while (central.connected()) {
      BLE.poll();
    }

    Serial.println("Disconnected.");
    digitalWrite(LED_BUILTIN, LOW);
  }
}
