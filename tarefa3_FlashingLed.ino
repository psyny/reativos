#define QTD_PINOS 14
#define QTD_TIMERS 14

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
#define BUT1_PIN 2
#define BUT2_PIN 3

#define BTN_DELAY 200
#define STOP_DELAY 500

int led_locked = 0;
int led_delay = 1000;
int led_state = 0;
int btn1_locked = 0;
int btn2_locked = 0;
int btn1_stop = 0;
int btn2_stop = 0;

void button_changed (int pin, int state) {
    if(led_locked==1) return; 

    // Botão aumentar
    if( pin == BUT1_PIN && state && btn1_locked == 0 ) {
      btn1_locked = 1; // Trava o accesso ao botao por um curto espaço de tempo
      timer_set( BTN_DELAY , 1); // Timer para destravar o botao  

      btn1_stop = 1; // Timer para encerrar o programa ( botao duplo )
      timer_set( STOP_DELAY , 3); // Timeout da janela para encerramento
      
      led_delay = led_delay / 2; // Muda a frenquencia do led
    }

    // Botão diminuir
    if( pin == BUT2_PIN && state && btn2_locked == 0 ) {
      btn2_locked = 1; // Trava o accesso ao botao por um curto espaço de tempo
      timer_set( BTN_DELAY , 2); // Timer para destravar o botao

      btn2_stop = 1; // Timer para encerrar o programa ( botao duplo )
      timer_set( STOP_DELAY , 3); // Timeout da janela para encerramento
      
      led_delay = led_delay * 2; // Muda a frenquencia do led
    }    

    // Encerramento ao apertar dois botões
    if( btn1_stop && btn2_stop ) {
      led_locked = 1;
      digitalWrite(LED_PIN, HIGH);
    }
}


void timer_expired (int timer) {
   if(led_locked==1) return; 
   
   switch(timer) {
    case 0:
      Serial.println("Led Delay...");
      led_state = !led_state;
      digitalWrite(LED_PIN, led_state);
      timer_set( led_delay , 0);
      break;

    case 1:
      btn1_locked = 0;
      break;

    case 2:
      btn2_locked = 0;
      break;

    case 3:
      btn1_stop = 0;
      btn2_stop = 0;
      break;
   }
}


void _init () {
    pinMode(LED_PIN, OUTPUT);
    pinMode(BUT1_PIN, INPUT);
    pinMode(BUT2_PIN, INPUT);
    
    button_listen(BUT1_PIN);
    button_listen(BUT2_PIN);
    timer_set( led_delay , 0); //Inicia a piscada do led
}
