
function(add_cpp_library_project_examples)
    set(example_output_dir ${CMAKE_BUILD_TYPE})

    get_property(project_targets DIRECTORY ${PROJECT_SOURCE_DIR} PROPERTY BUILDSYSTEM_TARGETS)
    foreach(target ${project_targets})
        get_target_property(target_type ${target} TYPE)
        if(${target_type} STREQUAL "SHARED_LIBRARY" OR ${target_type} STREQUAL "STATIC_LIBRARY")
            set(tested_lib ${target})
            break()
        endif()
    endforeach()

    if(tested_lib)
        file(GLOB cpp_program_files "*.cpp")
        foreach(filename ${cpp_program_files})
            get_filename_component(example_program_name ${filename} NAME_WE)
            add_executable(${example_program_name} ${filename})
            set_target_properties(${example_program_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${example_output_dir})
            target_link_libraries(${example_program_name} $<TARGET_NAME:${tested_lib}>)
            target_include_directories(${example_program_name} PUBLIC
                $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
                $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>)
        endforeach()
    endif()
endfunction()

function(add_cpp_library_project_tests)
    set(test_output_dir ${CMAKE_BUILD_TYPE})

    get_property(project_targets DIRECTORY ${PROJECT_SOURCE_DIR} PROPERTY BUILDSYSTEM_TARGETS)
    foreach(target ${project_targets})
        get_target_property(target_type ${target} TYPE)
        if(${target_type} STREQUAL "SHARED_LIBRARY" OR ${target_type} STREQUAL "STATIC_LIBRARY")
            set(tested_lib ${target})
            break()
        endif()
    endforeach()

    if(tested_lib)
        file(GLOB cpp_program_files "*.cpp")
        foreach(filename ${cpp_program_files})
            get_filename_component(test_program_name ${filename} NAME_WE)
            add_executable(${test_program_name} ${filename})
            set_target_properties(${test_program_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${test_output_dir})
            target_link_libraries(${test_program_name} $<TARGET_NAME:${tested_lib}>)
            target_include_directories(${test_program_name} PUBLIC
                $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
                $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>)
            add_test("${test_program_name}" ${test_output_dir}/${test_program_name})
        endforeach()
    endif()
endfunction()
