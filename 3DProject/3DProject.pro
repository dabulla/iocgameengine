# Add more folders to ship with the application, here
folder_01.source = qml/3DProject
folder_01.target = qml
DEPLOYMENTFOLDERS = folder_01

QMAKE_CXXFLAGS += -std=c++0x

QMAKE_CFLAGS_WARN_OFF = -Wunused-local-typedefs

# Additional import path used to resolve QML modules in Creator's code model
QML_IMPORT_PATH =

# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += main.cpp

# Installation path
# target.path =

# Please do not modify the following two lines. Required for deployment.
include(qtquick2controlsapplicationviewer/qtquick2controlsapplicationviewer.pri)
qtcAddDeployment()


INCLUDEPATH += "F:/devel/boost_1_55_0"

HEADERS += \
    any_ptr.h
