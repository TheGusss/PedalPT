#include "AudioDspDriver.h"  // Libreria principal

AudioDspDriver dsp_driver;

#define MAX_DELAY (700)
int16_t delay_buffer[MAX_DELAY];
int delay_tail = 0;
int delay_head = 1;
volatile int delay_depth = 2;

void setup() {
  dsp_driver.init();
}

void loop() {
  dsp_driver.process_controls();
  delay_depth = map(dsp_driver.pb_level(), 0, 1024, 0, MAX_DELAY);
}

/* transformador
  * la señal de entrada tiene signo de 16 bits (valores de -32768 a +32767, INT16_MIN a INT16_MAX)
  * pb_level es 0-1024 con punto medio 512
  */
int16_t transformer(int16_t input, int pb_level) {

  delay_head++;
  if(delay_head >= delay_depth) delay_head = 0;

  delay_tail++;
  if(delay_tail >= delay_depth) delay_tail = 0;

  delay_buffer[delay_head] = input;

  return input + delay_buffer[delay_tail];

}

/* Rutina de servicio de interrupción del temporizador 1
  Realiza la entrada/procesamiento/salida de audio aquí
  */
ISR(TIMER1_CAPT_vect) {
  dsp_driver.transform(&transformer);
}