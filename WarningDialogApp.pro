QT += core gui widgets network

CONFIG += c++11

TARGET = WarningDialogApp
TEMPLATE = app

SOURCES += \
    main.cpp \
    warningdialog.cpp \
    netchecker.cpp

HEADERS += \
    warningdialog.h \
    netchecker.h

# Для Qt 5.12
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

# ---------------------------------------------------------------------------
# Интеграция QCoro (корутины для Qt).
#
# QCoro требует C++20. По умолчанию проект собирается с fallback-реализацией
# проверки доступности на сигналах QNetworkReply (netchecker.cpp, #else).
# Чтобы включить корутинную версию с co_await:
#
#   qmake "CONFIG+=qcoro" QCORO_DIR=/usr          # Debian/Ubuntu: qcoro-qt5-dev
# ---------------------------------------------------------------------------
qcoro {
    # Явно переключаем стандарт на C++20 (c++11 в CONFIG выше при этом снимается)
    CONFIG -= c++11 c++14 c++17
    CONFIG += c++20
    # Страховка: если mkspec/Qt не подставляют флаг стандарта, добавим сами
    QMAKE_CXXFLAGS += -std=gnu++20

    !isEmpty(QCORO_DIR) {
        INCLUDEPATH += $$QCORO_DIR/include/qcoro5
        LIBS += -L$$QCORO_DIR/lib
    }

    equals(QT_MAJOR_VERSION, 6) {
        INCLUDEPATH += /usr/include/qcoro6
        LIBS += -lQCoro6Core -lQCoro6Network
    } else {
        contains(INCLUDEPATH, /usr/include/qcoro5) { } else: \
            INCLUDEPATH += /usr/include/qcoro5
        LIBS += -lQCoro5Core -lQCoro5Network
    }

    DEFINES += USE_QCORO
    message("Сборка с QCoro (USE_QCORO)")
} else {
    message("QCoro не включён — сборка с fallback-проверкой через QNetworkReply")
}
