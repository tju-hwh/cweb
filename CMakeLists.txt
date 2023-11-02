cmake_minimum_required(VERSION 3.1)

project(ACL_HELLO_WORLD)

# Compile options
add_compile_options(-std=c++11)

set(CMAKE_CXX_STANDARD 11)
set(SOURCE_FILES main.cpp handler.h controller.h controller.cpp infer.h infer.cpp util.h util.cpp)

set(INC_PATH $ENV{DDK_PATH})
set(LIB_PATH $ENV{NPU_HOST_LIB})

# Header path
include_directories(
    ${INC_PATH}/include/
)

# add host lib path
link_directories(
    ${LIB_PATH}
)

# 查找 oatpp 依赖
find_package(oatpp REQUIRED)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# 将目标文件与库文件进行链接
target_link_libraries(${PROJECT_NAME} oatpp::oatpp)

if (target STREQUAL "Simulator_Function")
    message(STATUS   "1")
    target_link_libraries(main funcsim)
else ()
    if (${CMAKE_HOST_SYSTEM_NAME} MATCHES "Windows")
        target_link_libraries(main
            libascendcl)
    else ()
        message(STATUS   "3")
        target_link_libraries(ACL_HELLO_WORLD
            ascendcl stdc++)
    endif ()
endif ()

install(TARGETS ACL_HELLO_WORLD DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})