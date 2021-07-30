/*
 * pid.h
 *
 *  Created on: 2019年1月24日
 *      Author: FYT
 */

#ifndef PID_H_
#define PID_H_
#include<stdio.h>
#include<stdlib.h>
struct _pid
    {
        float SetSpeed;         //定义设定值
        float ActualSpeed;      //定义实际值
        float err;              //定义上次偏差值
        float err_last;         //定义上上次偏差值
        float Kp,Ki,Kd;         //定义比例、积分、微分系数
        float voltage;          //定义电压值（控制执行器的变量）
        float integral;         //定义积分值
    }pidx,pidy,pidp;

extern void PID_Xinit(void);
extern void PID_Yinit(void);
extern float Pid_XSpeed(float goal_speed, float now_speed);
extern float Pid_YSpeed(float goal_speed, float now_speed);




#endif /* PID_H_ */
