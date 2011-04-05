TEMPLATE = app
TARGET = Create
QT += core \
    gui \
    xml \
    network
HEADERS += Library/Algorithm/VFF.h \
    Library/Algorithm/Grid.h \
    Library/VFF.h \
    Task/StraightPathMoveTask.h \
    Task/MoveTask.h \
    Task/SeeYouTask.h \
    Controller/BlockDriveController.h \
    Controller/SensorController.h \
    Controller/ArduinoController.h \
    COIL/ArduinoCOIL.h \
    Task/TestMoveTask.h \
    Controller/SeeYouController.h \
    Controller/FluidDriveController.h \
    GUI/TaskEditor.h \
    JoystickView.h \
    interface.h \
    COIL/COIL.h \
    qextserialport/qextserialbase.h \
    qextserialport/qextserialenumerator.h \
    qextserialport/qextserialport.h \
    Task/Task.h \
    Task/TaskList.h \
    Task/TaskManager.h \
    MovementTracker/MovementTracker.h \
    MovementTracker/RawMovementTracker.h \
    Controller/Controller.h \
    Controller/EmssController.h \
    Library/Debug.h \
    Library/Math/math-ext.h \
    Library/Math/spline1.h \
    Library/Math/trafo2.h \
    Library/Math/vector2.h \
    Library/Math.h \
    Library/SleeperThread.h \
    Library/Util.h \
    create.h
SOURCES += Library/Algorithm/VFF.cpp \
    Library/Algorithm/Grid.cpp \
    Task/StraightPathMoveTask.cpp \
    Task/MoveTask.cpp \
    Task/SeeYouTask.cpp \
    Controller/BlockDriveController.cpp \
    Controller/SensorController.cpp \
    Controller/ArduinoController.cpp \
    COIL/ArduinoCOIL.cpp \
    Task/TestMoveTask.cpp \
    Controller/SeeYouController.cpp \
    Controller/FluidDriveController.cpp \
    GUI/TaskEditor.cpp \
    JoystickView.cpp \
    interface.cpp \
    COIL/COIL.cpp \
    qextserialport/qextserialbase.cpp \
    qextserialport/qextserialenumerator.cpp \
    qextserialport/qextserialport.cpp \
    Task/Task.cpp \
    Task/TaskList.cpp \
    Task/TaskManager.cpp \
    MovementTracker/MovementTracker.cpp \
    MovementTracker/RawMovementTracker.cpp \
    Controller/Controller.cpp \
    Controller/EmssController.cpp \
    Library/Debug.cpp \
    main.cpp \
    create.cpp
FORMS += create.ui
RESOURCES += 
INCLUDEPATH += 
RESOURCES += 
unix:HEADERS += qextserialport/posix_qextserialport.h
unix:SOURCES += qextserialport/posix_qextserialport.cpp
unix:DEFINES += _TTY_POSIX_
win32:HEADERS += qextserialport/win_qextserialport.h
win32:SOURCES += qextserialport/win_qextserialport.cpp
win32:DEFINES += _TTY_WIN_
win32:LIBS += -lsetupapi
LIBS += -lqextserialport
