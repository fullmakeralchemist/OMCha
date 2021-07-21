#include <ArduinoBLE.h>

// Device name
const char* nameOfPeripheral = "ArduinoBLEnano33";
const char* uuidOfService = "00001101-0000-1000-8000-00805f9b34fb";
const char* uuidOfRxChar = "00001142-0000-1000-8000-00805f9b34fb";
const char* uuidOfTxChar = "00001143-0000-1000-8000-00805f9b34fb";

// BLE Service
BLEService microphoneService(uuidOfService);

// TX Characteristics
BLEByteCharacteristic txChar(uuidOfTxChar, BLERead | BLENotify | BLEBroadcast);


/*
 *  MAIN
 */
void setup() {

  // Start serial.
  Serial.begin(9600);

  // Ensure serial port is ready.
  while (!Serial);

  // Prepare LED pins.
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LEDR, OUTPUT);
  pinMode(LEDG, OUTPUT);


  // Start BLE.
  startBLE();

  // Create BLE service and characteristics.
  BLE.setLocalName(nameOfPeripheral);
  BLE.setAdvertisedService(microphoneService);
  microphoneService.addCharacteristic(txChar);
  BLE.addService(microphoneService);

  // Bluetooth LE connection handlers.
  BLE.setEventHandler(BLEConnected, onBLEConnected);
  BLE.setEventHandler(BLEDisconnected, onBLEDisconnected);
  

  
  // Let's tell devices about us.
  BLE.advertise();
  
  // Print out full UUID and MAC address.
  Serial.println("Peripheral advertising info: ");
  Serial.print("Name: ");
  Serial.println(nameOfPeripheral);
  Serial.print("MAC: ");
  Serial.println(BLE.address());
  Serial.print("Service UUID: ");
  Serial.println(microphoneService.uuid());
  Serial.print("txCharacteristics UUID: ");
  Serial.println(uuidOfTxChar);
  

  Serial.println("Bluetooth device active, waiting for connections...");
}


void loop()
{
  BLEDevice central = BLE.central();
  
  if (central)
  {
    // Only send data if we are connected to a central device.
    while (central.connected()) {
      connectedLight();

      // Send the microphone values to the central device.
      txChar.writeValue(69);
      Serial.println(txChar.writeValue(69));
      delay(1000);
    }
  } else {
    disconnectedLight();
  }
}


/*
 *  BLUETOOTH
 */
void startBLE() {
  if (!BLE.begin())
  {
    Serial.println("starting BLE failed!");
    while (1);
  }
}


void onBLEConnected(BLEDevice central) {
  Serial.print("Connected event, central: ");
  Serial.println(central.address());
  connectedLight();
}

void onBLEDisconnected(BLEDevice central) {
  Serial.print("Disconnected event, central: ");
  Serial.println(central.address());
  disconnectedLight();
}



/*
 * LEDS
 */
void connectedLight() {
  digitalWrite(LEDR, LOW);
  digitalWrite(LEDG, HIGH);
}


void disconnectedLight() {
  digitalWrite(LEDR, HIGH);
  digitalWrite(LEDG, LOW);
}
