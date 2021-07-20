# The following lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.5)
cmake_policy(SET CMP0076 NEW)

set (My_Project_Title "dispstepper")
set (My_Project_Binary "dispstepper.elf")
set (My_Project_Description "")
set (My_Project_Author "M. Lehmann")
set (My_Project_Revised_Date "2021-XX-XX")
set (My_Project_Version_Major 0)
set (My_Project_Version_Minor 4)
set (My_Project_Version_Patch 0)

set (My_Project_Version "${My_Project_Version_Major}.${My_Project_Version_Minor}.${My_Project_Version_Patch}")

set (My_Project_Copyright_Date "2021-2021")
set (My_Project_Copyright "Copyright © ${My_Project_Copyright_Date} ${My_Project_Author}.")

string(TOUPPER ${My_Project_Title} My_Project_Title_Caps  )
string(REGEX REPLACE " " "_" My_Project_Title_Caps ${My_Project_Title_Caps} )

set(CMAKE_CXX_STANDARD 17)
set(LINKER_SCRIPT "${CMAKE_SOURCE_DIR}/libs/atmel/samd21d/gcc/gcc/samd21e17d_flash.ld")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -T ${LINKER_SCRIPT}")
set_target_properties(${TARGET_NAME} PROPERTIES LINK_DEPENDS ${LINKER_SCRIPT})

project(${My_Project_Title})

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    include(CTest)
endif()

include_directories("include/")
include(samd21)
include(gdb-helper)

file(WRITE null.cpp "")
add_executable(${PROJECT_NAME} null.cpp)

add_subdirectory(src)
add_subdirectory(libs)

enable_testing()

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BUILD_TESTING)
    add_subdirectory(test)
endif() 

generate_run_gdb_openocd(${PROJECT_NAME})
generate_vscode_launch_openocd(${PROJECT_NAME})

# Print section sizes of target
print_section_sizes(${PROJECT_NAME})

# Create output in hex and binary format
create_bin_output(${PROJECT_NAME})
create_hex_output(${PROJECT_NAME})

