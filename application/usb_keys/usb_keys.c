#include "usb_keys.h"
#include "init_d.h"

static DEV_HAND keysfd;
static DEV_HAND usbfd;
xTaskHandle usb_keys_handle;


static void usb_keys_open( void )
{
  
    keysfd = c_open("gpio-keys", 0);
    
    INIT_PRINT( (keysfd==NULL)?INIT_FAIL:INIT_OK,"open gpio-keys");
    
    usbfd = c_open("usb-hid", 0);
    
    INIT_PRINT( (usbfd==NULL)?INIT_FAIL:INIT_OK,"open usb-hid");

}

uint8_t key_map[][2]=
{
  { 0x41 , 0x59},   //1
  { 0x42 , 0x5A},   //2
  { 0x43 , 0x5B},   //3
  { 0x31 , 0x5C},   //4
  { 0x32 , 0x5D},   //5
  { 0x33 , 0x5E},   //6
  { 0x21 , 0x5F},   //7
  { 0x22 , 0x60},   //8
  { 0x23 , 0x61},   //9
  { 0x51 , 0x62},   //0
  { 0x52 , 0x63},   //.
};

const uint8_t map_size =sizeof(key_map)/sizeof(key_map[0]);

uint8_t find_keyval( uint8_t key )
{
    uint8_t i = 0 ; 
    
    for( i = 0 ; i <  map_size ; i++ )
    {
        if( key == key_map[i][0])
        {
            return key_map[i][1];
        }
    }
    log(WARN,"no this key [%x]\n" , key);
    
    return 0;
}


static void usb_keys_process(void const *parma)
{
    uint8_t data[128];
    uint8_t usb_buf[8];
    int size = 0 , i = 0;

    task_assert(parma);
    
    usb_keys_open();
    

    while (1)
    {
        memset(usb_buf , 0 , sizeof(usb_buf));
        size = c_read( keysfd , data , sizeof(data));

        size = (size > 6)?6:size;
        
        for( i = 0 ; i < size ; i++ )
        {
            usb_buf[i+2] = find_keyval(data[i]);
        }

        c_write(usbfd , usb_buf , 8);
        memset(usb_buf , 0 , sizeof(usb_buf));
        c_write(usbfd , usb_buf , 8);
        if( size == 0)
        {
            memset(usb_buf , 0 , sizeof(usb_buf));
            c_write(usbfd , usb_buf , 8);
            c_write(usbfd , usb_buf , 8);
            c_write(usbfd , usb_buf , 8);
        }
    }
}


int usb_keys_init(void)
{
    osThreadDef(usb_keys_task, usb_keys_process, osPriorityLow, 0, configMINIMAL_STACK_SIZE * 6);
    usb_keys_handle = osThreadCreate(osThread(usb_keys_task), NULL);
    configASSERT(usb_keys_handle)

    return 0;
}

int usb_keys_resume( void )
{
    c_ioctl(keysfd , NULL  , 0 , IOCTL_FLAG_RESUME);
    
    return 0;
}

int usb_keys_suspend( void )
{
    
    c_ioctl(keysfd , NULL  , 0 , IOCTL_FLAG_SUSPEND);
    
    return 0;
}

task_init(usb_keys_init);
resume_register(usb_keys_resume);
suspend_register(usb_keys_suspend);