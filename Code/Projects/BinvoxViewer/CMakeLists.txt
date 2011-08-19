SET(BinvoxViewer_SRCS
	main.cpp
)

ADD_EXECUTABLE(BinvoxViewer ${BinvoxViewer_SRCS})

target_link_libraries(
  BinvoxViewer
  Amaterasu3D
)

install(TARGETS BinvoxViewer RUNTIME DESTINATION ${Amaterasu3D_BINARY_DIR})