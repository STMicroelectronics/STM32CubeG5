/**
  ******************************************************************************
  * file           : example.c
  * brief          : RTC timestamp from a passive tamper source with HAL API
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
#define PRINT_TIMESTAMP(h, m, s)                                 \
  do                                                             \
  {                                                              \
    PRINTF("[INFO] Step 3: Timestamp: hour=%2" PRIu32 " ", (h)); \
    PRINTF("min=%2" PRIu32 " ", (m));                            \
    PRINTF("sec=%2" PRIu32 "\n", (s));                           \
  } while (0)

#define PRINT_DATESTAMP(mon, mday)                               \
  do                                                            \
  {                                                             \
    PRINTF("[INFO] Step 3: Datestamp: month=%2d ", (mon));       \
    PRINTF("mday=%2" PRIu32 " \n", (mday));                      \
  } while (0)
/* Private variables ---------------------------------------------------------*/
volatile hal_rtc_time_t rtcTimeStamp; /* Captured timestamp time */
volatile hal_rtc_date_t rtcDateStamp; /* Captured timestamp date */
volatile hal_rtc_timestamp_information_t timeStampInfo; /* Timestamp event information */

/* Private functions prototype -----------------------------------------------*/

/** ########## Step 1 ##########
  * Initializes the RTC and tamper peripherals, enables the RTC timestamp
  * interrupt, and starts passive tamper detection.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  if (mx_tamp_init() != SYSTEM_OK)
  {
    goto _app_process_exit;
  }

  if (mx_example_rtc_init() != SYSTEM_OK)
  {
    goto _app_process_exit;
  }

  /* Ensure timestamp event information starts in a known state. */
  timeStampInfo.flag = HAL_RTC_TIMESTAMP_NO_EVENT;

  if (HAL_RTC_TIMESTAMP_EnableIT() != HAL_OK)
  {
    goto _app_process_exit;
  }

  /* Start passive tamper detection after the RTC timestamp interrupt is ready. */
  if (HAL_TAMP_PASSIVE_Start(MX_TAMP_INPUT, HAL_TAMP_IT_NONE) != HAL_OK)
  {
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 1: Device initialization COMPLETED.\n");
  PRINTF("[INFO] Step 1: Press the user push-button to trigger a timestamp event.\n");
  return_status = EXEC_STATUS_INIT_OK;

_app_process_exit:

  return return_status;
} /* end app_init */


app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_OK;
  hal_rtc_time_t rtc_time;
  hal_rtc_date_t rtc_date;
  /** ########## Step 2 ##########
    * Reads and displays the current RTC date and time, then waits for the
    * calendar to update.
    */

  if (HAL_RTC_CALENDAR_GetDateTime(&rtc_date, &rtc_time) != HAL_OK)
  {
    return_status = EXEC_STATUS_ERROR;
    goto _app_process_exit;
  }

  /* Display time Format : hh:mm:ss */
  PRINTF("[INFO] Step 2: Current Time : %2" PRIu32 ":%2" PRIu32 ":%2" PRIu32 " \n",
         rtc_time.hour, rtc_time.min, rtc_time.sec);

  /* Display date Format : mm-dd-yy */
  PRINTF("[INFO] Step 2: Current Date : %2d-%2" PRIu32 "-%2" PRIu32 " \n",
         rtc_date.mon, rtc_date.mday, 2000U + rtc_date.year);

  /* Delay to update and report the calendar once per second. */
  HAL_Delay(1000);

  /** ########## Step 3 ##########
    * Checks the RTC timestamp event and displays the captured date and time.
    */
  if (timeStampInfo.flag == (hal_rtc_timestamp_event_flag_t)HAL_RTC_TIMESTAMP_NO_EVENT)
  {
    goto _app_process_exit;
  }

  /* Check for timestamp overflow event. */
  if (timeStampInfo.flag != (hal_rtc_timestamp_event_flag_t)HAL_RTC_TIMESTAMP_EVENT)
  {
    timeStampInfo.flag = HAL_RTC_TIMESTAMP_NO_EVENT;
    return_status = EXEC_STATUS_ERROR;
    goto _app_process_exit;
  }

  if (timeStampInfo.source != (hal_rtc_timestamp_event_source_t)MX_RTC_TIMESTAMP_SOURCE)
  {
    timeStampInfo.flag = HAL_RTC_TIMESTAMP_NO_EVENT;
    return_status = EXEC_STATUS_ERROR;
    goto _app_process_exit;
  }

  /* Timestamp event source confirmed: display the captured date and time. */
  /* Display time Format : hh:mm:ss */
  PRINT_TIMESTAMP(rtcTimeStamp.hour, rtcTimeStamp.min, rtcTimeStamp.sec);

  /* Display date Format : mm-dd-yy */
  PRINT_DATESTAMP(rtcDateStamp.mon, rtcDateStamp.mday);

  timeStampInfo.flag = HAL_RTC_TIMESTAMP_NO_EVENT;

_app_process_exit:

  return return_status;
} /* end app_process */


/** Deinitializes the RTC instance.
  * In this example, app_deinit is never called and it is provided as a reference only.
  */
app_status_t app_deinit(void)
{
  mx_example_rtc_deinit();

  return EXEC_STATUS_OK;
} /* end app_deinit */


/** RTC timestamp event callback.
  * Captures the timestamp event and rearms passive tamper detection.
  */
void HAL_RTC_TimestampEventCallback(void)
{
  /* Stop passive tamper detection while the captured timestamp is read. */
  (void)HAL_TAMP_PASSIVE_Stop(MX_TAMP_INPUT);

  (void)HAL_RTC_TIMESTAMP_GetDateTime((hal_rtc_time_t *)&rtcTimeStamp, (hal_rtc_date_t *)&rtcDateStamp,
                                      (hal_rtc_timestamp_information_t *)&timeStampInfo);

  /* Restart passive tamper detection for the next timestamp event. */
  (void)HAL_TAMP_PASSIVE_Start(MX_TAMP_INPUT, HAL_TAMP_IT_NONE);
}

/**  User hook function called before the HAL_Init() function
  */
system_status_t pre_system_init_hook(void)
{
  /* Reset the RTC domain (RTC, TAMP, backup registers, LSE configuration, etc) */
  HAL_PWR_DisableRTCDomainWriteProtection();
  HAL_RCC_ResetRTCDomain();

  return SYSTEM_OK;
}
