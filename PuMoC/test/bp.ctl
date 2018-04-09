/* Loc 2677, Time: 70.19 s, Mem: 9.93 Mb  */


(!E[! sdv_WDF_DRIVER_CONFIG_INIT_8:L0 U sdv_WdfDriverCreate_8:L0 ]) &&
 (!E[!sdv_WdfDriverCreate_8:L0 U kmdf1394_EvtDeviceAdd_3:L0]) &&
 (!E[! sdv_WDF_PNPPOWER_EVENT_CALLBACKS_INIT_8:L0 U sdv_WdfDeviceInitSetPnpPowerEventCallbacks_8:L0])&& 
 (!E[!sdv_WDF_OBJECT_ATTRIBUTES_INIT_8:L0 U sdv_WdfDeviceCreate_8:L0  ])&& 
 AG(kmdf1394_EvtDeviceAdd_3:L0 -> EF (sdv_WdfDeviceCreate_8:L0 &&#.|.|sdv_main_8:c2bpreturnpt_13_0|.*# ))
