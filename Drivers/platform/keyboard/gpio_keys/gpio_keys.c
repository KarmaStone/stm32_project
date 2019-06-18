#include "gpio_keys.h"


void EXTI0_IRQHandler(void)
{
    if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_0) != RESET)
    {
        __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_0);
        log(DEBUG, " COL0 , V=%d\n" , HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0));
    }    
}
void EXTI1_IRQHandler(void)
{
	if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_1) != RESET)
    {
        __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_1);
        log(DEBUG, " COL1 , V=%d\n" , HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1));
    } 
}
void EXTI2_IRQHandler(void)
{
	if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_2) != RESET)
    {
        __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_2);
        log(DEBUG, " COL2 , V=%d\n" , HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_2));
    } 
}
void EXTI3_IRQHandler(void)
{
	if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_3) != RESET)
    {
        __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_3);
        log(DEBUG, " COL3 , V=%d\n" , HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_3));
    } 
}
void EXTI4_IRQHandler(void)
{
	if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_4) != RESET)
    {
        __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_4); 
        log(DEBUG, " COL4 , V=%d\n" , HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_4));
    } 
}
void EXTI9_5_IRQHandler(void)
{
    if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_5) != RESET)
    {
        __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_5);
        log(DEBUG, " COL5 , V=%d\n" , HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5));
    } 
    if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_6) != RESET)
    {
        __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_6); 
        log(DEBUG, " COL6 , V=%d\n" , HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_6));
    } 
    if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_7) != RESET)
    {
        __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_7);
        log(DEBUG, " COL7 , V=%d\n" , HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7));
    } 
    if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_8) != RESET)
    {
        __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_8);  
        log(DEBUG, " COL8 , V=%d\n" , HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_8));
    } 
    if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_9) != RESET)
    {
        __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_9);  
        log(DEBUG, " COL9 , V=%d\n" , HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_9));
    } 
}

void EXTI15_10_IRQHandler(void)
{

    if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_10) != RESET)
    {
        __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_10);
        log(DEBUG, " COL10 , V=%d\n" , HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_10));
    } 
    if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_11) != RESET)
    {
        __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_11); 
        log(DEBUG, " COL11 , V=%d\n" , HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_11));
    } 
    if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_12) != RESET)
    {
        __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_12);
        log(DEBUG, " COL12 , V=%d\n" , HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12));
    } 
    if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_13) != RESET)
    {
        __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_13);  
        log(DEBUG, " COL13 , V=%d\n" , HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_13));
    } 
    if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_14) != RESET)
    {
        __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_14);
        log(DEBUG, " COL14 , V=%d\n" , HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14));
    } 
    if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_15) != RESET)
    {
        __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_15); 
        log(DEBUG, " COL15 , V=%d\n" , HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_15));
    } 
    
}



int gpio_keys_open(FIL_HAND *fd)
{

    GPIO_InitTypeDef GPIO_InitStruct;
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();

    GPIO_InitStruct.Pin = GPIO_PIN_All;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* Enable and set Button EXTI Interrupt to the lowest priority */
    HAL_NVIC_SetPriority(EXTI0_IRQn, 5, 0x00);
    HAL_NVIC_EnableIRQ(EXTI0_IRQn);

    HAL_NVIC_SetPriority(EXTI1_IRQn, 5, 0x00);
    HAL_NVIC_EnableIRQ(EXTI1_IRQn);

    HAL_NVIC_SetPriority(EXTI2_IRQn, 5, 0x00);
    HAL_NVIC_EnableIRQ(EXTI2_IRQn);

    HAL_NVIC_SetPriority(EXTI3_IRQn, 5, 0x00);
    HAL_NVIC_EnableIRQ(EXTI3_IRQn);

    HAL_NVIC_SetPriority(EXTI4_IRQn, 5, 0x00);
    HAL_NVIC_EnableIRQ(EXTI4_IRQn);

    HAL_NVIC_SetPriority(EXTI9_5_IRQn, 5, 0x00);
    HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

    HAL_NVIC_SetPriority(EXTI15_10_IRQn, 5, 0x00);
    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);


    GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_6|GPIO_PIN_10|GPIO_PIN_11;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
    
    HAL_GPIO_WritePin(GPIOC , GPIO_PIN_0 , GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOC , GPIO_PIN_1 , GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOC , GPIO_PIN_2 , GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOC , GPIO_PIN_3 , GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOC , GPIO_PIN_6 , GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOC , GPIO_PIN_10 , GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOC , GPIO_PIN_11 , GPIO_PIN_SET);
    return 0;
}

int gpio_keys_write(FIL_HAND *fd, const void *buf, uint32_t count)
{

    
    return 0;
}

int gpio_keys_read(FIL_HAND *fd, void *data, uint32_t count)
{
 
    return 0;

}

struct file_operations gpio_keys_fops =
{
    .onwer = 0,
    .open = gpio_keys_open,
    .write = gpio_keys_write,
    .read = gpio_keys_read,
};

struct miscdevice gpio_keys_device =
{
    .minor = MISC_DYNAMIC_MINOR,
    .name = "gpio-keys",
    .fops = &gpio_keys_fops,
    .nodename = "device",
};

int gpio_keys_init(void)
{
    return (misc_register(&gpio_keys_device));
}

device_init(gpio_keys_init);