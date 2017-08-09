set(ROOT_DIR ${CMAKE_CURRENT_LIST_DIR}/..)
set(OPENCVTEST_INCL_DIRS)
set(OPENCVTEST_REQ_LIBS)

# platform independent include paths
list(APPEND OPENCVTEST_INCL_DIRS ${ROOT_DIR}/driver/include/)

# platform independent libraries
list(APPEND OPENCVTEST_REQ_LIBS PS4EyeCam)

# Requires libusb
find_package(USB1 REQUIRED)
list(APPEND OPENCVTEST_INCL_DIRS ${LIBUSB_INCLUDE_DIR})
list(APPEND OPENCVTEST_REQ_LIBS ${LIBUSB_LIBRARIES})

# OpenCV - empty on Windows
IF(MSVC) # not necessary for OpenCV > 2.8 on other build systems
    list(APPEND OPENCVTEST_INCL_DIRS ${OpenCV_INCLUDE_DIRS}) 
ENDIF()
list(APPEND OPENCVTEST_REQ_LIBS ${OpenCV_LIBS})
   
# Source files that are needed for the OPENCVTest tool
file(GLOB OPENCVTEST_SRC
    "${CMAKE_CURRENT_LIST_DIR}/*.h"
    "${CMAKE_CURRENT_LIST_DIR}/*.cpp"
)

# OPENCVTest
add_executable(OPENCVTest ${OPENCVTEST_SRC})
target_include_directories(OPENCVTest PUBLIC ${OPENCVTEST_INCL_DIRS})
target_link_libraries(OPENCVTest ${OPENCVTEST_REQ_LIBS})

# Post build dependencies (assets and external DLLs)
add_custom_command(TARGET OPENCVTest POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_LIST_DIR}/resources"
        $<TARGET_FILE_DIR:OPENCVTest>/resources)

# Install    
IF(${CMAKE_SYSTEM_NAME} MATCHES "Windows")   
    install(TARGETS OPENCVTest
        RUNTIME DESTINATION ${ROOT_DIR}/${ARCH_LABEL}/bin
        LIBRARY DESTINATION ${ROOT_DIR}/${ARCH_LABEL}/lib
        ARCHIVE DESTINATION ${ROOT_DIR}/${ARCH_LABEL}/lib)
    install(DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/resources/ 
            DESTINATION ${ROOT_DIR}/${ARCH_LABEL}/bin/resources
            FILES_MATCHING PATTERN "*.bin")         
ELSE() #Linux/Darwin
ENDIF()
