# toplevel CMakeLists.txt for a catkin workspace
# catkin/cmake/toplevel.cmake

cmake_minimum_required(VERSION 3.7.2)
project(Promobot_Examples)

if(POLICY CMP0054)
  cmake_policy(SET CMP0054 OLD)
endif()

set(CATKIN_TOPLEVEL TRUE)

# search for catkin within the workspace
set(_cmd "catkin_find_pkg" "catkin" "${CMAKE_SOURCE_DIR}")
execute_process(COMMAND ${_cmd}
  RESULT_VARIABLE _res
  OUTPUT_VARIABLE _out
  ERROR_VARIABLE _err
  OUTPUT_STRIP_TRAILING_WHITESPACE
  ERROR_STRIP_TRAILING_WHITESPACE
)
if(NOT _res EQUAL 0 AND NOT _res EQUAL 2)
  # searching fot catkin resulted in an error
  string(REPLACE ";" " " _cmd_str "${_cmd}")
  message(FATAL_ERROR "Search for 'catkin' in workspace failed (${_cmd_str}): ${_err}")
endif()

# include catkin from workspace or via find_package()
if(_res EQUAL 0)
  set(catkin_EXTRAS_DIR "${CMAKE_SOURCE_DIR}/${_out}/cmake")
  # include all.cmake without add_subdirectory to let it operate in same scope
  include(${catkin_EXTRAS_DIR}/all.cmake NO_POLICY_SCOPE)
  add_subdirectory("${_out}")

else()
  # use either CMAKE_PREFIX_PATH explicitly passed to CMake as a command line argument
  # or CMAKE_PREFIX_PATH from the environment
  if(NOT DEFINED CMAKE_PREFIX_PATH)
    if(NOT "$ENV{CMAKE_PREFIX_PATH}" STREQUAL "")
      string(REPLACE ":" ";" CMAKE_PREFIX_PATH $ENV{CMAKE_PREFIX_PATH})
    endif()
  endif()

  # list of catkin workspaces
  set(catkin_search_path "")
  foreach(path ${CMAKE_PREFIX_PATH})
    if(EXISTS "${path}/.catkin")
      list(FIND catkin_search_path ${path} _index)
      if(_index EQUAL -1)
        list(APPEND catkin_search_path ${path})
      endif()
    endif()
  endforeach()

  # search for catkin in all workspaces
  set(CATKIN_TOPLEVEL_FIND_PACKAGE TRUE)
  find_package(catkin QUIET
    NO_POLICY_SCOPE
    PATHS ${catkin_search_path}
    NO_DEFAULT_PATH NO_CMAKE_FIND_ROOT_PATH)
  unset(CATKIN_TOPLEVEL_FIND_PACKAGE)

  if(NOT catkin_FOUND)
    message( ${CMAKE_PREFIX_PATH} )
    message(FATAL_ERROR "find_package(catkin) failed. catkin was neither found in the workspace nor in the CMAKE_PREFIX_PATH. One reason may be that no ROS setup.sh was sourced before.")
  endif()
endif()

# configure all
set(CMAKE_CXX_STANDARD 11)
set(ENABLE_EXAMPLES TRUE)

option(USE_DB "Use database" ON)
if(USE_DB)
    add_definitions(-DUSE_DB)
endif()

option(IS_VERSION_3 "Set Promobot version 3" ON)
if(IS_VERSION_3)
    add_definitions(-DPromobotV3)
endif()

IF(CMAKE_BUILD_TYPE MATCHES Debug)
    add_definitions(-Ddebugging)
ENDIF()

set(CMAKE_USE_RELATIVE_PATHS TRUE)

# configure Qt
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(LANGUAGES ru_RU en_US de_DE zh_CN cs_CZ lt_LT es_ES tr_TR pt_PT ar_AE)

function( generate_qm_files PROJECT_NAME PROJECT_SOURCE_DIR _qm_files )
  file(MAKE_DIRECTORY ${PROJECT_SOURCE_DIR}/lang)
  set_directory_properties(PROPERTIES CLEAN_NO_CUSTOM true)

  file(GLOB_RECURSE _my_sources ${PROJECT_SOURCE_DIR} *.h *.hh *.hpp *.hxx *.c *.cc *.cpp *.cxx *.ui *.qml)
  if(_my_sources)
    set(_ts_lst_file "${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/ts_lst_file")
    foreach(_lst_file_src ${_my_sources})
      set(_lst_file_srcs "${_lst_file_src}\n${_lst_file_srcs}")
    endforeach()

    get_directory_property(_inc_DIRS INCLUDE_DIRECTORIES)
    foreach(_pro_include ${_inc_DIRS})
      get_filename_component(_abs_include "${_pro_include}" ABSOLUTE)
      set(_lst_file_srcs "-I${_pro_include}\n${_lst_file_srcs}")
    endforeach()
    set(_lst_file_srcs "-I${PROJECT_SOURCE_DIR}\n${_lst_file_srcs}")
    set(_lst_file_srcs "-I${PROJECT_SOURCE_DIR}/include\n${_lst_file_srcs}")
    set(_lst_file_srcs "-I${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}\n${_lst_file_srcs}")

    string(REGEX REPLACE "\n$" "" _lst_file_srcs "${_lst_file_srcs}")
    file(WRITE ${_ts_lst_file} "${_lst_file_srcs}")
  endif()

  foreach(_language ${LANGUAGES})
    set(_ts_file ${PROJECT_SOURCE_DIR}/lang/${PROJECT_NAME}_${_language}.ts)
    add_custom_command(OUTPUT ${_ts_file}
      COMMAND lupdate
      ARGS -extensions 'c,c++,cc,cpp,cxx,ch,h,h++,hh,hpp,hxx,ui,qml'
        -no-obsolete
        -locations none
        -target-language ${_language}
        "@${_ts_lst_file}"
        -ts ${_ts_file}
      COMMENT "generate .ts"
      DEPENDS ${_my_sources} ${_ts_lst_file} VERBATIM)

    get_filename_component(_abs_FILE ${_ts_file} ABSOLUTE)
    get_filename_component(qm ${_abs_FILE} NAME_WE)
    get_source_file_property(output_location ${_abs_FILE} OUTPUT_LOCATION)
    if(output_location)
      file(MAKE_DIRECTORY "${output_location}")
      set(qm "${output_location}/${qm}.qm")
    else()
      set(qm "${CMAKE_CURRENT_BINARY_DIR}/${qm}.qm")
    endif()
    add_custom_command(OUTPUT ${qm}
      COMMAND lrelease
      ARGS ${_ts_file} -qm ${qm}
      COMMENT "generate .qm"
      DEPENDS ${_ts_file} VERBATIM)
    list(APPEND ${_qm_files} ${qm})
  endforeach(_language)
  set(${_qm_files} ${${_qm_files}} PARENT_SCOPE)
endfunction( generate_qm_files )

function( generate_mo_files PROJECT_NAME PROJECT_SOURCE_DIR _mo_files )
  find_package( Gettext REQUIRED )
  file(MAKE_DIRECTORY ${PROJECT_SOURCE_DIR}/lang)
  set_directory_properties(PROPERTIES CLEAN_NO_CUSTOM true)

  file(GLOB_RECURSE _my_sources ${PROJECT_SOURCE_DIR} *.h *.hpp *.hxx *.c *.cpp *.cxx)
  if(_my_sources)
    set(_po_lst_file "${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/po_lst_file")
    set(_lst_file_srcs)
    foreach(_lst_file_src ${_my_sources})
      set(_lst_file_srcs "${_lst_file_src}\n${_lst_file_srcs}")
    endforeach()

    string(REGEX REPLACE "\n$" "" _lst_file_srcs "${_lst_file_srcs}")
    file(WRITE ${_po_lst_file} "${_lst_file_srcs}")
  endif()

  foreach(_language ${LANGUAGES})
    set(_po_file ${PROJECT_SOURCE_DIR}/lang/${PROJECT_NAME}_${_language}.po)
    add_custom_command(OUTPUT ${_po_file}
      COMMAND xgettext
      ARGS --from-code=utf-8
        --keyword=translate:1,1t
        --keyword=translate:1,2,3t
        --language=C++
        --package-name=${PROJECT_NAME}
        -f ${_po_lst_file}
        -o ${_po_file}.tmp
      COMMAND bash
      ARGS -c "[ -f ${_po_file} ] || cp ${_po_file}{.tmp,}"
      COMMAND msgmerge
      ARGS --lang=${_language}
        --backup=none
        --no-location
        --no-wrap
        --force-po
        -N -U ${_po_file} ${_po_file}.tmp
      COMMAND rm
      ARGS -f ${_po_file}.tmp
      COMMENT "generate .po"
      DEPENDS ${_my_sources} ${_po_lst_file} VERBATIM)
    gettext_process_po_files(${_language} ALL
      PO_FILES ${_po_file})
    install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}_${_language}.gmo"
      DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/lang/${_language}/LC_MESSAGES"
      RENAME "${PROJECT_NAME}.mo")
    list(APPEND ${_mo_files} "${CMAKE_CURRENT_BINARY_DIR}/lang/${_language}")
  endforeach(_language)
  set(${_mo_files} ${${_mo_files}} PARENT_SCOPE)
endfunction( generate_mo_files )

catkin_workspace()
