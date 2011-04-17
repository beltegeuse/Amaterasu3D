
IF(CMAKE_BUILD_TYPE MATCHES debug)
	ADD_DEFINITIONS("-Wall -W -D_DEBUG") #Pour afficher les warning lors de la compilation
    #SET(CMAKE_CXX_FLAGS_DEBUG ${CMAKE_CXX_FLAGS_DEBUG} " -D_DEBUG -ansi -Wall -Wextra -Wfatal-errors -pedantic -Wmain -Wswitch-default -Wunreachable-code -Winit-self -Wshadow -Wwrite-strings -Wuninitialized") #Pour afficher les warning lors de la compilation
ENDIF(CMAKE_BUILD_TYPE MATCHES debug)

SET(LIB_OPENGLPWM_SRCS
	Exceptions.cpp
	Window.cpp
	WindowImpl.cpp
	Win32/WindowImplWin32.cpp
)

INCLUDE_DIRECTORIES(
	${CMAKE_CURRENT_SOURCE_DIR}
	../TinyXML
)

ADD_LIBRARY(OpenGLPWM SHARED ${LIB_OPENGLPWM_SRCS})

target_link_libraries(
  OpenGLPWM
  ${OPENGL_LIBRARY}
  ${GLEW_LIBRARY}
)

install(TARGETS OpenGLPWM 
  RUNTIME DESTINATION ${PROJECT_BUILD_PATH}
  LIBRARY DESTINATION ${PROJECT_BUILD_PATH}
  ARCHIVE DESTINATION ${PROJECT_BUILD_PATH}
)