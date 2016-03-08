#define LED_PIN 13
#define BUT_UP_PIN 2
#define BUT_DOWN_PIN 3

int led_state = 0;
int led_time = 0;

unsigned long b1_last = 0;
unsigned long b2_last = 0;

int intervalo = 1000;

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUT_UP_PIN, INPUT);
  pinMode(BUT_DOWN_PIN, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:

  int but1 = digitalRead(BUT_UP_PIN);
  int but2 = digitalRead(BUT_DOWN_PIN);
  
  // Para caso os dois botões sejam apertados ao mesmo tempo
  if (but1 && but2 ) {
      digitalWrite(13,LOW); 
      while(1);    
  }
  
  // Botão 1 aumenta a velocidade
  if (but1 && millis() - b1_last > 250){
    b1_last = millis();
    intervalo /= 2;
  }
  
  // Botão 2 diminui a velocidade
  if (but2 && millis() - b2_last > 250){
    b2_last = millis();
    intervalo *= 2;
  }
   
  // Pisca o LED de acordo com o intervalo pre determinado
  if( millis() - led_time >= intervalo ) {
    led_time = millis();
    digitalWrite(LED_PIN, led_state);
    led_state = !led_state;
  }
  
}
