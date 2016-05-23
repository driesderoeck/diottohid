// Import libraries (BLEPeripheral depends on SPI)
#include <SPI.h>
#include <BLEHIDPeripheral.h>
#include <BLEKeyboard.h>

// define pins (varies per shield/board)
#define BLE_REQ   4
#define BLE_RDY   2
#define BLE_RST   3

const int BUTTONDOWN = 9; // key >
const int BUTTONUP = 8; // key <
const int BUTTONCONTACTS = 7; // key ctrl b
const int BUTTONPHOTOS = 6; // key ctrl q
const int BUTTONCOMMENT = 5; // key ctrl shift u

// We're using this BLEPeripheral with an Android device
#define ANDROID_CENTRAL

BLEHIDPeripheral bleHID = BLEHIDPeripheral(BLE_REQ, BLE_RDY, BLE_RST);
BLEKeyboard bleKeyboard;

int ButtonDOWNState;
int ButtonUPState;
int ButtonCONTACTSState;
int ButtonPHOTOSState;
int ButtonCOMMENTState;

int lastButtonDOWNState = LOW;
int lastButtonUPState = LOW;
int lastButtonCONTACTSState = LOW;
int lastButtonPHOTOSState = LOW;
int lastButtonCOMMENTState = LOW;

int OUT_DOWNState = LOW;
int OUT_UPState = LOW;
int OUT_CONTACTSState = LOW;
int OUT_PHOTOSState = LOW;
int OUT_COMMENTState = LOW;

long lastDebounceDOWNTime = 0;
long lastDebounceUPTime = 0;
long lastDebounceCONTACTSTime = 0;
long lastDebouncePHOTOSTime = 0;
long lastDebounceCOMMENTTime = 0;

long debounceDelay = 50;

void setup() {
 Serial.begin(9600);
// #if defined (__AVR_ATmega32U4__)
//  while (!Serial);
// #endif

  // clears bond data on every boot
  bleHID.clearBondStoreData();

#ifdef ANDROID_CENTRAL
  bleHID.setReportIdOffset(1);
#endif

  bleHID.setDeviceName("DIOTTO");

  bleHID.setLocalName("DIOTTOHID");

  bleHID.addHID(bleKeyboard);
  bleHID.begin();

  pinMode(BUTTONDOWN, INPUT_PULLUP);
  pinMode(BUTTONUP, INPUT_PULLUP);
  pinMode(BUTTONPHOTOS, INPUT_PULLUP);
  pinMode(BUTTONCONTACTS, INPUT_PULLUP);
  pinMode(BUTTONCOMMENT, INPUT_PULLUP);

 // Serial.println(F("BLE DIOTTO HID"));
}

void loop() {

  BLECentral central = bleHID.central();

  if (central) {

    // central connected to peripheral
    Serial.print(F("Connected to central: "));
    Serial.println(central.address());

    while (bleHID.connected()) {

      //read all the pins!
      //      int readingDOWN = digitalRead(BUTTONDOWN);
      //      int readingUP = digitalRead(BUTTONUP);
      //      int readingPHOTOS = digitalRead(BUTTONPHOTOS);
      //      int readingCONTACTS = digitalRead(BUTTONCONTACTS);
      //      int readingCOMMENT = digitalRead(BUTTONCOMMENT);

      int readingDOWN = 0;
      int readingUP = 0;
      int readingPHOTOS = 0;
      int readingCONTACTS = 0;
      int readingCOMMENT = 0;

      DOWN();
      UP();
      PHOTOS();
      CONTACTS();
      COMMENT();

    } // close while loop




    // central disconnected
    Serial.print(F("Disconnected from central: "));
    Serial.println(central.address());

  }

} // close main loop

void DOWN() {

  // Serial.println("DOWN");

  int readingDOWN = digitalRead(BUTTONDOWN);

  //-----debounce start

  if (readingDOWN != lastButtonDOWNState) {
    lastDebounceDOWNTime = millis();
  }

  if ((millis() - lastDebounceDOWNTime) > debounceDelay) {
    if (readingDOWN != ButtonDOWNState) {
      ButtonDOWNState = readingDOWN;

      // only toggle the LED if the new button state is HIGH
      if (ButtonDOWNState == HIGH) {
        OUT_DOWNState = !OUT_DOWNState;
      }
    }
  }

  if (OUT_DOWNState == HIGH) {
    Serial.println(F("PRESSR"));
    bleKeyboard.press(0x36,KEYCODE_MOD_LEFT_SHIFT);
    bleKeyboard.releaseAll();
    ButtonDOWNState = LOW;
  }

  lastButtonDOWNState = readingDOWN;

  //-----debounce stop

}

void UP() {

  // Serial.println("UP");

  int readingUP = digitalRead(BUTTONUP);

  //-----debounce start

  if (readingUP != lastButtonUPState) {
    lastDebounceUPTime = millis();
  }

  if ((millis() - lastDebounceUPTime) > debounceDelay) {
    if (readingUP != ButtonUPState) {
      ButtonUPState = readingUP;

      // only toggle the LED if the new button state is HIGH
      if (ButtonUPState == HIGH) {
        OUT_UPState = !OUT_UPState;
      }
    }
  }

  if (OUT_UPState == HIGH) {
    Serial.println(F("PRESSO"));
    bleKeyboard.press(0x37,KEYCODE_MOD_LEFT_SHIFT);
    bleKeyboard.releaseAll();
    ButtonUPState = LOW;
  }

  lastButtonUPState = readingUP;

  //-----debounce stop

}

void PHOTOS() {

  // Serial.println("PHOTOS");

  int readingPHOTOS = digitalRead(BUTTONPHOTOS);

  //-----debounce start

  if (readingPHOTOS != lastButtonPHOTOSState) {
    lastDebouncePHOTOSTime = millis();
  }

  if ((millis() - lastDebouncePHOTOSTime) > debounceDelay) {
    if (readingPHOTOS != ButtonPHOTOSState) {
      ButtonPHOTOSState = readingPHOTOS;

      // only toggle the LED if the new button state is HIGH
      if (ButtonPHOTOSState == HIGH) {
        OUT_PHOTOSState = !OUT_PHOTOSState;
      }
    }
  }

  if (OUT_PHOTOSState == HIGH) {
    Serial.println(F("PRESSS"));
    bleKeyboard.press(KEYCODE_Q,KEYCODE_MOD_LEFT_CONTROL);
    bleKeyboard.press(KEYCODE_Q,KEYCODE_MOD_RIGHT_CONTROL);
    bleKeyboard.releaseAll();
    ButtonPHOTOSState = LOW;
  }

  lastButtonPHOTOSState = readingPHOTOS;

  //-----debounce stop

}

void CONTACTS() {

  // Serial.println("CONTACTS");

  int readingCONTACTS = digitalRead(BUTTONCONTACTS);

  //-----debounce start

  if (readingCONTACTS != lastButtonCONTACTSState) {
    lastDebounceCONTACTSTime = millis();
  }

  if ((millis() - lastDebounceCONTACTSTime) > debounceDelay) {
    if (readingCONTACTS != ButtonCONTACTSState) {
      ButtonCONTACTSState = readingCONTACTS;

      // only toggle the LED if the new button state is HIGH
      if (ButtonCONTACTSState == HIGH) {
        OUT_CONTACTSState = !OUT_CONTACTSState;
      }
    }
  }

  if (OUT_CONTACTSState == HIGH) {
    Serial.println(F("PRESST"));
    bleKeyboard.press(KEYCODE_Q);
    bleKeyboard.releaseAll();
    ButtonCONTACTSState = LOW;
  }

  lastButtonCONTACTSState = readingCONTACTS;

  //-----debounce stop

}

void COMMENT() {

  // Serial.println("COMMENT");

  int readingCOMMENT = digitalRead(BUTTONCOMMENT);

  //-----debounce start

  if (readingCOMMENT != lastButtonCOMMENTState) {
    lastDebounceCOMMENTTime = millis();
  }

  if ((millis() - lastDebounceCOMMENTTime) > debounceDelay) {
    if (readingCOMMENT != ButtonCOMMENTState) {
      ButtonCOMMENTState = readingCOMMENT;

      // only toggle the LED if the new button state is HIGH
      if (ButtonCOMMENTState == HIGH) {
        OUT_COMMENTState = !OUT_COMMENTState;
      }
    }
  }

  if (OUT_COMMENTState == HIGH) {
    Serial.println(F("PRESSI"));
    bleKeyboard.press(KEYCODE_I);
    bleKeyboard.releaseAll();
    ButtonCOMMENTState = LOW;
  }

  lastButtonCOMMENTState = readingCOMMENT;

  //-----debounce stop

}

