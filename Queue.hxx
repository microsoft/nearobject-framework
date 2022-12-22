/**
 * @file Queue.hxx
 * @brief This file contains the queue entry points and callbacks.
 *
 * @copyright Copyright (c) 2022
 */

EXTERN_C_START

//
// This is the context that can be placed per queue
// and would contain per queue information.
//
typedef struct _QUEUE_CONTEXT
{
    ULONG PrivateDeviceData;

} QUEUE_CONTEXT, *PQUEUE_CONTEXT;

WDF_DECLARE_CONTEXT_TYPE_WITH_NAME(QUEUE_CONTEXT, QueueGetContext)

NTSTATUS
UwbSimulatorQueueInitialize(WDFDEVICE device);

//
// Events from the IoQueue object
//
EVT_WDF_IO_QUEUE_IO_DEVICE_CONTROL UwbSimulatorEvtIoDeviceControl;
EVT_WDF_IO_QUEUE_IO_STOP UwbSimulatorEvtIoStop;

EXTERN_C_END
