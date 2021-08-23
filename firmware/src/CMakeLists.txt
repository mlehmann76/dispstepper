set (ATMEL_PATH ${CMAKE_SOURCE_DIR}/libs/atmel)
set (HAL_PATH ${ATMEL_PATH}/hal)
set (HRI_PATH ${ATMEL_PATH}/hri)
set (HPL_PATH ${ATMEL_PATH}/hpl)
set (CMSIS_PATH ${ATMEL_PATH}/CMSIS/Core/)


target_sources(${PROJECT_NAME}
    PRIVATE
        main.cpp
        stepper.cpp
        board.cpp
        control.cpp
        config.cpp
        usb_cdc.cpp
        cmdparse.cpp
    PUBLIC
)

target_include_directories (${PROJECT_NAME}
PUBLIC
    ./
    ${CMAKE_BINARY_DIR}/
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
    ${CMAKE_SOURCE_DIR}/libs/simpleparser/include
)


find_package(Git)
if(GIT_FOUND AND EXISTS "${CMAKE_SOURCE_DIR}/../.git")
    INCLUDE(GetGitRevisionDescription)
    EXECUTE_PROCESS(COMMAND ${GIT_EXECUTABLE} describe --tags HEAD OUTPUT_VARIABLE SHORT_SHA OUTPUT_STRIP_TRAILING_WHITESPACE)

    SET(REVISION ${SHORT_SHA} CACHE STRING "git describe tags" FORCE)

    # only use the plugin to tie the configure state to the tag to force rebuilds
    # of files that depend on version.h
    include(GetGitRevisionDescription)
    #get_git_head_revision(REFSPEC COMMITHASH)
else()
    message(WARNING "Git not found, cannot set version info")

    SET(REVISION "unknown")
endif()

# generate version.h
include_directories(${CMAKE_BINARY_DIR})
configure_file("${CMAKE_SOURCE_DIR}/include/version.h.in" "${CMAKE_BINARY_DIR}/version.h" @ONLY)
    