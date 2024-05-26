cmake_minimum_required(VERSION 3.26)
project(15_PuzzleGame)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_PREFIX_PATH "D:/QTCreator/6.7.0/mingw_64")

set(QGIF_PLUGIN_PATH "D:/QTCreator/6.7.0/mingw_64/plugins/imageformats")

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

add_executable(15_PuzzleGame main.cpp
        ManagersSystem/IGameManger.h
        ManagersSystem/Managers.h
        ManagersSystem/Managers.cpp
        ManagersSystem/PreferencesManager.h
        ManagersSystem/PreferencesManager.cpp
        ManagersSystem/ProgressManager.h
        ManagersSystem/ProgressManager.cpp
        ManagersSystem/SavingsManager.h
        ManagersSystem/SavingsManager.cpp
        ManagersSystem/SceneManager.h
        ManagersSystem/SceneManager.cpp
        utils.cpp
        utils.h
        MenuSceneView.cpp
        MenuSceneView.h
        GameSceneView.cpp
        GameSceneView.h
        Scenes.h
        BoardModel.cpp
        BoardModel.h
        IGenericArray.h
        Algorithms.h
        GameController.cpp
        GameController.h
        BoardNode.cpp
        BoardNode.h
        Algorithms.cpp
        SolutionModel.cpp
        SolutionModel.h
        Point.h
        SearchException.cpp
        SearchException.h
        NullReferenceException.cpp
        NullReferenceException.h
        BoardNodeComparator.cpp
        BoardNodeComparator.h
        Point.cpp
        Point.cpp
        Tile.cpp
        Tile.h
        BoardView.cpp
        BoardView.h
        BoardGenerationOption.h
        GraphicsWidget.h
        MoveTileScoreOption.h
        FileException.cpp
        FileException.h
        Scene.cpp
        Scene.h
        MenuController.cpp
        MenuController.h
        AbstractController.h
        IConnectable.h
        IDisposable.h
        AbstractController.cpp
        ResourcesOption.h
)

target_link_libraries(15_PuzzleGame
        Qt::Core
        Qt::Gui
        Qt::Widgets
)

add_custom_command(TARGET 15_PuzzleGame POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:15_PuzzleGame>/plugins/imageformats"
        COMMAND ${CMAKE_COMMAND} -E copy "${QGIF_PLUGIN_PATH}/qgif.dll" "$<TARGET_FILE_DIR:15_PuzzleGame>/plugins/imageformats"
)

if (WIN32 AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(DEBUG_SUFFIX)
    if (MSVC AND CMAKE_BUILD_TYPE MATCHES "Debug")
        set(DEBUG_SUFFIX "d")
    endif ()
    set(QT_INSTALL_PATH "${CMAKE_PREFIX_PATH}")
    if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
        set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
            set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        endif ()
    endif ()
    if (EXISTS "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
    endif ()
    foreach (QT_LIB Core Gui Widgets)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/bin/Qt6${QT_LIB}${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
    endforeach (QT_LIB)
endif ()
