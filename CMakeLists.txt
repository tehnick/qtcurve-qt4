project(QtCurve-KDE4)
cmake_minimum_required(VERSION 2.4.4)

set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README")
set(CPACK_GENERATOR "DEB;RPM;TGZ")
set(DEBIAN_PACKAGE_DESCRIPTION "QtCurve for Qt4/KDE4")
set(DEBIAN_PACKAGE_SECTION "kde4")
set(CPACK_SOURCE_GENERATOR "TBZ2")
set(CPACK_PACKAGE_VERSION_MAJOR "1")
set(CPACK_PACKAGE_VERSION_MINOR "8")
set(CPACK_PACKAGE_VERSION_PATCH "14")
set(CPACK_PACKAGE_CONTACT "Craig Drummond <craig.p.drummond@gmail.com>")
set(QTCURVE_VERSION "${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}")
set(QTCURVE_VERSION_FULL "${QTCURVE_VERSION}.${CPACK_PACKAGE_VERSION_PATCH}")
set(CPACK_SOURCE_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}-${QTCURVE_VERSION_FULL}")
set(CPACK_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}-${QTCURVE_VERSION_FULL}")
include(CPack)

set(OLD_CMAKE_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})

# Try to find kde4-config...
file(TO_CMAKE_PATH "$ENV{KDEDIRS}" _KDEDIRS)

find_program(KDE3CONFIG_EXECUTABLE NAMES kde-config
             PATHS $ENV{KDEDIR}/bin /opt/kde3/bin /opt/kde/bin NO_DEFAULT_PATH)

find_program(KDE3CONFIG_EXECUTABLE kde-config)

if(KDE3CONFIG_EXECUTABLE)
    exec_program(${KDE3CONFIG_EXECUTABLE}
                    ARGS --prefix
                    OUTPUT_VARIABLE KDE3PREFIX)
endif(KDE3CONFIG_EXECUTABLE)

find_program(KDE4_KDECONFIG_EXECUTABLE NAMES kde4-config
             PATHS ${CMAKE_INSTALL_PREFIX}/bin ${_KDEDIRS} /opt/kde4/bin NO_DEFAULT_PATH)

if (NOT KDE4_KDECONFIG_EXECUTABLE)
   find_program(KDE4_KDECONFIG_EXECUTABLE NAMES kde4-config )
endif (NOT KDE4_KDECONFIG_EXECUTABLE)

# Then set install prefix...
if (KDE4_KDECONFIG_EXECUTABLE)
    if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
        exec_program(${KDE4_KDECONFIG_EXECUTABLE}
                     ARGS --prefix
                     OUTPUT_VARIABLE CMAKE_INSTALL_PREFIX)
    endif(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    exec_program(${KDE4_KDECONFIG_EXECUTABLE}
                    ARGS --prefix
                    OUTPUT_VARIABLE KDE4PREFIX)
endif (KDE4_KDECONFIG_EXECUTABLE)

if(NOT KDE3PREFIX)
    if(KDE4PREFIX)
        set(KDE3PREFIX ${KDE4PREFIX})
    else(KDE4PREFIX)
        set(KDE3PREFIX /usr)
    endif(KDE4PREFIX)
endif(NOT KDE3PREFIX)

if(NOT KDE4PREFIX)
    if(KDE3PREFIX)
        set(KDE4PREFIX ${KDE3PREFIX})
    else(KDE3PREFIX)
        set(KDE4PREFIX /usr)
    endif(KDE3PREFIX)
endif(NOT KDE4PREFIX)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules )

# search packages used by KDE
if (NOT QTC_QT_ONLY)
    find_package(KDE4 REQUIRED)
endif (NOT QTC_QT_ONLY)
find_package(Qt4 REQUIRED)

configure_file(config.h.cmake ${CMAKE_BINARY_DIR}/config.h @ONLY)

if (NOT QTC_QT_ONLY)
    find_package(KDE4 REQUIRED)
    include(KDE4Defaults)

    add_definitions(${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
    include_directories (${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR} ${KDE4_INCLUDES})
    set(QTCURVE_STYLE_DIR ${PLUGIN_INSTALL_DIR}/plugins/styles/)
    add_subdirectory(config)
    add_subdirectory(themes)
    add_subdirectory(colors)
    add_subdirectory(po)

    find_file(KWINGLOBALS_H kwinglobals.h ${KDE4_INCLUDES}) # kwinglobals.h is new for KDE4

    if (KWINGLOBALS_H)
        add_subdirectory(kwin)
        add_subdirectory(kwinconfig)
    else (KWINGLOBALS_H)
        message("** KWin headers not found - KWin decoration will not be built")
    endif (KWINGLOBALS_H)
else (NOT QTC_QT_ONLY)
    set(CMAKE_INSTALL_PREFIX ${OLD_CMAKE_INSTALL_PREFIX})
    add_definitions(${QT_DEFINITIONS})
    include_directories (${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR})
    set(QTCURVE_STYLE_DIR ${QT_PLUGINS_DIR}/styles/)
endif (NOT QTC_QT_ONLY)

add_subdirectory(style)

message("** PREFIX=${CMAKE_INSTALL_PREFIX}\n")

if (QTC_KWIN_MAX_BUTTON_HACK)
    message("**************************************************************\n"
            "Warning: You have enabled the hack to force drawing of maximise\n"
            "         buttons for windows that can be minimised. This is a 100%\n"
            "         hack, that may not work or compile, and may even crash\n"
            "         kwin.\n"
            "**************************************************************\n")
endif (QTC_KWIN_MAX_BUTTON_HACK)

if (QTC_ENABLE_PARENTLESS_DIALOG_FIX_SUPPORT)
    message("**************************************************************\n"
            "Warning: You have enabled support for the 'fix parentless dialogs'\n"
            "         option. This is known to BREAK some applications. Please\n"
            "         DO NOT report errors to application authors when you have\n"
            "         this enabled. Please reconsider DISABLING this option.\n"
            "**************************************************************\n")
endif (QTC_ENABLE_PARENTLESS_DIALOG_FIX_SUPPORT)
