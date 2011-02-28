ADD_CUSTOM_TARGET (testAll)

SET(TEST_Amaterasu3D
	test_main.cpp
	Math/test_vector2.cpp
	Math/test_vector3.cpp
)

INCLUDE_DIRECTORIES(
	${CMAKE_CURRENT_SOURCE_DIR}
	../../Libs/Amaterasu3D
)

ADD_EXECUTABLE(TestAmaterasu3D ${TEST_Amaterasu3D})

target_link_libraries(
	TestAmaterasu3D
	Amaterasu3D
	C:/MinGW/lib/libboost_unit_test_framework-mgw45-mt-1_46.a
	C:/MinGW/lib/libboost_unit_test_framework-mgw45-mt-d-1_46.a
	${Boost_LIBRARIES}
)

install(TARGETS TestAmaterasu3D RUNTIME DESTINATION ${PROJECT_BUILD_PATH})

add_custom_command(
	TARGET testAll POST_BUILD 
	COMMAND ./TestAmaterasu3D
	WORKING_DIRECTORY ${PROJECT_BUILD_PATH})
	
