SET( CRIMILD_INCLUDE_DIRECTORIES 
	${CRIMILD_SOURCE_DIR}/core/src 
	${CRIMILD_SOURCE_DIR}/scripting/src 
	${CRIMILD_SOURCE_DIR}/physics/src 
	${CRIMILD_SOURCE_DIR}/opengl/src
	${CRIMILD_SOURCE_DIR}/third-party/lua-5.2.3/src
	${CRIMILD_SOURCE_DIR}/third-party/glfw/include
	${CRIMILD_SOURCE_DIR}/third-party/glew/include )

SET( CRIMILD_LIBRARY_LINK_LIBRARIES 
	crimild_core
	crimild_scripting
	crimild_opengl
	glfw )

IF ( CRIMILD_ENABLE_PHYSICS )
	SET( CRIMILD_INCLUDE_DIRECTORIES
		 ${CRIMILD_INCLUDE_DIRECTORIES}
		 ${CRIMILD_SOURCE_DIR}/third-party/bullet-2.82-r2704/src )

	SET( CRIMILD_LIBRARY_LINK_LIBRARIES 
		 ${CRIMILD_LIBRARY_LINK_LIBRARIES} 
		 crimild_physics )
ENDIF ( CRIMILD_ENABLE_PHYSICS )

INCLUDE( ModuleBuildLibrary )
