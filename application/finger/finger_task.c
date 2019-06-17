#include "finger_task.h"
#include "init_d.h"

static DEV_HAND fd;
static DEV_HAND irqfd;
xTaskHandle finger_task_handle;

static void finger_task_process(void const *pvParameters)
{

    configASSERT(((unsigned long)pvParameters) == 0);

    irqfd = c_open("syswakeup1", 0);
    INIT_PRINT( (irqfd==NULL)?INIT_FAIL:INIT_OK,"open syswakeup1");

    while (1)
    {
        if( c_read(irqfd , NULL , 0) == 1)
        {
            log(DEBUG,"Finger check...\n");
        } 
    }
}


int finger_task_init(void)
{
    osThreadDef(finger_task, finger_task_process, osPriorityLow, 0, configMINIMAL_STACK_SIZE * 6);
    finger_task_handle = osThreadCreate(osThread(finger_task), NULL);
    configASSERT(finger_task_handle)

    return 0;
}

int finger_task_resume( void )
{
    c_ioctl(fd , NULL  , 0 , IOCTL_FLAG_RESUME);
    
    return 0;
}

int finger_task_suspend( void )
{
    
    c_ioctl(fd , NULL  , 0 , IOCTL_FLAG_SUSPEND);
    
    return 0;
}

task_init(finger_task_init);
resume_register(finger_task_resume);
suspend_register(finger_task_suspend);