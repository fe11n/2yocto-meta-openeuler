ROS_BUILD_DEPENDS += " \
    rosidl-adapter \
    ydlidar \
"

ROS_EXPORT_DEPENDS += " \
    ydlidar \
"

ROS_EXEC_DEPENDS += " \
    ydlidar \
"

FILES_${PN} += "/usr/share /usr/lib"
