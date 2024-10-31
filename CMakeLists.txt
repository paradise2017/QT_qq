cmake_minimum_required (VERSION 3.0)

# 项目名称 最终生成的sln文件
project ("QQ_Server")

# 可执行程序的名字
set(EXE_NAME QQ_Server_EXE)

# QT 必须设置选项
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

# 此处应包含项目所有的.h文件，包含第三方库的头文件
# CMAKE_CURRENT_SOURCE_DIR 当前CMakeLists.txt 目录
set(HEAD_PATH 
${CMAKE_CURRENT_SOURCE_DIR}
)

# 提供位置定位.cmake文件  QTDIR 系统的环境变量
set(Qt5_DIR "C:/Qt/Qt5.14.2/5.14.2/msvc2017_64")

# QT的包
find_package(Qt5 COMPONENTS Gui Core Widgets SerialPort Network Sql REQUIRED)

# 头文件搜索路径
include_directories(
    ${HEAD_PATH}
)

# Collect all source files
file(GLOB SRC ${HEAD_PATH}/*.cpp)
file(GLOB HEADER ${HEAD_PATH}/*.h)

# 链接路径
link_directories(
${Qt5_DIR}/lib
)
# 添加可执行文件
add_executable(${EXE_NAME} ${SRC} ${HEADER} QtQQ_Server.qrc QtQQ_Server.ui)
# 链接库
target_link_libraries(${EXE_NAME} 
    Qt5::Gui 
    Qt5::Core 
    Qt5::Widgets 
    Qt5::SerialPort 
    Qt5::Network
    Qt5::Sql
)