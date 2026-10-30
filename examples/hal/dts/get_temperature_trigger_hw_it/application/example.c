/**
  ******************************************************************************
  * file           : example.c
  * brief          : dts get temperature hardware trigger in interrupt mode.
  ******************************************************************************
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "example.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
hal_dts_handle_t   *pDTS   = NULL; /* pointer referencing the dts handle from the generated code   */
hal_lptim_handle_t *pLPTIM = NULL; /* pointer referencing the LPTIM handle generating the trigger */

volatile uint32_t TemperatureAvailable = 0U; /* incremented by the DTS end-of-measurement callback */
volatile int32_t  Temperature = 0;           /* latest DTS temperature in degree Celsius       */

/* Private functions prototype -----------------------------------------------*/
/* Functions allowing the user to configure dynamically the DTS callbacks instead of weak functions */
static void DtsCpltCallback(hal_dts_handle_t *hdts, hal_dts_sensor_t sensor);

/** ########## Step 1 ##########
  * Initialize the DTS instance (hardware trigger, interrupt mode) and register
  * the DTS completion callback.
  * Initialize the LPTIM instance used as the DTS hardware trigger source.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  pDTS = mx_example_dts_init();
  if (pDTS == NULL)
  {
    goto _app_init_exit;
  }

  /* Register the DTS callbacks */
  if (HAL_DTS_RegisterCpltCallback(pDTS, DtsCpltCallback) != HAL_OK)
  {
    goto _app_init_exit;
  }
  /* Initialize the LPTIM used as the DTS hardware trigger source. */
  pLPTIM = mx_example_lptim_init();
  if (pLPTIM == NULL)
  {
    goto _app_init_exit;
  }
  PRINTF("[INFO] Step 1: Device initialization COMPLETED.\n");

  /** ########## Step 2 ##########
    * Select the LPTIM channel compare event as the DTS hardware trigger source. */
  if (HAL_DTS_SetSensorTrigger(pDTS, HAL_DTS_SENSOR_1, MX_DTS_TRIGGER_SOURCE) != HAL_OK)
  {
    goto _app_init_exit;
  }
  PRINTF("[INFO] Step 2: DTS hardware trigger source selected.\n");
  return_status = EXEC_STATUS_INIT_OK;

_app_init_exit:
  return return_status;
} /* end app_init */


app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;
  int32_t      measured_temperature;

  /** ########## Step 3 ##########
    * Start the DTS sensor in interrupt mode.
    * The conversion starts on an LPTIM hardware trigger. */
  TemperatureAvailable = 0U;
  if (HAL_DTS_StartSensor_IT(pDTS, HAL_DTS_SENSOR_1) != HAL_OK)
  {
    goto _app_process_exit;
  }
  PRINTF("[INFO] Step 3: DTS sensor armed in interrupt mode.\n");

  /** ########## Step 4 ##########
    * Start the LPTIM generating the hardware trigger: arm the channel output
    * compare, then start the LPTIM counter. */
  if (HAL_LPTIM_OC_StartChannel(pLPTIM, HAL_LPTIM_CHANNEL_1) != HAL_OK)
  {
    goto _app_process_exit;
  }

  if (HAL_LPTIM_Start(pLPTIM) != HAL_OK)
  {
    goto _app_process_exit;
  }
  PRINTF("[INFO] Step 4: LPTIM hardware trigger source started.\n");

  /** ########## Step 5 ##########
    * Acquire one hardware-triggered measurement. Completion is signaled by the
    * DTS interrupt callback and the temperature is retrieved with
    * HAL_DTS_GetSensorTemperature().
    * For this example purpose, we poll for the callback flag but thanks to
    * interrupt mode, application can do other things instead of polling. */
  while (TemperatureAvailable == 0U)
  {
    /** Put the CPU in Wait For Interrupt state. A DTS interrupt or a SysTick
      * interrupt can wake up the CPU.
      * @user: This process is used to illustrate the interest of the interrupt
      *        mode. It can be replaced by your own code.
      */
    __WFI();
  }

  /* Retrieve the latest temperature value measured by the DTS sensor. */
  if (HAL_DTS_GetSensorTemperature(pDTS, HAL_DTS_SENSOR_1, &measured_temperature) != HAL_OK)
  {
    goto _app_process_exit;
  }

  Temperature = measured_temperature;
  PRINTF("[INFO] Step 5: Temperature measurement acquired: %" PRId32 " degC.\n", Temperature);

  /** ########## Step 6 ##########
    * Stop the hardware trigger source then the DTS sensor to release resources
    * and reduce power consumption. */
  if (HAL_LPTIM_Stop(pLPTIM) != HAL_OK)
  {
    goto _app_process_exit;
  }

  if (HAL_LPTIM_OC_StopChannel(pLPTIM, HAL_LPTIM_CHANNEL_1) != HAL_OK)
  {
    goto _app_process_exit;
  }

  if (HAL_DTS_StopSensor_IT(pDTS, HAL_DTS_SENSOR_1) != HAL_OK)
  {
    goto _app_process_exit;
  }
  PRINTF("[INFO] Step 6: DTS sensor and LPTIM trigger stopped.\n");

  return_status = EXEC_STATUS_OK;

_app_process_exit:
  return return_status;
} /* end app_process */


/** Deinitializes the DTS and LPTIM instances before application exit.
  */
app_status_t app_deinit(void)
{
  mx_example_lptim_deinit();
  mx_example_dts_deinit();

  return EXEC_STATUS_OK;
} /* end app_deinit */


/** DTS end-of-measurement interrupt callback.
  * Registered with HAL_DTS_RegisterCpltCallback(), executed from the DTS
  * interrupt context each time a hardware-triggered measurement completes.
  * @user: the completion callback can be customized according to the application needs.
  */
static void DtsCpltCallback(hal_dts_handle_t *hdts, hal_dts_sensor_t sensor)
{
  /* Signal that a new hardware-triggered measurement is ready. */
  TemperatureAvailable++;
}
