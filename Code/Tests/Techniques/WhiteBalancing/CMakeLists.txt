INCLUDE_DIRECTORIES(
	${CMAKE_CURRENT_SOURCE_DIR}
)

ADD_EXECUTABLE(TechniqueWhiteBalancing main.cpp)

target_link_libraries(
  TechniqueWhiteBalancing
  Amaterasu3D
)

install(TARGETS TechniqueWhiteBalancing RUNTIME DESTINATION ${Amaterasu3D_BINARY_DIR})