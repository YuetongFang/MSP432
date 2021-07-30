/*
 * pid.h
 *
 *  Created on: 2019��1��24��
 *      Author: FYT
 */

#ifndef PID_H_
#define PID_H_
#include<stdio.h>
#include<stdlib.h>
struct _pid
    {
        float SetSpeed;         //�����趨ֵ
        float ActualSpeed;      //����ʵ��ֵ
        float err;              //�����ϴ�ƫ��ֵ
        float err_last;         //�������ϴ�ƫ��ֵ
        float Kp,Ki,Kd;         //������������֡�΢��ϵ��
        float voltage;          //�����ѹֵ������ִ�����ı�����
        float integral;         //�������ֵ
    }pidx,pidy,pidp;

extern void PID_Xinit(void);
extern void PID_Yinit(void);
extern float Pid_XSpeed(float goal_speed, float now_speed);
extern float Pid_YSpeed(float goal_speed, float now_speed);




#endif /* PID_H_ */
