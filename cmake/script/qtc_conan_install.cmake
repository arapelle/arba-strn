include(CMakePrintHelpers)

cmake_print_variables(CMAKE_CURRENT_SOURCE_DIR)
cmake_print_variables(QTC_BUILD_DIR)
cmake_print_variables(CMAKE_BUILD_TYPE)

if("${CMAKE_CURRENT_SOURCE_DIR}" STREQUAL "${CMAKE_SOURCE_DIR}")
    execute_process(COMMAND conan install ${CMAKE_CURRENT_SOURCE_DIR} --output-folder ${QTC_BUILD_DIR} --build=missing -s build_type=${CMAKE_BUILD_TYPE}
            COMMAND_ECHO STDOUT
    )
endif()
