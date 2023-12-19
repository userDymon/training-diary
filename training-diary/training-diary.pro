QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    changepassword.cpp \
    changeusername.cpp \
    exercise.cpp \
    exercizedialog.cpp \
    login.cpp \
    main.cpp \
    mainwindow.cpp \
    signup.cpp \
    sqldbmanager.cpp \
    user.cpp

HEADERS += \
    changepassword.h \
    changeusername.h \
    dbmanager.h \
    exercise.h \
    exercizedialog.h \
    login.h \
    mainwindow.h \
    signup.h \
    sqldbmanager.h \
    user.h

FORMS += \
    changepassword.ui \
    changeusername.ui \
    exercizedialog.ui \
    login.ui \
    mainwindow.ui \
    signup.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
