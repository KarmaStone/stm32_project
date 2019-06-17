#ifndef __KB_TASK_H
#define __KB_TASK_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "cmsis_os.h"
#include "unit.h"
#include "miscdevice.h"
#include "priv_list.h"

typedef enum
{
    KEY_0,
    KEY_1,
    KEY_2,
    KEY_3,
    KEY_4,
    KEY_5,
    KEY_6,
    KEY_7,
    KEY_8,
    KEY_9,
    KEY_DEL,    // *
    KEY_ENT,    // #
    KEY_MAX,
}keyvalue_enum;


typedef enum
{
    SLEEP_MODE = 0,
    MAIN_MODE,
    PWD_MODE,
    FUNC_MODE,
    ADMIN_PWD_MODE,
    MENU_MODE,
} MenuRunModeE;

typedef uint8_t (*DO_FUNC)(void);

typedef struct menu_node
{
    uint8_t *name;
    uint8_t index;
    uint8_t sn;
    uint8_t istask;
    uint8_t (*fun)(void);
    struct menu_node *node[4];
    struct menu_node *priv;
} menu_node;


extern void item_init(void);
extern void menus( uint16_t  keyvalue);


#ifdef __cplusplus
}
#endif

#endif /* __KB_TASK_H */
