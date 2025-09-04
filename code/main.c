/**
 * @file  main.c
 * @brief LED 循环闪烁示例——STM32F103C8T6 标准库
 *        PC13 接 LED
 */
#include "stm32f10x.h"
#include "stm32f10x_conf.h"

void RCC_Config(void);
void GPIO_Config(void);
void Delay_ms(uint32_t ms);

int main(void)
{
    RCC_Config();
    GPIO_Config();

    while (1)
    {
        /* LED ON */
        GPIO_ResetBits(GPIOC, GPIO_Pin_13);
        Delay_ms(500);

        /* LED OFF */
        GPIO_SetBits(GPIOC, GPIO_Pin_13);
        Delay_ms(500);
    }
}

/* ---------- 底层配置 ---------- */
void RCC_Config(void)
{
    /* 开启 GPIOC 时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
}

void GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /* PC13 推挽输出，50 MHz */
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    /* 默认先关灯（高电平） */
    GPIO_SetBits(GPIOC, GPIO_Pin_13);
}

/* ---------- 简易阻塞延时 ---------- */
void Delay_ms(uint32_t ms)
{
    /* 72 MHz 时钟下，SysTick->LOAD 最大 24 位 */
    /* 1ms ≈ 72000 个时钟周期 */
    SysTick->LOAD = 72000 - 1;
    SysTick->VAL  = 0;
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;

    for (; ms != 0; ms--)
    {
        while (!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk));
    }

    SysTick->CTRL = 0;   /* 关闭 SysTick */
}