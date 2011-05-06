TEMPLATE = app
TARGET = Create
QT += core \
    gui \
    xml \
    opengl \
    network
HEADERS += Navigation/Weight/RightWallFollowerWeight.h \
    Task/WallFollowerTask.h \
    Navigation/Weight/WallFollowerWeight.h \
    Library/TerrainPoint.h \
    Library/DockIR.h \
    GUI/MapObjectsView.h \
    GUI/MapOverview.h \
    Task/NavigationTask.h \
    Library/DockingStation.h \
    Map/FadingCollisionMap.h \
    Map/GridMap.h \
    Map/MapObject/DockingStationMapObject.h \
    Map/MapObject/MarkerMapObject.h \
    Map/MapObject/NavPathMapObject.h \
    Map/MapObject/NavigationMapObject.h \
    Map/MapObject/RobotMapObject.h \
    Map/MapObject/TrackerMapObject.h \
    Map/MapObject/MapObject.h \
    Map/ObjectMap.h \
    Map/StructureMap.h \
    Map/TerrainMap.h \
    Map/ColorMap.h \
    Map/HeatMap.h \
    Map/MapMeta.h \
    GUI/DrawCanvas.h \
    GUI/DrawCanvasGL.h \
    GUI/Viewport.h \
    Library/Util.h \
    Library/KMLGroundOverlay.h \
    Map/PhysicalMap.h \
    Map/Map.h \
    Navigation/Weight/OrientationWeight.h \
    Navigation/Weight/ObstacleAvoidanceWeight.h \
    Navigation/Weight/CollisionAvoidanceWeight.h \
    Library/RFIDDBase.h \
    GUI/OptionsDialog.h \
    GUI/WeightEditor.h \
    Library/Math/complex.h \
    Library/Math/frenet.h \
    Library/Math/gps.h \
    MovementTracker/AveragedTracker.h \
    MovementTracker/CorrectedFrenetMovementTracker.h \
    MovementTracker/ExpectedMovementTracker.h \
    MovementTracker/FrenetMovementTracker.h \
    MovementTracker/SelfCorrectingTracker.h \
    MovementTracker/SingleTracker.h \
    MovementTracker/Tracker.h \
    Navigation/Navigation.h \
    Navigation/SystemOfWeightsNavigation.h \
    Navigation/Weight/AccelerationFilterWeight.h \
    Navigation/Weight/ControllerSpeedWeight.h \
    Navigation/Weight/FullSpeedWeight.h \
    Navigation/Weight/JoystickWeight.h \
    Navigation/Weight/RoamingWeight.h \
    Navigation/Weight/Weight.h \
    Task/JoystickNavigationTask.h \
    Controller/BlockDriveController.h \
    Controller/FluidDriveController.h \
    Library/Joystick2D.h \
    CoreFactory.h \
    Library/HeapLogger.h \
    CoreObject.h \
    CoreThread.h \
    Network/Network.h \
    Network/RemoteInterfaceMessage.h \
    Network/networkcommunication.h \
    Library/Algorithm/VFF.h \
    Library/Algorithm/Grid.h \
    Library/VFF.h \
    Task/StraightPathMoveTask.h \
    Task/MoveTask.h \
    Task/SeeYouTask.h \
    Controller/SensorController.h \
    Controller/ArduinoController.h \
    COIL/ArduinoCOIL.h \
    Task/TestMoveTask.h \
    Controller/SeeYouController.h \
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
    create.h
SOURCES += Task/WallFollowerTask.cpp \
    GUI/MapObjectsView.cpp \
    GUI/MapOverview.cpp \
    Task/NavigationTask.cpp \
    Map/FadingCollisionMap.cpp \
    Map/GridMap.cpp \
    Map/MapObject/DockingStationMapObject.cpp \
    Map/MapObject/MarkerMapObject.cpp \
    Map/MapObject/NavPathMapObject.cpp \
    Map/MapObject/NavigationMapObject.cpp \
    Map/MapObject/RobotMapObject.cpp \
    Map/MapObject/TrackerMapObject.cpp \
    Map/MapObject/MapObject.cpp \
    Map/ObjectMap.cpp \
    Map/StructureMap.cpp \
    Map/TerrainMap.cpp \
    Map/ColorMap.cpp \
    Map/HeatMap.cpp \
    GUI/Viewport.cpp \
    Library/Util.cpp \
    Library/KMLGroundOverlay.cpp \
    Map/PhysicalMap.cpp \
    Map/Map.cpp \
    Library/RFIDDBase.cpp \
    GUI/OptionsDialog.cpp \
    GUI/WeightEditor.cpp \
    Library/Math/gps.cpp \
    MovementTracker/AveragedTracker.cpp \
    MovementTracker/CorrectedFrenetMovementTracker.cpp \
    MovementTracker/ExpectedMovementTracker.cpp \
    MovementTracker/FrenetMovementTracker.cpp \
    MovementTracker/SelfCorrectingTracker.cpp \
    MovementTracker/SingleTracker.cpp \
    MovementTracker/Tracker.cpp \
    Navigation/Navigation.cpp \
    Navigation/SystemOfWeightsNavigation.cpp \
    Task/JoystickNavigationTask.cpp \
    Controller/BlockDriveController.cpp \
    Controller/FluidDriveController.cpp \
    CoreFactory.cpp \
    Library/HeapLogger.cpp \
    Network/Network.cpp \
    Network/RemoteInterfaceMessage.cpp \
    Network/networkcommunication.cpp \
    Library/Algorithm/VFF.cpp \
    Library/Algorithm/Grid.cpp \
    Task/StraightPathMoveTask.cpp \
    Task/MoveTask.cpp \
    Task/SeeYouTask.cpp \
    Controller/SensorController.cpp \
    Controller/ArduinoController.cpp \
    COIL/ArduinoCOIL.cpp \
    Task/TestMoveTask.cpp \
    Controller/SeeYouController.cpp \
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
RESOURCES += DefaultResources/Icons.qrc
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
