ADD_CUSTOM_TARGET (testAll)

SET(TEST_Amaterasu3D
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

ADD_EXECUTABLE(TestAmaterasu3D ${TEST_Amaterasu3D})

target_link_libraries(
	TestAmaterasu3D
	Amaterasu3D
	${Boost_LIBRARIES}
)

install(TARGETS TestAmaterasu3D RUNTIME DESTINATION ${PROJECT_BUILD_PATH})

add_custom_command(
	TARGET testAll POST_BUILD 
	COMMAND ./TestAmaterasu3D
	WORKING_DIRECTORY ${PROJECT_BUILD_PATH})
	
