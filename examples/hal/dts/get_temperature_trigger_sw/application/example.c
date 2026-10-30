/**
  ******************************************************************************
  * file           : example.c
  * brief          : dts get temperature trigger software.
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
#define TIMEOUT_MS  10

/* Private functions prototype -----------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
hal_dts_handle_t *pDTS = NULL; /* pointer referencing the dts handle from the generated code */
static int32_t Temperature = 0;      /* variable to store the measured temperature value */


/** ########## Step 1 ##########
  * Initializes DTS peripheral.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  pDTS = mx_example_dts_init();

  if (pDTS != NULL)
  {
    PRINTF("[INFO] Step 1: Device initialization COMPLETED.\n");
    return_status = EXEC_STATUS_INIT_OK;
  }

  return return_status;
} /* end app_init */


app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;


  /** ########## Step 2 ##########
    * Start the DTS sensor 1. */
  if (HAL_DTS_StartSensor(pDTS, HAL_DTS_SENSOR_1) != HAL_OK)
  {
    PRINTF("[ERROR] Step 2: DTS sensor start operation ERROR.\n");
    goto _app_process_exit;
  }
  PRINTF("[INFO] Step 2: DTS sensor start operation COMPLETED.\n");


  /** ########## Step 3 ##########
    * Wait (polling mode) until the DTS sensor 1 has completed the temperature. */
  if (HAL_DTS_PollForSensorTemperature(pDTS, HAL_DTS_SENSOR_1, TIMEOUT_MS) != HAL_OK)
  {
    PRINTF("[ERROR] Step 3: DTS temperature measurement NOT AVAILABLE within the expected timeout.\n");
    goto _app_process_exit;
  }
  PRINTF("[INFO] Step 3: DTS temperature measurement READY.\n");


  /** ########## Step 4 ##########
    * Retrieve the latest temperature value measured by DTS sensor 1 and store it. */
  if (HAL_DTS_GetSensorTemperature(pDTS, HAL_DTS_SENSOR_1, &Temperature) != HAL_OK)
  {
    PRINTF("[ERROR] Step 4: DTS temperature value NOT RETRIEVED.\n");
    goto _app_process_exit;
  }
  PRINTF("[INFO] Step 4: DTS temperature value RETRIEVED: %"PRId32" degree Celsius.\n", Temperature);


  /** ########## Step 5 ##########
    * Stop DTS sensor 1 to release resources and reduce power consumption.
    * If stopping the sensor fails, abort the process.
    */
  if (HAL_DTS_StopSensor(pDTS, HAL_DTS_SENSOR_1) != HAL_OK)
  {
    PRINTF("[ERROR] Step 5: DTS sensor stop operation NOT COMPLETED.\n");
    goto _app_process_exit;
  }
  PRINTF("[INFO] Step 5: DTS sensor stop operation COMPLETED.\n");

  /** At this point, all DTS operations have completed successfully.
    * Update the return status accordingly (EXEC_STATUS_OK).
    */
  return_status = EXEC_STATUS_OK;

_app_process_exit:
  return return_status;
} /* end app_process */


/** ########## Step 6 ##########
  * Deinitializes the DTS instance before application exit.
  */
app_status_t app_deinit(void)
{
  mx_example_dts_deinit();

  return EXEC_STATUS_OK;
} /* end app_deinit */
