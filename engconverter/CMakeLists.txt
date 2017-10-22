cmake_minimum_required(VERSION 2.9)
project(engconverter)

find_package(Qt4 REQUIRED)

include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})

set(engconverter_SRCS
    # UI-related
    engconverter.cpp
    convertertab.cpp
    main.cpp
    # Conversion
    fileconverter.cpp
    logger.cpp
    textfile.cpp
    textgroup.cpp
    textfileengstream.cpp
    textfilexmlstream.cpp
    messageentry.cpp
    messagefile.cpp
    messagefileengstream.cpp
)

set(CMAKE_AUTOMOC TRUE)

add_executable(engconverter ${engconverter_SRCS})
target_link_libraries(engconverter ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY})

install(TARGETS engconverter RUNTIME DESTINATION bin)
