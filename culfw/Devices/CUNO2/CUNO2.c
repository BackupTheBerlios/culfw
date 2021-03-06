/* Copyright Rudolf Koenig, 2008.
   Released under the GPL Licence, Version 2
*/

#include <avr/boot.h>
#include <avr/power.h>
#include <avr/eeprom.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/wdt.h>

#include <string.h>

#include "spi.h"
#include "cc1100.h"
#include "clock.h"
#include "delay.h"
#include "display.h"
#include "serial.h"
#include "fncollection.h"
#include "led.h"
#include "ringbuffer.h"
#include "rf_receive.h"
#include "rf_send.h"
#include "ttydata.h"
#include "fht.h"
#include "fastrf.h"
#include "rf_router.h"
#include "ethernet.h"
#include "tcplink.h"
#include "ntp.h"
#include "memory.h"
#include "onewire.h"

#include "i2cmaster.h"

#ifdef HAS_ASKSIN
#include "rf_asksin.h"
#endif

#if defined (HAS_IRRX) || defined (HAS_IRTX)
#include "ir.h"
#endif


PROGMEM t_fntab fntab[] = {

  { 'm', getfreemem },

  { 'B', prepare_boot },
  { 'C', ccreg },
  { 'F', fs20send },
#ifdef HAS_INTERTECHNO
  { 'i', it_func },
#endif
#ifdef HAS_ASKSIN
  { 'A', asksin_func },
#endif
#if defined (HAS_IRRX) || defined (HAS_IRTX)
  { 'I', ir_func },
#endif
#ifdef HAS_RAWSEND
  { 'G', rawsend },
  { 'M', em_send },
//  { 'S', esa_send },
#endif
  { 'R', read_eeprom },
  { 'T', fhtsend },
  { 'V', version },
  { 'W', write_eeprom },
  { 'X', set_txreport },
#ifdef HAS_ONEWIRE  
  { 'O', onewire_func },
#endif  
  { 'e', eeprom_factory_reset },
#ifdef HAS_FASTRF
  { 'f', fastrf_func },
#endif
  { 'l', ledfunc },
  { 't', gettime },
#ifdef HAS_RF_ROUTER
  { 'u', rf_router_func },
#endif
  { 'x', ccsetpa },
  { 'E', eth_func },
  { 'c', ntp_func },
  { 'q', tcplink_close },

  { 0, 0 },
};


void
start_bootloader(void)
{
  cli();

  /* move interrupt vectors to bootloader section and jump to bootloader */
  MCUCR = _BV(IVCE);
  MCUCR = _BV(IVSEL);

#define jump_to_bootloader ((void(*)(void))0x1FC00)
  jump_to_bootloader();
}

int
main(void)
{
  wdt_disable();

  // un-reset ethernet
  ENC28J60_RESET_DDR  |= _BV( ENC28J60_RESET_BIT );
  ENC28J60_RESET_PORT |= _BV( ENC28J60_RESET_BIT );
  
  led_init();
  LED_ON();

  spi_init();

  eeprom_init();

  // if we had been restarted by watchdog check the REQ BootLoader byte in the
  // EEPROM ...
  if(bit_is_set(MCUSR,WDRF) && eeprom_read_byte(EE_REQBL)) {
    eeprom_write_byte( EE_REQBL, 0 ); // clear flag
// TBD: This is useless as button needs to be pressed - needs moving into bootloader directly
//    start_bootloader();
  }


// Setup OneWire and make a full search at the beginning (takes some time)
#ifdef HAS_ONEWIRE
  i2c_init();
  onewire_Init();
  onewire_FullSearch();
#endif

  // Setup the timers. Are needed for watchdog-reset

#if defined (HAS_IRRX) || defined (HAS_IRTX)
  ir_init();
  // IR uses highspeed TIMER0 for sampling 
  OCR0A  = 1;                              // Timer0: 0.008s = 8MHz/256/2   == 15625Hz
#else
  OCR0A  = 249;                            // Timer0: 0.008s = 8MHz/256/250 == 125Hz
#endif

  TCCR0B = _BV(CS02);
  TCCR0A = _BV(WGM01);
  TIMSK0 = _BV(OCIE0A);

  TCCR1A = 0;
  TCCR1B = _BV(CS11) | _BV(WGM12);         // Timer1: 1us = 8MHz/8

  clock_prescale_set(clock_div_1);

  MCUSR &= ~(1 << WDRF);                   // Enable the watchdog
  wdt_enable(WDTO_2S);

  uart_init( UART_BAUD_SELECT_DOUBLE_SPEED(UART_BAUD_RATE,F_CPU) );

  fht_init();
  tx_init();
  input_handle_func = analyze_ttydata;
#ifdef HAS_RF_ROUTER
  rf_router_init();
  display_channel = (DISPLAY_USB|DISPLAY_RFROUTER);
#else
  display_channel = DISPLAY_USB;
#endif

  ethernet_init();
    
  LED_OFF();

  sei();

  for(;;) {
    uart_task();
    RfAnalyze_Task();
    Minute_Task();
#ifdef HAS_FASTRF
    FastRF_Task();
#endif
#ifdef HAS_RF_ROUTER
    rf_router_task();
#endif
#ifdef HAS_ASKSIN
    rf_asksin_task();
#endif
#ifdef HAS_IRRX
    ir_task();
#endif
#ifdef HAS_ETHERNET
    Ethernet_Task();
#endif
  }

}
