set (ATMEL_PATH atmel)
set (HAL_PATH ${ATMEL_PATH}/hal)
set (HRI_PATH ${ATMEL_PATH}/hri)
set (HPL_PATH ${ATMEL_PATH}/hpl)
set (CMSIS_PATH ${ATMEL_PATH}/CMSIS/Core/)

target_include_directories (${PROJECT_NAME}
PUBLIC
    ${ATMEL_PATH}/
    ${ATMEL_PATH}/config
    ${ATMEL_PATH}/samd21d/include
    ${CMSIS_PATH}/Include
    ${HAL_PATH}/include
    ${HAL_PATH}/utils/include
    ${HRI_PATH}/
    ${HPL_PATH}/core
    ${HPL_PATH}/gclk
    ${HPL_PATH}/pm
    ${HPL_PATH}/port
    ${HPL_PATH}/tc
    ${ATMEL_PATH}/persistent_storage
    ${ATMEL_PATH}/usb
    ${ATMEL_PATH}/usb/device
    ${ATMEL_PATH}/usb/class/cdc/
    ${ATMEL_PATH}/usb/class/cdc/device
    ${ATMEL_PATH}/examples
    ) 

target_sources(${PROJECT_NAME} 
PUBLIC
    ${ATMEL_PATH}/samd21d/gcc/system_samd21.c
    ${ATMEL_PATH}/samd21d/gcc/gcc/startup_samd21.c
    ${ATMEL_PATH}/atmel_start.c
    ${ATMEL_PATH}/driver_init.c
    ${ATMEL_PATH}/persistent_storage_start.c
    ${ATMEL_PATH}/usb_start.c
    ${ATMEL_PATH}/usb/usb_protocol.c
    ${ATMEL_PATH}/usb/device/usbdc.c
    ${ATMEL_PATH}/usb/class/cdc/device/cdcdf_acm.c
    ${HAL_PATH}/src/hal_atomic.c
    ${HAL_PATH}/src/hal_delay.c
    ${HAL_PATH}/src/hal_flash.c
    ${HAL_PATH}/src/hal_gpio.c
    ${HAL_PATH}/src/hal_init.c
    ${HAL_PATH}/src/hal_io.c
    ${HAL_PATH}/src/hal_sleep.c
    ${HAL_PATH}/src/hal_timer.c
    ${HAL_PATH}/src/hal_usart_sync.c
    ${HAL_PATH}/src/hal_usb_device.c
    ${HAL_PATH}/utils/src/utils_assert.c
    ${HAL_PATH}/utils/src/utils_event.c
    ${HAL_PATH}/utils/src/utils_list.c
    ${HAL_PATH}/utils/src/utils_syscalls.c
    ${HPL_PATH}/core/hpl_init.c
    ${HPL_PATH}/dmac/hpl_dmac.c
    ${HPL_PATH}/gclk/hpl_gclk.c
    ${HPL_PATH}/nvmctrl/hpl_nvmctrl.c
    ${HPL_PATH}/pm/hpl_pm.c
    ${HPL_PATH}/sercom/hpl_sercom.c
    ${HPL_PATH}/sysctrl/hpl_sysctrl.c
    ${HPL_PATH}/tc/tc_lite.c
    ${HPL_PATH}/usb/hpl_usb.c
    ${ATMEL_PATH}/persistent_storage/nv_internal_flash_ultra.c
    ${ATMEL_PATH}/persistent_storage/nv_storage.c
    ${ATMEL_PATH}/examples/driver_examples.c
    )
