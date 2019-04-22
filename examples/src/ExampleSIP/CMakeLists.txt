cmake_minimum_required(VERSION 2.8.3)
project(promobot_example_app_sip)

find_package(Qt5 5.6.2 REQUIRED
    Core
    Widgets
)
find_package(catkin REQUIRED COMPONENTS
    libqtpromobot
    libqtapplicationbase
)

catkin_package(
    LIBRARIES
        promobot_example_app_sip
    CATKIN_DEPENDS
        libqtpromobot
        libqtapplicationbase
    DEPENDS
        Qt5Core
        Qt5Widgets
)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/../plugin/${PROJECT_NAME})

###########
## Build ##
###########

add_library(${PROJECT_NAME} MODULE
    include/examplesipobject.h
    src/examplesipobject.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
    include
    include/${PROJECT_NAME}
    ${catkin_INCLUDE_DIRS}
    ${Qt5Core_INCLUDE_DIRS}
    ${Qt5Widgets_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME}
    ${catkin_LIBRARIES}
    Qt5::Core
    Qt5::Widgets
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
    ${Qt5Core_DEFINITIONS}
    ${Qt5Widgets_DEFINITIONS}
    -DQT_PLUGIN
    -DQT_SHARED
    -DBUILD_HACK:BOOL=ON
)

set_property(TARGET ${PROJECT_NAME} PROPERTY AUTOGEN_TARGET_DEPENDS description.json)

#############
## Install ##
#############

install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}/../plugin/${PROJECT_NAME}
)

install(FILES description.json icon.ico
    DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}/../plugin/${PROJECT_NAME}
)

#############
## Testing ##
#############
