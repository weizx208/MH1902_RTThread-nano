/************************ (C) COPYRIGHT Megahuntmicro *************************
 * File Name            : mhscpu_adc.c
 * Author               : Megahuntmicro
 * Version              : V1.0.0
 * Date                 : 05/28/2017
 * Description          : ADC module functions set.
 *****************************************************************************/
 
/* Include ------------------------------------------------------------------*/
#include "mhscpu_adc.h"
/* Private typedef ----------------------------------------------------------*/
/* Private define -----------------------------------------------------------*/	
/* Private macro ------------------------------------------------------------*/	
/* Private variables --------------------------------------------------------*/	
/* Ptivate function prototypes ----------------------------------------------*/	


/******************************************************************************
* Function Name  : ADC_Init
* Description    : ��ʼ��ADC,��ʼ���ο�ֵ
* Input          : ADC_InitStruct��Ҫ��ʼ�������ݽṹָ��
* Return         : NONE
******************************************************************************/
void ADC_Init(ADC_InitTypeDef *ADC_InitStruct)
{
    assert_param(IS_ADC_CHANNEL(ADC_InitStruct->ADC_Channel));
    assert_param(IS_ADC_SAMP(ADC_InitStruct->ADC_SampSpeed));
    assert_param(IS_FUNCTIONAL_STATE(ADC_InitStruct->ADC_IRQ_EN));
    assert_param(IS_FUNCTIONAL_STATE(ADC_InitStruct->ADC_FIFO_EN));
	
    
    ADC0->ADC_CR2 = (ADC0->ADC_CR2 & ~(ADC_CR2_REFTRIM_MASK)) | *(uint8_t *)(ADC_CR2_REFTRIM_OTPADDR);
    /* Select ADC Channel */
    ADC0->ADC_CR1 = (ADC0->ADC_CR1 & ~(ADC_CR1_CHANNEL_MASK)) | ADC_InitStruct->ADC_Channel; 
    /* Select ADC Channel Samping */
    ADC0->ADC_CR1 = (ADC0->ADC_CR1 & ~(ADC_CR1_SAMPLE_RATE_MASK)) | (ADC_InitStruct->ADC_SampSpeed << ADC_CR1_SAMPLE_RATE_POS);
    /* Set ADC Interrupt */
    if (ENABLE == ADC_InitStruct->ADC_IRQ_EN )
    {
        NVIC_EnableIRQ(ADC0_IRQn);
        ADC0->ADC_CR1 |= ADC_IRQ_EN_BIT;
    }else
    {
        ADC0->ADC_CR1 &= ~ADC_IRQ_EN_BIT;
    }
    /* Set ADC FIFO */
    if (ENABLE == ADC_InitStruct->ADC_FIFO_EN)
    {
        ADC0->ADC_FIFO |= ADC_FIFO_EN_BIT;
    }else
    {
        ADC0->ADC_FIFO &= ~ADC_FIFO_EN_BIT;
    }
}

/******************************************************************************
* Function Name  : ADC_GetChannel
* Description    : ��ȡADC��ǰʹ�õ�ͨ��
* Input          : None
* Return         : ADC_ChxTypeDef
******************************************************************************/
ADC_ChxTypeDef ADC_GetChannel(void)
{
    return (ADC_ChxTypeDef)(ADC0->ADC_CR1 & ADC_CR1_CHANNEL_MASK);
}

/******************************************************************************
* Function Name  : ADC_GetResult
* Description    : ������ȡADC��Ӧͨ����ֵ
* Input          : None
* Return         : 0:��ȡ��ʱ  Other:ADCֵ
******************************************************************************/
int32_t ADC_GetResult(void)
{
    while((!(ADC0->ADC_SR & ADC_DONE_FLAG)));
    return (ADC0->ADC_DATA & ADC_DATA_MASK) - *(int16_t *)(ADC_DATA_OFFSET_OTPADDR);
}

/******************************************************************************
* Function Name  : ADC_GetFIFOResult
* Description    : ������ȡADC��Ӧͨ����ֵ,�г�ʱ���
* Input          : ADCdata:FIFOָ�� 
                   len:����
* Return         : FIFO��ADC����
******************************************************************************/
int32_t ADC_GetFIFOResult(uint16_t *ADCdata, uint32_t len)
{
    uint32_t i, adccount = 0;
    //��ȡ���ȴ���FIFO���޸���
    if(NULL == ADCdata)
    {
        return 0;
    }

    adccount = ADC_GetFIFOCount();
    if(adccount > len)
    {
        adccount = len;
    }
    
    for(i = 0; i < adccount; i++)
    {
        ADCdata[i] = (ADC0->ADC_DATA & ADC_DATA_MASK) - *(int16_t *)(ADC_DATA_OFFSET_OTPADDR);
    }
    return adccount;
}

/******************************************************************************
* Function Name  : ADC_GetFIFOCount
* Description    : ��ȡFIFO�����ݵĸ���
* Input          : None
* Return         : 0:δ����FIFOģʽ other:FIFO�����ݵĸ���
******************************************************************************/
int32_t ADC_GetFIFOCount(void)
{
    if ((ADC0->ADC_FIFO & ADC_FIFO_EN_BIT) != ADC_FIFO_EN_BIT)
    {
        return 0;
    }
    return ADC0->ADC_FIFO_FL & 0x1F;
}

/******************************************************************************
* Function Name  : ADC_FifoReset
* Description    : ����ADC fifo
* Input          : None
* Return         : None
******************************************************************************/
void ADC_FIFOReset(void)
{
    ADC0->ADC_FIFO |= ADC_FIFO_RESET_BIT;
}

/******************************************************************************
* Function Name  : ADC_ITCmd
* Description    : ����ADC�жϵĿ���
* Input          : NewState��ADC�жϿ��ز���������ȡENABLE��DISABLE
* Output         : NONE
* Return         : NONE
******************************************************************************/
void ADC_ITCmd(FunctionalState NewState)
{
    assert_param(IS_FUNCTIONAL_STATE(NewState));
    if(DISABLE != NewState)
	{
		ADC0->ADC_CR1 |= ADC_IRQ_EN_BIT;
	}
	else
	{
		ADC0->ADC_CR1 &= ~ADC_IRQ_EN_BIT;
	}
}

/******************************************************************************
* Function Name  : ADC_FIFOOverflowITcmd
* Description    : ����ADC FIFO����жϵĿ���
* Input          : NewState��ENABLE/DISABLE
* Output         : NONE
* Return         : NONE
******************************************************************************/
void ADC_FIFOOverflowITcmd(FunctionalState NewState)
{
    assert_param(IS_FUNCTIONAL_STATE(NewState));
    if(DISABLE != NewState)
	{
		ADC0->ADC_FIFO |= ADC_FIFOOVERFLOWIRQ_EN_BIT;
	}
	else
	{
		ADC0->ADC_FIFO &= ~ADC_FIFOOVERFLOWIRQ_EN_BIT;
	}
}

/******************************************************************************
* Function Name  : ADC_ISFIFOOverflow
* Description    : ��ȡADC����ж��Ƿ���λ
* Input          : NONE
* Output         : ADC_OverFlow/ADC_NoOverFlow
* Return         : NONE
******************************************************************************/
ADC_OverFlowTypeDef ADC_ISFIFOOverflow(void)
{
    if((ADC0->ADC_SR & ADC_FIFOOVERFLOWIRQ_FLAG) == ADC_FIFOOVERFLOWIRQ_FLAG)
    {
        return ADC_OverFlow;
    }
    else
    {
        return ADC_NoOverFlow;
    }
}

/******************************************************************************
* Function Name  : ADC_StartCmd
* Description    : ADCת����������
* Input          : NewState��ADCת���������ز���������ȡENABLE��DISABLE
* Output         : NONE
* Return         : NONE
******************************************************************************/
void ADC_StartCmd(FunctionalState NewState)
{
    if (DISABLE != NewState)
    {
        ADC0->ADC_CR1 |= ADC_SAMP_EN_BIT;
        ADC0->ADC_CR1 &= ~ADC_START_BIT;
    }
    else
    {
        ADC0->ADC_CR1 &= ~ADC_SAMP_EN_BIT;
        ADC0->ADC_CR1 |= ADC_START_BIT;
    }
}

/******************************************************************************
* Function Name  : ADC_FIFODeepth
* Description    : ADC fifo �������
* Input          : FIFO_Deepth ����fifo��ȣ����ֵ0x20
* Output         : NONE
* Return         : NONE
******************************************************************************/
void ADC_FIFODeepth(uint32_t FIFO_Deepth)
{
    ADC0->ADC_FIFO_THR = FIFO_Deepth - 1;
}

/******************************************************************************
* Function Name  : ADC_CalVoltage
* Description    : ����ת����ĵ�ѹֵ
* Input          : u32ADC_Value: ADC�ɼ�ֵ
*                : u32ADC_Ref_Value: ADC�ο���ѹ
* Output         : NONE
* Return         : ����ת����ĵ�ѹֵ
******************************************************************************/
uint32_t ADC_CalVoltage(uint32_t u32ADC_Value, uint32_t u32ADC_Ref_Value)
{
    return (u32ADC_Value * u32ADC_Ref_Value / 1023);
}

/************************ (C) COPYRIGHT 2017 Megahuntmicro ****END OF FILE****/
