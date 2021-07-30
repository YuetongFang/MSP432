/*
 * kelman.c
 *
 *  Created on: 2019年3月27日
 *      Author: FYT
 */
#include "kalman.h"

////////////////////////////////////////////////////// 卡尔曼滤波参数设定
#define     Qinc    1.1           	 	    //	噪声协方差矩阵Q——加速度计参数
#define     Qgyro	0.001		    		//	噪声协方差矩阵Q——陀螺仪参数
#define     Rmeans	100 		     		//	噪声协方差矩阵R——Rmeans参数
#define     Ts      0.02 				    //	采样周期
////////////////////////////////////////////////////// 	卡尔曼滤波参数设定

////////////////////////////////////////////////////// 卡尔曼滤波变量
float           P_f[2][2]={	0.01,0.01,
				0.01,0.01
						};

float           P_b[2][2]={	0.01,0.01,
				0.01,0.01
						};
float			Zinc_f=0;							// 加速度计的先验状态估计值
float 			Zinc_b=0;							// 加速度计的后验状态估计值
float 			U_gyro=0;							// 陀螺仪的漂移——对应X2
float  			y_k;								// 加速度计的参差
float			s_tk;								// 残差的协方差
float			k_1;								// 最优卡尔曼增益——1
float			k_2;								// 最优卡尔曼增益——2
////////////////////////////////////////////////////// 卡尔曼滤波变量

float kalman(float Gyro_speed,float Mma_speed){
	Zinc_f = Zinc_b + Ts*Gyro_speed; 
	P_f[0][0] = P_b[0][0] - Ts*(P_b[0][1] + P_b[1][0]) + Ts*Ts*P_b[1][1] + Qinc;
	P_f[1][0] = P_f[0][1] = P_b[0][1] - Ts*P_b[1][1];
	P_f[1][1] =	P_b[1][1] + Qgyro;
	y_k = Mma_speed - Zinc_f;
	k_1	= P_f[0][0] / (P_f[0][0] + Rmeans);
	k_2	= P_f[1][0] / (P_f[0][0] + Rmeans);
	Zinc_b = Zinc_f + k_1*y_k;
	U_gyro = U_gyro + k_2*y_k;
	P_b[0][0] =	(1- k_1)*P_f[0][0];
	P_b[0][1] =	(1- k_1)*P_f[0][1];
	P_b[1][0] =	-1*k_2*P_f[0][0] + P_f[0][1];
	P_b[1][1] =	-1*k_2*P_f[0][1] + P_f[1][1];
	return Zinc_b;
}


