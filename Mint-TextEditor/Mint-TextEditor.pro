# Base project config
QT += core widgets

# Modern C++
CONFIG += c++17

# Executable
TARGET = Mint

# Application Type
TEMPLATE = app

# Input (source code)
INCLUDEPATH += src

# Sources folder
SOURCES += \
    main.cpp \
    src/MainWindow.cpp \
    src/FindDialog.cpp

# Header files
HEADERS += \
    src/MainWindow.h \
    src/FindDialog.h

# Interface files
FORMS += \
    src/MainWindow.ui

# Output (exit folder)
DESTDIR = build

# debug/release config
CONFIG(debug, debug|release) {
    OBJECTS_DIR = build/debug
    MOC_DIR = build/debug
    RCC_DIR = build/debug
    UI_DIR = build/debug
}

CONFIG(release, debug|release) {
    OBJECTS_DIR = build/release
    MOC_DIR = build/release
    RCC_DIR = build/release
    UI_DIR = build/release
}
