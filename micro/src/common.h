/* 
 * File:   common.h
 * Author: Tim
 *
 * Created on 7 August 2012, 6:58 PM
 */

#ifndef COMMON_H
    #define COMMON_H
    #include <stdio.h>
    
    #define TEST

    // When debug mode is enabled acts as printf, otherwise does nothing
    #ifdef DEBUG
        #define DEBUG_PRINT(a, ...) printf(a, ##__VA_ARGS__)
    #else
        #define DEBUG_PRINT(a, ...) (void)0
    #endif

    /**
     * Definitions for development board
     */
    #ifdef ARDUINO
        #define F_CPU 16000000UL
        #define UBRRH_def UBRR0H
        #define UBRRL_def UBRR0L
        #define UCSRA_def UCSR0A
        #define U2X_def U2X0
        #define UCSRC_def UCSR0C
        #define UCSRB_def UCSR0B
        #define UCSZ1_def UCSZ01
        #define UCSZ0_def UCSZ00
        #define RXEN_def RXEN0
        #define TXEN_def TXEN0
        #define UDR_def UDR0
        #define UDRE_def UDRE0
        #define TIMSK_def TIMSK1
        #define PCMSK_def PCMSK
        #define MCUCR_def MCUCR
        #define GIMSK_def GIMSK
        #define INT0_def INT0
        #define F_CPU 16000000UL
        #define LATCH_PORT1 PORTB
        #define LATCH1_DDR DDRB
        #define LATCH_PORT2 PORTD
        #define LATCH2_DDR DDRD
        #define LATCH1_MEMORY 0x30
        #define LATCH2_MEMORY 0xA0
        #define LATCH1_CLEAR 0x10
        #define LATCH2_CLEAR 0x20
        #define LATCH1_SET 0x20
        #define LATCH2_SET 0x80
        #define EICR_def EICRA
        #define EIMSK_def EIMSK
        #define EICR_value 0x0F
        #define EIMSK_IR_VALUE 0x01
        #define EIMSK_OPTIC_VALUE 0x02
        #define SENSOR_PIN PIND
        #define SENSOR_OPTICAL_MASK 0x02
        #define SENSOR_IR_MASK 0x01
        #define LATCH1_DDRMASK 0x37
        #define LATCH2_DDRMASK 0xE0
        #define ALARM_LED_PORT PORTC
        #define ALARM_BUZZER_PORT PORTC
        #define ALARM_LED_VALUE 0x10
        #define ALARM_BUZZER_VALUE 0x20
        #define PM_LED_VALUE 0x08
        #define PM_LED_PORT PORTC
        #define WEATHER_PORT PORTC
        #define WEATHER_CLEAR_MASK 0x07
        #define WEATHER_FINE 0x01
        #define WEATHER_CLOUDY 0x02
        #define WEATHER_RAINY 0x04
        #define SENSOR_PORT PORTD
        #define SENSOR_DDR_MASK 0x03
        #define CLOCK_COUNT 0x0BDC
    #else
        #define UBRRH_def UBRR0H
        #define UBRRL_def UBRR0L
        #define UCSRA_def UCSR0A
        #define U2X_def U2X0
        #define UCSRC_def UCSR0C
        #define UCSRB_def UCSR0B
        #define UCSZ1_def UCSZ01
        #define UCSZ0_def UCSZ00
        #define RXEN_def RXEN0
        #define TXEN_def TXEN0
        #define UDR_def UDR0
        #define UDRE_def UDRE0
        #define TIMSK_def TIMSK1
        #define F_CPU 8000000UL 
        #define LATCH_PORT1 PORTB
        #define LATCH1_DDR DDRB
        #define LATCH_PORT2 PORTD
        #define LATCH2_DDR DDRD
        #define LATCH1_MEMORY 0x30
        #define LATCH2_MEMORY 0xA0
        #define LATCH1_CLEAR 0x10
        #define LATCH2_CLEAR 0x20
        #define LATCH1_SET 0x20
        #define LATCH2_SET 0x80
        #define EICR_def EICRA
        #define EIMSK_def EIMSK
        #define EICR_value 0x0F
        #define EIMSK_IR_VALUE 0x01
        #define EIMSK_OPTIC_VALUE 0x02
        #define SENSOR_PIN PIND
        #define SENSOR_OPTICAL_MASK 0x08
        #define LATCH1_DDRMASK 0x37
        #define LATCH2_DDRMASK 0xE0
        #define ALARM_LED_PORT PORTC
        #define ALARM_BUZZER_PORT PORTC
        #define ALARM_LED_VALUE 0x10
        #define ALARM_BUZZER_VALUE 0x20
        #define PM_LED_VALUE 0x08
        #define PM_LED_PORT PORTC
        #define WEATHER_PORT PORTC
        #define WEATHER_CLEAR_MASK 0x07
        #define WEATHER_FINE 0x01
        #define WEATHER_CLOUDY 0x02
        #define WEATHER_RAINY 0x04
        #define SENSOR_PORT PORTD
        #define SENSOR_DDR_MASK 0x0C
        #define CLOCK_COUNT 34286
        #define SENSOR_IR_MASK 0x04
    #endif

    volatile int commandWaiting;

    void setup();
    void loop();

#endif	/* COMMON_H */

