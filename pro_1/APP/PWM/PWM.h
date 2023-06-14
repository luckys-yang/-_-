#ifndef __PWM_H
#define __PWM_H
#include "main.h"

extern uint16_t pa6_duty;	//PA6ͨ��1��ʼռ�ձ�10%
extern uint16_t pa7_duty;	//PA7ͨ��1��ʼռ�ձ�50%
extern uint16_t pa6_fre;	//��ʱ����ʼARRֵ
extern uint32_t ch1_pulse_val,ch2_pulse_val;	//ͨ��1��CRRxֵ   ͨ��2��CRRxֵ
extern uint32_t ch1_duty_val,ch2_duty_val;	//ͨ��1��Ч��ƽռ�ձ�����ֵ   ͨ��2��Ч��ƽռ�ձ�����ֵ
extern uint32_t ch1_high,ch2_high;
extern uint32_t ch1_low,ch2_low;

void pA6_SetDuty(uint16_t d);
void pA7_SetDuty(uint16_t d);
void p_SetFre(uint16_t f) ;
void pwm_oc_setCH1(uint32_t freq,uint8_t duty);
void pwm_oc_setCH2(uint32_t freq,uint8_t duty);

#endif
