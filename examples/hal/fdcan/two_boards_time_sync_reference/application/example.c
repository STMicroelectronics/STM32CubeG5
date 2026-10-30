/**
  ******************************************************************************
  * file           : example.c
  * brief          : The controller's application handling SYNC/follow-Up time
  *                  synchronization over FDCAN with HAL API.
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

#include <string.h> /* importing memset function */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Timer resolution: one timer tick equals 125 ns. */
#define TIMER_TICK_NS     (125ULL)
/* One second expressed in nanoseconds. */
#define ONE_SECOND_NS     (1000000000ULL)
/* Standard ID for SYNC frames. */
#define SYNC_FRAME_ID (0x111U)

/* Private macro -------------------------------------------------------------*/
#define GET_32BIT_FROM_BYTES(ARRAY) \
  ((((uint32_t)(ARRAY)[3U]) << 24U) | (((uint32_t)(ARRAY)[2U]) << 16U) \
   | (((uint32_t)(ARRAY)[1U]) << 8U) | (uint32_t)(ARRAY)[0U])


/* Private variables ---------------------------------------------------------*/
hal_fdcan_handle_t *pFDCAN; /* Pointer referencing the FDCAN handle from the generated code */
hal_tim_handle_t *pTIM;     /* Pointer referencing the TIM handle from the generated code */

/* Interrupt-driven state flags and software time base. */
volatile uint8_t SyncCaptureCompleted = 0U;
volatile uint8_t TransmitCompleted = 0U;
volatile uint32_t UpdateCallbackCount = 0U;

/* Captured Transmit EOF timestamp ticks. */
uint8_t TransmitStamp[4U] = {0U};

/* Private functions prototype -----------------------------------------------*/
static void InputCaptureCallback(hal_tim_handle_t *, hal_tim_channel_t);
static void UpdateCallback(hal_tim_handle_t *);
static app_status_t StartTimestampCapture(void);
static app_status_t StopTimestampCapture(void);
static void ResetTimestampCaptureState(void);
static void FillTimeSyncHeader(hal_fdcan_tx_header_t *header);
static void BuildSyncFrame(uint8_t *payload, uint32_t sync_seconds);
static void BuildFollowUpFrame(uint8_t *payload, uint32_t follow_up_time_ns);
static app_status_t TransmitComplete(const hal_fdcan_tx_header_t *header,
                                     uint8_t *payload,
                                     uint8_t wait_for_sync_capture);
static uint64_t TimToNanosec(uint64_t timer_value);
static uint64_t GetCapturedTime(const uint8_t *timestamp_capture);
static uint64_t GetCurrentTime(void);


app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;
  hal_status_t hal_status;

  /** ########## Step 1 ##########
    * The init of the FDCAN and TIM instances is triggered by the applicative code.
    */
  pFDCAN = mx_example_fdcan_init();
  if (pFDCAN == NULL)
  {
    goto _app_init_exit;
  }

  /* Enable Transmitter Delay Compensation for reliable FD data phase at 2 Mbps. */
  hal_status = HAL_FDCAN_EnableTxDelayCompensation(pFDCAN);
  if (hal_status != HAL_OK)
  {
    goto _app_init_exit;
  }

  pTIM = mx_example_tim_init();
  if (pTIM == NULL)
  {
    goto _app_init_exit;
  }

  hal_status = HAL_TIM_RegisterInputCaptureCallback(pTIM,
                                                    InputCaptureCallback);
  if (hal_status != HAL_OK)
  {
    goto _app_init_exit;
  }

  hal_status = HAL_TIM_RegisterUpdateCallback(pTIM, UpdateCallback);
  if (hal_status != HAL_OK)
  {
    goto _app_init_exit;
  }

  hal_status = HAL_TIM_Start_IT(pTIM);
  if (hal_status != HAL_OK)
  {
    goto _app_init_exit;
  }

  hal_status = HAL_FDCAN_Start(pFDCAN);
  if (hal_status != HAL_OK)
  {
    goto _app_init_exit;
  }

  PRINTF("[INFO] Reference: initialization COMPLETED. Starting time synchronization.\n");
  return_status = EXEC_STATUS_INIT_OK;

_app_init_exit:
  return return_status;
}


app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR; /* Function return status. */
  hal_fdcan_tx_header_t transmit_header; /* Common header for SYNC/follow-Up frames. */
  uint8_t payload[8U] = {0U};            /* Frame payload buffer. */
  uint32_t sync_seconds;                 /* SYNC field. */
  uint32_t transmit_end_of_frame_ticks;  /* Captured Tx EOF timer ticks. */
  uint32_t follow_up_time_ns;            /* Follow-Up field. */

  /** ########## Step 2 ##########
    * Start timestamp capture for the transmit channel.
    */
  if (StartTimestampCapture() != EXEC_STATUS_OK)
  {
    goto _app_process_exit;
  }

  /** ########## Step 3 ##########
    * Build and transmit SYNC frame.
    */
  /* Build SYNC Header */
  FillTimeSyncHeader(&transmit_header);

  /* Build the current controller absolute time. */
  sync_seconds = (uint32_t)(GetCurrentTime() / ONE_SECOND_NS);

  /* Build SYNC payload from current controller absolute time. */
  BuildSyncFrame(payload, sync_seconds);

  /* Transmit the SYNC payload. */
  if (TransmitComplete(&transmit_header, payload, 1U) != EXEC_STATUS_OK)
  {
    goto _app_process_exit;
  }

  /* Wait until the SYNC frame transmission and its EOF capture are complete */
  while ((TransmitCompleted == 0U) || (SyncCaptureCompleted == 0U))
  {
    __WFI();
  }

  /* Read captured EOF tick value for the Transmit channel. */
  transmit_end_of_frame_ticks = GET_32BIT_FROM_BYTES(TransmitStamp);

  if (transmit_end_of_frame_ticks == 0U)
  {
    /* Transmit capture must be present after SYNC transmission. */
    goto _app_process_exit;
  }

  /** ########## Step 4 ##########
    * End the capture window and build Follow-Up timestamp payload.
    */
  if (StopTimestampCapture() != EXEC_STATUS_OK)
  {
    goto _app_process_exit;
  }

  /* Calculate Follow-Up time relative to SYNC seconds. */
  follow_up_time_ns = (uint32_t)(
                        GetCapturedTime(TransmitStamp) - ((uint64_t)sync_seconds * ONE_SECOND_NS));

  /* Build Follow-Up nanoseconds field relative to SYNC seconds. */
  BuildFollowUpFrame(payload, follow_up_time_ns);

  /* Transmit the Follow-Up payload. */
  if (TransmitComplete(&transmit_header, payload, 0U) != EXEC_STATUS_OK)
  {
    goto _app_process_exit;
  }

  PRINTF("[INFO] Reference: SYNC sent (seconds = %" PRIu32 "), "
         "FOLLOW_UP sent (nanoseconds = %" PRIu32 "). Reference time = %" PRIu64 " ns.\n",
         sync_seconds, follow_up_time_ns, GetCurrentTime());

  /* Clear local state before starting the next synchronization sequence. */
  ResetTimestampCaptureState();

  return_status = EXEC_STATUS_OK;

_app_process_exit:
  return return_status;
}


app_status_t app_deinit(void)
{
  /* Stop timer interrupt-driven operation. */
  HAL_TIM_Stop_IT(pTIM);
  /* Release timer resources. */
  mx_example_tim_deinit();

  /* Stop FDCAN peripheral activity. */
  HAL_FDCAN_Stop(pFDCAN);
  /* Release FDCAN resources. */
  mx_example_fdcan_deinit();

  /* Signal clean shutdown. */
  return EXEC_STATUS_OK;
}


static void InputCaptureCallback(hal_tim_handle_t *htim,
                                 hal_tim_channel_t channel)
{
  (void)htim;

  if (channel == HAL_TIM_CHANNEL)
  {
    /* Mark SYNC transmit EOF timestamp capture completion. */
    SyncCaptureCompleted = 1U;
  }
}


static void UpdateCallback(hal_tim_handle_t *htim)
{
  (void)htim;
  /* Count timer overflows (seconds accumulation). */
  UpdateCallbackCount++;
}


static app_status_t StartTimestampCapture(void)
{
  hal_status_t hal_status;

  hal_status = HAL_TIM_IC_StartChannel_DMA(pTIM,
                                           HAL_TIM_CHANNEL,
                                           TransmitStamp,
                                           sizeof(uint32_t));
  if (hal_status != HAL_OK)
  {
    /* Start DMA capture on Transmit EOF event channel. */
    return EXEC_STATUS_ERROR;
  }

  return EXEC_STATUS_OK;
}


static app_status_t StopTimestampCapture(void)
{
  hal_status_t hal_status;

  hal_status = HAL_TIM_IC_StopChannel_DMA(pTIM, HAL_TIM_CHANNEL);
  if (hal_status != HAL_OK)
  {
    /* Stop capture DMA on channel 2. */
    return EXEC_STATUS_ERROR;
  }

  return EXEC_STATUS_OK;
}

static void ResetTimestampCaptureState(void)
{
  /* Reset state for the next synchronization sequence. */
  SyncCaptureCompleted = 0U;
  TransmitCompleted = 0U;
  memset(TransmitStamp, 0x00, sizeof(TransmitStamp));
}


static void FillTimeSyncHeader(hal_fdcan_tx_header_t *header)
{
  /* Configure fixed header fields shared by SYNC and Follow-Up frames. */
  header->b.identifier = SYNC_FRAME_ID;
  header->b.identifier_type = HAL_FDCAN_ID_STANDARD;
  header->b.frame_type = HAL_FDCAN_FRAME_DATA;
  header->b.error_state_indicator = HAL_FDCAN_ERROR_STATE_IND_ACTIVE;
  header->b.data_length = HAL_FDCAN_DATA_LEN_CAN_FDCAN_8_BYTE;
  header->b.bit_rate_switch = HAL_FDCAN_BIT_RATE_SWITCH_ON;
  header->b.frame_format = HAL_FDCAN_HEADER_FRAME_FORMAT_FD_CAN;
  header->b.event_fifo_control = HAL_FDCAN_TX_EVENTS_FIFO_STORE;
  header->b.message_marker = 0x52U;
}


static void BuildSyncFrame(uint8_t *payload, uint32_t sync_seconds)
{
  payload[0] = 0x10U; /* Byte 0: SYNC message type. */
  payload[1] = 0x00U; /* Byte 1: reserved field, fixed to 0. */
  payload[2] = 0x00U; /* Byte 2: reserved field, fixed to 0. */
  payload[3] = 0x00U; /* Byte 3: reserved field, fixed to 0. */
  /* Bytes 4 to 7: SYNC seconds field in little-endian format. */
  payload[4] = (uint8_t)(sync_seconds & 0x000000FFU);
  payload[5] = (uint8_t)((sync_seconds & 0x0000FF00U) >> 8U);
  payload[6] = (uint8_t)((sync_seconds & 0x00FF0000U) >> 16U);
  payload[7] = (uint8_t)((sync_seconds & 0xFF000000U) >> 24U);
}


static void BuildFollowUpFrame(uint8_t *payload, uint32_t follow_up_time_ns)
{
  uint32_t follow_up_time_field;

  payload[0] = 0x18U; /* Byte 0: Follow-Up message type. */
  payload[1] = 0x00U; /* Byte 1: reserved field, fixed to 0. */
  payload[2] = 0x00U; /* Byte 2: reserved field, fixed to 0. */
  payload[3] = 0x00U; /* Byte 3: reserved field, fixed to 0. */
  /* Bytes 4 to 7: Follow-Up nanoseconds field in little-endian format. */
  follow_up_time_field = follow_up_time_ns & 0x3FFFFFFFU;
  payload[4] = (uint8_t)(follow_up_time_field & 0x000000FFU);
  payload[5] = (uint8_t)((follow_up_time_field & 0x0000FF00U) >> 8U);
  payload[6] = (uint8_t)((follow_up_time_field & 0x00FF0000U) >> 16U);
  payload[7] = (uint8_t)((follow_up_time_field & 0xFF000000U) >> 24U);
}


/* Queue one frame for transmission and wait until Tx buffer is not pending. */
static app_status_t TransmitComplete(const hal_fdcan_tx_header_t *header,
                                     uint8_t *payload,
                                     uint8_t wait_for_sync_capture)
{
  hal_status_t hal_status;
  uint32_t tx_buffer_idx;

  TransmitCompleted = 0U;
  if (wait_for_sync_capture == 1U)
  {
    SyncCaptureCompleted = 0U;
  }

  /* Add a message to the Tx FIFO/Queue and activate the transmission request*/
  hal_status = HAL_FDCAN_ReqTransmitMsgFromFIFOQ(pFDCAN,
                                                 header,
                                                 payload);
  if (hal_status != HAL_OK)
  {
    return EXEC_STATUS_ERROR;
  }

  tx_buffer_idx = HAL_FDCAN_GetLatestTxFifoQRequestBuffer(pFDCAN);

  /* Wait until the requested frame is no longer pending in the Tx buffer. */
  while (HAL_FDCAN_GetTxBufferMessageStatus(pFDCAN,
                                            tx_buffer_idx)
         != HAL_FDCAN_TX_BUFFER_NOT_PENDING)
  {
    __WFI();
  }

  TransmitCompleted = 1U;

  return EXEC_STATUS_OK;
}


static uint64_t TimToNanosec(uint64_t timer_value)
{
  return timer_value * TIMER_TICK_NS;
}


static uint64_t GetCapturedTime(const uint8_t *timestamp_capture)
{
  /* Build absolute time from captured timer ticks and overflow seconds. */
  return TimToNanosec(GET_32BIT_FROM_BYTES(timestamp_capture))
         + ((uint64_t)UpdateCallbackCount * ONE_SECOND_NS);
}


static uint64_t GetCurrentTime(void)
{
  /* Build absolute time from timer counter and overflow seconds. */
  return TimToNanosec((uint64_t)HAL_TIM_GetCounter(pTIM))
         + ((uint64_t)UpdateCallbackCount * ONE_SECOND_NS);
}
