set (ATMEL_PATH ${CMAKE_SOURCE_DIR}/libs/atmel)
set (HAL_PATH ${ATMEL_PATH}/hal)
set (HRI_PATH ${ATMEL_PATH}/hri)
set (HPL_PATH ${ATMEL_PATH}/hpl)
set (CMSIS_PATH ${ATMEL_PATH}/CMSIS/Core/)


target_sources(${PROJECT_NAME}
    PRIVATE
        main.cpp
        stepper.cpp
    PUBLIC
)

target_include_directories (${PROJECT_NAME}
PUBLIC
    ./
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
    ${ATMEL_PATH}/examples
)
