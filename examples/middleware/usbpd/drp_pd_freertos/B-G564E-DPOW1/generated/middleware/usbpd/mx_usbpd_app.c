

/**
  ******************************************************************************
  * @file    mx_usbpd_app.c
  * @brief   USBX applicative source file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025-2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the mx_usbpd_license.md file
  * in the same directory as the generated code.
  * If no mx_usbpd_license.md file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
#define USBPD_TRACE_VERBOSE_LEVEL     USBPD_TRACE_VERBOSE_LEVEL_INFO /*<! Trace verbosity level for this file. */

/* Includes ------------------------------------------------------------------*/
#include "mx_usbpd_app.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define OS_PE_PRIORITY    (4)     /*!< Priority of PE task                        */
#define OS_CAD_PRIORITY   (5)     /*!< Priority of CAD task (higher prio than PE) */
#define OS_PE_STACK_SIZE  (1024U) /*!< Stack size for PE task                     */
#define OS_CAD_STACK_SIZE (1024U) /*!< Stack size for CAD task                    */
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

usbpd_params_t       usbpd_dpm_params[USBPD_PORT_COUNT]; /*!< DPM parameters */
static QueueHandle_t cad_queue; /*!< CAD queue handle */
static TaskHandle_t  cad_task;  /*!< CAD task handle */

static QueueHandle_t pe_queue[USBPD_PORT_COUNT]; /*!< PE queue handles */
static TaskHandle_t  pe_task[USBPD_PORT_COUNT];  /*!< PE task handles */

usbpd_settings_t usbpd_dpm_settings[USBPD_PORT_COUNT] =
{
  {
    /* Supported SOP : SOP, SOP' SOP" SOP'Debug SOP"Debug */
    .pe_supported_sop = USBPD_SUPPORTED_SOP_NONE,
    /* Spec revision value */
    .pe_spec_revision = USBPD_SPECIFICATION_REV3,
    /* Default port role */
    .pe_default_role = USBPD_PORT_PWR_ROLE_SNK,
    /* Support port role swap */
    .pe_pwr_role_swap_support = USBPD_TRUE,
    /* VDM Support */
    .pe_vdm_support = USBPD_FALSE,
    /*!< Can respond successfully to a Discover Identity */
    .pe_respond_to_discov_sop_support = USBPD_FALSE,
    /*!< Can send a Discover Identity */
    .pe_attempts_to_discov_sop_support = USBPD_FALSE,
    /* Support Ping (only for PD3.0) */
    .pe_ping_support = USBPD_FALSE,
    /* Support caps counter */
    .pe_caps_counter_support = USBPD_FALSE,
    /* CAD role toggle */
    .cad_role_toggle_support = USBPD_TRUE,
    /* CAD default Rp resistor */
    .cad_default_resistor = USBPD_CAD_RP_1A5,
    /* CAD try feature */
    .cad_try_feature_support = USBPD_FALSE,
    /* CAD accessory support */
    .cad_accessory_support = USBPD_FALSE,
    /* CAD src toggle time in ms */
    .cad_src_toggle_time_ms = 40U,
    /* CAD snk toggle time in ms */
    .cad_snk_toggle_time_ms = 40U,

    /*!< PD3 SUPPORT FEATURE */
    .pe_pd3_support.b =
    {
      /* Support Unchunked mode (valid only spec revision 3.0) */
      .pe_unchunk_support = USBPD_FALSE,
      /* Support fast role swap only spec revision 3.0 */
      .pe_fast_role_swap_support = USBPD_FALSE,
      /*!< PPS message NOT supported by PE stack */
      .is_get_pps_status_supported = USBPD_FALSE,
      /*!< Source_Capabilities_Extended message supported or not by DPM */
      .is_src_capa_ext_supported = USBPD_FALSE,
      /*!< Alert message supported or not by DPM */
      .is_alert_supported = USBPD_FALSE,
      /*!< Status message supported or not by DPM (is_alert_supported must be enabled) */
      .is_get_status_supported = USBPD_FALSE,
      /*!< Manufacturer_Info message supported or not by DPM */
      .is_get_manufacturer_info_supported = USBPD_FALSE,
      /*!< Country_Codes message supported or not by DPM */
      .is_get_country_codes_supported = USBPD_FALSE,
      /*!< Country_Info message supported or not by DPM */
      .is_get_country_info_supported = USBPD_FALSE,
      /*!< Security_Response message supported or not by DPM */
      .is_security_request_supported = USBPD_FALSE,
      /*!< Firmware update response message supported by PE */
      .is_fw_update_request_supported = USBPD_FALSE,
      /*!< Get Battery Capabitity and Status messages supported by PE */
      .is_get_battery_status_supported = USBPD_FALSE,
    },
  }
};

usbpd_dpm_id_settings_t usbpd_dpm_id_settings[USBPD_PORT_COUNT] =
{
  {
    /*!< Value provided by the USB-IF assigned to the product */
    .xid = USBPD_XID,
    /*!< Vendor ID (assigned by the USB-IF) */
    .vid = USBPD_VID,
    /*!< Product ID (assigned by the manufacturer) */
    .pid = USBPD_PID,
  },
};

/* Private function prototypes -----------------------------------------------*/
static void DPM_Delay(uint8_t port_num, uint32_t time_ms);
static void USBPD_CAD_Task(void *args);
static void DPM_CADTaskWakeUp(void);
static void DPM_CADCallback(uint8_t port_num, usbpd_cad_event_t state, usbpd_cc_line_t cc);
static void USBPD_PE_Task(void *args);
static void DPM_PE_TaskWakeUp(uint8_t port_num);
static void DPM_StartPETask(uint8_t port_num);
/**
  * @brief  Application USBPD Initialization.
  * @param  status
  */
usbpd_status_t app_usbpd_init(void)
{

  return USBPD_DPM_Init();
}

/**
  * @brief  Application USBPD De-Initialization.
  * @retval none
  */
void app_usbpd_deinit(void)
{

  USBPD_DPM_DeInit();
}

/**
  * @brief  Initialize the core stack (port power role, PWR_IF, CAD and PE Init procedures).
  * @retval USBPD status.
  */
usbpd_status_t USBPD_DPM_Init(void)
{
  uint32_t stack_dynamemsize;
  usbpd_status_t ret = USBPD_OK;
  /* CAD callback definition */
  static const usbpd_pe_cb_t dpmCallbacks =
  {
    USBPD_DPM_SetupNewPower,
    USBPD_DPM_HardReset,
    USBPD_DPM_EvaluatePowerRoleSwap,
    USBPD_DPM_Notification,
    USBPD_DPM_ExtendedMessageReceived,
    USBPD_DPM_GetDataInfo,
    USBPD_DPM_SetDataInfo,
    USBPD_DPM_EvaluateRequest,
    USBPD_DPM_SNK_EvaluateCapabilities,
    USBPD_DPM_PowerRoleSwap,
    DPM_PE_TaskWakeUp,
    USBPD_DPM_EvaluateVconnSwap,
    USBPD_DPM_PE_VconnPwr,
    USBPD_DPM_EnterErrorRecovery,
    USBPD_DPM_EvaluateDataRoleSwap,
    USBPD_DPM_IsPowerReady,
    USBPD_DPM_UserTimerCounter
  };
  /* Check the lib selected */
  if (USBPD_TRUE != USBPD_PE_CheckLIB(LIB_ID))
  {
    ret = USBPD_ERROR;
    return ret;
  }

  /* Initialize DPM User */
  if (USBPD_DPM_UserInit() != USBPD_OK)
  {
    ret = USBPD_ERROR;
    return ret;
  }

  /* Get dynamic memory allocated by usbpd stack */
  stack_dynamemsize = USBPD_PE_GetMemoryConsumption();
  (void)stack_dynamemsize;

  /* Initialise the USBPD trace */
  USBPD_TRACE_Init();

  static const usbpd_cad_cb_t CAD_cbs = { DPM_CADCallback, DPM_CADTaskWakeUp };

  static const usbpd_pwr_cb_t PWR_cbs =
  {
   USBPD_PWR_HW_IF_Init,
   USBPD_PWR_HW_IF_DeInit,
   USBPD_PWR_HW_IF_VBUSInit,
   USBPD_PWR_HW_IF_VBUSDeInit,
   USBPD_PWR_HW_IF_EnableVBUS,
   USBPD_PWR_HW_IF_DisableVBUS,
   USBPD_PWR_HW_IF_IsEnabledVBUS,
   USBPD_PWR_HW_IF_VConnInit,
   USBPD_PWR_HW_IF_VConnDeInit,
   USBPD_PWR_HW_IF_EnableVConn,
   USBPD_PWR_HW_IF_DisableVConn,
   USBPD_PWR_HW_IF_IsEnabledVConn,
   USBPD_PWR_HW_IF_VBUSGetVoltage,
   USBPD_PWR_HW_IF_VBUSGetCurrent,
   USBPD_PWR_HW_IF_VBUSSetVoltage_Fixed,
   USBPD_PWR_HW_IF_VBUSSetVoltage_Variable,
   USBPD_PWR_HW_IF_VBUSSetVoltage_Battery,
   USBPD_PWR_HW_IF_VBUSSetVoltage_APDO,
   USBPD_PWR_HW_IF_SetRole,
   USBPD_PWR_HW_IF_SetPowerMode,
   USBPD_PWR_HW_IF_RegisterVBUSDetectCallback,
  };

  /* Register the power hardware interface callbacks  */
  if (USBPD_PWR_Register_HW_IF_Callbacks(&PWR_cbs) != USBPD_OK)
  {
    ret = USBPD_ERROR;
    return ret;
  }

  cad_queue = xQueueCreate(USBPD_PORT_COUNT, sizeof(uint16_t));
  if (cad_queue == 0)
  {
    USBPD_TRACE_ERROR_PRINTF(USBPD_PORT_0, "CAD Queue creation failed");
    ret = USBPD_ERROR;
    return ret;
  }

  /* Register the queue to be viewable in a RTOS kernel aware debugger */
  vQueueAddToRegistry(cad_queue, "cad_queue");

  if (xTaskCreate(USBPD_CAD_Task,
                  "CAD Task",
                  OS_CAD_STACK_SIZE,
                  (void *) NULL,
                  OS_CAD_PRIORITY,
                  &cad_task) != pdPASS)
  {
    /* If task creation fails try to increase configTOTAL_HEAP_SIZE */
    USBPD_TRACE_ERROR_PRINTF(USBPD_PORT_0, "Task creation failed, try to increase configTOTAL_HEAP_SIZE");
    ret = USBPD_ERROR;
    return ret;
  }

  for (uint8_t port_index = 0U; port_index < (uint8_t)USBPD_PORT_COUNT; port_index++)
  {
    /* Create the queue corresponding to PE task */
    pe_queue[port_index] = xQueueCreate(1U, sizeof(uint16_t));
    if (pe_queue[port_index] == 0)
    {
      USBPD_TRACE_ERROR_PRINTF(USBPD_PORT_0, "PE (port %d) Queue creation failed", port_index);
      ret = USBPD_ERROR;
      return ret;
    }

    /* Register the queue to be viewable in a RTOS kernel aware debugger */
#if (USBPD_PORT_COUNT > 1)
    vQueueAddToRegistry(pe_queue[port_index], (port_index == USBPD_PORT_0) ? "pe_queue[0]" : "pe_queue[1]");
#else
    vQueueAddToRegistry(pe_queue[port_index], "pe_queue[0]");
#endif /* USBPD_PORT_COUNT > 1 */

    /* Tasks definition */
    if (xTaskCreate(USBPD_PE_Task,
#if (USBPD_PORT_COUNT > 1)
                    (port_index == USBPD_PORT_0) ? "PE port0 Task" : "PE port1 Task",
#else
                    "PE port0 Task",
#endif /* USBPD_PORT_COUNT > 1 */
                    OS_PE_STACK_SIZE,
                    (void *) NULL,
                    OS_PE_PRIORITY,
                    &pe_task[port_index]) != pdPASS)
    {
      /* If task creation fails try to increase configTOTAL_HEAP_SIZE */
      USBPD_TRACE_ERROR_PRINTF(port_index, "Task creation failed, try to increase configTOTAL_HEAP_SIZE");
      ret = USBPD_ERROR;
      return ret;
    }

    /* Register the DPM delay function */
    if (USBPD_DPM_User_RegisterDelayCallback(port_index, DPM_Delay) != USBPD_OK)
    {
      ret = USBPD_ERROR;
      return ret;
    }

    /* Variable to be sure that DPM is correctly initialized */
    usbpd_dpm_params[port_index].dpm_flag_initialized = USBPD_FALSE;

    /* Apply the stack settings */
    usbpd_dpm_params[port_index].pe_spec_revision     = usbpd_dpm_settings[port_index].pe_spec_revision;
    usbpd_dpm_params[port_index].pe_power_role        = usbpd_dpm_settings[port_index].pe_default_role;
    usbpd_dpm_params[port_index].pe_flag_swap_ongoing = USBPD_FALSE;
    usbpd_dpm_params[port_index].active_cc            = CCNONE;
    usbpd_dpm_params[port_index].vconn_cc             = CCNONE;
    usbpd_dpm_params[port_index].vconn_status         = USBPD_FALSE;

    if (usbpd_dpm_params[port_index].pe_power_role == USBPD_PORT_PWR_ROLE_SNK)
    {
      usbpd_dpm_params[port_index].skip_snk_Wait_cap_Timer = USBPD_FALSE;
    }

    /* Get RxOredered Set from UCPD configuration */
    USBPD_CAD_HW_IF_GetRxOrderedSet(port_index, &usbpd_dpm_settings[port_index]);

    /* Init CAD */
    usbpd_cad_status_t retcad = USBPD_CAD_Init(port_index, &CAD_cbs,
                                               &usbpd_dpm_settings[port_index], &usbpd_dpm_params[port_index]);
    if (retcad == USBPD_CAD_MALLOCERROR)
    {
      /* Malloc error, try to increase heap size in linker settings */

      USBPD_TRACE_ERROR_PRINTF(port_index, "Malloc error, try to increase heap size in linker settings");

      ret = USBPD_MALLOCERROR;
      return ret;
    }
    if (retcad != USBPD_CAD_OK)
    {
      ret = USBPD_ERROR;
      return ret;
    }

    /* Initialize PE */
    usbpd_status_t retpe = USBPD_PE_Init(port_index, (usbpd_settings_t *)&usbpd_dpm_settings[port_index],
                                         &usbpd_dpm_params[port_index], &dpmCallbacks);
    if (retpe == USBPD_MALLOCERROR)
    {
      /* Malloc error, try to increase heap size in linker settings */
      USBPD_TRACE_ERROR_PRINTF(port_index, "Malloc error, try to increase heap size in linker settings");
      ret = USBPD_MALLOCERROR;
      return ret;
    }
    else if (retpe != USBPD_OK)
    {
      ret = USBPD_ERROR;
      return ret;
    }
    else
    {
      /* Success case */
    }

    /* DPM is correctly initialized */
    usbpd_dpm_params[port_index].dpm_flag_initialized = USBPD_TRUE;

    /* Enable CAD on Port 0 */
    USBPD_CAD_PortEnable(port_index);
  }

  return ret;
}

/**
  * @brief  DeInitialize the core stack (port power role, PWR_IF, CAD and PE Init procedures).
  */
void USBPD_DPM_DeInit(void)
{
  /* DeInitialize DPM User */
  USBPD_DPM_UserDeInit();

  /* DeInitialize the USBPD trace */
  USBPD_TRACE_DeInit();

  for (uint8_t port_index = 0U; port_index < (uint8_t)USBPD_PORT_COUNT; ++port_index)
  {
    /* DeInitialize CAD */
    USBPD_CAD_DeInit(port_index);
    vTaskDelete(cad_task);

    /* DeInitialize PE */
    vTaskDelete(pe_task[port_index]);
    /* Disable CAD on Port 0 */
    USBPD_CAD_PortDisable(port_index);
  }
}
/**
  * @brief This function is called to increment a global variable @ref uwTick
  *        used as application time base, and increment the USBPD timebase.
  * @note This function overwrite the default __WEAK function to add the USBPD TimerCounter handling.
  */
void HAL_IncTick(void)
{
  /* Default HAL IncTick implementation */
  uwTick += (uint32_t)uwTickFreq;

  /* Additional USBPD tick */
  USBPD_TIMERSERVER_TimerCounter();
}

/**
  * @brief  DPM error handler.
  *         This function is called to block application execution in case of an unexpected fatal error
  */
void USBPD_DPM_ErrorHandler(void)
{
  for (;;)
  {
    /* Blocking error */
  }
}
/**
  * @}
  */

/** @defgroup USBPD_DPM_Private_Functions USBPD_DPM Private Functions
  * @{
  */

/**
  * @brief  OS specific delay.
  */
static void DPM_Delay(uint8_t port_num, uint32_t time_ms)
{
  (void)port_num;
  vTaskDelay(pdMS_TO_TICKS(time_ms));
}

/**
  * @brief  WakeUp CAD task.
  */
static void DPM_CADTaskWakeUp(void)
{
  /* WakeUp the CAD task */
  uint32_t event = 0xFFFFU;
  long taskWoken = pdFALSE;


  /* Determine whether we are in thread mode or handler mode. */
  if (__get_IPSR() != 0)
  {
    (void)xQueueSendFromISR(cad_queue, &event, &taskWoken);
    portYIELD_FROM_ISR(taskWoken);
  }
  else
  {
    (void)xQueueSend(cad_queue, &event, 0);
  }
}
/**
  * @brief  Main task for CAD layer.
  * @param  args Not used.
  */
static void USBPD_CAD_Task(void *args)
{
  uint32_t timing;
  uint32_t event;
  TickType_t ticks;

  for (;;)
  {
    timing = USBPD_CAD_Process();

    ticks = 0;
    if (timing == 0xFFFFFFFFU)
    {
      ticks = portMAX_DELAY;
    }
    else if (timing != 0)
    {
      ticks = timing / portTICK_PERIOD_MS;
      if (ticks == 0)
      {
        ticks = 1;
      }
    }
    (void)xQueueReceive(cad_queue, &event, ticks);
  }
}
/**
  * @brief  CallBack reporting events on a specified port from CAD layer.
  * @param  port_num Port number.
  * @param  state    CAD state.
  * @param  cc       CC line for the USBPD communication.
  */
static void DPM_CADCallback(uint8_t port_num, usbpd_cad_event_t state, usbpd_cc_line_t cc)
{
  USBPD_TRACE_Add(USBPD_TRACE_CADEVENT, port_num, (uint8_t)state, NULL, 0U);

  (void)cc; /* Unused */
  switch (state)
  {
    case USBPD_CAD_EVENT_ATTEMC:
    {
      usbpd_dpm_params[port_num].vconn_status = USBPD_TRUE;

      USBPD_DPM_UserCableDetection(port_num, USBPD_CAD_EVENT_ATTEMC);
      DPM_StartPETask(port_num);
      break;
    }
    case USBPD_CAD_EVENT_ATTACHED:
      USBPD_DPM_UserCableDetection(port_num, USBPD_CAD_EVENT_ATTACHED);
      DPM_StartPETask(port_num);
      break;

    case USBPD_CAD_EVENT_DETACHED:
    case USBPD_CAD_EVENT_EMC:
    {
      /* Terminate PE task */
      uint8_t timeout = 0;

      /* WakeUp PE task to let him enter suspend mode */
      DPM_PE_TaskWakeUp(port_num);

      /* Wait PE Let time to PE to complete the ongoing action */
      while (eSuspended != eTaskGetState(pe_task[port_num]))
      {
        vTaskDelay(pdMS_TO_TICKS(1U));

        timeout++;
        if (timeout > 30U)
        {
          /* Suspend the PE task */
          vTaskSuspend(pe_task[port_num]);
          break;
        }
      }

      /* Stop the PE state machine */
      USBPD_PE_StateMachine_Stop(port_num);
      usbpd_dpm_params[port_num].pe_flag_swap_ongoing = USBPD_FALSE;
      usbpd_dpm_params[port_num].pe_pwr_status = USBPD_PWR_STATE_NO;
      USBPD_DPM_UserCableDetection(port_num, state);

      usbpd_dpm_params[port_num].vconn_status = USBPD_FALSE;

      USBPD_DPM_Notification(port_num, USBPD_NOTIFY_USBSTACK_STOP);
      break;
    }
    default:
      /* Nothing to do */
      break;
  }
}
/**
  * @brief  WakeUp PE task.
  * @param  port_num port number.
  */
static void DPM_PE_TaskWakeUp(uint8_t port_num)
{
  uint32_t event = 0xFFFFU;
  long taskWoken = pdFALSE;

  /* Determine whether we are in thread mode or handler mode. */
  if (__get_IPSR() != 0)
  {
    (void)xQueueSendFromISR(pe_queue[port_num], &event, &taskWoken);
    portYIELD_FROM_ISR(taskWoken);
  }
  else
  {
    (void)xQueueSend(pe_queue[port_num], &event, 0);
  }
}

/**
  * @brief  Start the PE task.
  * @param  port_num Port number.
  */
static void DPM_StartPETask(uint8_t port_num)
{
  USBPD_PE_StateMachine_Reset(port_num);
  /* Resume the PE task */
  switch (port_num)
  {
    case USBPD_PORT_0:
    case USBPD_PORT_1:
    {
      vTaskResume(pe_task[port_num]);
      break;
    }
    default:
    {
      USBPD_DPM_ErrorHandler();
      break;
    }
  }
  USBPD_DPM_Notification(port_num, USBPD_NOTIFY_USBSTACK_START);
}

/**
  * @brief  Main task for PE layer.
  * @param  args Contains the port number.
  */
static void USBPD_PE_Task(void *args)
{
  uint8_t port_num = (uint32_t)args;
  uint32_t timing;
  uint32_t event;
  TickType_t ticks;

  for (;;)
  {
    if (usbpd_dpm_params[port_num].pe_flag_is_connected == USBPD_FALSE)
    {
      /* If the port is no more connected, suspend the PE thread */
      vTaskSuspend(xTaskGetCurrentTaskHandle());
    }

#if defined(USBPD_SNK)
    timing = USBPD_PE_StateMachine_SNK(port_num);
#elif defined(USBPD_SRC)
    timing = USBPD_PE_StateMachine_SRC(port_num);
#else
    timing = USBPD_PE_StateMachine_DRP(port_num);
#endif /* USBPD_SNK */

    ticks = 0;
    if (timing == 0xFFFFFFFFU)
    {
      ticks = portMAX_DELAY;
    }
    else if (timing != 0)
    {
      ticks = timing / portTICK_PERIOD_MS;
      if (ticks == 0)
      {
        ticks = 1;
      }
    }
    (void)xQueueReceive(pe_queue[port_num], &event, ticks);
  }
}
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
