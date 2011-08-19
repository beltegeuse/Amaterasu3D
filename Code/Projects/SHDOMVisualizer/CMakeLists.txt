SET(SHDOMVisualizer_SRCS
	main.cpp
)

INCLUDE_DIRECTORIES(
	${CMAKE_CURRENT_SOURCE_DIR}
)

ADD_EXECUTABLE(SHDOMVisualizer ${SHDOMVisualizer_SRCS})

target_link_libraries(
  SHDOMVisualizer
  Amaterasu3D
)

install(TARGETS SHDOMVisualizer RUNTIME DESTINATION ${Amaterasu3D_BINARY_DIR})