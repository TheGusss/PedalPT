#include "mult16x8.h"

//Define los recursos de hardware.
#define LED 13
#define FOOTSWITCH 12
#define TOGGLE 2
#define PUSHBUTTON_1 A5
#define PUSHBUTTON_2 A4

//define los paraametros de salida del PWM
#define PWM_FREQ 0x00FF // Frecuencia PWM  - 31.3KHz
#define PWM_MODE 0 // Modo: Rapido (1) o Fase correcta (0)
#define PWM_QTY 2 // 2 PWMs en paralelo

//Define los parametros de FX
#define MIN 2 // Delay minimo de ~60us
#define MAX 200 // Delay maximo de ~6ms
#define SIZE MAX+10 // Tamano del buffer de datos - debe ser mayor que el valor de delay maximo


int buffer[SIZE]; // crea un buffer de datos
byte dir = 1; // mantiene el seguimiento del conteo arriba/abajo
unsigned int location = 0; // incoming data buffer pointer
byte button; // boton monitoreador del contador (timer)
byte counter = 4; // Boton contador (y valor inicial)
unsigned int fractional = 0x00; // posicion fraccional de ejemplo
int data_buffer; // almacenamiento temporal de datos para dar un 1 de ejemplo al buffer

int toggle_position=0;
int counter2=0; //boton contador del temporizador


void setup() {
  //configuracion del IO (Entrada/Salida)
  pinMode(FOOTSWITCH, INPUT_PULLUP);
  pinMode(TOGGLE, INPUT_PULLUP);
  pinMode(PUSHBUTTON_1, INPUT_PULLUP);
  pinMode(PUSHBUTTON_2, INPUT_PULLUP);
  pinMode(LED, OUTPUT);
  
  // Configuracion del ADC
  ADMUX = 0x60; // Ajuste izquierdo adc0, interno VCC
  ADCSRA = 0xe5; // Enciende el ADC, ck/32, auto trigger
  ADCSRB = 0x07; // t1 capture for trigger
  DIDR0 = 0x01; // Apaga las entradas digitales para el adc0

  // Configuracion del PWM
  TCCR1A = (((PWM_QTY - 1) << 5) | 0x80 | (PWM_MODE << 1)); //
  TCCR1B = ((PWM_MODE << 3) | 0x11); // ck/1
  TIMSK1 = 0x20; // interrumpe en la captura interrumpida / interrupt on capture interrupt
  ICR1H = (PWM_FREQ >> 8);
  ICR1L = (PWM_FREQ & 0xff);
  DDRB |= ((PWM_QTY << 1) | 0x02); // Enciende las salidas
  sei(); // Encienda las interrupciones - Realmente no es necesario con arduino
  }

void loop() {
  if (digitalRead(FOOTSWITCH)) digitalWrite(LED, HIGH); 
    else  digitalWrite(LED, LOW);}






ISR(TIMER1_CAPT_vect) { //Todo el procesamiento ocurre aqui

  // ultimos valores calculados de las salidas - output the last value calculated
  OCR1AL = ((data_buffer + 0x8000) >> 8); // Convierte a no firmado, envia fuera bytes altos - convert to unsigned, send out high byte
  OCR1BL = data_buffer; // Envia fuera bytes bajos
  
  // Obtiene los datos del ADC
  byte temp1 = ADCL; // se necesita para buscar el byte mas bajo primero - you need to fetch the low byte first
  byte temp2 = ADCH; // yes it needs to be done this way
  int input = ((temp2 << 8) | temp1) + 0x8000; // hace un valor 16b firmado - make a signed 16b value

//Botones
  counter2++; //para resguardar los recursos, el pushbutton es revisado cada 2000 ciclos de reloj - to save resources, the pushbuttons are checked every 2000 times.
if(counter2==2000)
{ 
counter2=0;
  if (!digitalRead(PUSHBUTTON_1)) {
     if (counter > MIN) counter-=1; // Si no es el minimo, decrementa
      }
      if (!digitalRead(PUSHBUTTON_2)) {
        if (counter < MAX) counter+=1; // Si no es el maximo, incrementa
      }

//toggle switch
{
toggle_position = digitalRead(TOGGLE);
    }
  }
  
  
  // busca/almacena datos - fetch/store data
  buffer[location] = input; // almacena el ejemplo actual
  location++; // va hacia la siguente posicion de ejemplo - go to next sample position
  if (location >= SIZE) location = 0; // deal with buffer wrap
  int temp = location - (fractional >> 8); // encuentra el ejemplo demorado - find delayed sample
  if (temp < 0) temp += SIZE; // deal with buffer wrap
  int output = buffer[temp]; // busca el ejemplo demorado - fetch delayed sample
  if (toggle_position=1) output = buffer[temp]+input;
  if (toggle_position=0) output = buffer[temp];
  temp -= 1; // encuentra el ejemplo adyasente - find adjacent sample
  if (temp < 0) temp += SIZE; // deal with buffer wrap
  int output2 = buffer[temp]; // obtiene el ejemplo adyasente - get adjacent sample
  if (toggle_position=1) output2 = buffer[temp]+input;
  if (toggle_position=0) output2 = buffer[temp];
  
  // intercala entre los ejemplos adyasentes - interpolate between adjacent samples
  int temp4; // crea algunas variables temporales - create some temp variables
  int temp5;

  // multiplica por la distacia a la posicion fracional - multiply by distance to fractional position
  MultiSU16X8toH16(temp4, output, (0xff - (fractional & 0x00ff)));
  MultiSU16X8toH16(temp5, output2, (fractional & 0x00ff));
  output = temp4 + temp5; // sum weighted samples
  
  // guarda el valor para reproducirlo en la siguiente interrupcion 
  data_buffer = output; 
  
 
  if (dir) {
    if ((fractional >> 8) >= MAX) dir = 0;
    fractional += counter;
  }
  else {
    if ((fractional >> 8) <= MIN) dir = 1;
    fractional -= counter;;
  } 
}
