INCLUDE_DIRECTORIES(
	${CMAKE_CURRENT_SOURCE_DIR}
)

ADD_EXECUTABLE(TechniqueLPV main.cpp)

target_link_libraries(
  TechniqueLPV
  Amaterasu3D
)

install(TARGETS TechniqueLPV RUNTIME DESTINATION ${Amaterasu3D_BINARY_DIR})