/*
 * gpio.h
 *
 *  Created on: Dec 9, 2018
 *      Author: livingston
 */

#ifndef GPIO_H_
#define GPIO_H_

/**
 * How to use:
 *   In the C file you wish to use gpio port do the following:
 *   	0. include gpio.h header
 *
 *   		#include "gpio.h"
 *
 *   	1. Create a gpio pointer to the structure.
 *   					(usually as a file static pointer)
 *
 *   		EG: static volatile GPIO* GPIOC = 0x40020800;
 *
 *   	2. To use the control registers use the '->' operator
 *
 *   		EG:  GPIOC->MODER |= .......;
 *
 *   				or
 *
 *   		 	(*GPIOC).MODER (I like the above form).
 */

#include <inttypes.h>
#include <stdint.h>

#define RCC_AHB1ENR (volatile uint32_t*) 0x40023830
#define RCC_APB2ENR (volatile uint32_t*) 0x40023844
#define RCC_APB1ENR (volatile uint32_t*) 0x40023840
#define GPIOAEN 0		// GPIOA Enable is bit 0 in RCC_APB1LPENR
#define GPIOBEN 1
#define GPIOCEN 2
#define ADC1EN  8
#define TIM3_EN 1
#define TIM2_EN 0

#define INPUT 0
#define OUTPUT 1
#define ALTERNATE_FUNCTION 2
#define ANALOG 3

typedef struct {
	uint32_t MODER;
	uint32_t OTYPER;
	uint32_t OSPEEDER;
	uint32_t PUPDR;
	uint32_t IDR;
	uint32_t ODR;
	uint32_t BSRR;
	uint32_t LCKR;
	uint32_t AFRL;
	uint32_t AFRH;
} GPIO;

// Analog to Digital Converter
typedef struct {
	uint32_t SR;
	uint32_t CR1;	
	uint32_t CR2;
	uint32_t SMPR1;
	uint32_t SMPR2;
	uint32_t JOFR1;
	uint32_t JOFR2;
	uint32_t JOFR3;
	uint32_t JOFR4;
	uint32_t HTR;
	uint32_t LTR;
	uint32_t SQR1;
	uint32_t SQR2;
	uint32_t SQR3;
	uint32_t JSQR;
	uint32_t JDR1;
	uint32_t JDR2;
	uint32_t JDR3;
	uint32_t JDR4;
	uint32_t DR;
} ADC;



// TIM2 to TIM5
// 17.4.1  TIMx control register 1 (TIMx_CR1) . . . . . . . . . . . . . . . . . . . . . .559
// 17.4.2  TIMx control register 2 (TIMx_CR2) . . . . . . . . . . . . . . . . . . . . . .561
// 17.4.3  TIMx slave mode control register (TIMx_SMCR) . . . . . . . . . . . . . . . . .562
// 17.4.4  TIMx DMA/Interrupt enable register (TIMx_DIER) . . . . . . . . . . . . . . . .564
// 17.4.5  TIMx status register (TIMx_SR) . . . . . . . . . . . . . . . . . . . . . . . .565
// 17.4.6  TIMx event generation register (TIMx_EGR) . . . . . . . . . . . . . . . . . . 567
// 17.4.7  TIMx capture/compare mode register 1 (TIMx_CCMR1) . . . . . . . . . . . . . . 568
// 17.4.8  TIMx capture/compare mode register 2 (TIMx_CCMR2) . . . . . . . . . . . . . . 571
// 17.4.9  TIMx capture/compare enable register (TIMx_CCER) . . . . . . . . . . . . . . .572
// 17.4.10 TIMx counter (TIMx_CNT) . . . . . . . . . . . . . . . . . . . . . . . . . . . 574
// 17.4.11 TIMx prescaler (TIMx_PSC) . . . . . . . . . . . . . . . . . . . . . . . . . . 574
// 17.4.12 TIMx auto-reload register (TIMx_ARR) . . . . . . . . . . . . . . . . . . . . .574
// 17.4.13 TIMx capture/compare register 1 (TIMx_CCR1) . . . . . . . . . . . . . . . . . 575
// 17.4.14 TIMx capture/compare register 2 (TIMx_CCR2) . . . . . . . . . . . . . . . . . 575
// 17.4.15 TIMx capture/compare register 3 (TIMx_CCR3) . . . . . . . . . . . . . . . . . 576
// 17.4.16 TIMx capture/compare register 4 (TIMx_CCR4) . . . . . . . . . . . . . . . . . 576
// 17.4.17 TIMx DMA control register (TIMx_DCR) . . . . . . . . . . . . . . . . . . . . .577
// 17.4.18 TIMx DMA address for full transfer (TIMx_DMAR) . . . . . . . . . . . . . . . .577
// 17.4.19 TIM2 option register (TIM2_OR) . . . . . . . . . . . . . . . . . . . . . . . .578
// 17.4.20 TIM5 option register (TIM5_OR) . . . . . . . . . . . . . . . . . . . . . . . .579
typedef struct {
    uint32_t CR1;
    uint32_t CR2;
    uint32_t SMCR;
    uint32_t DIER; 
    uint32_t SR;
    uint32_t EGR;
    uint32_t CCMR1;
    uint32_t CCMR2;
    uint32_t CCER;
    uint32_t CNT;
    uint32_t PSC;
    uint32_t ARR;
	uint32_t RESERVED1; // reserved register
    uint32_t CCR1;
    uint32_t CCR2;
    uint32_t CCR3;
    uint32_t CCR4;
	uint32_t RESERVED2; // reserved register
    uint32_t DCR;
    uint32_t DMAR;
    uint32_t OR2;
    uint32_t OR5;
} TIMER;

// 25.6.8 USART register map
typedef struct {
	uint32_t SR;
	uint32_t DR;
	uint32_t BRR;
	uint32_t CR1;
	uint32_t CR2;
	uint32_t CR3;
	uint32_t GTPR;
} USART_T;

#endif /* GPIO_H_ */
