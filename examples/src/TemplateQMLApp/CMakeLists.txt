cmake_minimum_required(VERSION 2.8.3)
project(promobot_app_template_qml)

find_package( Qt5 5.6.2 REQUIRED Core Widgets Qml Quick QuickWidgets )
find_package( catkin REQUIRED COMPONENTS libqtpromobot libqtapplicationbase )

catkin_package(
    INCLUDE_DIRS include
    LIBRARIES promobot_app_template
    CATKIN_DEPENDS libqtpromobot libqtapplicationbase
    DEPENDS Qt5::Core Qt5::Widgets Qt5::Qml Qt5::Quick Qt5::QuickWidgets
)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/../plugin/${PROJECT_NAME})

###########
## Build ##
###########

add_library( ${PROJECT_NAME} MODULE
    include/templateobject.h
    src/templateobject.cpp

    res/resources.qrc
)

target_include_directories( ${PROJECT_NAME} PRIVATE
    include
    include/${PROJECT_NAME}
    ${catkin_INCLUDE_DIRS}
    ${Qt5Core_INCLUDE_DIRS}
    ${Qt5Widgets_INCLUDE_DIRS}
    ${Qt5Qml_INCLUDE_DIRS}
    ${Qt5Quick_INCLUDE_DIRS}
    ${Qt5QuickWidgets_INCLUDE_DIRS}
)

target_link_libraries( ${PROJECT_NAME}
    ${catkin_LIBRARIES}
    Qt5::Core
    Qt5::Widgets
    Qt5::Qml
    Qt5::Quick
    Qt5::QuickWidgets
)

target_compile_definitions( ${PROJECT_NAME} PRIVATE
    ${Qt5Core_DEFINITIONS}
    ${Qt5Widgets_DEFINITIONS}
    -DQT_PLUGIN
    -DQT_SHARED
    -DBUILD_HACK:BOOL=ON
)

set_property(TARGET ${PROJECT_NAME} PROPERTY AUTOGEN_TARGET_DEPENDS description.json)

generate_qm_files( ${PROJECT_NAME} ${PROJECT_SOURCE_DIR} qm_files )
add_custom_target( ${PROJECT_NAME}_update_ts ALL DEPENDS ${qm_files})

#############
## Install ##
#############

install( TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}/../plugin/${PROJECT_NAME}
)

install( FILES ${qm_files}
    DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}/lang
)

install( FILES description.json icon.ico
    DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}/../plugin/${PROJECT_NAME}
)

#############
## Testing ##
#############
