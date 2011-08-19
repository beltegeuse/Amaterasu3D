INCLUDE_DIRECTORIES(
	${CMAKE_CURRENT_SOURCE_DIR}
)

ADD_EXECUTABLE(TechniqueGBuffer main.cpp)

target_link_libraries(
  TechniqueGBuffer
  Amaterasu3D
)

install(TARGETS TechniqueGBuffer RUNTIME DESTINATION ${Amaterasu3D_BINARY_DIR})