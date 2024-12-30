cmake_minimum_required(VERSION 3.16)

project(deepin-shortcut-viewer VERSION 5.0.5)

# 提高CMake最低版本要求，因为Qt6需要更高版本的CMake
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# 设置安装路径前缀
if (CMAKE_INSTALL_PREFIX_INITIALIZED_FALSY)
    set(CMAKE_INSTALL_PREFIX /usr)
endif()

# 查找组件
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Network)
find_package(Dtk6 COMPONENTS Core Widget  REQUIRED)

# 添加定义
add_definitions(-DQT_MESSAGELOGCONTEXT)

# 源文件
set(SOURCES
    main.cpp
    view/mainwidget.cpp
    commandlinemanager.cpp
    singleapplication.cpp
    view/shortcutitem.cpp
    view/shortcutview.cpp
)

# 头文件
set(HEADERS
    view/mainwidget.h
    commandlinemanager.h
    singleapplication.h
    view/shortcutitem.h
    view/shortcutview.h
)

# 创建可执行文件
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# 链接库
target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Network
    Dtk6::Core
    Dtk6::Widget
)

# 包含目录
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# 安装目标
install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX}/bin) 
