CMAKE_MINIMUM_REQUIRED( VERSION 2.8.10 FATAL_ERROR )

IF ( APPLE )
	# Enable C++11 features
	SET( CMAKE_CXX_FLAGS "-std=c++11 -stdlib=libc++ -U__STRICT_ANSI__" )

	set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "c++0x")
	set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++")
ELSE ( APPLE )
	SET( CMAKE_CXX_FLAGS "-std=c++11 -static-libgcc -static-libstdc++ -static -U__STRICT_ANSI__" )
ENDIF ( APPLE )

PROJECT( CRIMILD )

# Register custom modules
SET ( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CRIMILD_SOURCE_DIR}/CMakeTools" )

INCLUDE( MacroExtractArchive )

ADD_DEFINITIONS( -DCRIMILD_VERSION_MAJOR=4 )
ADD_DEFINITIONS( -DCRIMILD_VERSION_MINOR=6 )
ADD_DEFINITIONS( -DCRIMILD_VERSION_PATCH=0 )

# Configuration options
OPTION( CRIMILD_ENABLE_TESTS "Would you like to enable unit tests?" OFF )
IF ( CRIMILD_ENABLE_TESTS )
	ENABLE_TESTING()

	ADD_DEFINITIONS( -DGTEST_LANG_CXX11=1 )
	ADD_DEFINITIONS( -DGTEST_USE_OWN_TR1_TUPLE=1 )

	EXTRACT_ARCHIVE( gmock-1.6.0.zip gmock-1.6.0 ${CRIMILD_SOURCE_DIR}/third-party )

	ADD_SUBDIRECTORY( ${CRIMILD_SOURCE_DIR}/third-party/gmock-1.6.0 "${CMAKE_CURRENT_BINARY_DIR}/third-party/gmock-1.6.0" )
ENDIF ( CRIMILD_ENABLE_TESTS )

# Add core sources
ADD_SUBDIRECTORY( core )
ADD_SUBDIRECTORY( raytracing )

OPTION( CRIMILD_ENABLE_IMPORT "Build importers?" OFF )
IF ( CRIMILD_ENABLE_IMPORT )
	ADD_DEFINITIONS( -DCRIMILD_ENABLE_IMPORT=1 )
	SET( ASSIMP_BUILD_STATIC_LIB ON CACHE BOL "Build Shared libs" )
	SET( BUILD_SHARED_LIBS OFF CACHE BOOL "Build Shared Libs" )
	SET( ASSIMP_BUILD_TESTS OFF CACHE BOOL "Build AssImp tests" )
	ADD_SUBDIRECTORY( ${CRIMILD_SOURCE_DIR}/third-party/assimp )
	ADD_SUBDIRECTORY( import )
ENDIF ( CRIMILD_ENABLE_IMPORT )

OPTION( CRIMILD_ENABLE_AL "Would you like to build the crimild-al extension?" OFF )
IF ( CRIMILD_ENABLE_AL )
	ADD_DEFINITIONS( -DCRIMILD_ENABLE_AL=1 )
	ADD_SUBDIRECTORY( al )
ENDIF ( CRIMILD_ENABLE_AL )

OPTION( CRIMILD_ENABLE_PHYSICS "Would you like to build the crimild-physics extension?" OFF )
IF ( CRIMILD_ENABLE_PHYSICS )
	ADD_DEFINITIONS( -DCRIMILD_ENABLE_PHYSICS=1 )
	ADD_SUBDIRECTORY( physics )
ENDIF ( CRIMILD_ENABLE_PHYSICS )

# Add scripting sources
OPTION( CRIMILD_ENABLE_SCRIPTING "Would you like to build the crimild-scripting extension?" OFF )
iF ( CRIMILD_ENABLE_SCRIPTING )
	# Extract lua sources if needed
	EXTRACT_ARCHIVE( lua-5.2.3.tar.gz lua-5.2.3 ${CRIMILD_SOURCE_DIR}/third-party )

	OPTION( CRIMILD_SCRIPTING_LOG_VERBOSE "Enable verbose logs for scripting system" OFF )
	IF ( CRIMILD_SCRIPTING_LOG_VERBOSE )
		ADD_DEFINITIONS( -DCRIMILD_SCRIPTING_LOG_VERBOSE=1 )
	ENDIF ()
	ADD_SUBDIRECTORY( scripting )
ENDIF ()

OPTION( CRIMILD_ENABLE_GLFW "Would you like to build the crimild-glfw extension?" OFF )
IF ( CRIMILD_ENABLE_GLFW )
	ADD_DEFINITIONS( -DCRIMILD_ENABLE_GLFW=1 )

	SET( GLFW_BUILD_DOCS OFF CACHE BOOL "Build the GLFW documentation" )
	SET( GLFW_BUILD_EXAMPLES OFF CACHE BOOL "Build the GLFW example programs" )
	SET( GLFW_BUILD_TESTS OFF CACHE BOOL "Build the GLFW test programs" )
	
	ADD_SUBDIRECTORY( ${CRIMILD_SOURCE_DIR}/third-party/glfw )

	ADD_SUBDIRECTORY( opengl )
	ADD_SUBDIRECTORY( glfw )
	
ENDIF ( CRIMILD_ENABLE_GLFW )

OPTION( CRIMILD_BUILD_TOOLS "Build tools?" OFF )
IF ( CRIMILD_BUILD_TOOLS )
	ADD_SUBDIRECTORY( tools )
ENDIF ( CRIMILD_BUILD_TOOLS )

# Options for app bundles
OPTION( CRIMILD_BUNDLE_APPS "Enable bundles for apps" OFF )

