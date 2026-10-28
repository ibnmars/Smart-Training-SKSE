if(MSVC)
    add_compile_options("$<$<CONFIG:Release>:/d1trimfile:${PROJECT_SOURCE_DIR}/>")
endif()
