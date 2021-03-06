#include "stm32f10x.h"
#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "pwm.h"
#include "timer.h"


 int main(void)
 {	
		u32 temp=0;
		int Distance1;
	 int Distance2;
	 int Distance3;
		int cache[14];
	 int flag1=0;
	 int flag2=0;
	 int flag3=0;
	 int i;
		delay_init();	    	 //延时函数初始化	
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// 设置中断优先级分组2
		uart_init(9600);	 //串口初始化为9600
		LED_Init();		  	 //初始化与LED连接的硬件接口 
		TIM1_PWM_Init(899,0);//不分频。PWM频率=72000/(899+1)=80Khz//控制电机
		TIM_SetCompare1(TIM1,899);	 
		TIM_SetCompare4(TIM1,899);	 
//		TIM2_Int_Init(899,0);//超声波
		TIM2_Cap_Init(0XFFFF,72-1);
		TIM3_PWM_Init(899,0);
		TIM5_Cap_Init(0XFFFF,72-1);	//以1Mhz的频率计�		
		TIM4_Cap_Init(0XFFFF,72-1);	//以1Mhz的频率计�		
		while(1)
		{	
			
			GPIO_SetBits(GPIOB,GPIO_Pin_0);
			delay_us(15);
			GPIO_ResetBits(GPIOB,GPIO_Pin_0);
			delay_ms(70);
			
			GPIO_SetBits(GPIOB,GPIO_Pin_1);
			delay_us(15);
			GPIO_ResetBits(GPIOB,GPIO_Pin_1);
			delay_ms(70);
			
			GPIO_SetBits(GPIOB,GPIO_Pin_11);
			delay_us(15);
			GPIO_ResetBits(GPIOB,GPIO_Pin_11);
			delay_ms(70);
					 
			if(TIM5CH1_CAPTURE_STA&0X80)//成功捕获到了一次上升沿
			{
				temp=TIM5CH1_CAPTURE_STA&0X3F;
				temp*=65536;//溢出时间总和
				temp+=TIM5CH1_CAPTURE_VAL;//得到总的高电平时间
				Distance1=temp*170.0/10000;
//				if(Distance1<100)
//					printf("00%d",Distance1);//打印总的高点平时间
//				if((Distance1>=100)&&(Distance1<1000))
//					printf("0%d",Distance1);//打印总的高点平时间
//				if(Distance1>=1000)
//					printf("%d",Distance1);//打印总的高点平时间
				TIM5CH1_CAPTURE_STA=0;//开启下一次捕获
				flag1=1;
			}
			
			if(TIM4CH1_CAPTURE_STA&0X80)//成功捕获到了一次上升沿
			{
				temp=TIM4CH1_CAPTURE_STA&0X3F;
				temp*=65536;//溢出时间总和
				temp+=TIM4CH1_CAPTURE_VAL;//得到总的高电平时间
				Distance2=temp*170.0/10000;
				
//				if(Distance2<100)
//					printf("00%d",Distance2);//打印总的高点平时间
//				if((Distance2>=100)&&(Distance2<1000))
//					printf("0%d",Distance2);//打印总的高点平时间
//				if(Distance2>=1000)
//					printf("%d",Distance2);//打印总的高点平时间
				TIM4CH1_CAPTURE_STA=0;//开启下一次捕获
				flag2=1;
			}
			
			if(TIM2CH1_CAPTURE_STA&0X80)//成功捕获到了一次上升沿
			{
				temp=TIM2CH1_CAPTURE_STA&0X3F;
				temp*=65536;//溢出时间总和
				temp+=TIM2CH1_CAPTURE_VAL;//得到总的高电平时间
				Distance3=temp*170.0/10000;
				
//				if(Distance3<100)
//					printf("00%d",Distance3);//打印总的高点平时间
//				if((Distance3>=100)&&(Distance3<1000))
//					printf("0%d",Distance3);//打印总的高点平时间
//				if(Distance3>=1000)
//					printf("%d",Distance3);//打印总的高点平时间
				TIM2CH1_CAPTURE_STA=0;//开启下一次捕获
				flag3=1;
			}
			
			if(flag1==1&&flag2==1&&flag3==1)
			{
					flag1=0;flag2=0;flag3=0;
					cache[0]=0x0b;
					cache[1]=Distance1/1000;
					cache[2]=Distance1%1000/100;
					cache[3]=Distance1%1000%100/10;
					cache[4]=Distance1%1000%100%10;
				
					cache[5]=Distance2/1000;
					cache[6]=Distance2%1000/100;
					cache[7]=Distance2%1000%100/10;
					cache[8]=Distance2%1000%100%10;
				
					cache[9]=Distance3/1000;
					cache[10]=Distance3%1000/100;
					cache[11]=Distance3%1000%100/10;
					cache[12]=Distance3%1000%100%10;
				
					cache[13]=0x0d;
				for(i=0;i<14;i++)
					{
						//while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);
						//printf("%d",cache[i]);
						USART_SendData(USART1,cache[i]);
						while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);
					}
				//printf("\n");
			}
			delay_ms(500);
		}	 
}


