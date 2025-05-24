cmake_minimum_required(VERSION 3.16)

project(MyAudioPlayer2 VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick Multimedia Sql)


qt_standard_project_setup(REQUIRES 6.5)

qt_add_executable(appMyAudioPlayer2
    main.cpp
)

qt_add_qml_module(appMyAudioPlayer2
    URI MyAudioPlayer2
    VERSION 1.0
    QML_FILES
        Main.qml

        RESOURCES
        assets/images/defaultImage.png
        assets/images/backward10.svg
        assets/images/mute.svg
        assets/images/open_new.svg
        assets/images/pause_symbol.svg
        assets/images/play_symbol.svg
        assets/images/forward10.svg
        assets/images/more.svg
        assets/images/speaker.svg
        assets/images/stop_symbol.svg
        assets/images/volume.svg
        assets/images/volume_mute.svg
        assets/images/zoom_maximize.svg
        assets/images/zoom_minimize.svg
        assets/images/link.svg
        assets/images/loop.svg
        assets/icons/Shuffle_Active.svg
        assets/icons/Shuffle_Icon.svg
        assets/icons/Shuffle_Icon_Dark.svg
        assets/icons/plus_icon.png
        assets/icons/cross_icon.png
        assets/icons/app_icon.ico
        assets/icons/add_icon.png
        assets/icons/menu_icon.png
        assets/icons/trash_icon.png
        assets/icons/next_icon.png
        assets/icons/previous_icon.png
        assets/icons/play_icon.png
        assets/icons/play.png
        assets/icons/pause_icon.png
        assets/icons/my_menu_icon.png
        assets/icons/heartFav.png
        assets/icons/heartNoFav.png
        assets/icons/delete.png
        assets/icons/edit.png
        assets/audio/symphony_no_5.mp3
        assets/audio/air_on_the_g_string.mp3
        assets/audio/eine_kleine_nachtmusik.mp3
        assets/audio/reprezentuje_biede.mp3
        QML_FILES AudioControl.qml
        QML_FILES
        QML_FILES CustomComboBox.qml
        QML_FILES EditPanel.qml
        QML_FILES ImageButton.qml
        QML_FILES MenuButton.qml
        QML_FILES
        QML_FILES PlaybackControl.qml
        QML_FILES PlaybackSeekControl.qml
        QML_FILES
        QML_FILES
        QML_FILES
        QML_FILES TrackDelegate.qml
        QML_FILES
        QML_FILES
        QML_FILES
        SOURCES playercontroller.h
        SOURCES audioinfo.h
        SOURCES databasehandler.h
        SOURCES
        SOURCES databasehandler.cpp
        SOURCES playercontroller.cpp
        SOURCES audioinfo.cpp
        SOURCES imagehandler.h imagehandler.cpp
        QML_FILES TrackEditPanel.qml
        SOURCES myaudioplayer.h myaudioplayer.cpp
        SOURCES
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appMyAudioPlayer2 PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appMyAudioPlayer2
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appMyAudioPlayer2
    PRIVATE Qt6::Quick
    Qt6::Multimedia
    Qt6::Sql
)


# target_compile_definitions(appMyAudioPlayer2 PRIVATE TAGLIB_STATIC)

include(GNUInstallDirs)
install(TARGETS appMyAudioPlayer2
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
