cmake_minimum_required(VERSION 2.8.3)

project(vision)

##Include non qt-stuff

find_package(catkin)


find_package( OpenCV REQUIRED )
include_directories(SYSTEM ${OpenCV_INCLUDE_DIRS})

find_package(catkin REQUIRED sensor_msgs cv_bridge roscpp std_msgs image_transport)

find_package(Boost REQUIRED COMPONENTS system)
include_directories(include ${Boost_INCLUDE_DIR} ${catkin_INCLUDE_DIRS})

FIND_PACKAGE( OpenCV REQUIRED )
IF (${OpenCV_VERSION} VERSION_LESS 2.3.0)
	MESSAGE(FATAL_ERROR "OpenCV version is not compatible : ${OpenCV_VERSION}")
ENDIF()

## End of include non-qt stuff

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Find the QtWidgets library
find_package(Qt5Widgets)

# Add the include directories for the Qt 5 Widgets module to
# the compile lines.
include_directories(${Qt5Widgets_INCLUDE_DIRS})

# Use the compile definitions defined in the Qt 5 Widgets module
add_definitions(${Qt5Widgets_DEFINITIONS})

# Add compiler flags for building executables (-fPIE)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")

if (Qt5_POSITION_INDEPENDENT_CODE)
  set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif()

set(VISION_HEADER_FILES
        src/quadgui.h
        src/cameraworker.h
        src/camlabel.h
)

set(VISION_SOURCE_FILES    
  		src/quadgui.cpp
        src/cameraworker.cpp
        src/camlabel.cpp
)

# qt5_generate_moc(${VISION_SOURCE_FILES})

qt5_wrap_cpp(QT_MOC_SRCS ${VISION_HEADER_FILES})

qt5_wrap_ui(UI_HEADER src/quadgui.ui)

# Tell CMake to create the helloworld executable
add_executable(imageListener src/vision.cpp ${VISION_SOURCE_FILES} ${VISION_HEADER_FILES} ${QT_MOC_SRCS} ${UI_HEADER})

#Link the helloworld executable to the Qt 5 widgets library.
target_link_libraries(imageListener Qt5::Widgets ${catkin_LIBRARIES} ${OpenCV_LIBRARIES} ${Qt5Widgets_LIBRARIES})