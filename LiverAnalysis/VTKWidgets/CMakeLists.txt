PROJECT(vtkSlicer${MODULE_NAME}ModuleVTKWidgets)

find_package(OpenMP)

if(OPENMP_FOUND)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

set(KIT ${PROJECT_NAME})

set(${KIT}_EXPORT_DIRECTIVE "VTK_SLICER_${MODULE_NAME_UPPER}_MODULE_VTKWIDGETS_EXPORT")

#set(${KIT}_INCLUDE_DIRECTORIES
#  ${CMAKE_CURRENT_BINARY_DIR}/..
#  ${vtkSlicer${MODULE_NAME}ModuleMRML_SOURCE_DIR}
#  ${vtkSlicer${MODULE_NAME}ModuleMRML_BINARY_DIR}
#  ${vtkSlicer${MODULE_NAME}VTKWidgets_INCLUDE_DIRS}
#  ${vtkSlicer${MODULE_NAME}VTKWidgets_INCLUDE_DIRS}
#  )

set(${KIT}_SRCS
  vtkBezierSurfaceSource.h
  vtkBezierSurfaceSource.cxx
  vtkResectionSurfaceWidget.h
  vtkResectionSurfaceWidget.cxx
  )

set(${KIT}_TARGET_LIBRARIES
  ${VTK_LIBRARIES}
  #vtkSlicer${MODULE_NAME}ModuleVTKWidgets
  #vtkSlicerMarkupsModuleVTKWidgets
  vtkSlicerMarkupsModuleVTKWidgets
  )

#-----------------------------------------------------------------------------
SlicerMacroBuildModuleLogic(
  NAME ${KIT}
  EXPORT_DIRECTIVE ${${KIT}_EXPORT_DIRECTIVE}
  INCLUDE_DIRECTORIES ${${KIT}_INCLUDE_DIRECTORIES}
  SRCS ${${KIT}_SRCS}
  TARGET_LIBRARIES ${${KIT}_TARGET_LIBRARIES}
  )
