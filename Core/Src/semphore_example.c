#include "rtthread.h"
#include "main.h"

static rt_sem_t dynamic_sem;
void LedThreadRevSemphoreEntry(void* parameter)
{
     static rt_err_t result;
    while (1)
    {
        result = rt_sem_take(dynamic_sem, RT_WAITING_FOREVER);
        if(result == RT_EOK)
        {
            HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
        }
    }
}
void LedThreadSndSemphoreEntry(void* parameter)
{
    static rt_uint8_t flg = 1;
     while (1)
    {
        if(flg == 1)
        {
            flg = 0;
            rt_thread_mdelay(5000);
        }
        rt_thread_mdelay(100);
        rt_sem_release(dynamic_sem);
    }
    
}

void DynamicSemExample(void)
{
    dynamic_sem = rt_sem_create("dsem", 0, RT_IPC_FLAG_FIFO);
    if(dynamic_sem == RT_NULL)
    {
        Error_Handler();
    }
    rt_thread_t led_thread_rev;
    led_thread_rev = rt_thread_create(
        "led_rev_entry" ,
        LedThreadRevSemphoreEntry ,
        RT_NULL ,
        256 ,
        20 ,
        20
    );
    if(led_thread_rev != RT_NULL)
    {
        rt_thread_startup(led_thread_rev);
    }
    rt_thread_t led_thread_snd;
    led_thread_snd = rt_thread_create(
        "led_snd_entry" ,
        LedThreadSndSemphoreEntry ,
        RT_NULL ,
        256 ,
        20 ,
        20
    );
    if(led_thread_snd != RT_NULL)
    {
        rt_thread_startup(led_thread_snd);
    }
}
 