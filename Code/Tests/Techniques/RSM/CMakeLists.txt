INCLUDE_DIRECTORIES(
	${CMAKE_CURRENT_SOURCE_DIR}
)

ADD_EXECUTABLE(TechniqueRSM main.cpp)

target_link_libraries(
  TechniqueRSM
  Amaterasu3D
)

install(TARGETS TechniqueRSM RUNTIME DESTINATION ${Amaterasu3D_BINARY_DIR})