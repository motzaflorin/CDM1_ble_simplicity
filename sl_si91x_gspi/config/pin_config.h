#ifndef PIN_CONFIG_H
#define PIN_CONFIG_H

// $[USART0]
// [USART0]$

// $[UART1]
// [UART1]$

// $[ULP_UART]
// [ULP_UART]$

// $[I2C0]
// [I2C0]$

// $[I2C1]
// [I2C1]$

// $[ULP_I2C]
// [ULP_I2C]$

// $[SSI_MASTER]
// [SSI_MASTER]$

// $[SSI_SLAVE]
// [SSI_SLAVE]$

// $[ULP_SSI]
// [ULP_SSI]$

// $[GSPI_MASTER]
// GSPI_MASTER SCK_ on GPIO_25
#ifndef GSPI_MASTER_SCK__PORT                   
#define GSPI_MASTER_SCK__PORT                    HP
#endif
#ifndef GSPI_MASTER_SCK__PIN                    
#define GSPI_MASTER_SCK__PIN                     25
#endif
#ifndef GSPI_MASTER_SCK_LOC                     
#define GSPI_MASTER_SCK_LOC                      1
#endif

// GSPI_MASTER CS0_ on GPIO_28
#ifndef GSPI_MASTER_CS0__PORT                   
#define GSPI_MASTER_CS0__PORT                    HP
#endif
#ifndef GSPI_MASTER_CS0__PIN                    
#define GSPI_MASTER_CS0__PIN                     28
#endif
#ifndef GSPI_MASTER_CS0_LOC                     
#define GSPI_MASTER_CS0_LOC                      5
#endif

// GSPI_MASTER MOSI_ on GPIO_27
#ifndef GSPI_MASTER_MOSI__PORT                  
#define GSPI_MASTER_MOSI__PORT                   HP
#endif
#ifndef GSPI_MASTER_MOSI__PIN                   
#define GSPI_MASTER_MOSI__PIN                    27
#endif
#ifndef GSPI_MASTER_MOSI_LOC                    
#define GSPI_MASTER_MOSI_LOC                     17
#endif

// GSPI_MASTER MISO_ on GPIO_26
#ifndef GSPI_MASTER_MISO__PORT                  
#define GSPI_MASTER_MISO__PORT                   HP
#endif
#ifndef GSPI_MASTER_MISO__PIN                   
#define GSPI_MASTER_MISO__PIN                    26
#endif
#ifndef GSPI_MASTER_MISO_LOC                    
#define GSPI_MASTER_MISO_LOC                     22
#endif

// [GSPI_MASTER]$

// $[I2S0]
// [I2S0]$

// $[ULP_I2S]
// [ULP_I2S]$

// $[SCT]
// [SCT]$

// $[SIO]
// [SIO]$

// $[PWM]
// [PWM]$

// $[PWM_CH0]
// [PWM_CH0]$

// $[PWM_CH1]
// [PWM_CH1]$

// $[PWM_CH2]
// [PWM_CH2]$

// $[PWM_CH3]
// [PWM_CH3]$

// $[ADC_CH1]
// [ADC_CH1]$

// $[ADC_CH2]
// [ADC_CH2]$

// $[ADC_CH3]
// [ADC_CH3]$

// $[ADC_CH4]
// [ADC_CH4]$

// $[ADC_CH5]
// [ADC_CH5]$

// $[ADC_CH6]
// [ADC_CH6]$

// $[ADC_CH7]
// [ADC_CH7]$

// $[ADC_CH8]
// [ADC_CH8]$

// $[ADC_CH9]
// [ADC_CH9]$

// $[ADC_CH10]
// [ADC_CH10]$

// $[ADC_CH11]
// [ADC_CH11]$

// $[ADC_CH12]
// [ADC_CH12]$

// $[ADC_CH13]
// [ADC_CH13]$

// $[ADC_CH14]
// [ADC_CH14]$

// $[ADC_CH15]
// [ADC_CH15]$

// $[ADC_CH16]
// [ADC_CH16]$

// $[ADC_CH17]
// [ADC_CH17]$

// $[ADC_CH18]
// [ADC_CH18]$

// $[ADC_CH19]
// [ADC_CH19]$

// $[COMP1]
// [COMP1]$

// $[COMP2]
// [COMP2]$

// $[DAC0]
// [DAC0]$

// $[DAC1]
// [DAC1]$

// $[SYSRTC]
// [SYSRTC]$

// $[UULP_VBAT_GPIO]
// [UULP_VBAT_GPIO]$

// $[GPIO]
// [GPIO]$

// $[QEI]
// [QEI]$

// $[CUSTOM_PIN_NAME]
#ifndef _PORT                                   
#define _PORT                                    HP
#endif
#ifndef _PIN                                    
#define _PIN                                     6
#endif

// [CUSTOM_PIN_NAME]$

// $[HSPI_SECONDARY]
// [HSPI_SECONDARY]$
#endif // pin_config_h
