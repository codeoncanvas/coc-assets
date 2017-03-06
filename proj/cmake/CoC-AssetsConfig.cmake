if( NOT TARGET CoC-Assets )
    get_filename_component( COC-ASSETS_SOURCE_PATH "${CMAKE_CURRENT_LIST_DIR}/../../src" ABSOLUTE )

    FILE(GLOB COC-ASSETS_SOURCES ${COC-ASSETS_SOURCE_PATH}/*.cpp)

    add_library( CoC-Assets ${COC-ASSETS_SOURCES} )
    target_include_directories( CoC-Assets PUBLIC "${COC-ASSETS_SOURCE_PATH}" )

    # If Cinder included from this block:

    target_include_directories( CoC-Assets SYSTEM BEFORE PUBLIC "${CINDER_PATH}/include" )

    if( NOT TARGET cinder )
        include( "${CINDER_PATH}/proj/cmake/configure.cmake" )
        find_package( cinder REQUIRED PATHS
                "${CINDER_PATH}/${CINDER_LIB_DIRECTORY}"
                "$ENV{CINDER_PATH}/${CINDER_LIB_DIRECTORY}" )
    endif()

    #    target_link_libraries( ${BLOCK_NAME} PRIVATE cinder )

    # Add CoC-Core block as a dependency
    get_filename_component( COC-CORE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/../../../CoC-Core/proj/cmake" ABSOLUTE )
    find_package( CoC-Core REQUIRED PATHS "${COC-CORE_MODULE_PATH}" )
    if (CoC-Core_FOUND)
        add_dependencies( CoC-Assets CoC-Core )
        target_link_libraries( CoC-Assets PUBLIC CoC-Core)
    endif()

    target_link_libraries( CoC-Assets PRIVATE cinder)

endif()