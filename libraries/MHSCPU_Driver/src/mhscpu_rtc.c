/************************ (C) COPYRIGHT Megahuntmicro *************************
 * @file                : mhscpu_rtc.c
 * @author              : Megahuntmicro
 * @version             : V1.0.0
 * @date                : 21-October-2014
 * @brief               : This file provides all the RTC firmware functions
 *****************************************************************************/
 
/* Includes -----------------------------------------------------------------*/ 
#include "mhscpu_rtc.h"

#define RTC_CS_CLR						    ((uint32_t)0x10)
#define RTC_READY_FLAG						((uint32_t)0x08)
#define RTC_IT_ALARM						((uint32_t)0x04)
#define RTC_LOCK_TIM						((uint32_t)0x02)
#define RTC_FLAG_ALARM						((uint32_t)0x01)


/**
  * @brief  Check whether RTC is ready or not
  * @param  None
  * @retval The status of RTC ready flag
  */
FlagStatus RTC_IsReady(void)
{
	if ((RTC->RTC_CS & RTC_READY_FLAG) != (uint32_t)RESET)
	{
		return SET;
	}

	return RESET;
}

/**
  * @brief  Reset RTC counter value
  * @param  None
  * @retval None
  */
void RTC_ResetCounter(void)
{
	while (RTC_IsReady() == RESET);
	RTC->RTC_CS |= RTC_CS_CLR;
}

/**
  * @brief  Get RTC counter value
  * @param  None
  * @retval RTC counter value
  */
uint32_t RTC_GetCounter(void)
{
	uint32_t CurrentCounter = 0;
	
	/* RTC can't config when in reset status */
	while (RTC_IsReady() == RESET);
	
	RTC->RTC_CS |= RTC_LOCK_TIM;
	CurrentCounter = RTC->RTC_TIM;
	RTC->RTC_CS &= ~RTC_LOCK_TIM;
	
	return CurrentCounter;
}

/**
  * @brief  Set RTC alarm value
  * @param  AlarmValue: the new RTC alarm value
  * @retval None
  */
void RTC_SetAlarm(uint32_t AlarmValue)
{
	/* RTC can't config when in reset status */
	while (RTC_IsReady() == RESET);
	
	RTC->RTC_ARM = AlarmValue;
}

/**
  * @brief  Set RTC refernece value
  * @param  RefValue: the new RTC refernece value
  * @retval None
  */
void RTC_SetRefRegister(uint32_t RefValue)
{
	/* RTC can't config when in reset status */	
	while (RTC_IsReady() == RESET);
	
	RTC->RTC_REF = RefValue;
}

/**
  * @brief  Get RTC refernece value
  * @param  None
  * @retval RTC refernece value
  */
uint32_t RTC_GetRefRegister(void)
{
	while (RTC_IsReady() == RESET);
	
	return RTC->RTC_REF;
}

/**
  * @brief  Enable or disable RTC interrupt
  * @param  NewState: new state of the RTC interrupt
  *			This parameter can be ENABLE or DISABLE
  * @retval None
  */
void RTC_ITConfig(FunctionalState NewState)
{
	/* RTC can't config when in reset status */		
    assert_param(IS_FUNCTIONAL_STATE(NewState));
	while (RTC_IsReady() == RESET);
	if (DISABLE != NewState)
    {
        RTC->RTC_CS |= RTC_IT_ALARM;
    }
    else 
    {
        RTC->RTC_CS &= ~RTC_IT_ALARM;
    }
}

/**
  * @brief  Clear RTC interrupt pending bit
  * @param  None
  * @retval None
  */
void RTC_ClearITPendingBit(void)
{
	/* RTC can't config when in reset status */			
	while (RTC_IsReady() == RESET);
	
	RTC->RTC_INTCLR = 0;
}

/**
  * @brief  Check whether the RTC interrupt has occurred or not
  * @param  None
  * @retval The new state of the RTC interrupt(SET or RESET)
  */
ITStatus RTC_GetITStatus(void)
{
	if ((RTC->RTC_CS & RTC_FLAG_ALARM) != (uint32_t)RESET)
	{
		return SET;
	}

	return RESET;
}

/**
  * @brief  Get RTC attrack time value
  * @param  None
  * @retval RTC attrack time value
  */
uint32_t RTC_GetAttrackTime(void)
{
	/* RTC can't config when in reset status */		
	while (RTC_IsReady() == RESET);
	
    return RTC->RTC_ATTA_TIM;
}

/**
  * @brief  Select rtc clock source
  * @param  source��rtc clock source value��
  * @retval None
  */
void RTC_SourceSelect(RTC_SOURCE_TypeDef source)
{
    assert_param(IS_RTC_SOURCE(source));
    switch (source)
    {
        case SELECT_EXT32K:
            BPU->SEN_ANA0 |= BIT(30);
            break;
        case SELECT_INC32K:
            BPU->SEN_ANA0 &= ~BIT(30);
            break;
    }
}

/**************************      (C) COPYRIGHT Megahunt    *****END OF FILE****/
