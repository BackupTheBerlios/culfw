#ifndef _BOARD_H
#define _BOARD_H

// Feature definitions
#define BOARD_ID_STR            "CUL868"
#define BOARD_ID_USTR           L"CUL868"

#define MULTI_FREQ_DEVICE	// available in multiple versions: 433MHz,868MHz
#define BOARD_ID_STR433         "CUL433"
#define BOARD_ID_USTR433        L"CUL433"

#define HAS_USB                  1
#define USB_BUFSIZE             64      // Must be a supported USB endpoint size
#define USB_MAX_POWER	       100
#define HAS_FHT_80b                     // PROGMEM: 1374b, RAM: 90b
#define HAS_FHT_8v                      // PROGMEM:  586b  RAM: 23b
#define HAS_RF_ROUTER                   // PROGMEM: 1248b  RAM: 44b

#if defined(CUL_V3) || defined(CUL_V4)
#  define TTY_BUFSIZE           64      // RAM: TTY_BUFSIZE*4
#  define FHTBUF_SIZE          174      //                 RAM: 174b
#  define RCV_BUCKETS            4      //                 RAM: 25b * bucket
#  define RFR_DEBUG                     // PROGMEM:  354b  RAM: 14b
#  define FULL_CC1100_PA                // PROGMEM:  108b
#  define HAS_RAWSEND                   //
#  define HAS_FASTRF                    // PROGMEM:  468b  RAM:  1b
#  define HAS_ASKSIN
#  define HAS_ESA
#  define HAS_TX3
#  define HAS_INTERTECHNO
#  define HAS_MEMFN
#endif

#ifdef CUL_V2
#  define TTY_BUFSIZE           48
#  define FHTBUF_SIZE           74
#  define RCV_BUCKETS            2 
#  define RFR_SHADOW                    // PROGMEM: 10b    RAM: -(TTY_BUFSIZE+3)
#  define HAS_TX3
#endif

#ifdef CUL_V2_HM
#  define CUL_V2
#  define HAS_ASKSIN
#  define TTY_BUFSIZE           64
#  define RCV_BUCKETS            2 
#  undef  HAS_RF_ROUTER
#  undef  HAS_FHT_80b
#  undef  HAS_FHT_8v
#  define FHTBUF_SIZE            0
#  undef  BOARD_ID_STR
#  define BOARD_ID_STR            "CUL_HM"
#  undef  BOARD_ID_USTR
#  define BOARD_ID_USTR           L"CUL_HM"
#  define HAS_INTERTECHNO
#endif

// No features to define below

#include <avr/io.h>
#include <avr/power.h>

#ifndef clock_prescale_set
#  warning "avr/power.h needs patching for prescaler functions to work."
#  warning "for the m32u4 add __AVR_ATmega32U4__ for cpu types on prescale block"
#  warning "for the m32u2 add __AVR_ATmega32U2__ for cpu types on prescale block"
#endif

#if defined(CUL_V3)      // not sure why libc is missing those ...
#  define PB0 PORTB0
#  define PB1 PORTB1
#  define PB2 PORTB2
#  define PB3 PORTB3
#  define PB6 PORTB6
#  define PD2 PORTD2
#  define PD3 PORTD3
#endif  // CUL_V3

#define SPI_PORT		PORTB
#define SPI_DDR			DDRB
#define SPI_SS			PB0
#define SPI_MISO		PB3
#define SPI_MOSI		PB2
#define SPI_SCLK		PB1

#if defined(CUL_V4)
#  define CC1100_CS_DDR		SPI_DDR
#  define CC1100_CS_PORT        SPI_PORT
#  define CC1100_CS_PIN		SPI_SS
#  define CC1100_OUT_DDR        DDRD
#  define CC1100_OUT_PORT       PORTD
#  define CC1100_OUT_PIN        PD3
#  define CC1100_IN_DDR         DDRD
#  define CC1100_IN_PORT        PIND
#  define CC1100_IN_PIN         PD2
#  define CC1100_INT		INT2
#  define CC1100_INTVECT        INT2_vect
#  define CC1100_ISC		ISC20
#  define CC1100_EICR           EICRA
#  define LED_DDR               DDRC
#  define LED_PORT              PORTC
#  define LED_PIN               PC5
#endif

#if defined(CUL_V3)
#  define CC1100_CS_DDR		SPI_DDR
#  define CC1100_CS_PORT        SPI_PORT
#  define CC1100_CS_PIN		SPI_SS
#  define CC1100_OUT_DDR        DDRD
#  define CC1100_OUT_PORT       PORTD
#  define CC1100_OUT_PIN        PD3
#  define CC1100_IN_DDR		DDRD
#  define CC1100_IN_PORT        PIND
#  define CC1100_IN_PIN         PD2
#  define CC1100_INT		INT2
#  define CC1100_INTVECT        INT2_vect
#  define CC1100_ISC		ISC20
#  define CC1100_EICR           EICRA
#  define LED_DDR               DDRE
#  define LED_PORT              PORTE
#  define LED_PIN               6
#endif

#if defined(CUL_V2)
#  define CC1100_CS_DDR		DDRC
#  define CC1100_CS_PORT        PORTC
#  define CC1100_CS_PIN		PC5
#  define CC1100_IN_DDR		DDRC
#  define CC1100_IN_PORT        PINC
#  define CC1100_IN_PIN         PC7
#  define CC1100_OUT_DDR	DDRC
#  define CC1100_OUT_PORT       PORTC
#  define CC1100_OUT_PIN        PC6
#  define CC1100_INT		INT4
#  define CC1100_INTVECT        INT4_vect
#  define CC1100_ISC		ISC40
#  define CC1100_EICR           EICRB
#  define LED_DDR               DDRC
#  define LED_PORT              PORTC
#  define LED_PIN               PC4
#endif

#endif // __BOARD_H__
