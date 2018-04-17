/* This is one of two projects;
 * This project describes slaves behavior in SPI communication.
 * Operating sequence:
 * 1) Slaves configuring GPIO, DMA, SPI, ADC peripherals and sending confirmation (rising edge);
 * 2) Slaves receiving pwm signal from master, through ADC peripheral recognizing one of them as
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
 * ADC pin: PA0 -- ADC12_IN0;
 * DMA: SPI1_RX -- channel 2; SPI1_TX -- channel 3 (dma1);
 */

#include<stm32f10x_rcc.h>
#include<stm32f10x_gpio.h>

#define sl1
//#define sl2

#define RX_BUF_SZ 50

#ifdef sl1
	uint8_t tx_buf[]="Hi, master, it is sl1 responding \0";
	uint32_t adc_lth=0, adc_hth=0x7FF;//as adc has 12-bit resolution, high_threshold is ((2^12)-1)/2);

#elif defined(sl2)
	uint8_t tx_buf[]="Hi, master, current response is the second one \0";
	uint32_t adc_lth=0x7FF, adc_hth=0xF;
#endif

uint8_t rx_buf[RX_BUF_SZ];

void init_gpios(){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO,ENABLE);
	GPIO_InitTypeDef gpio_struct;

	gpio_struct.GPIO_Mode=GPIO_Mode_Out_PP;
	gpio_struct.GPIO_Pin=GPIO_Pin_1;
	gpio_struct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&gpio_struct);

//spi configuration;
	gpio_struct.GPIO_Mode=GPIO_Mode_AIN;
	gpio_struct.GPIO_Pin=GPIO_Pin_5|GPIO_Pin_7;//PA5 (SPI1_SCK) and PA7 (SPI1_MOSI) are inputs;
	GPIO_Init(GPIOA,&gpio_struct);

	gpio_struct.GPIO_Mode=GPIO_Mode_AF_PP;
	gpio_struct.GPIO_Pin=GPIO_Pin_6;//PA6 (SPI1_MISO) -- output;
	GPIO_Init(GPIOA,&gpio_struct);

//ADC configuration;
	gpio_struct.GPIO_Mode=GPIO_Mode_AIN;
	gpio_struct.GPIO_Pin=GPIO_Pin_0;
	GPIO_Init(GPIOA,&gpio_struct);
}

bool is_selected(){
	ADC1->CR2|=ADC_CR2_SWSTART;//Starts conversion of regular channels (1st channel);
	while(!(ADC1->SR&ADC_SR_EOC));//waiting for conversion to be completed;
	uint32_t adc_res=ADC1->DR;//read resulted value; this clearing ADC_SR_EOC bit in ADC1->SR;
//adc peripheral registers have to be accessed by words (32-bit);
	if((adc_res>adc_lth)&&(adc_res<adc_hth))
		return 1;
	else
		return 0;
}

void init_adc(){
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
}

void init_dma(){//SPI1_RX -- channel 2; SPI1_TX -- channel 3 (dma1);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	DMA1_Channel2->CPAR=(uint32_t)(SPI1->DR);
	DMA1_Channel2->CMAR=(uint32_t)(rx_buf);
	DMA1_Channel2->CNDTR=RX_BUF_SZ/2;//SPI data transmitted in half-word mode;
	DMA1_Channel2->CCR|=DMA_CCR2_PL_0;//DMA channel transfer priority at medium level;
	DMA1_Channel2->CCR|=DMA_CCR2_MSIZE_0|DMA_CCR2_PSIZE_0|DMA_CCR2_MINC;//DMA configured as folows:
//memory size is half-word; peripheral size is half-word; memory increment mode enabled;
//DMA_CCR2_DIR is 0x0 by default (data read from peripheral);
	DMA1_Channel2->CCR|=DMA_CCR2_EN;//enable DMA1_Channel2;

	DMA1_Channel3->CPAR=(uint32_t)(SPI1->DR);
	DMA1_Channel3->CMAR=(uint32_t)(tx_buf);
	DMA1_Channel3->CNDTR=sizeof(tx_buf)/2;//SPI data transmitted in half-word mode;
	DMA1_Channel3->CCR|=DMA_CCR3_PL_0;//DMA channel transfer priority at medium level;
	DMA1_Channel3->CCR|=DMA_CCR3_MSIZE_0|DMA_CCR3_PSIZE_0|DMA_CCR3_MINC|DMA_CCR3_DIR;//DMA
//configured as folows: memory size is half-word; peripheral size is half-word; memory increment
//mode enabled; data read from memory;
	DMA1_Channel3->CCR|=DMA_CCR3_EN;//enable DMA1_Channel3;
}

void init_spi(){//PA5 -- SPI1_SCK; PA6 -- SPI1_MISO; PA7 -- SPI1_MOSI;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);
	SPI1->CR1|=SPI_CR1_DFF|SPI_CR1_SSM|SPI_CR1_CRCEN;//16-bit data frame format is selected;
//CK to 0 when idle, first clock transition is the first data capture edge, MSB-first, SSI bit
//and MSTR bit are cleared by default; NSS software mode selected; CRC calculation selected;
	SPI1->CR2|=SPI_CR2_RXDMAEN/*|SPI_CR2_TXDMAEN*/;//Rx buffer DMA enabled;
	SPI1->CR1|=SPI_CR1_SPE;

	GPIO_SetBits(GPIOA,GPIO_Pin_1);
}

int main(void){
	init_gpios();
	init_dma();
	init_spi();

    while(1){

    }
}
