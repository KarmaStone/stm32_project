#ifndef __LPUART1_H
#define __LPUART1_H

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

/**
  * @}
  */

/**
  * @}
  */
extern void lpuart1_hal_init(void);

  /**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __LPUART1_H */
