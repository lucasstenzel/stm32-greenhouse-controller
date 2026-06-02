/*
 * stm32f446xx.h
 * Based on stm32f446xx.s provided by professor Mark Krueger
 */

//include guards
#ifndef STM_H_
#define STM_H_

// System Control Block
#define SCB_BASE (volatile uint32_t*) 0xE000ED00
// System Handler Control and State Register
#define SHCSR (volatile uint32_t*) 0x24
#define SHCSR_MONITORACT (volatile uint32_t*) 1 << 8
// Debug Halting Control and Status Register
#define DHCSR (volatile uint32_t*) 0xE000EDF0

#define PERIPH_BASE (volatile uint32_t*) 0x40000000

#define GPIO_BASE (volatile uint32_t*) PERIPH_BASE + 0x20000
#define GPIO_PORT_SPACING (volatile uint32_t*) 0x400
#define GPIO_PINS_PER_PORT (volatile uint32_t*) 16
#define GPIO_PORT_SHIFT (volatile uint32_t*) 4
#define GPIO_MODER (volatile uint32_t*) 0x00
#define GPIO_OTYPER (volatile uint32_t*) 0x04
#define GPIO_OSPEEDR (volatile uint32_t*) 0x08
#define GPIO_PUPDR (volatile uint32_t*) 0x0C
#define GPIO_IDR (volatile uint32_t*) 0x10
#define GPIO_ODR (volatile uint32_t*) 0x14
#define GPIO_BSRR (volatile uint32_t*) 0x18
#define GPIO_LCKR (volatile uint32_t*) 0x1C
#define GPIO_AFRL (volatile uint32_t*) 0x20
#define GPIO_AFRH (volatile uint32_t*) 0x24

// * Note: Most all of the values below are offsets, unless specified with "_BASE"
#define GPIO_A (volatile uint32_t*) 0 * GPIO_PINS_PER_PORT
#define GPIO_B (volatile uint32_t*) 1 * GPIO_PINS_PER_PORT
#define GPIO_C (volatile uint32_t*) 2 * GPIO_PINS_PER_PORT
#define GPIO_D (volatile uint32_t*) 3 * GPIO_PINS_PER_PORT
#define GPIO_E (volatile uint32_t*) 4 * GPIO_PINS_PER_PORT
#define GPIO_F (volatile uint32_t*) 5 * GPIO_PINS_PER_PORT
#define GPIO_G (volatile uint32_t*) 6 * GPIO_PINS_PER_PORT
#define GPIO_H (volatile uint32_t*) 7 * GPIO_PINS_PER_PORT

/** DO NOT DEFINE HERE, DEFINE IN PROJECT */
#define HSE_VALUE (volatile uint32_t*) 8000000

#define RCC_BASE (volatile uint32_t*) PERIPH_BASE + 0x23800

#define RCC_AHB1ENR (volatile uint32_t*) RCC_BASE + 0x30

#define RCC_APB1ENR (volatile uint32_t*) RCC_BASE + 0x40
#define RCC_APB1ENR_TIM2EN (volatile uint32_t*) (1 << 0)
#define RCC_APB1ENR_TIM3EN (volatile uint32_t*) (1 << 1)
#define RCC_APB1ENR_USART2EN (volatile uint32_t*) (1 << 17)
#define RCC_APB1ENR_PWREN (volatile uint32_t*) (1 << 28)

#define RCC_APB2ENR (volatile uint32_t*) RCC_BASE + 0x44
#define RCC_APB2ENR_ADC1EN (volatile uint32_t*) (1 << 8)
#define RCC_APB2ENR_ADC2EN (volatile uint32_t*) (1 << 9)
#define RCC_APB2ENR_ADC3EN (volatile uint32_t*) (1 << 10)

#define RCC_CR (volatile uint32_t*) RCC_BASE + 0x00

#define RCC_CR_PLLRDY_SHIFT (volatile uint32_t*) 25
#define RCC_CR_PLLON_SHIFT (volatile uint32_t*) 24
#define RCC_CR_HSEBYP_SHIFT (volatile uint32_t*) 18
#define RCC_CR_HSERDY_SHIFT (volatile uint32_t*) 17
#define RCC_CR_HSEON_SHIFT (volatile uint32_t*) 16

#define RCC_PLLCFGR (volatile uint32_t*) RCC_BASE + 0x04

#define RCC_PLLCFGR_PLL_SRC_HSI (volatile uint32_t*) 0 << 22
#define RCC_PLLCFGR_PLL_SRC_HSE (volatile uint32_t*) 1 << 22
#define RCC_PLLCFGR_PLLM_SHIFT (volatile uint32_t*) 0
#define RCC_PLLCFGR_PLLM_MASK (volatile uint32_t*) 0b111111 << RCC_PLLCFGR_PLLM_SHIFT
#define RCC_PLLCFGR_PLLN_SHIFT (volatile uint32_t*) 6
#define RCC_PLLCFGR_PLLN_MASK (volatile uint32_t*) 0b111111111 << RCC_PLLCFGR_PLLN_SHIFT
#define RCC_PLLCFGR_PLLP_SHIFT (volatile uint32_t*) 16
#define RCC_PLLCFGR_PLLP_MASK (volatile uint32_t*) 0b11 << RCC_PLLCFGR_PLLN_SHIFT
#define RCC_PLLCFGR_PLLP_DIV2 (volatile uint32_t*) 0b00 << RCC_PLLCFGR_PLLP_SHIFT
#define RCC_PLLCFGR_PLLP_DIV4 (volatile uint32_t*) 0b01 << RCC_PLLCFGR_PLLP_SHIFT
#define RCC_PLLCFGR_PLLP_DIV6 (volatile uint32_t*) 0b10 << RCC_PLLCFGR_PLLP_SHIFT
#define RCC_PLLCFGR_PLLP_DIV8 (volatile uint32_t*) 0b11 << RCC_PLLCFGR_PLLP_SHIFT

#define RCC_CFGR (volatile uint32_t*) RCC_BASE + 0x08

#define RCC_CFGR_SW_MASK (volatile uint32_t*) 0b11 << 0
#define RCC_CFGR_SW_PLL_HSI (volatile uint32_t*) 0b00 << 0
#define RCC_CFGR_SW_PLL_HSE (volatile uint32_t*) 0b01 << 0
#define RCC_CFGR_SW_PLL_P (volatile uint32_t*) 0b10 << 0
#define RCC_CFGR_SW_PLL_R (volatile uint32_t*) 0b11 << 0

#define RCC_CFGR_SWS_MASK (volatile uint32_t*) 0b11 << 2
#define RCC_CFGR_SWS_PLL_HSI (volatile uint32_t*) 0b00 << 2
#define RCC_CFGR_SWS_PLL_HSE (volatile uint32_t*) 0b01 << 2
#define RCC_CFGR_SWS_PLL_P (volatile uint32_t*) 0b10 << 2
#define RCC_CFGR_SWS_PLL_R (volatile uint32_t*) 0b11 << 2

#define RCC_CFGR_PPRE1_MASK (volatile uint32_t*) 0b111 << 10
#define RCC_CFGR_PPRE1_DIV1 (volatile uint32_t*) 0b000 << 10
#define RCC_CFGR_PPRE1_DIV2 (volatile uint32_t*) 0b100 << 10
#define RCC_CFGR_PPRE1_DIV4 (volatile uint32_t*) 0b101 << 10
#define RCC_CFGR_PPRE1_DIV8 (volatile uint32_t*) 0b110 << 10
#define RCC_CFGR_PPRE1_DIV16 (volatile uint32_t*) 0b111 << 10

#define RCC_CFGR_PPRE2_MASK (volatile uint32_t*) 0b111 << 13
#define RCC_CFGR_PPRE2_DIV1 (volatile uint32_t*) 0b000 << 13
#define RCC_CFGR_PPRE2_DIV2 (volatile uint32_t*) 0b100 << 13
#define RCC_CFGR_PPRE2_DIV4 (volatile uint32_t*) 0b101 << 13
#define RCC_CFGR_PPRE2_DIV8 (volatile uint32_t*) 0b110 << 13
#define RCC_CFGR_PPRE2_DIV16 (volatile uint32_t*) 0b111 << 13

#define RCC_CFGR_HPRE_MASK (volatile uint32_t*) 0b1111 << 4
#define RCC_CFGR_HPRE_DIV1 (volatile uint32_t*) 0b0000 << 4
#define RCC_CFGR_HPRE_DIV2 (volatile uint32_t*) 0b1000 << 4
#define RCC_CFGR_HPRE_DIV4 (volatile uint32_t*) 0b1001 << 4
#define RCC_CFGR_HPRE_DIV8 (volatile uint32_t*) 0b1010 << 4
#define RCC_CFGR_HPRE_DIV16 (volatile uint32_t*) 0b1011 << 4
#define RCC_CFGR_HPRE_DIV64 (volatile uint32_t*) 0b1100 << 4
#define RCC_CFGR_HPRE_DIV128 (volatile uint32_t*) 0b1101 << 4
#define RCC_CFGR_HPRE_DIV256 (volatile uint32_t*) 0b1110 << 4
#define RCC_CFGR_HPRE_DIV512 (volatile uint32_t*) 0b1111 << 4

#define PWR_BASE (volatile uint32_t*) PERIPH_BASE + 0x07000
#define PWR_CR (volatile uint32_t*) PWR_BASE + 0x00
#define PWR_CR_VOS_SHIFT (volatile uint32_t*) 14
#define PWR_CR_VOS_MASK (volatile uint32_t*) (0b11 << PWR_CR_VOS_SHIFT)

#define NVIC_BASE (volatile uint32_t*) 0xE000E100
#define NVIC_ISER0 (volatile uint32_t*) 0x000
#define NVIC_ISER1 (volatile uint32_t*) 0x004
#define NVIC_ISER2 (volatile uint32_t*) 0x008
#define NVIC_ICER0 (volatile uint32_t*) 0x080
#define NVIC_ICER1 (volatile uint32_t*) 0x084
#define NVIC_ICER2 (volatile uint32_t*) 0x088
#define NVIC_ICPR0 (volatile uint32_t*) 0x180
#define NVIC_ICPR1 (volatile uint32_t*) 0x184
#define NVIC_ICPR2 (volatile uint32_t*) 0x188

#define FLASH_BASE (volatile uint32_t*) PERIPH_BASE + 0x23C00
#define FLASH_ACR (volatile uint32_t*) FLASH_BASE + 0x00
#define FLASH_ACR_LATENCY (volatile uint32_t*) 0
#define FLASH_ACR_LATENCY_MASK (volatile uint32_t*) 0xF << FLASH_ACR_LATENCY

#define TIM2_BASE (volatile uint32_t*) PERIPH_BASE + 0x00000
#define TIM3_BASE (volatile uint32_t*) PERIPH_BASE + 0x00400
#define TIM4_BASE (volatile uint32_t*) PERIPH_BASE + 0x00800
#define TIM5_BASE (volatile uint32_t*) PERIPH_BASE + 0x00C00
#define TIM6_BASE (volatile uint32_t*) PERIPH_BASE + 0x01000
#define TIM7_BASE (volatile uint32_t*) PERIPH_BASE + 0x01400

#define TIM_CR1 (volatile uint32_t*) 0x00
#define TIM_CR1_CEN (volatile uint32_t*) 1 << 0
#define TIM_CR1_UDIS (volatile uint32_t*) 1 << 1
#define TIM_CR1_URS (volatile uint32_t*) 1 << 2
#define TIM_CR1_OPM (volatile uint32_t*) 1 << 3
#define TIM_CR1_DIR (volatile uint32_t*) 1 << 4

#define TIM_SR (volatile uint32_t*) 0x10
#define TIM_SR_UIF (volatile uint32_t*) 1 << 0

#define TIM_EGR (volatile uint32_t*) 0x14
#define TIM_EGR_UG (volatile uint32_t*) 1 << 0

#define TIM_CCMR1 (volatile uint32_t*) 0x18
#define TIM_CCMR1_OC2M_BIT (volatile uint32_t*) 12
#define TIM_CCMR1_OC2M_MASK (volatile uint32_t*) 0b111 << TIM_CCMR1_OC2M_BIT
#define TIM_CCMR1_CC2S_BIT (volatile uint32_t*) 8
#define TIM_CCMR1_CC2S_MASK (volatile uint32_t*) 0b11 << TIM_CCMR1_CC2S_BIT
#define TIM_CCMR1_OC1M_BIT (volatile uint32_t*) 4
#define TIM_CCMR1_OC1M_MASK (volatile uint32_t*) 0b111 << TIM_CCMR1_OC1M_BIT
#define TIM_CCMR1_CC1S_BIT (volatile uint32_t*) 0
#define TIM_CCMR1_CC1S_MASK (volatile uint32_t*) 0b11 << TIM_CCMR1_CC1S_BIT

#define TIM_CCER (volatile uint32_t*) 0x20
#define TIM_CCER_CC1E (volatile uint32_t*) 1 << 0

#define TIM_CNT (volatile uint32_t*) 0x24
#define TIM_PSC (volatile uint32_t*) 0x28
#define TIM_ARR (volatile uint32_t*) 0x2C
#define TIM_CCR1 (volatile uint32_t*) 0x34

#define ADC1_BASE (volatile uint32_t*) PERIPH_BASE + 0x12000

#define ADC_SR (volatile uint32_t*) 0x00
#define ADC_EOC (volatile uint32_t*) 1 << 1

#define ADC_CR2 (volatile uint32_t*) 0x08
#define ADC_CR2_ADON (volatile uint32_t*) 1 << 0
#define ADC_CR2_CONT (volatile uint32_t*) 1 << 1
#define ADC_CR2_SWSTART (volatile uint32_t*) 1 << 30
#define ADC_CR2_JSWSTART (volatile uint32_t*) 1 << 22

#define ADC_DR (volatile uint32_t*) 0x4C

#define ADC_SQR1 (volatile uint32_t*) 0x2C
#define ADC_SQR1_L_BIT (volatile uint32_t*) 20
#define ADC_SQR1_L_MASK (volatile uint32_t*) 0b1111 << ADC_SQR1_L_BIT

#define ADC_SQR2 (volatile uint32_t*) 0x30

#define ADC_SQR3 (volatile uint32_t*) 0x34
#define ADC_SQR3_SQ1_BIT (volatile uint32_t*) 0
#define ADC_SQR3_SQ1_MASK (volatile uint32_t*) 0b11111 << ADC_SQR3_SQ1_BIT

// // Could not get this to work in an equate
// .macro GPIO_SPEC port, pin
//     \port * 16 + \pin
// .endm

#define SEMIHOSTING_EnterSVC (volatile uint32_t*) 0x17
#define SEMIHOSTING_ReportException (volatile uint32_t*) 0x18
#define SEMIHOSTING_SYS_CLOSE (volatile uint32_t*) 0x02
#define SEMIHOSTING_SYS_CLOCK (volatile uint32_t*) 0x10
#define SEMIHOSTING_SYS_ELAPSED (volatile uint32_t*) 0x30
#define SEMIHOSTING_SYS_ERRNO (volatile uint32_t*) 0x13
#define SEMIHOSTING_SYS_FLEN (volatile uint32_t*) 0x0C
#define SEMIHOSTING_SYS_GET_CMDLINE (volatile uint32_t*) 0x15
#define SEMIHOSTING_SYS_HEAPINFO (volatile uint32_t*) 0x16
#define SEMIHOSTING_SYS_ISERROR (volatile uint32_t*) 0x08
#define SEMIHOSTING_SYS_ISTTY (volatile uint32_t*) 0x09
#define SEMIHOSTING_SYS_OPEN (volatile uint32_t*) 0x01
#define SEMIHOSTING_SYS_READ (volatile uint32_t*) 0x06
#define SEMIHOSTING_SYS_READC (volatile uint32_t*) 0x07
#define SEMIHOSTING_SYS_REMOVE (volatile uint32_t*) 0x0E
#define SEMIHOSTING_SYS_RENAME (volatile uint32_t*) 0x0F
#define SEMIHOSTING_SYS_SEEK (volatile uint32_t*) 0x0A
#define SEMIHOSTING_SYS_SYSTEM (volatile uint32_t*) 0x12
#define SEMIHOSTING_SYS_TICKFREQ (volatile uint32_t*) 0x31
#define SEMIHOSTING_SYS_TIME (volatile uint32_t*) 0x11
#define SEMIHOSTING_SYS_TMPNAM (volatile uint32_t*) 0x0D
#define SEMIHOSTING_SYS_WRITE (volatile uint32_t*) 0x05
#define SEMIHOSTING_SYS_WRITEC (volatile uint32_t*) 0x03
#define SEMIHOSTING_SYS_WRITE0 (volatile uint32_t*) 0x04

#define UART_BASE (volatile uint32_t*) PERIPH_BASE + 0x4000
#define UART_SPACING (volatile uint32_t*) 0x400
#define USART2_BASE (volatile uint32_t*) UART_BASE + 1 * UART_SPACING
#define USART3_BASE (volatile uint32_t*) UART_BASE + 2 * UART_SPACING
#define UART4_BASE (volatile uint32_t*) UART_BASE + 3 * UART_SPACING
#define UART5_BASE (volatile uint32_t*) UART_BASE + 4 * UART_SPACING

#define USART_SR (volatile uint32_t*) 0x00
#define USART_DR (volatile uint32_t*) 0x04
#define USART_BRR (volatile uint32_t*) 0x08
#define USART_CR1 (volatile uint32_t*) 0x0C
#define USART_CR2 (volatile uint32_t*) 0x10
#define USART_CR3 (volatile uint32_t*) 0x14
#define USART_GTPR (volatile uint32_t*) 0x18

#define USART_SR_CTS_BIT (volatile uint32_t*) 9
#define USART_SR_LBD_BIT (volatile uint32_t*) 8
#define USART_SR_TXE_BIT (volatile uint32_t*) 7
#define USART_SR_TC_BIT (volatile uint32_t*) 6
#define USART_SR_RXNE_BIT (volatile uint32_t*) 5
#define USART_SR_IDLE_BIT (volatile uint32_t*) 4
#define USART_SR_ORE_BIT (volatile uint32_t*) 3
#define USART_SR_NF_BIT (volatile uint32_t*) 2
#define USART_SR_FE_BIT (volatile uint32_t*) 1
#define USART_SR_PE_BIT (volatile uint32_t*) 0 

#define USART_SR_CTS (volatile uint32_t*) 1 << USART_SR_CTS_BIT
#define USART_SR_LBD (volatile uint32_t*) 1 << USART_SR_LBD_BIT
#define USART_SR_TXE (volatile uint32_t*) 1 << USART_SR_TXE_BIT
#define USART_SR_TC (volatile uint32_t*) 1 << USART_SR_TC_BIT
#define USART_SR_RXNE (volatile uint32_t*) 1 << USART_SR_RXNE_BIT
#define USART_SR_IDLE (volatile uint32_t*) 1 << USART_SR_IDLE_BIT
#define USART_SR_ORE (volatile uint32_t*) 1 << USART_SR_ORE_BIT
#define USART_SR_NF (volatile uint32_t*) 1 << USART_SR_NF_BIT
#define USART_SR_FE (volatile uint32_t*) 1 << USART_SR_FE_BIT
#define USART_SR_PE (volatile uint32_t*) 1 << USART_SR_PE_BIT 

#define USART_CR1_OVER8 (volatile uint32_t*) 1 << 15
#define USART_CR1_UE (volatile uint32_t*) 1 << 13
#define USART_CR1_M (volatile uint32_t*) 1 << 12
#define USART_CR1_WAKE (volatile uint32_t*) 1 << 11
#define USART_CR1_PCE (volatile uint32_t*) 1 << 10
#define USART_CR1_PS (volatile uint32_t*) 1 << 9
#define USART_CR1_PEIE (volatile uint32_t*) 1 << 8
#define USART_CR1_TXEIE (volatile uint32_t*) 1 << 7
#define USART_CR1_TCIE (volatile uint32_t*) 1 << 6
#define USART_CR1_RXNEIE (volatile uint32_t*) 1 << 5
#define USART_CR1_IDLEIE (volatile uint32_t*) 1 << 4
#define USART_CR1_TE (volatile uint32_t*) 1 << 3
#define USART_CR1_RE (volatile uint32_t*) 1 << 2
#define USART_CR1_RWU (volatile uint32_t*) 1 << 1
#define USART_CR1_SBK (volatile uint32_t*) 1 << 0 

#define USART_CR2_LINEN (volatile uint32_t*) 1 << 14
#define USART_CR2_STOP (volatile uint32_t*) 0b11 << 12
#define USART_CR2_STOP_1 (volatile uint32_t*) 0b00 << 12
#define USART_CR2_STOP_0_5 (volatile uint32_t*) 0b01 << 12
#define USART_CR2_STOP_2 (volatile uint32_t*) 0b10 << 12
#define USART_CR2_STOP_1_5 (volatile uint32_t*) 0b11 << 12
#define USART_CR2_CLKEN (volatile uint32_t*) 1 << 11
#define USART_CR2_CPOL (volatile uint32_t*) 1 << 10
#define USART_CR2_CPHA (volatile uint32_t*) 1 << 9
#define USART_CR2_LBCL (volatile uint32_t*) 1 << 8
#define USART_CR2_LBDIE (volatile uint32_t*) 1 << 6
#define USART_CR2_LBDL (volatile uint32_t*) 1 << 5
#define USART_CR2_ADD (volatile uint32_t*) 0b1111 << 0 

#define USART_CR3_ONEBIT (volatile uint32_t*) 1 << 11
#define USART_CR3_CTSIE (volatile uint32_t*) 1 << 10
#define USART_CR3_CTSE (volatile uint32_t*) 1 << 9
#define USART_CR3_RTSE (volatile uint32_t*) 1 << 8
#define USART_CR3_DMAT (volatile uint32_t*) 1 << 7
#define USART_CR3_DMAR (volatile uint32_t*) 1 << 6
#define USART_CR3_SCEN (volatile uint32_t*) 1 << 5
#define USART_CR3_NACK (volatile uint32_t*) 1 << 4
#define USART_CR3_HDSEL (volatile uint32_t*) 1 << 3
#define USART_CR3_IRLP (volatile uint32_t*) 1 << 2
#define USART_CR3_IREN (volatile uint32_t*) 1 << 1
#define USART_CR3_EIE (volatile uint32_t*) 1 << 0 


// Assuming 16 MHz clock and OVER8 (volatile uint32_t*) 0,
// want 104.1875 * 16 (volatile uint32_t*) 1667 (0.1875 (volatile uint32_t*) 3/16)
// assuming internal 16 MHz HSI clock with no divide
// (16000000) / (16 * 9600) (volatile uint32_t*) 104.167 (volatile uint32_t*) 104 3/16
#define USART_BAUD_9600 (volatile uint32_t*) (104 << 4) | (3) // 16 MHz
//USART_BAUD_9600 (volatile uint32_t*) (78 << 4) | (2) // 12 Mhz
//USART_BAUD_9600 (volatile uint32_t*) (52 << 4) | (1) // 8 Mhz
//USART_BAUD_9600 (volatile uint32_t*) (26 << 4) | (0) // 168 MHz setup
// 168 Mhz / 4 PCLK (default setup)
// (168000000 / 4) / (16 * 9600) (volatile uint32_t*) 273.4375 or 273 7/16
//USART_BAUD_9600 (volatile uint32_t*) (273 << 4) | 7
#define USART_BAUD_1200 (volatile uint32_t*) (833 << 4) | (5) // 16 MHz
//USART_BAUD_300 (volatile uint32_t*) (3333 << 4) | (5) // 16 MHz <- did not work, why?
// (16000000) / (16 * 110) (volatile uint32_t*) 9090.9091 (volatile uint32_t*) 9090 15/16
// USART_BAUD_110 (volatile uint32_t*) (9090 << 4) | (15) // 16 MHz - value does not fit in 16#define bits#define 


#define LCD_DATA_GPIO_PORT (volatile uint32_t*) 0 // GPIO A
#define LCD_DATA_GPIO_BASE (volatile uint32_t*) 8
#define LCD_RS_GPIO (volatile uint32_t*) GPIO_C + 8
#define LCD_RW_GPIO (volatile uint32_t*) GPIO_C + 9
#define LCD_E_GPIO (volatile uint32_t*) GPIO_C + 10

#define LCD_RW_WRITE (volatile uint32_t*) 0
#define LCD_RW_READ (volatile uint32_t*) 1
#define LCD_RS_INSTR (volatile uint32_t*) 0
#define LCD_RS_DATA (volatile uint32_t*) 1
#define LCD_E_START (volatile uint32_t*) 1
#define LCD_E_IDLE (volatile uint32_t*) 0
#define LCD_BUSY (volatile uint32_t*) 0x80 

#define LCD_INSTR_CLEAR_DISPLAY (volatile uint32_t*) 0x01
#define LCD_INSTR_RETURN_HOME (volatile uint32_t*) 0x02
#define LCD_INSTR_ENTRY_MODE_SET (volatile uint32_t*) 0x04
#define LCD_INSTR_DISPLAY_ON_OFF (volatile uint32_t*) 0x08
#define LCD_INSTR_CURSOR_OR_DISPLAY_SHIFT (volatile uint32_t*) 0x10
#define LCD_INSTR_FUNCTION_SET (volatile uint32_t*) 0x20
#define LCD_INSTR_SET_CGRAM_ADDRESS (volatile uint32_t*) 0x40
#define LCD_INSTR_SET_DDRAM_ADDRESS (volatile uint32_t*) 0x80

#define LCD_DISPLAY_BLINK_ON (volatile uint32_t*) 0x01
#define LCD_DISPLAY_BLINK_OFF (volatile uint32_t*) 0x00
#define LCD_DISPLAY_CURSOR_ON (volatile uint32_t*) 0x02
#define LCD_DISPLAY_CURSOR_OFF (volatile uint32_t*) 0x00
#define LCD_DISPLAY_DISPLAY_ON (volatile uint32_t*) 0x04
#define LCD_DISPLAY_DISPLAY_OFF (volatile uint32_t*) 0x00 

#define LCD_FUNCTION_DATALENGTH_4_BIT (volatile uint32_t*) 0x00
#define LCD_FUNCTION_DATALENGTH_8_BIT (volatile uint32_t*) 0x10
#define LCD_FUNCTION_DISPLAY_LINES_1 (volatile uint32_t*) 0x00
#define LCD_FUNCTION_DISPLAY_LINES_2 (volatile uint32_t*) 0x08
#define LCD_FUNCTION_FONT_5X8 (volatile uint32_t*) 0x00
#define LCD_FUNCTION_FONT_5X11 (volatile uint32_t*) 0x04 

// *** Nucleo-64 defines ***

#define BUTTON_GPIO_PORT (volatile uint32_t*) 2 // Port C
#define BUTTON_GPIO_PIN (volatile uint32_t*) 13
#define BUTTON_GPIO_PORT_BASE (volatile uint32_t*) (GPIO_BASE + BUTTON_GPIO_PORT * GPIO_PORT_SPACING)
#define BUTTON_GPIO (volatile uint32_t*) (GPIO_C + BUTTON_GPIO_PIN) 

#define LED_NUCLEO_GPIO_PORT (volatile uint32_t*) 0 // Port A
#define LED_NUCLEO_GPIO_PIN (volatile uint32_t*) 5
#define LED_NUCLEO_GPIO_PORT_BASE (volatile uint32_t*) (GPIO_BASE + LED_NUCLEO_GPIO_PORT * GPIO_PORT_SPACING)
#define LED_NUCLEO_GPIO (volatile uint32_t*) (GPIO_A + LED_NUCLEO_GPIO_PIN) 

// USART2 PIN's, connected to SLINK for virtual COM port
#define USART2_TX (volatile uint32_t*) GPIO_A + 2
#define USART2_RX (volatile uint32_t*) GPIO_A + 3 

// *** MSOE Computer Engineering Development Board defines ***#define 

// Six left most LED's
#define LED_GPIO_PORT (volatile uint32_t*) 1 // Port B
#define LED_BASE_GPIO_PIN (volatile uint32_t*) 5
#define LED_GPIO_PORT_BASE (volatile uint32_t*) (GPIO_BASE + LED_GPIO_PORT * GPIO_PORT_SPACING)
#define LED_BASE_GPIO (volatile uint32_t*) (GPIO_B + LED_BASE_GPIO_PIN) 

// All LED's
#define LED0_GPIO (volatile uint32_t*) GPIO_B + 5
#define LED1_GPIO (volatile uint32_t*) GPIO_B + 6
#define LED2_GPIO (volatile uint32_t*) GPIO_B + 7
#define LED3_GPIO (volatile uint32_t*) GPIO_B + 8
#define LED4_GPIO (volatile uint32_t*) GPIO_B + 9
#define LED5_GPIO (volatile uint32_t*) GPIO_B + 10
#define LED6_GPIO (volatile uint32_t*) GPIO_B + 12
#define LED7_GPIO (volatile uint32_t*) GPIO_B + 13
#define LED8_GPIO (volatile uint32_t*) GPIO_B + 14
#define LED9_GPIO (volatile uint32_t*) GPIO_B + 15

#define LED_COUNT (volatile uint32_t*) 10 

// Specify the GPIO's that each pin of the keypad is connected to
#define KEYPAD_PIN_1 (volatile uint32_t*) GPIO_C + 0
#define KEYPAD_PIN_2 (volatile uint32_t*) GPIO_C + 1
#define KEYPAD_PIN_3 (volatile uint32_t*) GPIO_C + 2
#define KEYPAD_PIN_4 (volatile uint32_t*) GPIO_C + 3
#define KEYPAD_PIN_5 (volatile uint32_t*) GPIO_C + 4
#define KEYPAD_PIN_6 (volatile uint32_t*) GPIO_C + 5
#define KEYPAD_PIN_7 (volatile uint32_t*) GPIO_C + 6
#define KEYPAD_PIN_8 (volatile uint32_t*) GPIO_C + 7

// Note that rows and columns are numbered starting at 1
// but in code we refer to row and column INDEXES which start at 0

#define KEYPAD_ROW1 (volatile uint32_t*) KEYPAD_PIN_5
#define KEYPAD_ROW2 (volatile uint32_t*) KEYPAD_PIN_6
#define KEYPAD_ROW3 (volatile uint32_t*) KEYPAD_PIN_7
#define KEYPAD_ROW4 (volatile uint32_t*) KEYPAD_PIN_8
#define KEYPAD_COL1 (volatile uint32_t*) KEYPAD_PIN_1
#define KEYPAD_COL2 (volatile uint32_t*) KEYPAD_PIN_2
#define KEYPAD_COL3 (volatile uint32_t*) KEYPAD_PIN_3
#define KEYPAD_COL4 (volatile uint32_t*) KEYPAD_PIN_4 

#define ROW_COUNT (volatile uint32_t*) 4
#define COL_COUNT (volatile uint32_t*) 4 

#define TOTAL_KEYS (volatile uint32_t*) 16 

#define PIEZO_GPIO (volatile uint32_t*) GPIO_B + 4 

#define TEMP_GPIO (volatile uint32_t*) GPIO_B + 0
#define TEMPERATURE_SENSOR_ADC_CHANNEL (volatile uint32_t*) 8 

#define IR_GPIO (volatile uint32_t*) GPIO_B + 2 

#define ROTARY_PIN_1 (volatile uint32_t*) GPIO_A + 0
#define ROTARY_PIN_2 (volatile uint32_t*) GPIO_A + 1
#define ROTARY_PIN_SW (volatile uint32_t*) GPIO_A + 12

#endif /* STM_H_ */
