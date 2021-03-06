set(qtcurve_style_common_SRCS
        ${CMAKE_CURRENT_SOURCE_DIR}/../common/common.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../common/config_file.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../common/colorutils.c )
set_source_files_properties(${qtcurve_style_common_SRCS} PROPERTIES LANGUAGE CXX)

if (Q_WS_X11)
    set(qtcurve_SRCS qtcurve.cpp windowmanager.cpp macmenu.cpp blurhelper.cpp utils.cpp shortcuthandler.cpp shadowhelper.cpp ${qtcurve_style_common_SRCS})
    set(qtcurve_MOC_HDRS qtcurve.h windowmanager.h macmenu.h macmenu-dbus.h blurhelper.h shortcuthandler.h shadowhelper.h)
else (Q_WS_X11)
    set(qtcurve_SRCS qtcurve.cpp windowmanager.cpp blurhelper.cpp utils.cpp shortcuthandler.cpp ${qtcurve_style_common_SRCS})
    set(qtcurve_MOC_HDRS qtcurve.h windowmanager.h blurhelper.h shortcuthandler.h)
endif (Q_WS_X11)


add_definitions(-DQT_PLUGIN)
include_directories (${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_BINARY_DIR} ${QT_INCLUDE_DIR}
                     ${QT_QTGUI_INCLUDE_DIR} ${QT_QTCORE_INCLUDE_DIR} ${CMAKE_SOURCE_DIR}/common)
#kde4_add_plugin(qtcurve ${qtcurve_SRCS} )
qt4_automoc(${qtcurve_SRCS})
qt4_wrap_cpp(qtcurve_MOC_SRCS ${qtcurve_MOC_HDRS})
add_library(qtcurve MODULE ${qtcurve_SRCS} ${qtcurve_MOC_SRCS})
set_target_properties(qtcurve PROPERTIES PREFIX "")

if (NOT QTC_QT_ONLY)
    install(FILES qtcurve.themerc DESTINATION ${DATA_INSTALL_DIR}/kstyle/themes)
    set(QTC_KDE_LIBS ${KDE4_KDEUI_LIBS})
    if (NOT QTC_DISABLE_KDEFILEDIALOG_CALLS)
        set(QTC_KFILEDIALOG_LIBS ${KDE4_KIO_LIBS} ${KDE4_KDECORE_LIBS})
    endif (NOT QTC_DISABLE_KDEFILEDIALOG_CALLS)
endif (NOT QTC_QT_ONLY)

if (Q_WS_X11)
    target_link_libraries(qtcurve ${QTC_KDE_LIBS} ${QTC_KFILEDIALOG_LIBS} ${QT_QTGUI_LIBRARY}
                                ${QT_QTDBUS_LIBRARY} ${QT_QTCORE_LIBRARY} ${QT_QTSVG_LIBRARY} ${X11_X11_LIB})
else (Q_WS_X11)
    target_link_libraries(qtcurve ${QTC_KDE_LIBS} ${QTC_KFILEDIALOG_LIBS} ${QT_QTGUI_LIBRARY}
                                  ${QT_QTCORE_LIBRARY} ${QT_QTSVG_LIBRARY})
endif (Q_WS_X11)


if (NOT QTC_QT_ONLY)
    install(TARGETS qtcurve DESTINATION ${QTCURVE_STYLE_DIR})
else (NOT QTC_QT_ONLY)
    install(TARGETS qtcurve LIBRARY DESTINATION ${QTCURVE_STYLE_DIR})
endif (NOT QTC_QT_ONLY)

