#define QTD_PINOS 14
#define QTD_TIMERS 3

// Registros Globais
int btn_state = 0;
int pin_listen_list[QTD_PINOS]; // Pinos registraveis
int btn_lastState[QTD_PINOS]; // Ultimo estado de um botão  espefico
long timer_expire[QTD_TIMERS]; // Ultima marcação de um timer

/* Callbacks */
void button_changed (int pin, int state);
void timer_expired (int timer);

/* Registrador de botões */
void button_listen (int pin) {
  pin_listen_list[pin] = 1;
}

/* Registrador de timers */
void timer_set (int ms, int timer) {
  timer_expire[timer] = millis() + ms;
}

/* Setup Inicial */
void setup() {
  Serial.begin(9600);
  int i = 0;
  
  // Inicializa Lista de pinos a serem ouvidos
  for( i = 0 ; i < QTD_PINOS; i++ ) {
    pin_listen_list[i] = 0;
    btn_lastState[i] = 0;
  }
  
  // Inicializa Lista de TIMERS  a serem ouvidos
  for( i = 0 ; i < QTD_TIMERS; i++ ) {
    timer_expire[i] = -1;
  }

  // Init do observer
  _init();
}




void loop() {
  int i = 0;
  int dRead = 0;
  unsigned long instant = 0;

  // Detecta Botões apertados
  for( i = 0 ; i < QTD_PINOS ; i++ ) {
    if( pin_listen_list[i] == 0 ) continue;  // Não tem mais botão a ser testado
    
    

    // Detecta se o estado do botão mudou, chama a função changed APENAS caso tenha mudado
    dRead = digitalRead(i);
    if( dRead != btn_lastState[i] ) {
      btn_lastState[i] = dRead;
      button_changed(i, dRead); 
    }
  }

  // Detecta timers expirados
  instant = millis();
  for( i = 0 ; i < QTD_TIMERS ; i++ ) {
    if (timer_expire[i] == -1) continue; //Timer não setado

    // Checa se o timer expirou
    if ( instant > timer_expire[i] ) {
      timer_expire[i] = -1;
      timer_expired(i);         
    }
  }
}


/* --------------------------------- EVENT.C END ------------------ */

/* --------------------------------- USER AREA START ------------------ */

#define LED_PIN 10
#define BUT_PIN 2

int state = 0;

void button_changed (int pin,  int state) {
  if ( pin == BUT_PIN ) digitalWrite(LED_PIN, state );
}

void timer_expired( int timer) {
}

void _init(void) {
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUT_PIN, INPUT);
  
  button_listen(BUT_PIN);
}
  


