# TODO добавить Boost
cmake_minimum_required(VERSION 2.8...3.13)
project(GammaKnife)

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# # Если не установлен тип сборки, ставим его на Debug
#
if(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
    message(STATUS "It seems build type doesn't set, so I set it to debug by default")
    set(CMAKE_BUILD_TYPE Debug)
endif()

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# # Lile-a-pro приказываем сохранять бинарники в <build-dir>/bin
#
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# # Макрос для вывода всех переменных. Полезен в процессе отладки.
#
macro(print_all_variables)
    message(STATUS "print_all_variables------------------------------------------{")
    get_cmake_property(_variableNames VARIABLES)
    foreach (_variableName ${_variableNames})
        message(STATUS "${_variableName}=${${_variableName}}")
    endforeach()
    message(STATUS "print_all_variables------------------------------------------}")
endmacro()

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# # Отключаем все языковые расширения от греха подальше
# # (бесполезное на самом-то деле действие)
# # + задаём стандарт c++17 (это действие имеет смысл)
#
if(NOT CMAKE_CXX_EXTENSIONS)
    message(STATUS "Disabling cxx extensions")
    set(CMAKE_CXX_EXTENSIONS OFF)
endif()

set(CMAKE_CXX_STANDARD 17)

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# # Приказываем добавлять к исполняемым файлам суффикс .xx, чтобы отличать их
# # от простых смертных
#
# TODO суффиксы задавать через конфигурацию проекта !!! (будет важно в run.py при запуске исполняемого файла)
if(WIN32)
    set(CMAKE_EXECUTABLE_SUFFIX_CXX ".exe")
else()
    set(CMAKE_EXECUTABLE_SUFFIX_CXX ".xx")
endif()

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# # Отлаживаем на максималках!
#
if(CMAKE_BUILD_TYPE STREQUAL Debug)
    add_compile_options(-O0 -g -pg
        -Wall -Wextra -Wpedantic)

    # Мост для тех, кто сдался при отладке на максималках
    # add_compile_options(-O0 -g -pg)


    # TODO протестировать на других компьютерах и написать, что приводит к ошибке 
    # """
    # terminate called after throwing an instance of 'std::bad_alloc'
    # what():  std::bad_alloc

    # ### CAUGHT SIGNAL: 6 ### address: 0x3e80000695d,  signal =  SIGABRT, value =    6, description = abort program (formerly SIGIOT). 
    # Backtrace:
    # [PID=26973, TID=-1][ 0/20]> /lib/x86_64-linux-gnu/libc.so.6(gsignal+0xcb) [0x7fd94011118b]
    # [PID=26973, TID=-1][ 1/20]> /lib/x86_64-linux-gnu/libc.so.6(abort+0x12b) [0x7fd9400f0859]
    # [PID=26973, TID=-1][ 2/20]> /usr/lib/x86_64-linux-gnu/libstdc++.so.6(+0x9e911) [0x7fd9404e4911]
    # [PID=26973, TID=-1][ 3/20]> /usr/lib/x86_64-linux-gnu/libstdc++.so.6(+0xaa38c) [0x7fd9404f038c]
    # [PID=26973, TID=-1][ 4/20]> /usr/lib/x86_64-linux-gnu/libstdc++.so.6(+0xaa3f7) [0x7fd9404f03f7]
    # [PID=26973, TID=-1][ 5/20]> /usr/lib/x86_64-linux-gnu/libstdc++.so.6(+0xaa6a9) [0x7fd9404f06a9]
    # [PID=26973, TID=-1][ 6/20]> /usr/lib/x86_64-linux-gnu/libstdc++.so.6(+0x9e522) [0x7fd9404e4522]
    # [PID=26973, TID=-1][ 7/20]> ./prog.xx(+0xb2e4) [0x56406a1462e4]
    # [PID=26973, TID=-1][ 8/20]> ./prog.xx(+0xb108) [0x56406a146108]
    # [PID=26973, TID=-1][ 9/20]> ./prog.xx(+0xadcc) [0x56406a145dcc]
    # [PID=26973, TID=-1][10/20]> ./prog.xx(+0xa399) [0x56406a145399]
    # [PID=26973, TID=-1][11/20]> ./prog.xx(+0x9f28) [0x56406a144f28]
    # [PID=26973, TID=-1][12/20]> ./prog.xx(+0x98b0) [0x56406a1448b0]
    # [PID=26973, TID=-1][13/20]> ./prog.xx(+0x8bbb) [0x56406a143bbb]
    # [PID=26973, TID=-1][14/20]> ./prog.xx(+0x8dd2) [0x56406a143dd2]
    # [PID=26973, TID=-1][15/20]> /usr/local/lib/libG4vis_management.so(_ZN12G4VisManager10InitialiseEv+0x4e) [0x7fd94193125e]
    # [PID=26973, TID=-1][16/20]> ./prog.xx(+0x8be0) [0x56406a143be0]
    # [PID=26973, TID=-1][17/20]> ./prog.xx(+0x8269) [0x56406a143269]
    # [PID=26973, TID=-1][18/20]> /lib/x86_64-linux-gnu/libc.so.6(__libc_start_main+0xf3) [0x7fd9400f20b3]
    # [PID=26973, TID=-1][19/20]> ./prog.xx(+0x7eee) [0x56406a142eee]
    # """
    # if(CMAKE_COMPILER_IS_GNUCXX)
    #     add_compile_definitions(_GLIBCXX_DEBUG)
    # endif()

    add_compile_definitions(_DEBUG)
endif()

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# # А когда релиз, оптимизируем по-максимуму
#
if(CMAKE_BUILD_TYPE STREQUAL Release)
    add_compile_options(-Ofast -march=native)
endif()

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# # Подрубаем (ищем) Boost
#
if(CMAKE_BUILD_TYPE STREQUAL Debug)
    set(  Boost_USE_DEBUG_LIBS ON)
    set(Boost_USE_RELEASE_LIBS OFF)
elseif(CMAKE_BUILD_TYPE STREQUAL Release)
    set(  Boost_USE_DEBUG_LIBS OFF)
    set(Boost_USE_RELEASE_LIBS ON)
endif()

set(Boost_USE_STATIC_RUNTIME ON)
set(   Boost_USE_STATIC_LIBS ON)
find_package(Boost REQUIRED)

# print_all_variables()

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# # Подрубаем (ищем) P7 logger
#
find_package(libp7-baical REQUIRED)
get_target_property(P7_INCLUDE_DIR libp7-baical::P7 INTERFACE_INCLUDE_DIRECTORIES)
get_target_property(P7_LIB libp7-baical::P7 INTERFACE_LINK_LIBRARIES)


# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# Теперь, когда настроили общие моменты, подключаем и настраиваем Geant4 
# -----------------------------------------------------------------------------
# "Подключаем" Geant4 к проекту, активируя все доступные UI и драйвера 
# визуализации. Чтобы отключить визуализацию и собрать программу чисто 
# для расчётов , необходимо установить WITH_GEANT4_UIVIS в OFF, через 
# cmake-gui (ccmake), либо используя аргументы командной строки в cmake:
# cmake -DWITH_GEANT4_UIVIS=OFF
#
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

#----------------------------------------------------------------------------
# Активируем мультипоточность и поднимаем флаги совместимости версий
#
add_compile_definitions(G4MULTITHREADED)
if(${Geant4_VERSION_MAJOR} EQUAL 11)
  message("Geant4 v11 detected")
  add_compile_definitions(G4v11)
elseif(${Geant4_VERSION_MAJOR} EQUAL 10)
  message("Geant4 v10 detected")
  add_compile_definitions(G4v10)
endif()

#----------------------------------------------------------------------------
# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs
#
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
message("Boost libraries: " ${Boost_LIBRARIES})
add_executable(GammaKnife GammaKnife.cc ${sources} ${headers}) # TODO шаблонизировать
include_directories(${Boost_INCLUDE_DIRS})
include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${P7_INCLUDE_DIR})
target_link_libraries(GammaKnife LINK_PUBLIC ${Geant4_LIBRARIES} # всё
                                                 boost_program_options 
                                                 ${Boost_LIBRARIES}  #` как
                                                 P7 ${P7_LIB}        #   у людей
                      )

#----------------------------------------------------------------------------
# Copy all scripts to the build directory, i.e. the directory in which we
# build prog. This is so that we can run the executable directly because it
# relies on these scripts being in the current working directory.
#

foreach(_script ${GammaKnife_SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
  )
endforeach()

