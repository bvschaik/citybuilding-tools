
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
    messagefilexmlstream.cpp
    xmlstream.cpp
)

add_executable(engconverter ${engconverter_SRCS})
target_link_libraries(engconverter ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY})

install(TARGETS engconverter RUNTIME DESTINATION bin)
