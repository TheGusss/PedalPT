#include "AudioDspDriver.h"


AudioDspDriver::AudioDspDriver() {
  reset();
}


void AudioDspDriver::reset() {
  this->led_pin = DEFAULT_LED_PIN;
  this->footswitch_pin = DEFAULT_FOOTSWITCH_PIN;
  this->toggle_pin = DEFAULT_TOGGLE_PIN;
  this->pb_left_pin = DEFAULT_PUSHBUTTON_LEFT_PIN;
  this->pb_right_pin = DEFAULT_PUSHBUTTON_RIGHT_PIN;
  this->pb_step = DEFAULT_PUSHBUTTON_STEP;
  this->pwm_freq = DEFAULT_PWM_FREQ;
  this->pwm_mode = DEFAULT_PWM_MODE;
  this->pwm_qty = DEFAULT_PWM_QTY;
  this->pb_level_value = 512;
  this->current_input = 0;
  this->current_output = 0;
}


void AudioDspDriver::init() {
 // configurar IO - configuraciones de pines de entradas/salidas y pull-ups
  pinMode(led_pin, OUTPUT);
  pinMode(footswitch_pin, INPUT_PULLUP);
  pinMode(toggle_pin, INPUT_PULLUP);
  pinMode(pb_left_pin, INPUT_PULLUP);
  pinMode(pb_right_pin, INPUT_PULLUP);
  init_adc();
  init_pwm();
  sei();
}


void AudioDspDriver::init_adc() {
  // configurar ADC: configurado para leer automáticamente todo el tiempo.

  ADMUX = 0x60;  /* Registro de selección de multiplexor ADC = b01100000
                   * REFS[1:0] = 01 = AVCC con capacitor externo en el pin AREF
                   * ADLAR: ADC Ajuste Izquierdo Resultado = 1
                   * MUX[3:0]: Bits de selección de canal analógico = 0 = ADC0
                   */
  ADCSRA = 0xe5; /* Registro de estado y control ADC A = b11100101
                   * ADÉN: Habilitar ADC
                   * ADSC: conversión de inicio de ADC
                   * ADATE: Habilitar activación automática de ADC
                   * ADPS[2:0] Prescaler ADC = CLOCK/32
                   */
  ADCSRB = 0x07;/* Registro de estado y control ADC B = b00000111
                   * ADTS[2:0]: Fuente de activación automática de ADC = 111 = Evento de captura de temporizador/contador1
                   */
  DIDR0 = 0x01;  /* Registro de deshabilitación de entrada digital 0
                   * Búfer de entrada digital ADC0D para adc0 deshabilitado
                   */
}


void AudioDspDriver::init_pwm() {

  /* TCCR1A: Registro de control de temporizador/contador1 A
    * COM1A1:0: modo de salida de comparación para el canal A = 10 = Borrar OC1A en la coincidencia de comparación
    * COM1B1:0: Modo de salida de comparación para el canal B = (PWM_QTY - 1) << 5 = 10 para PWM_QTY = 2 = Borrar OC1B en Comparar coincidencia
    * WGM11:0: Modo de generación de forma de onda = PWM_MODE << 1
    */
  TCCR1A = (((pwm_qty - 1) << 5) | 0x80 | (pwm_mode << 1));

 /* TCCR1B: Registro B de control de temporizador/contador1
    * WGM13: Modo de generación de forma de onda = 1
    * WGM12: Modo de generación de forma de onda = PWM_MODE
    * Clock Select = 1 = clk/1 (sin escala previa)
    */
  TCCR1B = ((pwm_mode << 3) | 0x11);

  /* TIMSK1: Registro de máscara de interrupción del temporizador/contador1
    * 0x20 =
    * ICIE1 no habilitado: Timer/Counter1, Habilitar interrupción de captura de entrada
    * OCIE1A habilitado: temporizador/contador 1, comparación de salida, una interrupción de coincidencia habilitada
    */
  TIMSK1 = 0x20;

  /* ICR1H e ICR1L: Registro de captura de entrada 1
    * La captura de entrada se actualiza con el valor del contador (TCNT1) cada vez que ocurre un evento en el pin ICP1 (u opcionalmente en el analógico
    * Salida del comparador para Timer/Contador1). La captura de entrada se puede utilizar para definir el valor SUPERIOR del contador.
    */
  ICR1H = (pwm_freq >> 8);
  ICR1L = (pwm_freq & 0xff);

 /* DDRB: El registro de dirección de datos del puerto B  */

  DDRB |= ((pwm_qty << 1) | 0x02); // enciende las salidas pwm

}


void AudioDspDriver::process_controls() {
  process_footswitch();
  process_pushbuttons();
  process_toggle();
}


void AudioDspDriver::process_footswitch() {
  this->footswitch_value = digitalRead(footswitch_pin);
  led(footswitch_value);
}

void AudioDspDriver::process_toggle() {
  this->toggle_value = digitalRead(toggle_pin);
}


void AudioDspDriver::process_pushbuttons() {

  if (!digitalRead(pb_right_pin)) {
    //aumenta al presionar el boton de la derecha
    if (pb_level_value<1024) this->pb_level_value += pb_step;
  }
  if (!digitalRead(pb_left_pin)) {
   // disminuye al presionar el boton de la izquierda
    if (pb_level_value>0) this->pb_level_value -= pb_step;
  }

}


void AudioDspDriver::led(bool state) {
  digitalWrite(led_pin, state);
}


int16_t AudioDspDriver::read() {
  uint8_t ADC_low, ADC_high;

  // lee los datos de la señal de entrada del ADC: 2 bytes bajo y alto.
   // El byte bajo debe obtenerse primero
  ADC_low = ADCL;
  ADC_high = ADCH;

  // construye la muestra de entrada sumando el byte bajo y alto del ADC.
  this->current_input = ((ADC_high << 8) | ADC_low) - 0x8000; // crea un valor de 16b con signo
  this->current_output = this->current_input;
  return current_output;
}


void AudioDspDriver::write() {
 // escribir la señal de salida PWM
  OCR1AL = ((current_output + 0x8000) >> 8); // convertir a sin asignar, envia byte alto
  OCR1BL = current_output;// enviar byte bajo
}


void AudioDspDriver::transform() {
  read();
  write();
}


void AudioDspDriver::transform(int16_t (*transformer)(int16_t, int)) {
  read();
  this->current_output = (*transformer)(current_output, pb_level_value);
  write();
}


void AudioDspDriver::pb_level(int new_level) {
  this->pb_level_value = new_level;
}

int AudioDspDriver::pb_level() {
  return pb_level_value;
}

bool AudioDspDriver::toggle_state() {
  return (bool)toggle_value;
}

bool AudioDspDriver::footswitch_state() {
  return (bool)footswitch_value;
}