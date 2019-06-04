/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef __ARDUINO_H__
#define __ARDUINO_H__

#include <stddef.h>
#include <stdint.h>
#include <math.h>

#include <LPC17xx.h>

#include <binary.h>
#include <const_functions.h>
#include <pinmapping.h>
#include <gpio.h>

#include <pwm.h>
using boolean = bool;

#define HIGH         0x01
#define LOW          0x00

#define INPUT          0x00
#define OUTPUT         0x01
#define INPUT_PULLUP   0x02
#define INPUT_PULLDOWN 0x03

#define LSBFIRST     0
#define MSBFIRST     1

#define CHANGE       0x02
#define FALLING      0x03
#define RISING       0x04

typedef uint8_t byte;
#define PROGMEM
#define PSTR(v) (v)
#define PGM_P const char *

using util::min;
using util::max;
using util::abs;
using std::isnan;
using std::isinf;

#define sq(v) ((v) * (v))
#define square(v) sq(v)
#define constrain(value, arg_min, arg_max) ((value) < (arg_min) ? (arg_min) :((value) > (arg_max) ? (arg_max) : (value)))

//Interrupts
void cli(void); // Disable
void sei(void); // Enable
void noInterrupts();
void interrupts();

void attachInterrupt(const pin_t pin, void (*callback)(void), uint32_t mode);
void detachInterrupt(const pin_t pin);
extern "C" void GpioEnableInt(uint32_t port, uint32_t pin, uint32_t mode);
extern "C" void GpioDisableInt(uint32_t port, uint32_t pin);

// Program Memory
#define pgm_read_ptr(addr)        (*((void**)(addr)))
#define pgm_read_byte_near(addr)  (*((uint8_t*)(addr)))
#define pgm_read_float_near(addr) (*((float*)(addr)))
#define pgm_read_word_near(addr)  (*((uint16_t*)(addr)))
#define pgm_read_dword_near(addr) (*((uint32_t*)(addr)))
#define pgm_read_byte(addr)       pgm_read_byte_near(addr)
#define pgm_read_float(addr)      pgm_read_float_near(addr)
#define pgm_read_word(addr)       pgm_read_word_near(addr)
#define pgm_read_dword(addr)      pgm_read_dword_near(addr)

#define memcpy_P memcpy
#define sprintf_P sprintf
#define strstr_P strstr
#define strncpy_P strncpy
#define vsnprintf_P vsnprintf
#define strcpy_P strcpy
#define snprintf_P snprintf
#define strlen_P strlen
#define strchr_P strchr

// Time functions
extern "C" {
  void delay(const int milis);
}
void _delay_ms(const int delay);
void delayMicroseconds(unsigned long);
uint32_t millis();
uint32_t micros();

//IO functions
void pinMode(const pin_t, const uint8_t);
void digitalWrite(pin_t, uint8_t);
bool digitalRead(pin_t);
void analogWrite(pin_t, int);
uint16_t analogRead(pin_t);

// EEPROM
void eeprom_write_byte(uint8_t *pos, unsigned char value);
uint8_t eeprom_read_byte(uint8_t *pos);
void eeprom_read_block (void *__dst, const void *__src, size_t __n);
void eeprom_update_block (const void *__src, void *__dst, size_t __n);

int32_t random(int32_t);
int32_t random(int32_t, int32_t);
void randomSeed(uint32_t);

char *dtostrf (double __val, signed char __width, unsigned char __prec, char *__s);

using util::map;

void tone(const pin_t _pin, const uint32_t frequency, const uint32_t duration = 0);
void noTone(const pin_t _pin);

#include "HardwareSerial.h"

#endif // __ARDUINO_DEF_H__
