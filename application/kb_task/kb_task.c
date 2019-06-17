#include "kb_task.h"
#include "stdlib.h"
#include "init_d.h"

static DEV_HAND fd;

xTaskHandle kb_task_handle;
uint8_t run_stats = SLEEP_MODE;

#define INPUT_MAX_CNT 30
uint8_t in_arry[INPUT_MAX_CNT];
uint8_t in_cnt = 0;
uint8_t admin_pwd[6] = {1, 1, 1, 1, 1, 1};

void open_keyboard(void)
{
    fd = c_open("touch_keyboard", 0);
    INIT_PRINT((fd == NULL) ? INIT_FAIL : INIT_OK, "open touch keyboard");
}




uint8_t pwd_check(uint8_t mode, uint8_t *pwd, uint8_t len)
{
    return 0;
}

static uint8_t read_key_value(void)
{
    uint8_t value = 0;
    do
    {
        value = c_read(fd, NULL, 0);

    } while ((value == 0) || (value > KEY_ENT));

    return value;
}

void init_menu(void)
{
    run_stats = MAIN_MODE;
    log(DEBUG, "主菜单页面\n");
}

void main_menu_tsk(uint8_t key)
{
    switch (key)
    {
    case KEY_DEL:
    {
        log(DEBUG,"再次输入#，进入菜单页面，输入*进入休眠\n");
        run_stats = FUNC_MODE;
    }
    break;
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    {
        in_arry[in_cnt++] = key;
        run_stats = PWD_MODE;
        log(DEBUG, "输入开门密码: %d", key);
    }
    break;
    default:
    {
        log(WARN, "按键键值输入错误,VALUE=%d\n", key);
    }
    break;
    }
}

void func_menu_tsk(uint8_t key)
{
    switch (key)
    {
    case KEY_DEL:
    {
        log(DEBUG,"进入休眠模式\n");
        run_stats = SLEEP_MODE;
    }
    break;
    case KEY_ENT:
    {
        log(DEBUG, "进入管理员密码输入模式\n");
        run_stats = ADMIN_PWD_MODE;
    }
    break;
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    {
        in_arry[in_cnt++] = key;
        run_stats = PWD_MODE;
        log(DEBUG, "输入开门密码: %d", key);
    }
    break;
    default:
    {
        log(WARN, "按键键值输入错误,VALUE=%d\n", key);
    }
    break;
    }
}

void admin_pwd_menu_tsk(uint8_t key)
{
    switch (key)
    {
    case KEY_DEL:
    {
        if (in_cnt > 0)
        {
            in_arry[in_cnt - 1] = 0;
            in_cnt--;
        }
        else
        {
            log(DEBUG,"退回主菜单\n");
            run_stats = MAIN_MODE;
        }
    }
    break;
    case KEY_ENT:
    {
        if (in_cnt != 6)
        {
            log(DEBUG, "管理员密码错误\n");
            run_stats = MAIN_MODE;
            memset(in_arry, 0x00, sizeof(in_arry));
            in_cnt = 0;
        }
        else
        {
            if (memcmp(in_arry, admin_pwd, sizeof(admin_pwd)) == 0)
            {
                run_stats = MENU_MODE;
                memset(in_arry, 0x00, sizeof(in_arry));
                in_cnt = 0;
                log(DEBUG, "密码输入正确\n");
                printf("\033[2J");
                menus(KEY_ENT);
            }
        }
    }
    break;
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    {
        if (in_cnt < INPUT_MAX_CNT)
        {
            in_arry[in_cnt++] = key;
        }
    }
    break;
    default:
    {
        log(WARN, "按键键值输入错误,VALUE=%d\n", key);
    }
    break;
    }
}

void open_pwd_tsk(uint8_t key)
{
    switch (key)
    {
    case KEY_DEL:
    {
        if (in_cnt > 0)
        {
            in_arry[in_cnt - 1] = 0;
            in_cnt--;
        }
        else
        {
            log(DEBUG,"退回主菜单\n");
            run_stats = MAIN_MODE;
        }
    }
    break;
    case KEY_ENT:
    {

        log(DEBUG, "密码输入正确\n");
        log_arry(DEBUG, "开门密码输入：", in_arry, in_cnt);

        run_stats = MAIN_MODE;
        memset(in_arry, 0x00, sizeof(in_arry));
        in_cnt = 0;
    }
    break;
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    {
        if (in_cnt < INPUT_MAX_CNT)
        {
            in_arry[in_cnt++] = key;
        }
    }
    break;
    default:
    {
        log(WARN, "按键键值输入错误,VALUE=%d\n", key);
    }
    break;
    }
}

static void kb_task_process(void const *parma)
{
    uint8_t keyvalue = 0;

    task_assert(parma);

    open_keyboard(); //注册键盘驱动

    item_init(); //菜单列表初始化

    init_menu(); //主菜单初始化

    while (1)
    {
        keyvalue = read_key_value();

        switch (run_stats)
        {
        case SLEEP_MODE:
        {
            run_stats = MAIN_MODE;
            log(DEBUG, "有按键触发，退出休眠模式，KEY=%d\n", keyvalue);
        }
        break;
        case MAIN_MODE: 
        {
            main_menu_tsk(keyvalue);
        }
        break;
        case FUNC_MODE:
        {
            func_menu_tsk(keyvalue);
        }
        break;
        case ADMIN_PWD_MODE:
        {
            admin_pwd_menu_tsk(keyvalue);
        }
        break;
        case PWD_MODE:
        {
            open_pwd_tsk(keyvalue);
        }
        break;
        case MENU_MODE:
        {
            menus(keyvalue);
        }
        break;
        default:
            log(DEBUG, "模式错误 ,MODE=%d\n", run_stats);
        }
    }
}

int kb_task_init(void)
{
    osThreadDef(kb_task, kb_task_process, osPriorityLow, 0, configMINIMAL_STACK_SIZE * 6);
    kb_task_handle = osThreadCreate(osThread(kb_task), NULL);
    configASSERT(kb_task_handle);
    
    return 0;
}

int kb_task_resume(void)
{
    c_ioctl(fd, NULL, 0, IOCTL_FLAG_RESUME);

    return 0;
}

int kb_task_suspend(void)
{

    c_ioctl(fd, NULL, 0, IOCTL_FLAG_SUSPEND);

    return 0;
}

task_init(kb_task_init);
resume_register(kb_task_resume);
suspend_register(kb_task_suspend);
