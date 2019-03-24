#include "blink.h"
#include <stdint.h>
#include "reg.h"

void GPIO_init() ;

int main(void)
{
	GPIO_init();
	while (! ( READ_BIT( GPIO_BASE(GPIO_PORTA) + GPIOx_IDR_OFFSET , IDRy_0_BIT(GPIO_PORTA) ) ) ) ;
	blink(LED_BLUE);
}

void GPIO_init() {
	// before using gpio need set rcc
	SET_BIT(RCC_BASE + RCC_AHB1ENR_OFFSET, GPIO_EN_BIT(GPIO_PORTA));

	// MODER = 00 => floating
	CLEAR_BIT(GPIO_BASE(GPIO_PORTA) + GPIOx_MODER_OFFSET, MODERy_1_BIT(GPIO_PORTA));
	CLEAR_BIT(GPIO_BASE(GPIO_PORTA) + GPIOx_MODER_OFFSET, MODERy_0_BIT(GPIO_PORTA));

	// PUPDR = 00 => No pull-up, pull-down
	CLEAR_BIT(GPIO_BASE(GPIO_PORTA) + GPIOx_PUPDR_OFFSET, PUPDRy_1_BIT(GPIO_PORTA));
	CLEAR_BIT(GPIO_BASE(GPIO_PORTA) + GPIOx_PUPDR_OFFSET, PUPDRy_0_BIT(GPIO_PORTA));
}