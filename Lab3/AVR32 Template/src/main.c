#include <asf.h>
#include <board.h>
#include <gpio.h>
#include <sysclk.h>
#include "busy_delay.h"

#define CONFIG_USART_IF (AVR32_USART2)

// defines for BRTT interface
#define TEST_A      AVR32_PIN_PA31
#define RESPONSE_A  AVR32_PIN_PA30
#define TEST_B      AVR32_PIN_PA29
#define RESPONSE_B  AVR32_PIN_PA28
#define TEST_C      AVR32_PIN_PA27
#define RESPONSE_C  AVR32_PIN_PB00

volatile int flag_A = 0;
volatile int flag_B = 0;
volatile int flag_C = 0;

__attribute__((__interrupt__)) static void interrupt_J3(void);

void init(){
    sysclk_init();
    board_init();
    busy_delay_init(BOARD_OSC0_HZ);
    
    cpu_irq_disable();
    INTC_init_interrupts();
    INTC_register_interrupt(&interrupt_J3, AVR32_GPIO_IRQ_3, AVR32_INTC_INT1);
    cpu_irq_enable();
    
    stdio_usb_init(&CONFIG_USART_IF);
	
	gpio_configure_pin (TEST_A, GPIO_DIR_INPUT | GPIO_INIT_HIGH);
    gpio_configure_pin (RESPONSE_A, GPIO_DIR_OUTPUT | GPIO_INIT_HIGH);
	gpio_configure_pin (TEST_B, GPIO_DIR_INPUT | GPIO_INIT_HIGH);
	gpio_configure_pin (RESPONSE_B, GPIO_DIR_OUTPUT | GPIO_INIT_HIGH);
	gpio_configure_pin (TEST_C, GPIO_DIR_INPUT | GPIO_INIT_HIGH);
	gpio_configure_pin (RESPONSE_C, GPIO_DIR_OUTPUT | GPIO_INIT_HIGH);

    #if defined(__GNUC__) && defined(__AVR32__)
        setbuf(stdout, NULL);
        setbuf(stdin,  NULL);
    #endif
}

__attribute__((__interrupt__)) static void interrupt_J3(void){
	if (gpio_get_pin_interrupt_flag(TEST_A)){
		flag_A = 1;
		gpio_clear_pin_interrupt_flag(TEST_A);
	}
	if (gpio_get_pin_interrupt_flag(TEST_B)){
		flag_B = 1;
		gpio_clear_pin_interrupt_flag(TEST_B);
	}
	if (gpio_get_pin_interrupt_flag(TEST_C)){
		flag_C = 1;
		gpio_clear_pin_interrupt_flag(TEST_C);
	}
}

void task_A(){
	while(1){
		if (!gpio_get_pin_value(TEST_A)){
			gpio_set_pin_low(RESPONSE_A);
			busy_delay_us(5);
			gpio_set_pin_high(RESPONSE_A);
		}
	}
}

void task_B(){
	while(1){
		if (!gpio_get_pin_value(TEST_A)){
			gpio_set_pin_low(RESPONSE_A);
			busy_delay_us(5);
			gpio_set_pin_high(RESPONSE_A);
		}
		if (!gpio_get_pin_value(TEST_B)){
			gpio_set_pin_low(RESPONSE_B);
			busy_delay_us(5);
			gpio_set_pin_high(RESPONSE_B);
		}
		if (!gpio_get_pin_value(TEST_C)){
			gpio_set_pin_low(RESPONSE_C);
			busy_delay_us(5);
			gpio_set_pin_high(RESPONSE_C);
		}
	}
}

void task_C(){
	gpio_enable_pin_interrupt(TEST_A,GPIO_FALLING_EDGE);
	gpio_enable_pin_interrupt(TEST_B,GPIO_FALLING_EDGE);
	gpio_enable_pin_interrupt(TEST_C,GPIO_FALLING_EDGE);
	while(1){
		
	}
}


void task_D(){
	gpio_enable_pin_interrupt(TEST_A,GPIO_FALLING_EDGE);
	gpio_enable_pin_interrupt(TEST_B,GPIO_FALLING_EDGE);
	gpio_enable_pin_interrupt(TEST_C,GPIO_FALLING_EDGE);
	while(1){
		if (flag_A){
			gpio_set_pin_low(RESPONSE_A);
			busy_delay_us(5);
			gpio_set_pin_high(RESPONSE_A);
			flag_A = 0;
		}
		if (flag_B){
			busy_delay_us(100);
			gpio_set_pin_low(RESPONSE_B);
			busy_delay_us(5);
			gpio_set_pin_high(RESPONSE_B);
			flag_B = 0;
		}
		if (flag_C){
			gpio_set_pin_low(RESPONSE_C);
			busy_delay_us(5);
			gpio_set_pin_high(RESPONSE_C);
			flag_C = 0;
		}
	}
}

int main (void){
    init();
	//task_A();
	//task_B();
	//task_C();
	task_D();
}
