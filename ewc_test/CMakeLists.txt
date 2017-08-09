set(ROOT_DIR ${CMAKE_CURRENT_LIST_DIR}/..)
set(EWCTEST_INCL_DIRS)
set(EWCTEST_REQ_LIBS)

# EWC webcam library
list(APPEND EWCTEST_INCL_DIRS ${ROOT_DIR}/thirdparty/EWC)

# OpenCV - empty on Windows
IF(MSVC) # not necessary for OpenCV > 2.8 on other build systems
    list(APPEND EWCTEST_INCL_DIRS ${OpenCV_INCLUDE_DIRS}) 
ENDIF()
list(APPEND EWCTEST_REQ_LIBS ${OpenCV_LIBS})
   
# Source files that are needed for the EWCTest tool
file(GLOB EWCTEST_SRC
    "${CMAKE_CURRENT_LIST_DIR}/*.h"
    "${CMAKE_CURRENT_LIST_DIR}/*.cpp"
)

# EWCTest
add_executable(EWCTest ${EWCTEST_SRC})
target_include_directories(EWCTest PUBLIC ${EWCTEST_INCL_DIRS})
target_link_libraries(EWCTest ${EWCTEST_REQ_LIBS})

# Install    
IF(${CMAKE_SYSTEM_NAME} MATCHES "Windows")   
    install(TARGETS EWCTest
        RUNTIME DESTINATION ${ROOT_DIR}/${ARCH_LABEL}/bin
        LIBRARY DESTINATION ${ROOT_DIR}/${ARCH_LABEL}/lib
        ARCHIVE DESTINATION ${ROOT_DIR}/${ARCH_LABEL}/lib)
ELSE() #Linux/Darwin
ENDIF()
