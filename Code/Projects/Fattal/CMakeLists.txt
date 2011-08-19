SET(Fattal_SRCS
	main.cpp
)

ADD_EXECUTABLE(Fattal ${Fattal_SRCS})

target_link_libraries(
  Fattal
  Amaterasu3D
)

install(TARGETS Fattal RUNTIME DESTINATION ${Amaterasu3D_BINARY_DIR})