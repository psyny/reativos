#define LED_A_PIN 2
#define LED_B_PIN 3
#define LED_C_PIN 4

#define LED_A_TEMP 20.0
#define LED_B_TEMP 26.0
#define LED_C_TEMP 30.0

#define TERMO_PIN 0


float tempC;
int reading;


long print_delay;

void setup() {
  // put your setup code here, to run once:
  analogReference(INTERNAL);
  Serial.begin(9600);
  
  pinMode(LED_A_PIN, OUTPUT);
  pinMode(LED_B_PIN, OUTPUT);
  pinMode(LED_C_PIN, OUTPUT);  
  
  print_delay = millis() + 1000;
}

void loop() {
  // Leitura do termometro
  reading = analogRead(TERMO_PIN);
  tempC = reading / 9.31;
 
 // LEDS
 if(tempC >= LED_A_TEMP) digitalWrite(LED_A_PIN, HIGH);
 else digitalWrite(LED_A_PIN, LOW);
 if(tempC >= LED_B_TEMP) digitalWrite(LED_B_PIN, HIGH);
 else digitalWrite(LED_B_PIN, LOW);
 if(tempC >= LED_C_TEMP) digitalWrite(LED_C_PIN, HIGH);
 else digitalWrite(LED_C_PIN, LOW);
 
  // Serial Feedback
  if(millis() > print_delay) {
    Serial.println(tempC);
    print_delay = millis() + 1000;
  }
 
  
}
