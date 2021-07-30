/*
 * pid.c
 *
 *  Created on: 2019年1月24日
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
    pidx.Kp         = 0.72; //自己设定
    pidx.Ki         = 0;   //自己设定
    pidx.Kd         = 13;//自己设定
}

void PID_Yinit(void)
{
    pidy.SetSpeed = 0;
    pidy.ActualSpeed=0.0;
    pidy.err        =0.0;
    pidy.err_last   =0.0;
    pidy.voltage    =0.0;
    pidy.integral   =0.0;
    pidy.Kp         = 0.72; //自己设定
    pidy.Ki         = 0;   //自己设定
    pidy.Kd         = 13;//自己设定
}

float Pid_XSpeed(float goal_speed, float now_speed){
    pidx.SetSpeed    = goal_speed;                //设定值
    pidx.ActualSpeed = now_speed;                //实际值
    pidx.err         = now_speed - goal_speed;  //设定值-实际值
    pidx.integral   += pidx.err;                //积分值，偏差累加
    pidx.voltage     = pidx.Kp*pidx.err +
                       pidx.Ki*pidx.integral +
                       pidx.Kd*(pidx.err-pidx.err_last);
    pidx.err_last    = pidx.err;            //上一个偏差值
    pidx.ActualSpeed = pidx.voltage;   //算出实际值

    return pidx.ActualSpeed;            //返回
}

float Pid_YSpeed(float goal_speed, float now_speed){
    pidy.SetSpeed    = goal_speed;                //设定值
    pidy.ActualSpeed = now_speed;                //实际值
    pidy.err         = now_speed - goal_speed;  //设定值-实际值
    pidy.integral   += pidy.err;                //积分值，偏差累加
    pidy.voltage     = pidy.Kp*pidy.err +
                       pidy.Ki*pidy.integral +
                       pidy.Kd*(pidy.err-pidy.err_last);
    pidy.err_last    = pidy.err;            //上一个偏差值
    pidy.ActualSpeed = pidy.voltage;   //算出实际值

    return pidy.ActualSpeed;            //返回
}
