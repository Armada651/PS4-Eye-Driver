cmake_minimum_required(VERSION 3.0)

set(ROOT_DIR ${CMAKE_CURRENT_LIST_DIR}/../..)
set(PS4EYECAM_INCL_DIRS)

#Requires libusb
find_package(USB1 REQUIRED)

list(APPEND PS4EYECAM_INCL_DIRS
    ${ROOT_DIR}/driver/include/
    ${LIBUSB_INCLUDE_DIR})
    
# Source files that are needed for the static library
file(GLOB PS4EYECAM_LIBRARY_SRC
    "${CMAKE_CURRENT_LIST_DIR}/*.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/*.h"
)

# Static library
add_library(PS4EyeCam STATIC ${PS4EYECAM_LIBRARY_SRC})
target_include_directories(PS4EyeCam PUBLIC ${PS4EYECAM_INCL_DIRS} ${CMAKE_CURRENT_BINARY_DIR})
set_target_properties(PS4EyeCam PROPERTIES
    COMPILE_FLAGS -DBUILDING_STATIC_LIBRARY)