SET(TestCustomAttributs_SRCS
	main.cpp
)


INCLUDE_DIRECTORIES(
	${CMAKE_CURRENT_SOURCE_DIR}
	../../Libs/Amaterasu3D
)

ADD_EXECUTABLE(TestCustomAttributs ${TestCustomAttributs_SRCS})

target_link_libraries(
  TestCustomAttributs
  Amaterasu3D
)

SET_TARGET_PROPERTIES(TestCustomAttributs PROPERTIES
  INSTALL_RPATH ${PROJECT_BUILD_PATH}
  INSTALL_RPATH_USE_LINK_PATH true
)

install(TARGETS TestCustomAttributs RUNTIME DESTINATION ${PROJECT_BUILD_PATH})