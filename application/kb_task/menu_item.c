
#include "kb_task.h"
#include "stdlib.h"
#include "init_d.h"
#include "kb_task.h"


extern uint8_t run_stats;

menu_node menu_zero =
    {
        NULL,
        0,
        0,
        0,
        NULL,
        NULL, NULL, NULL, NULL,
        NULL};

menu_node main_node =
    {
        "main nume",
        0,
        0,
        0,
        NULL,
        NULL, NULL, NULL, NULL,
        NULL};

menu_node *curr_node = &main_node;

menu_node *find_parent_node(menu_node *node, uint8_t *name)
{
    menu_node *parent_node = NULL;
    uint8_t i;

    if (strncmp((char *)node->name, (char *)name, strlen((char *)name)) == 0)
    {
        return node;
    }

    for (i = 0; i < node->sn; i++)
    {
        if (node->node[i] != NULL)
        {
            if ((parent_node = find_parent_node(node->node[i], name)) != NULL)
            {
                return parent_node;
            }
        }
    }

    return parent_node;
}

void add_item(uint8_t *praent_name, uint8_t *name, DO_FUNC func)
{
    menu_node *node;
    menu_node *parent_node = NULL;

    parent_node = find_parent_node(&main_node, praent_name);
    if (parent_node == NULL)
    {
        log(WARN, "%s parent node is not find.\n", praent_name);
        return;
    }
    node = malloc(sizeof(menu_node));
    if (node != NULL)
    {

        memcpy(node, &menu_zero, sizeof(menu_node));
        node->name = malloc((strlen((char *)name) + 1) * sizeof(uint8_t));

        if (node->name != NULL)
        {
            memset(node->name, 0x00, strlen((char *)name) + 1);
            memcpy(node->name, name, strlen((char *)name));

            node->index = parent_node->sn++;

            if (func != NULL)
            {
                node->istask = 1;
                node->fun = func;
            }

            if (parent_node->node[node->index] != NULL)
            {
                log(WARN, "The current node has data and will override it\n");
            }

            parent_node->node[node->index] = node;
            node->priv = parent_node;
        }
        else
        {
            log(DEBUG, "malloc error . line =%d\n", __LINE__);
        }
    }
    else
    {
        log(DEBUG, "malloc error . line =%d\n", __LINE__);
    }
}

void into_current_menu_funct(menu_node *node)
{
    uint8_t i = 0;
    menu_node *ptf_node;

    if (node->istask)
    {
        node->fun();
    }
    else
    {
        for (i = 0; i < node->sn; i++)
        {
            ptf_node = node->node[i];
            if (ptf_node != NULL)
            {
                printf("%d.%s\n", ptf_node->index + 1, ptf_node->name);
            }
        }
    }
}

void item_init(void)
{
    add_item("main nume", "管理员设置", NULL);
    add_item("main nume", "用户设置", NULL);
    add_item("main nume", "系统设置", NULL);
    add_item("main nume", "数据统计及记录", NULL);

    add_item("管理员设置", "录入管理员", NULL);
    add_item("管理员设置", "修改管理员", NULL);
    add_item("管理员设置", "删除管理员", NULL);
    add_item("管理员设置", "回复出厂", NULL);

    add_item("用户设置", "录入用户", NULL);
    add_item("用户设置", "修改用户", NULL);
    add_item("用户设置", "删除用户", NULL);
    add_item("用户设置", "设置开锁区间", NULL);

    add_item("系统设置", "时间设置", NULL);
    add_item("系统设置", "语言&语音设置", NULL);
    add_item("系统设置", "所模式设置", NULL);
    add_item("系统设置", "网络设置", NULL);

    add_item("语言&语音设置", "语音设置", NULL);
    add_item("语言&语音设置", "语言设置", NULL);

    add_item("语音设置", "增加音量", NULL);
    add_item("语音设置", "减小音量", NULL);
    add_item("语音设置", "语音开关", NULL);

    add_item("语言设置", "Englist", NULL);
    add_item("语言设置", "简体中文", NULL);
    add_item("语言设置", "繁体中文", NULL);
}

void menus( uint16_t  keyvalue)
{
    switch (keyvalue)
    {
    case KEY_ENT:
    {
        curr_node = &main_node;
    }
    break;
    case 1:
    {
        if (curr_node->node[0] != NULL)
        {
            curr_node = curr_node->node[0];
        }
    }
    break;
    case 2:
    {
        if (curr_node->node[1] != NULL)
        {
            curr_node = curr_node->node[1];
        }
    }
    break;
    case 3:
    {
        if (curr_node->node[2] != NULL)
        {
            curr_node = curr_node->node[2];
        }
    }
    break;
    case 4:
    {
        if (curr_node->node[3] != NULL)
        {
            curr_node = curr_node->node[3];
        }
    }
    break;
    case KEY_DEL:
    {
        if( curr_node == &main_node )
        {
            run_stats = MAIN_MODE;
            log(DEBUG,"进入主界面\n");
        }
        
        if (curr_node->priv != NULL)
        {
            curr_node = curr_node->priv;
        }
        
    }
    break;

    default:
        break;
    }
    into_current_menu_funct(curr_node);
}