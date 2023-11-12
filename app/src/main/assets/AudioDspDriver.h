#ifndef AudioDspDriver_h
#define AudioDspDriver_h

#define __STDC_LIMIT_MACROS
#include <stdint.h>

#include <Arduino.h>

#define DEFAULT_LED_PIN (13)
#define DEFAULT_FOOTSWITCH_PIN (12)
#define DEFAULT_TOGGLE_PIN (2)
#define DEFAULT_PUSHBUTTON_LEFT_PIN (A5)
#define DEFAULT_PUSHBUTTON_RIGHT_PIN (A4)
#define DEFAULT_PUSHBUTTON_STEP (4)       // movimiento de nivel para cada push button
#define DEFAULT_PWM_FREQ (0x00FF)         // pwm frecuencia - 31.3KHz
#define DEFAULT_PWM_MODE (0)              // Fast (1) o Phase (0)
#define DEFAULT_PWM_QTY (2)


class AudioDspDriver {

  public:

    AudioDspDriver();

    /*
     * Inicializar controles y puertos.
     */
    void init();

    /*
     * Inicializar ADC.
     */
    void init_adc();

    /*
     * Inicializar PWM.
     */
    void init_pwm();

    /*
     * Restablecer todas las configuraciones a los valores predeterminados.
     */
    void reset();

    /*
     * Escanea y actualiza todos los controles.
     */
    void process_controls();

    /*
     * Enciende el LED si el efecto está activado.
     */
    void process_footswitch();

    /*
     * Los botones controlan el volumen arriba/abajo
     */
    void process_pushbuttons();

    /*
     *Comprueba el estado del interruptor de palanca.
     */
    void process_toggle();

    /*
     * Controlar el LED (encendido/apagado)
     */
    void led(bool state);

    /*
     * Leer valor ADC. Establece y devuelve `entrada_actual`.
     */
    int16_t read();

    /*
     * Escribir el valor de `salida_actual`
     */
    void write();

    /*
     * Lectura/escritura de entrada a salida con transformación estándar.
     * Establece `entrada_actual` y `salida_actual` como subproducto.
     */
    void transform();

    /*
     * Entrada de lectura/escritura a salida con función de transformador suministrada.
      * la función de transformador toma dos parámetros: entrada y pb_level
      * transformador int (entrada int, pb_level int)
      * la entrada es una señal de entrada con signo de 16 bits (valores de -32768 a +32768, INT16_MIN a INT16_MAX)
      * pb_level es 0-1024 con punto medio 512
      * Establece `entrada_actual` y `salida_actual` como subproducto.
     */
    void transform(int16_t (*transformer)(int16_t, int));

   /*
     Comando: establecer el nivel de los pulsadores.
      pb_level es 0-1024 con punto medio 512
      */
    void pb_level(int new_level);

   /*
      Devuelve el nivel actual del pulsador.
      pb_level es 0-1024 con punto medio 512
      */
    int pb_level();

   /*
      Devuelve el estado actual del interruptor de palanca.
      */
    bool toggle_state();
/*
      * Devuelve el estado actual del footswitch.
      */
    bool footswitch_state();

    volatile int16_t current_input;
    volatile int16_t current_output;

  private:

    int led_pin;
    int footswitch_pin;
    int toggle_pin;
    int pb_left_pin;
    int pb_right_pin;

    int pb_level_value;
    byte footswitch_value;
    byte toggle_value;

    byte pb_step;

    int pwm_freq;
    int pwm_mode;
    int pwm_qty;

};


#endif

