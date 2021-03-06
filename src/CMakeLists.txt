# Copyright 2010, François Bleibel, Olivier Stasse, JRL, CNRS/AIST,
#                 Florent Lamiraux (CNRS/LAAS)

LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})

# Verbosity level
IF (NOT (\"${CMAKE_VERBOSITY_LEVEL}\" STREQUAL \"\"))
  ADD_DEFINITIONS(-DVP_DEBUG_MODE=${CMAKE_VERBOSITY_LEVEL} -DVP_DEBUG)
ENDIF (NOT (\"${CMAKE_VERBOSITY_LEVEL}\" STREQUAL \"\"))

IF(CMAKE_BUILD_TYPE STREQUAL "DEBUG")
  ADD_DEFINITIONS(-DDEBUG=2)
ENDIF(CMAKE_BUILD_TYPE STREQUAL "DEBUG")

SET(integrator-force-rk4_plugins_dependencies integrator-force)
SET(integrator-force-exact_plugins_dependencies integrator-force)

FOREACH(lib ${plugins})
  SET(libname dp-${lib})
  ADD_LIBRARY(${libname} SHARED ${lib}.cpp)
  SET_TARGET_PROPERTIES(${libname} PROPERTIES
    PREFIX ""
    SOVERSION ${PROJECT_VERSION})

  IF(${lib}_plugins_dependencies)
    ADD_DEPENDENCIES(${libname} "dp-${${lib}_plugins_dependencies}")
    TARGET_LINK_LIBRARIES(${libname} "dp-${${lib}_plugins_dependencies}")
  ENDIF(${lib}_plugins_dependencies)

  TARGET_LINK_LIBRARIES(${libname} ${Boost_LIBRARIES})

  PKG_CONFIG_USE_DEPENDENCY(${libname} pinocchio)
  IF(BUILD_PYTHON_INTERFACE)
    PKG_CONFIG_USE_DEPENDENCY(${libname} eigenpy)
  ENDIF(BUILD_PYTHON_INTERFACE)

  PKG_CONFIG_USE_DEPENDENCY(${libname} sot-core)
  PKG_CONFIG_USE_DEPENDENCY(${libname} dynamic-graph)

  INSTALL(TARGETS ${libname} DESTINATION ${DYNAMIC_GRAPH_PLUGINDIR})

  # build python submodule
  IF(BUILD_PYTHON_INTERFACE)
    PKG_CONFIG_USE_DEPENDENCY(${libname} eigenpy)
    STRING(REPLACE - _ PYTHON_LIBRARY_NAME ${lib})

    DYNAMIC_GRAPH_PYTHON_MODULE("sot/dynamics_pinocchio/${PYTHON_LIBRARY_NAME}"
      ${libname}
      sot-dynamic-pinocchio-${PYTHON_LIBRARY_NAME}-wrap
      1
      "src/python-module-py.cpp"
      )
    PKG_CONFIG_USE_DEPENDENCY(sot-dynamic-pinocchio-${PYTHON_LIBRARY_NAME}-wrap pinocchio)
  ENDIF(BUILD_PYTHON_INTERFACE)

  UNSET({libname})
ENDFOREACH(lib)

# Main Library
ADD_LIBRARY(${LIBRARY_NAME} SHARED sot-dynamic-pinocchio.cpp)
TARGET_LINK_LIBRARIES(${LIBRARY_NAME} pinocchio)
TARGET_LINK_LIBRARIES(${LIBRARY_NAME} sot-core)
TARGET_LINK_LIBRARIES(${LIBRARY_NAME} dynamic-graph)
PKG_CONFIG_USE_DEPENDENCY(${LIBRARY_NAME} pinocchio)
PKG_CONFIG_USE_DEPENDENCY(${LIBRARY_NAME} sot-core)
PKG_CONFIG_USE_DEPENDENCY(${LIBRARY_NAME} dynamic-graph)
TARGET_LINK_LIBRARIES(${LIBRARY_NAME} ${Boost_LIBRARIES})
INSTALL(TARGETS ${LIBRARY_NAME} DESTINATION ${CMAKE_INSTALL_LIBDIR})

TARGET_LINK_LIBRARIES(dp-dynamic ${LIBRARY_NAME})

# Install empty __init__.py files in intermediate directories.
IF(BUILD_PYTHON_INTERFACE)
  INSTALL(FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/dynamic_graph/sot/dynamics_pinocchio/__init__.py
    ${CMAKE_CURRENT_SOURCE_DIR}/dynamic_graph/sot/dynamics_pinocchio/humanoid_robot.py
    ${CMAKE_CURRENT_SOURCE_DIR}/dynamic_graph/sot/dynamics_pinocchio/tools.py
    ${CMAKE_CURRENT_SOURCE_DIR}/dynamic_graph/sot/dynamics_pinocchio/parser.py
    DESTINATION ${PYTHON_SITELIB}/dynamic_graph/sot/dynamics_pinocchio
    )
ENDIF(BUILD_PYTHON_INTERFACE)
