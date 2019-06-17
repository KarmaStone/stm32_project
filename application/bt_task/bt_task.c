#include "bt_task.h"
#include "init_d.h"
static DEV_HAND fd;
static DEV_HAND irqfd;
xTaskHandle bt_task_handle;


static void bt_open_uart( void )
{
    fd = c_open("lpuart1", 0);

    INIT_PRINT( (fd==NULL)?INIT_FAIL:INIT_OK,"open lpuart1");
}

static void bt_open_irq( void )
{
    irqfd = c_open("syswakeup4", 0);
 
    INIT_PRINT( (irqfd==NULL)?INIT_FAIL:INIT_OK,"open syswakeup4");
}

static void bt_task_process(void const *parma)
{
    uint8_t data[128];

    task_assert(parma);
    
    bt_open_uart();

    bt_open_irq();
    
    if((fd ==NULL)||(irqfd ==NULL))
    {
        log(ERR,"open lpuart1 or system wakeup5 fail ,delet task.\n");
        osThreadTerminate(bt_task_handle);
    }


    while (1)
    {
        if( c_read(irqfd , NULL , 0) == 1)
        {
            log(DEBUG," Bt will get somethings\n");
            c_write(fd , "lock\r\n" , 6);
            c_read(fd , data , sizeof(data));
            log(DEBUG,"Lpuart read:%s\n" , data);
            memset(data , 0x00 , sizeof(data));
        } 
    }
}


int bt_task_init(void)
{
    osThreadDef(bt_task, bt_task_process, osPriorityLow, 0, configMINIMAL_STACK_SIZE * 6);
    bt_task_handle = osThreadCreate(osThread(bt_task), NULL);
    configASSERT(bt_task_handle)

    return 0;
}

int bt_task_resume( void )
{
    c_ioctl(fd , NULL  , 0 , IOCTL_FLAG_RESUME);
    
    return 0;
}

int bt_task_suspend( void )
{
    
    c_ioctl(fd , NULL  , 0 , IOCTL_FLAG_SUSPEND);
    
    return 0;
}

task_init(bt_task_init);
resume_register(bt_task_resume);
suspend_register(bt_task_suspend);