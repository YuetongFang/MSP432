/*
 * pid.c
 *
 *  Created on: 2019��1��24��
 *      Author: FYT
 */

#include "pid.h"

void PID_Xinit(void)
{
    pidx.SetSpeed = 0;
    pidx.ActualSpeed=0.0;
    pidx.err        =0.0;
    pidx.err_last   =0.0;
    pidx.voltage    =0.0;
    pidx.integral   =0.0;
    pidx.Kp         = 0.72; //�Լ��趨
    pidx.Ki         = 0;   //�Լ��趨
    pidx.Kd         = 13;//�Լ��趨
}

void PID_Yinit(void)
{
    pidy.SetSpeed = 0;
    pidy.ActualSpeed=0.0;
    pidy.err        =0.0;
    pidy.err_last   =0.0;
    pidy.voltage    =0.0;
    pidy.integral   =0.0;
    pidy.Kp         = 0.72; //�Լ��趨
    pidy.Ki         = 0;   //�Լ��趨
    pidy.Kd         = 13;//�Լ��趨
}

float Pid_XSpeed(float goal_speed, float now_speed){
    pidx.SetSpeed    = goal_speed;                //�趨ֵ
    pidx.ActualSpeed = now_speed;                //ʵ��ֵ
    pidx.err         = now_speed - goal_speed;  //�趨ֵ-ʵ��ֵ
    pidx.integral   += pidx.err;                //����ֵ��ƫ���ۼ�
    pidx.voltage     = pidx.Kp*pidx.err +
                       pidx.Ki*pidx.integral +
                       pidx.Kd*(pidx.err-pidx.err_last);
    pidx.err_last    = pidx.err;            //��һ��ƫ��ֵ
    pidx.ActualSpeed = pidx.voltage;   //���ʵ��ֵ

    return pidx.ActualSpeed;            //����
}

float Pid_YSpeed(float goal_speed, float now_speed){
    pidy.SetSpeed    = goal_speed;                //�趨ֵ
    pidy.ActualSpeed = now_speed;                //ʵ��ֵ
    pidy.err         = now_speed - goal_speed;  //�趨ֵ-ʵ��ֵ
    pidy.integral   += pidy.err;                //����ֵ��ƫ���ۼ�
    pidy.voltage     = pidy.Kp*pidy.err +
                       pidy.Ki*pidy.integral +
                       pidy.Kd*(pidy.err-pidy.err_last);
    pidy.err_last    = pidy.err;            //��һ��ƫ��ֵ
    pidy.ActualSpeed = pidy.voltage;   //���ʵ��ֵ

    return pidy.ActualSpeed;            //����
}
