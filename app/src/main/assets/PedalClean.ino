#include "AudioDspDriver.h"  // Libreria principal

AudioDspDriver dsp_driver;

void setup() {
  dsp_driver.init();
}

void loop() {
  dsp_driver.process_controls();
}

/* Rutina de servicio de interrupción del temporizador 1
  Realiza la entrada/procesamiento/salida de audio aquí
  */
ISR(TIMER1_CAPT_vect) {
  dsp_driver.transform();
}
