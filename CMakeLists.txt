cmake_minimum_required(VERSION 3.16)

project(WarningDialogApp LANGUAGES CXX)

# ---------------------------------------------------------------------------
# Интеграция QCoro (корутины для Qt) требует C++20.
#   cmake -B build -DUSE_QCORO=ON        # корутинная версия (co_await)
#   cmake -B build                        # fallback-версия на сигналах QNetworkReply
# ---------------------------------------------------------------------------
option(USE_QCORO "Build with QCoro coroutine-based availability check" OFF)

set(CMAKE_CXX_STANDARD 20)          # QCoro требует C++20
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)               # автоматическая обработка Q_OBJECT (moc)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Поддержка и Qt5, и Qt6 (предпочитаем Qt6, если он установлен)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Gui Widgets Network)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui Widgets Network)

add_executable(WarningDialogApp
    main.cpp
    warningdialog.cpp
    warningdialog.h
    netchecker.cpp
    netchecker.h
)

target_link_libraries(WarningDialogApp PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Network
)

if(USE_QCORO)
    # QCoro устанавливается с CMake-конфигами: find_package(QCoro6 ...) или
    # find_package(QCoro5 ...). При необходимости укажите путь:
    #   cmake -B build -DUSE_QCORO=ON -DCMAKE_PREFIX_PATH=/usr/lib/qt6
    if(TARGET Qt6::Core)
        find_package(QCoro6 REQUIRED COMPONENTS Core Network)
        qcoro_setup_target(WarningDialogApp QT6 USE_DEPS)
        target_link_libraries(WarningDialogApp PRIVATE QCoro::Core QCoro::Network)
    else()
        find_package(QCoro5 REQUIRED COMPONENTS Core Network)
        qcoro_setup_target(WarningDialogApp QT5 USE_DEPS)
        target_link_libraries(WarningDialogApp PRIVATE QCoro::Core QCoro::Network)
    endif()

    target_compile_definitions(WarningDialogApp PRIVATE USE_QCORO)
    message(STATUS "Сборка с QCoro (USE_QCORO)")
else()
    message(STATUS "QCoro не включён — сборка с fallback-проверкой через QNetworkReply")
endif()
