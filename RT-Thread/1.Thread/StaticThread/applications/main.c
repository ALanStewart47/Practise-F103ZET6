/*
 * Copyright (c) 2006-2023, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-08-14     RT-Thread    first version
 */

#include <rtthread.h>


#define DBG_TAG "main"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

/*定义线程控制块*/
static  struct rt_thread led1_thread;
/* 定义线程栈 */
static rt_uint8_t rt_led1_thread_stack[1024];
/* 定义线程控栈时要求RT_ALIGN_SIZE个字节对齐 */
ALIGN(RT_ALIGN_SIZE)

static void led1_thread_entry(void * parameter);

int main(void)
{
    int count = 1;
/*
    while (count++)
    {
        LOG_D("Hello RT-Thread!");
        rt_thread_mdelay(1000);
    }
*/

    rt_thread_init(&led1_thread, "led1", led1_thread_entry, RT_NULL, &rt_led1_thread_stack[0], sizeof(rt_led1_thread_stack), 3, 20);
    rt_thread_startup(&led1_thread);
    return RT_EOK;
}


static void led1_thread_entry(void * parameter)
{
    while(1)
    {

        LOG_D("1");
        rt_thread_delay(500);

        LOG_D("2");
        rt_thread_delay(500);
    }

}


