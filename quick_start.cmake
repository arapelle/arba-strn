# cmake -P quick_install.cmake

set(project "strn")

if(WIN32)
    set(temp_dir $ENV{TEMP}\\${project}-build)
elseif(UNIX)
    set(temp_dir /tmp/${project}-build)
else()
    message(FATAL_ERROR "No temporary directory found!")
endif()

set(src_dir ${CMAKE_CURRENT_LIST_DIR})
set(build_dir ${temp_dir})

execute_process(COMMAND ${CMAKE_COMMAND} -S ${src_dir} -B ${build_dir}
    RESULT_VARIABLE cmd_res)
if(NOT cmd_res EQUAL 0)
    file(REMOVE_RECURSE ${build_dir})
else()
    execute_process(COMMAND ${CMAKE_COMMAND} --build ${build_dir}
        RESULT_VARIABLE cmd_res)
    if(NOT cmd_res EQUAL 0)
        file(REMOVE_RECURSE ${build_dir})
    else()
        execute_process(COMMAND ${CMAKE_COMMAND} --install ${build_dir})
    endif()
endif()
