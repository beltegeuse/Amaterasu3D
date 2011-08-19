SET(Unittests_SRCS
	test_main.cpp
	test_helper.cpp
	Math/test_vector2.cpp
	Math/test_vector3.cpp
	Math/test_matrix4.cpp
	Math/test_quaternion.cpp
	
	# Old test version
	#System/test_mediamanager.cpp
)

INCLUDE_DIRECTORIES(
	${CMAKE_CURRENT_SOURCE_DIR}
	../../Libs/Amaterasu3D
	../../Libs/TinyXML
)

ADD_EXECUTABLE(TestAmaterasu3D ${Unittests_SRCS})

target_link_libraries(
	TestAmaterasu3D
	Amaterasu3D
)

install(TARGETS TestAmaterasu3D RUNTIME DESTINATION ${Amaterasu3D_BINARY_DIR})
	
