int button = 2;
bool ledState = 0;
bool buttonState = 0;
int mappedIndex = 0;
int potentiometerPin = A1;
const int ledPins[] = { 11, 10, 9, 8 };
const int potentiometerThresholds[] = { 0, 256, 512, 768 };


void setup() {
  Serial.begin(9600);
  for (int i = 0; i < sizeof(ledPins); i++) {
    // Initialisation des DEL en sortie
    pinMode(ledPins[i], OUTPUT);
  }
  pinMode(button, INPUT_PULLUP);  // Initialisation du boutton en entrée avec résitance de tirage
}

int estClic(unsigned long ct) {

  static unsigned long lastTime = 0;
  static int lastState = !buttonState;
  const int rate = 50;
  int clic = 0;

  if (ct - lastTime < rate) {
    return clic;  // Trop rapide
  }

  lastTime = ct;

  int state = digitalRead(button);

  if (state == LOW) {
    if (lastState == !buttonState) {
      clic = 1;
    }
  }

  lastState = state;

  return clic;
}

void allumeLed() {

  int potentiometerValue = analogRead(potentiometerPin);

  mappedIndex = map(potentiometerValue, 0, 1023, 0, 100);

  for (int i = 0; i <= sizeof(potentiometerThresholds); i++) {

    if (potentiometerValue >= potentiometerThresholds[i]) {
      digitalWrite(ledPins[i], !ledState);
    } else {
      digitalWrite(ledPins[i], ledState);
    }
  }
}

void useButton() {

  unsigned long tempActuel = millis();
  short int progressionBarre = 20;
  int barreIndex = map(mappedIndex, 0, 100, 0, 20);

  if (estClic(tempActuel)) {
    Serial.print(mappedIndex);
    Serial.print("% [");
    for (int i = 1; i <= barreIndex; i++) {
      Serial.print("@");
    }

    if ((barreIndex) <= progressionBarre) {
      for (int j = barreIndex; j < progressionBarre; j++) {
        Serial.print(".");
      }
    }
    Serial.println("]");
  }
}

void loop() {

  allumeLed();
  useButton();
}