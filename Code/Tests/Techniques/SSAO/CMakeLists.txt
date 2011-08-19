INCLUDE_DIRECTORIES(
	${CMAKE_CURRENT_SOURCE_DIR}
)

ADD_EXECUTABLE(TechniqueSSAO main.cpp)

target_link_libraries(
  TechniqueSSAO
  Amaterasu3D
)

install(TARGETS TechniqueSSAO RUNTIME DESTINATION ${Amaterasu3D_BINARY_DIR})