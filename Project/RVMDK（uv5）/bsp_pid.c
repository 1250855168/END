#include"bsp_pid.h"

struct _pid{
    float SetTemp;            //定义设定值
    float ActualTemp;        //定义实际值
    float err;                //定义偏差值
    float err_next;            //定义上一个偏差值
    float err_last;            //定义最上前的偏差值
    float Kp,Ki,Kd;            //定义比例、积分、微分系数
}pid;

void PID_init(void){
    pid.SetTemp=45.0;
    pid.ActualTemp=0.0;
    pid.err=0.0;
    pid.err_last=0.0;
    pid.err_next=0.0;
    pid.Kp=0.2;
    pid.Ki=0.015;
    pid.Kd=0.2;
}

float PID_realize(float Temp){
    pid.SetTemp=Temp;
    pid.err=pid.SetTemp-pid.ActualTemp;
    float incrementSpeed=pid.Kp*(pid.err-pid.err_next)+pid.Ki*pid.err+pid.Kd*(pid.err-2*pid.err_next+pid.err_last);
    pid.ActualTemp+=incrementSpeed;
    pid.err_last=pid.err_next;
    pid.err_next=pid.err;
    return pid.ActualTemp;
}


