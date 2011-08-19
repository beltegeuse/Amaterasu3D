INCLUDE_DIRECTORIES(
	${CMAKE_CURRENT_SOURCE_DIR}
)

ADD_EXECUTABLE(TechniqueShadowMap main.cpp)

target_link_libraries(
  TechniqueShadowMap
  Amaterasu3D
)

install(TARGETS TechniqueShadowMap RUNTIME DESTINATION ${Amaterasu3D_BINARY_DIR})