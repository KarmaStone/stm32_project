#ifndef __SPI1_H
#define __SPI1_H

#ifdef __cplusplus
extern "C"
{
#endif

  /**
  * @}
  */
#include <stdbool.h>
#include "stm32l4xx_hal.h"
#include "major.h"
#include "unit.h"
#include "init_d.h"
#include "miscdevice.h"
#include "priv_os.h"
#include "startup.h"
  /**
  * @}
  */
typedef struct 
{
    uint8_t *w_d;
    uint8_t *r_d;
    uint16_t cnt;
}spi_wr_t;

#define IOCTL_SET_WR_FLAG IOCTL_FLAG_USER_FLAG1


/**
  * @}
  */

/**
  * @}
  */


  /**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __SPI1_H */
