
set(app_src
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

qt4_add_resources(app_res
    ../res/resources.qrc
)

add_executable(engconverter ${app_src} ${app_res})
target_link_libraries(engconverter ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY})

install(TARGETS engconverter RUNTIME DESTINATION bin)
