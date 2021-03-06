/* This is one of two projects;
 * This project describes slaves behavior in SPI communication.
 * Operating sequence:
 * 1) Slaves configuring GPIO, DMA, SPI, ADC peripherals and sending confirmation (rising edge);
 * 2) Slaves receiving pwm signal from master, through TIM2 peripheral recognizing one of them as
 * selected, selected slave sends confirmation to master (falling edge); this GPIO pin
 * reconfiguring as EXTI input (rising edge);
 * 3) DMA is used for communication;
 * 4) Rising edge on EXTI pin is a signal that the message received and new SPI communication
 * starts;
 * 5) SPI interface disabled, reconfigured and new iteration starts. Slave transmit it's message
 * to the master;
 * 6) After transferring slave generates falling edge on GPIO pin;
 * 7) SPI interface disabled;
 *
 * GPIO,DMA,SPI,ADC,EXTI;
 * confirmation pin (same for slaves): PA1;
 * SPI pins: PA5 -- SPI1_SCK; PA6 -- SPI1_MISO; PA7 -- SPI1_MOSI;
 * ADC pin: PA0 TIM2_CH1_ETR;
 * DMA: SPI1_RX -- channel 2; SPI1_TX -- channel 3 (dma1);
 */

#include<stm32f10x_rcc.h>
#include<stm32f10x_gpio.h>

//#define sl1
#define sl2

#define RX_BUF_SZ 50

#ifdef sl1
	uint8_t tx_buf[]="Hi, master, it is sl1 responding \0";
	double pwm_lth=0.,pwm_hth=0.45;//if master's pwm duty cycle equals or below 0.45, sl1 selected;

#elif defined(sl2)
	uint8_t tx_buf[]="Hi, master, current response is the second one \0";
	float pwm_lth=0.55,pwm_hth=1.;//if master's pwm duty cycle equals or above 0.55, sl2 selected;
//(transition region in 0.1 made to prevent simultaneous triggering of both modules);
#endif

volatile uint8_t rx_buf[RX_BUF_SZ];

void init_gpios(){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO,ENABLE);
	GPIO_InitTypeDef gpio_struct;

	gpio_struct.GPIO_Mode=GPIO_Mode_Out_PP;
	gpio_struct.GPIO_Pin=GPIO_Pin_1;
	gpio_struct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&gpio_struct);
	GPIO_ResetBits(GPIOA,GPIO_Pin_1);

//spi configuration;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);
	gpio_struct.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	gpio_struct.GPIO_Pin=GPIO_Pin_5|GPIO_Pin_7;//PA5 (SPI1_SCK) and PA7 (SPI1_MOSI) are inputs;
	GPIO_Init(GPIOA,&gpio_struct);
	GPIO_ResetBits(GPIOA,GPIO_Pin_5);


//	EXTI->FTSR |= EXTI_FTSR_TR5;//falling PA0 (button released);
//	EXTI->RTSR|=EXTI_RTSR_TR5;//rising PA0 (button pushed);
//    EXTI->IMR |= EXTI_IMR_MR5;//mark interrupt as masked on line 0;
//    NVIC_EnableIRQ(EXTI9_5_IRQn);//enable interrupt on line 0;
//    __enable_irq();//enable interrupts in global scope;




	gpio_struct.GPIO_Mode=GPIO_Mode_AF_PP;
	gpio_struct.GPIO_Pin=GPIO_Pin_6;//PA6 (SPI1_MISO) -- output;
	GPIO_Init(GPIOA,&gpio_struct);

//pwm_ic configuration;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	gpio_struct.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	gpio_struct.GPIO_Pin=GPIO_Pin_0;
	GPIO_Init(GPIOA,&gpio_struct);
}



/*void EXTI9_5_IRQHandler(){
	if(GPIOA->IDR&GPIO_IDR_IDR5){//if button pressed;
		GPIO_ResetBits(GPIOA, GPIO_Pin_1);//turn on the diode;
	}
	else{
		GPIO_SetBits(GPIOA,GPIO_Pin_1);
	}
	EXTI->PR |= EXTI_PR_PR5;//clear pending bit for line 0;
}*/


/*void SPI1_IRQHandler(){
	uint16_t tmp_dt=SPI1->DR;
	if(DMA1_Channel2->CNDTR<5){
		tmp_dt+=0;
	}


	SPI1->SR&=~SPI_SR_RXNE;
}*/



uint8_t is_selected(){
	float m_d_cycle=(TIM2->CCR2+0.)/(TIM2->CCR1+0.);
	if((m_d_cycle>pwm_lth)&&(m_d_cycle<pwm_hth)){
		return 1;
	}
	else{
		return 0;
	}

/*
	ADC1->CR2|=ADC_CR2_SWSTART;//Starts conversion of regular channels (1st channel);
	while(!(ADC1->SR&ADC_SR_EOC));//waiting for conversion to be completed;
	uint32_t adc_res=ADC1->DR;//read resulted value; this clearing ADC_SR_EOC bit in ADC1->SR;
//adc peripheral registers have to be accessed by words (32-bit);
	if((adc_res>adc_lth)&&(adc_res<adc_hth))
		return 1;
	else
		return 0;*/
}

/*void init_adc(){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	ADC1->SQR3=0;//selected channel 0;
	ADC1->SMPR2|=ADC_SMPR2_SMP0;//set maximum sample time for ADC1_CH0; (for accuracy);
	ADC1->CR2|=ADC_CR2_EXTSEL|ADC_CR2_EXTTRIG;//select SWSTART as external event used to
//trigger the start of conversion of a regular group and enable this external trigger;
	ADC1->CR2|=ADC_CR2_CAL;//enable ADC1 calibration;
	while(!(ADC1->CR2&ADC_CR2_CAL)){//ADC_CR2_CAL should be reset by hardware after
//calibration is complete, but is seems that this works some another way because
//while((ADC1->CR2&ADC_CR2_CAL)) causes looping;
		//tmp_chk=ADC1->CR2&ADC_CR2_CAL;
		//tmp_chk1=!(ADC1->CR2&ADC_CR2_CAL);
	}
	ADC1->CR2|=ADC_CR2_ADON;//Enable ADC to start conversion;
}*/

void init_pwm_ic(){
	TIM2->CCMR1|=TIM_CCMR1_CC1S_0;//CC1 channel is configured as input, IC1 is mapped on TI1;
	TIM2->CCER&=~TIM_CCER_CC1P;//capture is done on a rising edge of IC1;
	TIM2->CCMR1|=TIM_CCMR1_CC2S_1;//CC2 channel is configured as input, IC2 is mapped on TI1;
	TIM2->CCER|=TIM_CCER_CC2P;//capture is done on a falling edge of IC1;
	TIM2->SMCR|=TIM_SMCR_TS_2|TIM_SMCR_TS_0|TIM_SMCR_SMS_2;//Filtered Timer Input 1 (TI1FP1)
//selected as trigger input; reset mode selected;
	TIM2->CCER|=TIM_CCER_CC1E|TIM_CCER_CC2E;//IC1 and IC2 enabled;
	TIM2->ARR=0xFFFF;//ARR value should be grater than master's Tp;
	TIM2->CR1|=TIM_CR1_CEN;
}

void init_dma(){//SPI1_RX -- channel 2; SPI1_TX -- channel 3 (dma1);
	DMA1_Channel2->CPAR=(uint32_t)(&(SPI1->DR));
	DMA1_Channel2->CMAR=(uint32_t)(rx_buf);
	DMA1_Channel2->CNDTR=RX_BUF_SZ/2-1;//SPI data transmitted in half-word mode;
	DMA1_Channel2->CCR|=DMA_CCR2_PL_0;//DMA channel transfer priority at medium level;
	DMA1_Channel2->CCR|=DMA_CCR2_MSIZE_0|DMA_CCR2_PSIZE_0|DMA_CCR2_MINC;//DMA configured as folows:
//memory size is half-word; peripheral size is half-word; memory increment mode enabled;
//DMA_CCR2_DIR is 0x0 by default (data read from peripheral);
	DMA1_Channel2->CCR|=DMA_CCR2_EN;//enable DMA1_Channel2;

	DMA1_Channel3->CPAR=(uint32_t)(&(SPI1->DR));
	DMA1_Channel3->CMAR=(uint32_t)(tx_buf);
	DMA1_Channel3->CNDTR=sizeof(tx_buf)/2;//SPI data transmitted in half-word mode;
	DMA1_Channel3->CCR|=DMA_CCR3_PL_0;//DMA channel transfer priority at medium level;
	DMA1_Channel3->CCR|=DMA_CCR3_MSIZE_0|DMA_CCR3_PSIZE_0|DMA_CCR3_MINC|DMA_CCR3_DIR;//DMA
//configured as follows: memory size is half-word; peripheral size is half-word; memory increment
//mode enabled; data read from memory;
	DMA1_Channel3->CCR|=DMA_CCR3_EN;//enable DMA1_Channel3;
}

void init_spi(){//PA5 -- SPI1_SCK; PA6 -- SPI1_MISO; PA7 -- SPI1_MOSI;
	SPI1->CR1|=SPI_CR1_DFF|SPI_CR1_SSM;/*|SPI_CR1_CRCEN*/;//16-bit data frame format is selected;
//NSS software mode selected; CRC calculation selected (but it may not match SPI1->RXCRCR
//when in full duplex mode different data transmitted and received);
	SPI1->CR1&=~(SPI_CR1_CPOL|SPI_CR1_CPHA|SPI_CR1_LSBFIRST|SPI_CR1_SSI|SPI_CR1_MSTR);//CK to 0
//when idle, first clock transition is the first data capture edge, MSB-first, SSI bit
	//and MSTR bit are cleared
	SPI1->CR2|=SPI_CR2_RXDMAEN|SPI_CR2_TXDMAEN;//Rx buffer DMA enabled;

	GPIO_SetBits(GPIOA,GPIO_Pin_1);//send 'spi_ready' confirmation to master;


/*	SPI1->CR2|=SPI_CR2_RXNEIE;
	NVIC_EnableIRQ(SPI1_IRQn);
	__enable_irq();*/




}

void restrt(){
	if(is_selected()){
		SPI1->CR1|=SPI_CR1_SPE;
		GPIO_ResetBits(GPIOA,GPIO_Pin_1);//send 'selected' confirmation to master;
		while(!(DMA1->ISR&DMA_ISR_TCIF2)||
				!(DMA1->ISR&DMA_ISR_TCIF3)||
				(SPI1->SR&SPI_SR_BSY));//data transfer is completed;
//SPI1_RX -- channel 2; SPI1_TX -- channel 3 (dma1);
		GPIO_InitTypeDef gpio_struct;
		GPIO_SetBits(GPIOA,GPIO_Pin_1);//send 'spi data transfered' confirmation to master;

		gpio_struct.GPIO_Mode=GPIO_Mode_IPD;
		gpio_struct.GPIO_Pin=GPIO_Pin_1;
		GPIO_Init(GPIOA,&gpio_struct);//release line;

		DMA1->IFCR|=DMA_IFCR_CTCIF2|DMA_IFCR_CTCIF3;//clearing the corresponding TCIF flags
//in the DMA_ISR register;
		DMA1_Channel2->CCR&=~DMA_CCR2_EN;
		DMA1_Channel3->CCR&=~DMA_CCR3_EN;
		SPI1->CR2&=~(SPI_CR2_RXDMAEN|SPI_CR2_TXDMAEN);
		SPI1->CR1&=~SPI_CR1_SPE;
		while(is_selected()!=0);
		gpio_struct.GPIO_Mode=GPIO_Mode_Out_PP;
		gpio_struct.GPIO_Pin=GPIO_Pin_1;
		gpio_struct.GPIO_Speed=GPIO_Speed_50MHz;
		GPIO_Init(GPIOA,&gpio_struct);
		while(is_selected()!=1);
	}
}

int main(void){
	init_gpios();
//	init_adc();
	init_pwm_ic();

	//uint16_t tmp_dt=0, tmp_cnt=0;

    while(1){
    	init_dma();
    	init_spi();
    	restrt();

/*    	if(SPI1->SR&SPI_SR_RXNE){
    		tmp_dt=SPI1->DR;
    		tmp_cnt++;*/


    }
}
