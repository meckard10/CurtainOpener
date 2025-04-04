#ifndef PINS_h
#define PINS_h

#if defined(__MSP430FR5739__)

/* ------------------------------------------------------------------------------------------------
 *                                      GDO0 Pin Configuration
 * ------------------------------------------------------------------------------------------------
 */
#define GDO0_BIT__ 5
#define GDO0_PORT__ 2
#define CONFIG_GDO0_PIN_AS_INPUT() st(INFIX(P, GDO0_PORT__, SEL0) &= ~BV(GDO0_BIT__);) /* clear pin special function default */
#define GDO0_PIN_IS_HIGH() (INFIX(P, GDO0_PORT__, IN) & BV(GDO0_BIT__))

#define GDO0_INT_VECTOR INFIX(PORT, GDO0_PORT__, _VECTOR)
#define ENABLE_GDO0_INT() st(INFIX(P, GDO0_PORT__, IE) |= BV(GDO0_BIT__);)   /* atomic operation */
#define DISABLE_GDO0_INT() st(INFIX(P, GDO0_PORT__, IE) &= ~BV(GDO0_BIT__);) /* atomic operation */
#define GDO0_INT_IS_ENABLED() (INFIX(P, GDO0_PORT__, IE) & BV(GDO0_BIT__))
#define CLEAR_GDO0_INT_FLAG() st(INFIX(P, GDO0_PORT__, IFG) &= ~BV(GDO0_BIT__);) /* atomic operation */
#define GDO0_INT_FLAG_IS_SET() (INFIX(P, GDO0_PORT__, IFG) & BV(GDO0_BIT__))
#define CONFIG_GDO0_RISING_EDGE_INT() st(INFIX(P, GDO0_PORT__, IES) &= ~BV(GDO0_BIT__);) /* atomic operation */
#define CONFIG_GDO0_FALLING_EDGE_INT() st(INFIX(P, GDO0_PORT__, IES) |= BV(GDO0_BIT__);) /* atomic operation */

/* ------------------------------------------------------------------------------------------------
 *                                      GDO2 Pin Configuration
 * ------------------------------------------------------------------------------------------------
 */
#define GDO2_BIT__ 6
#define GDO2_PORT__ 3
#define CONFIG_GDO2_PIN_AS_INPUT() st(INFIX(P, GDO2_PORT__, SEL0) &= ~BV(GDO2_BIT__);) /* clear pin special function default */
#define GDO2_PIN_IS_HIGH() (INFIX(P, GDO2_PORT__, IN) & BV(GDO2_BIT__))

#define GDO2_INT_VECTOR INFIX(PORT, GDO2_PORT__, _VECTOR)
#define ENABLE_GDO2_INT() st(INFIX(P, GDO2_PORT__, IE) |= BV(GDO2_BIT__);)   /* atomic operation */
#define DISABLE_GDO2_INT() st(INFIX(P, GDO2_PORT__, IE) &= ~BV(GDO2_BIT__);) /* atomic operation */
#define GDO2_INT_IS_ENABLED() (INFIX(P, GDO2_PORT__, IE) & BV(GDO2_BIT__))
#define CLEAR_GDO2_INT_FLAG() st(INFIX(P, GDO2_PORT__, IFG) &= ~BV(GDO2_BIT__);) /* atomic operation */
#define GDO2_INT_FLAG_IS_SET() (INFIX(P, GDO2_PORT__, IFG) & BV(GDO2_BIT__))
#define CONFIG_GDO2_RISING_EDGE_INT() st(INFIX(P, GDO2_PORT__, IES) &= ~BV(GDO2_BIT__);) /* atomic operation */
#define CONFIG_GDO2_FALLING_EDGE_INT() st(INFIX(P, GDO2_PORT__, IES) |= BV(GDO2_BIT__);) /* atomic operation */

/* ------------------------------------------------------------------------------------------------
 *                                      SPI Configuration
 * ------------------------------------------------------------------------------------------------
 */

/* CSn Pin Configuration */
#define SPI_CSN_GPIO_BIT__ 3
#define SPI_CONFIG_CSN_PIN_AS_OUTPUT() st(P1DIR |= BV(SPI_CSN_GPIO_BIT__);)
#define SPI_DRIVE_CSN_HIGH() st(P1OUT |= BV(SPI_CSN_GPIO_BIT__);) /* atomic operation */
#define SPI_DRIVE_CSN_LOW() st(P1OUT &= ~BV(SPI_CSN_GPIO_BIT__);) /* atomic operation */
#define SPI_CSN_IS_HIGH() (P1OUT & BV(SPI_CSN_GPIO_BIT__))

/* SCLK Pin Configuration */
#define SPI_SCLK_GPIO_BIT__ 2
#define SPI_CONFIG_SCLK_PIN_AS_OUTPUT() st(P2DIR |= BV(SPI_SCLK_GPIO_BIT__);)
#define SPI_DRIVE_SCLK_HIGH() st(P2OUT |= BV(SPI_SCLK_GPIO_BIT__);)
#define SPI_DRIVE_SCLK_LOW() st(P2OUT &= ~BV(SPI_SCLK_GPIO_BIT__);)

/* SI Pin Configuration */
#define SPI_SI_GPIO_BIT__ 6
#define SPI_CONFIG_SI_PIN_AS_OUTPUT() st(P1DIR |= BV(SPI_SI_GPIO_BIT__);)
#define SPI_DRIVE_SI_HIGH() st(P1OUT |= BV(SPI_SI_GPIO_BIT__);)
#define SPI_DRIVE_SI_LOW() st(P1OUT &= ~BV(SPI_SI_GPIO_BIT__);)

/* SO Pin Configuration */
#define SPI_SO_GPIO_BIT__ 7
#define SPI_CONFIG_SO_PIN_AS_INPUT() st(P1DIR &= ~BV(SPI_SO_GPIO_BIT__);)
#define SPI_SO_IS_HIGH() (P1IN & BV(SPI_SO_GPIO_BIT__))

/* SPI Port Configuration */
#define SPI_CONFIG_PORT() st(P1SEL1 |= BV(SPI_SI_GPIO_BIT__) | \
                                       BV(SPI_SO_GPIO_BIT__);  \
                             P2SEL1 |= BV(SPI_SCLK_GPIO_BIT__);)

#define SPI_INIT()                                  \
  st(                                               \
      UCB0CTLW0 |= UCSWRST;                         \
      UCB0CTLW0 |= UCSWRST | UCSSEL_2;              \
      UCB0CTLW0 |= UCCKPH | UCMSB | UCMST | UCSYNC; \
      UCB0BR0 = 0x02;                               \
      UCB0BR1 = 0;                                  \
      SPI_CONFIG_PORT();                            \
      UCB0CTLW0 &= ~UCSWRST;)

/* read/write macros */
#define SPI_WRITE_BYTE(x) st(UCB0IFG &= ~UCRXIFG; UCB0TXBUF = x;)
#define SPI_READ_BYTE() UCB0RXBUF
#define SPI_WAIT_DONE()        \
  while (!(UCB0IFG & UCRXIFG)) \
    ;

#elif defined(__MSP430FR5969__)

/* ------------------------------------------------------------------------------------------------
 *                                      GDO0 Pin Configuration
 * ------------------------------------------------------------------------------------------------
 */
#define GDO0_BIT__ 4
#define GDO0_PORT__ 3
#define CONFIG_GDO0_PIN_AS_INPUT() st(INFIX(P, GDO0_PORT__, SEL0) &= ~BV(GDO0_BIT__);) /* clear pin special function default */
#define GDO0_PIN_IS_HIGH() (INFIX(P, GDO0_PORT__, IN) & BV(GDO0_BIT__))

#define GDO0_INT_VECTOR INFIX(PORT, GDO0_PORT__, _VECTOR)
#define ENABLE_GDO0_INT() st(INFIX(P, GDO0_PORT__, IE) |= BV(GDO0_BIT__);)   /* atomic operation */
#define DISABLE_GDO0_INT() st(INFIX(P, GDO0_PORT__, IE) &= ~BV(GDO0_BIT__);) /* atomic operation */
#define GDO0_INT_IS_ENABLED() (INFIX(P, GDO0_PORT__, IE) & BV(GDO0_BIT__))
#define CLEAR_GDO0_INT_FLAG() st(INFIX(P, GDO0_PORT__, IFG) &= ~BV(GDO0_BIT__);) /* atomic operation */
#define GDO0_INT_FLAG_IS_SET() (INFIX(P, GDO0_PORT__, IFG) & BV(GDO0_BIT__))
#define CONFIG_GDO0_RISING_EDGE_INT() st(INFIX(P, GDO0_PORT__, IES) &= ~BV(GDO0_BIT__);) /* atomic operation */
#define CONFIG_GDO0_FALLING_EDGE_INT() st(INFIX(P, GDO0_PORT__, IES) |= BV(GDO0_BIT__);) /* atomic operation */

/* ------------------------------------------------------------------------------------------------
 *                                      GDO2 Pin Configuration
 * ------------------------------------------------------------------------------------------------
 */
#define GDO2_BIT__ 6
#define GDO2_PORT__ 3
#define CONFIG_GDO2_PIN_AS_INPUT() st(INFIX(P, GDO2_PORT__, SEL0) &= ~BV(GDO2_BIT__);) /* clear pin special function default */
#define GDO2_PIN_IS_HIGH() (INFIX(P, GDO2_PORT__, IN) & BV(GDO2_BIT__))

#define GDO2_INT_VECTOR INFIX(PORT, GDO2_PORT__, _VECTOR)
#define ENABLE_GDO2_INT() st(INFIX(P, GDO2_PORT__, IE) |= BV(GDO2_BIT__);)   /* atomic operation */
#define DISABLE_GDO2_INT() st(INFIX(P, GDO2_PORT__, IE) &= ~BV(GDO2_BIT__);) /* atomic operation */
#define GDO2_INT_IS_ENABLED() (INFIX(P, GDO2_PORT__, IE) & BV(GDO2_BIT__))
#define CLEAR_GDO2_INT_FLAG() st(INFIX(P, GDO2_PORT__, IFG) &= ~BV(GDO2_BIT__);) /* atomic operation */
#define GDO2_INT_FLAG_IS_SET() (INFIX(P, GDO2_PORT__, IFG) & BV(GDO2_BIT__))
#define CONFIG_GDO2_RISING_EDGE_INT() st(INFIX(P, GDO2_PORT__, IES) &= ~BV(GDO2_BIT__);) /* atomic operation */
#define CONFIG_GDO2_FALLING_EDGE_INT() st(INFIX(P, GDO2_PORT__, IES) |= BV(GDO2_BIT__);) /* atomic operation */

/* ------------------------------------------------------------------------------------------------
 *                                      SPI Configuration
 * ------------------------------------------------------------------------------------------------
 */

/* CSn Pin Configuration */
#define SPI_CSN_GPIO_BIT__ 3
#define SPI_CONFIG_CSN_PIN_AS_OUTPUT() st(P1DIR |= BV(SPI_CSN_GPIO_BIT__);)
#define SPI_DRIVE_CSN_HIGH() st(P1OUT |= BV(SPI_CSN_GPIO_BIT__);) /* atomic operation */
#define SPI_DRIVE_CSN_LOW() st(P1OUT &= ~BV(SPI_CSN_GPIO_BIT__);) /* atomic operation */
#define SPI_CSN_IS_HIGH() (P1OUT & BV(SPI_CSN_GPIO_BIT__))

/* SCLK Pin Configuration */
#define SPI_SCLK_GPIO_BIT__ 2
#define SPI_CONFIG_SCLK_PIN_AS_OUTPUT() st(P2DIR |= BV(SPI_SCLK_GPIO_BIT__);)
#define SPI_DRIVE_SCLK_HIGH() st(P2OUT |= BV(SPI_SCLK_GPIO_BIT__);)
#define SPI_DRIVE_SCLK_LOW() st(P2OUT &= ~BV(SPI_SCLK_GPIO_BIT__);)

/* SI Pin Configuration */
#define SPI_SI_GPIO_BIT__ 6
#define SPI_CONFIG_SI_PIN_AS_OUTPUT() st(P1DIR |= BV(SPI_SI_GPIO_BIT__);)
#define SPI_DRIVE_SI_HIGH() st(P1OUT |= BV(SPI_SI_GPIO_BIT__);)
#define SPI_DRIVE_SI_LOW() st(P1OUT &= ~BV(SPI_SI_GPIO_BIT__);)

/* SO Pin Configuration */
#define SPI_SO_GPIO_BIT__ 7
#define SPI_CONFIG_SO_PIN_AS_INPUT() st(P1DIR &= ~BV(SPI_SO_GPIO_BIT__);)
#define SPI_SO_IS_HIGH() (P1IN & BV(SPI_SO_GPIO_BIT__))

/* SPI Port Configuration */
#define SPI_CONFIG_PORT() st(P1SEL1 |= BV(SPI_SI_GPIO_BIT__) | \
                                       BV(SPI_SO_GPIO_BIT__);  \
                             P2SEL1 |= BV(SPI_SCLK_GPIO_BIT__);)

#define SPI_INIT()                                  \
  st(                                               \
      UCB0CTLW0 |= UCSWRST;                         \
      UCB0CTLW0 |= UCSWRST | UCSSEL_2;              \
      UCB0CTLW0 |= UCCKPH | UCMSB | UCMST | UCSYNC; \
      UCB0BR0 = 0x02;                               \
      UCB0BR1 = 0;                                  \
      SPI_CONFIG_PORT();                            \
      UCB0CTLW0 &= ~UCSWRST;)

/* read/write macros */
#define SPI_WRITE_BYTE(x) st(UCB0IFG &= ~UCRXIFG; UCB0TXBUF = x;)
#define SPI_READ_BYTE() UCB0RXBUF
#define SPI_WAIT_DONE()        \
  while (!(UCB0IFG & UCRXIFG)) \
    ;

#elif defined(__MSP430FR6989__)

/* ------------------------------------------------------------------------------------------------
 *                                      GDO0 Pin Configuration
 * ------------------------------------------------------------------------------------------------
 */
#define GDO0_BIT__ 3
#define GDO0_PORT__ 3
#define CONFIG_GDO0_PIN_AS_INPUT() st(INFIX(P, GDO0_PORT__, SEL0) &= ~BV(GDO0_BIT__);) /* clear pin special function default */
#define GDO0_PIN_IS_HIGH() (INFIX(P, GDO0_PORT__, IN) & BV(GDO0_BIT__))

#define GDO0_INT_VECTOR INFIX(PORT, GDO0_PORT__, _VECTOR)
#define ENABLE_GDO0_INT() st(INFIX(P, GDO0_PORT__, IE) |= BV(GDO0_BIT__);)   /* atomic operation */
#define DISABLE_GDO0_INT() st(INFIX(P, GDO0_PORT__, IE) &= ~BV(GDO0_BIT__);) /* atomic operation */
#define GDO0_INT_IS_ENABLED() (INFIX(P, GDO0_PORT__, IE) & BV(GDO0_BIT__))
#define CLEAR_GDO0_INT_FLAG() st(INFIX(P, GDO0_PORT__, IFG) &= ~BV(GDO0_BIT__);) /* atomic operation */
#define GDO0_INT_FLAG_IS_SET() (INFIX(P, GDO0_PORT__, IFG) & BV(GDO0_BIT__))
#define CONFIG_GDO0_RISING_EDGE_INT() st(INFIX(P, GDO0_PORT__, IES) &= ~BV(GDO0_BIT__);) /* atomic operation */
#define CONFIG_GDO0_FALLING_EDGE_INT() st(INFIX(P, GDO0_PORT__, IES) |= BV(GDO0_BIT__);) /* atomic operation */

/* ------------------------------------------------------------------------------------------------
 *                                      GDO2 Pin Configuration
 * ------------------------------------------------------------------------------------------------
 */
#define GDO2_BIT__ 6
#define GDO2_PORT__ 2
#define CONFIG_GDO2_PIN_AS_INPUT() st(INFIX(P, GDO2_PORT__, SEL0) &= ~BV(GDO2_BIT__);) /* clear pin special function default */
#define GDO2_PIN_IS_HIGH() (INFIX(P, GDO2_PORT__, IN) & BV(GDO2_BIT__))

#define GDO2_INT_VECTOR INFIX(PORT, GDO2_PORT__, _VECTOR)
#define ENABLE_GDO2_INT() st(INFIX(P, GDO2_PORT__, IE) |= BV(GDO2_BIT__);)   /* atomic operation */
#define DISABLE_GDO2_INT() st(INFIX(P, GDO2_PORT__, IE) &= ~BV(GDO2_BIT__);) /* atomic operation */
#define GDO2_INT_IS_ENABLED() (INFIX(P, GDO2_PORT__, IE) & BV(GDO2_BIT__))
#define CLEAR_GDO2_INT_FLAG() st(INFIX(P, GDO2_PORT__, IFG) &= ~BV(GDO2_BIT__);) /* atomic operation */
#define GDO2_INT_FLAG_IS_SET() (INFIX(P, GDO2_PORT__, IFG) & BV(GDO2_BIT__))
#define CONFIG_GDO2_RISING_EDGE_INT() st(INFIX(P, GDO2_PORT__, IES) &= ~BV(GDO2_BIT__);) /* atomic operation */
#define CONFIG_GDO2_FALLING_EDGE_INT() st(INFIX(P, GDO2_PORT__, IES) |= BV(GDO2_BIT__);) /* atomic operation */

/* ------------------------------------------------------------------------------------------------
 *                                      SPI Configuration
 * ------------------------------------------------------------------------------------------------
 */

/* CSn Pin Configuration */
#define SPI_CSN_GPIO_BIT__ 7
#define SPI_CONFIG_CSN_PIN_AS_OUTPUT() st(P2DIR |= BV(SPI_CSN_GPIO_BIT__);)
#define SPI_DRIVE_CSN_HIGH() st(P2OUT |= BV(SPI_CSN_GPIO_BIT__);) /* atomic operation */
#define SPI_DRIVE_CSN_LOW() st(P2OUT &= ~BV(SPI_CSN_GPIO_BIT__);) /* atomic operation */
#define SPI_CSN_IS_HIGH() (P2OUT & BV(SPI_CSN_GPIO_BIT__))

/* SCLK Pin Configuration */
#define SPI_SCLK_GPIO_BIT__ 4
#define SPI_CONFIG_SCLK_PIN_AS_OUTPUT() st(P1DIR |= BV(SPI_SCLK_GPIO_BIT__);)
#define SPI_DRIVE_SCLK_HIGH() st(P1OUT |= BV(SPI_SCLK_GPIO_BIT__);)
#define SPI_DRIVE_SCLK_LOW() st(P1OUT &= ~BV(SPI_SCLK_GPIO_BIT__);)

/* SI Pin Configuration */
#define SPI_SI_GPIO_BIT__ 6
#define SPI_CONFIG_SI_PIN_AS_OUTPUT() st(P1DIR |= BV(SPI_SI_GPIO_BIT__);)
#define SPI_DRIVE_SI_HIGH() st(P1OUT |= BV(SPI_SI_GPIO_BIT__);)
#define SPI_DRIVE_SI_LOW() st(P1OUT &= ~BV(SPI_SI_GPIO_BIT__);)

/* SO Pin Configuration */
#define SPI_SO_GPIO_BIT__ 7
#define SPI_CONFIG_SO_PIN_AS_INPUT() st(P1DIR &= ~BV(SPI_SO_GPIO_BIT__);)
#define SPI_SO_IS_HIGH() (P1IN & BV(SPI_SO_GPIO_BIT__))

/* SPI Port Configuration */
#define SPI_CONFIG_PORT() st(P1SEL0 |= BV(SPI_SI_GPIO_BIT__) | \
                                       BV(SPI_SO_GPIO_BIT__);  \
                             P1SEL0 |= BV(SPI_SCLK_GPIO_BIT__);)

#define SPI_INIT()                                  \
  st(                                               \
      UCB0CTLW0 |= UCSWRST;                         \
      UCB0CTLW0 |= UCSWRST | UCSSEL_2;              \
      UCB0CTLW0 |= UCCKPH | UCMSB | UCMST | UCSYNC; \
      UCB0BR0 = 0x02;                               \
      UCB0BR1 = 0;                                  \
      SPI_CONFIG_PORT();                            \
      UCB0CTLW0 &= ~UCSWRST;)

/* read/write macros */
#define SPI_WRITE_BYTE(x) st(UCB0IFG &= ~UCRXIFG; UCB0TXBUF = x;)
#define SPI_READ_BYTE() UCB0RXBUF
#define SPI_WAIT_DONE()        \
  while (!(UCB0IFG & UCRXIFG)) \
    ;

#elif defined(__MSP430FR5994__)
/* ------------------------------------------------------------------------------------------------
 *                                      GDO0 Pin Configuration
 * ------------------------------------------------------------------------------------------------
 */
/*changed from 4.3 to 3.7 */
#define GDO0_BIT__ 3
#define GDO0_PORT__ 7
#define CONFIG_GDO0_PIN_AS_INPUT()                                             \
  st(INFIX(P, GDO0_PORT__, SEL0) &= ~BV(GDO0_BIT__);                           \
     INFIX(P, GDO0_PORT__, DIR) &=                                             \
     ~BV(GDO0_BIT__);) /* clear pin special function default */
#define GDO0_PIN_IS_HIGH() (INFIX(P, GDO0_PORT__, IN) & BV(GDO0_BIT__))

#define GDO0_INT_VECTOR INFIX(PORT, GDO0_PORT__, _VECTOR)
#define ENABLE_GDO0_INT()                                                      \
  st(INFIX(P, GDO0_PORT__, IE) |= BV(GDO0_BIT__);) /* atomic operation */
#define DISABLE_GDO0_INT()                                                     \
  st(INFIX(P, GDO0_PORT__, IE) &= ~BV(GDO0_BIT__);) /* atomic operation */
#define GDO0_INT_IS_ENABLED() (INFIX(P, GDO0_PORT__, IE) & BV(GDO0_BIT__))
#define CLEAR_GDO0_INT_FLAG()                                                  \
  st(INFIX(P, GDO0_PORT__, IFG) &= ~BV(GDO0_BIT__);) /* atomic operation */
#define GDO0_INT_FLAG_IS_SET() (INFIX(P, GDO0_PORT__, IFG) & BV(GDO0_BIT__))
#define CONFIG_GDO0_RISING_EDGE_INT()                                          \
  st(INFIX(P, GDO0_PORT__, IES) &= ~BV(GDO0_BIT__);) /* atomic operation */
#define CONFIG_GDO0_FALLING_EDGE_INT()                                         \
  st(INFIX(P, GDO0_PORT__, IES) |= BV(GDO0_BIT__);) /* atomic operation */

/* ------------------------------------------------------------------------------------------------
 *                                      GDO2 Pin Configuration
 * ------------------------------------------------------------------------------------------------
 */
/*changed from 3.6 to 5.3 */
#define GDO2_BIT__ 3
#define GDO2_PORT__ 5
#define CONFIG_GDO2_PIN_AS_INPUT()                                             \
  st(INFIX(P, GDO2_PORT__, SEL0) &= ~BV(GDO2_BIT__);                           \
     INFIX(P, GDO2_PORT__, DIR) &=                                             \
     ~BV(GDO2_BIT__);) /* clear pin special function default */
#define GDO2_PIN_IS_HIGH() (INFIX(P, GDO2_PORT__, IN) & BV(GDO2_BIT__))

#define GDO2_INT_VECTOR INFIX(PORT, GDO2_PORT__, _VECTOR)
#define ENABLE_GDO2_INT()                                                      \
  st(INFIX(P, GDO2_PORT__, IE) |= BV(GDO2_BIT__);) /* atomic operation */
#define DISABLE_GDO2_INT()                                                     \
  st(INFIX(P, GDO2_PORT__, IE) &= ~BV(GDO2_BIT__);) /* atomic operation */
#define GDO2_INT_IS_ENABLED() (INFIX(P, GDO2_PORT__, IE) & BV(GDO2_BIT__))
#define CLEAR_GDO2_INT_FLAG()                                                  \
  st(INFIX(P, GDO2_PORT__, IFG) &= ~BV(GDO2_BIT__);) /* atomic operation */
#define GDO2_INT_FLAG_IS_SET() (INFIX(P, GDO2_PORT__, IFG) & BV(GDO2_BIT__))
#define CONFIG_GDO2_RISING_EDGE_INT()                                          \
  st(INFIX(P, GDO2_PORT__, IES) &= ~BV(GDO2_BIT__);) /* atomic operation */
#define CONFIG_GDO2_FALLING_EDGE_INT()                                         \
  st(INFIX(P, GDO2_PORT__, IES) |= BV(GDO2_BIT__);) /* atomic operation */

/* ------------------------------------------------------------------------------------------------
 *                                      SPI Configuration
 * ------------------------------------------------------------------------------------------------
 */

/* Radio CSn Pin Configuration P3.4*/
/*changed to 8.3 */
#define SPI_CSN_GPIO_BIT__ 8
#define SPI_CONFIG_CSN_PIN_AS_OUTPUT() st(P3DIR |= BV(SPI_CSN_GPIO_BIT__);)
#define SPI_DRIVE_CSN_HIGH()                                                   \
  st(P3OUT |= BV(SPI_CSN_GPIO_BIT__);) /* atomic operation */
#define SPI_DRIVE_CSN_LOW()                                                    \
  st(P3OUT &= ~BV(SPI_CSN_GPIO_BIT__);) /* atomic operation */
#define SPI_CSN_IS_HIGH() (P3OUT & BV(SPI_CSN_GPIO_BIT__))

/* SCLK Pin Configuration */
#define SPI_SCLK_GPIO_BIT__ 2
#define SPI_CONFIG_SCLK_PIN_AS_OUTPUT() st(P5DIR |= BV(SPI_SCLK_GPIO_BIT__);)
#define SPI_DRIVE_SCLK_HIGH() st(P5OUT |= BV(SPI_SCLK_GPIO_BIT__);)
#define SPI_DRIVE_SCLK_LOW() st(P5OUT &= ~BV(SPI_SCLK_GPIO_BIT__);)

/* SI Pin Configuration */
#define SPI_SI_GPIO_BIT__ 0
#define SPI_CONFIG_SI_PIN_AS_OUTPUT() st(P5DIR |= BV(SPI_SI_GPIO_BIT__);)
#define SPI_DRIVE_SI_HIGH() st(P5OUT |= BV(SPI_SI_GPIO_BIT__);)
#define SPI_DRIVE_SI_LOW() st(P5OUT &= ~BV(SPI_SI_GPIO_BIT__);)

/* SO Pin Configuration */
#define SPI_SO_GPIO_BIT__ 1
#define SPI_CONFIG_SO_PIN_AS_INPUT() st(P5DIR &= ~BV(SPI_SO_GPIO_BIT__);)
#define SPI_SO_IS_HIGH() (P5IN & BV(SPI_SO_GPIO_BIT__))

/* SPI Port Configuration */
#define SPI_CONFIG_PORT()                                                      \
  st(P5SEL0 |= BV(SPI_SI_GPIO_BIT__) | BV(SPI_SO_GPIO_BIT__);                  \
     P5SEL0 |= BV(SPI_SCLK_GPIO_BIT__);                                        \
     P5SEL1 &= ~(BV(SPI_SI_GPIO_BIT__) | BV(SPI_SO_GPIO_BIT__));               \
     P5SEL1 &= ~BV(SPI_SCLK_GPIO_BIT__);)

#define SPI_INIT()                                                             \
  st(UCB1CTLW0 |= UCSWRST; UCB1CTLW0 |= UCSWRST | UCSSEL_2;                    \
     UCB1CTLW0 |= UCCKPH | UCMSB | UCMST | UCSYNC; UCB1BR0 = 0x08;             \
     UCB1BR1 = 0; SPI_CONFIG_PORT(); UCB1CTLW0 &= ~UCSWRST;)

/* read/write macros */
#define SPI_WRITE_BYTE(x) st(UCB1IFG &= ~UCRXIFG; UCB1TXBUF = x;)
#define SPI_READ_BYTE() UCB1RXBUF
#define SPI_WAIT_DONE()  while (!(UCB1IFG & UCRXIFG));

#else
#error "This MSP430 is not supported by the CC1101 radio library."
#endif
#endif