#include <BleKeyboard.h>
#include <RotaryEncoder.h>

const int button1Pin = 5;
const int button2Pin = 6;
const int button3Pin = 7;
const int encoderAPin = 4;
const int encoderBPin = 3;
const int encoderSwitchPin = 2;

bool button1Pressed = false;
bool button2Pressed = false;
bool button3Pressed = false;
bool encoderSwitchPressed = false;

BleKeyboard bleKeyboard;
RotaryEncoder encoder(encoderAPin, encoderBPin, RotaryEncoder::LatchMode::TWO03);

void setup() {
  Serial.begin(9600);
  
  pinMode(button1Pin, INPUT_PULLUP);
  pinMode(button2Pin, INPUT_PULLUP);
  pinMode(button3Pin, INPUT_PULLUP);
  pinMode(encoderAPin, INPUT_PULLUP);
  pinMode(encoderBPin, INPUT_PULLUP);
  pinMode(encoderSwitchPin, INPUT_PULLUP);
  
  bleKeyboard.begin();
}

void loop() {
  
  // Check button inputs
  if (digitalRead(button1Pin) == LOW && !button1Pressed) {
    button1Pressed = true;
    Serial.write("Prev\n");
    bleKeyboard.press(KEY_MEDIA_PREVIOUS_TRACK);
  } else if (digitalRead(button1Pin) == HIGH && button1Pressed) {
    button1Pressed = false;
    bleKeyboard.release(KEY_MEDIA_PREVIOUS_TRACK);
  }

  if (digitalRead(button2Pin) == LOW && !button2Pressed) {
    button2Pressed = true;
    Serial.write("Play/Pause\n");
    bleKeyboard.press(KEY_MEDIA_PLAY_PAUSE);
  } else if (digitalRead(button2Pin) == HIGH && button2Pressed) {
    button2Pressed = false;
    bleKeyboard.release(KEY_MEDIA_PLAY_PAUSE);
  }

  if (digitalRead(button3Pin) == LOW && !button3Pressed) {
    button3Pressed = true;
    Serial.write("Next\n");
    bleKeyboard.press(KEY_MEDIA_NEXT_TRACK);
  } else if (digitalRead(button3Pin) == HIGH && button3Pressed) {
    button3Pressed = false;
    bleKeyboard.release(KEY_MEDIA_NEXT_TRACK);
  }

  if (digitalRead(encoderSwitchPin) == LOW && !encoderSwitchPressed) {
    encoderSwitchPressed = true;
    Serial.write("Mute\n");
    bleKeyboard.press(KEY_MEDIA_MUTE);
  } else if (digitalRead(encoderSwitchPin) == HIGH && encoderSwitchPressed) {
    encoderSwitchPressed = false;
    bleKeyboard.release(KEY_MEDIA_MUTE);
  }

  // Check encoder rotation
  static int pos = 0;
  encoder.tick();

  int newPos = encoder.getPosition();
  if (pos != newPos) {
    int dir = (-1) * (int)(encoder.getDirection());

    if (dir > 0) {
      bleKeyboard.press(KEY_MEDIA_VOLUME_UP);
      delay(200); // Increase this delay for larger moves
      bleKeyboard.release(KEY_MEDIA_VOLUME_UP);
      Serial.write("Volume Up\n");
    } else {
      bleKeyboard.press(KEY_MEDIA_VOLUME_DOWN);
      delay(200); // Increase this delay for larger moves
      bleKeyboard.release(KEY_MEDIA_VOLUME_DOWN);
      Serial.write("Volume Down\n");
    }
    pos = newPos;
  }
}
