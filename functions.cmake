

function(_add_lib libaryName returnValue)
	set(libFolders "/usr/lib/")
	foreach(folder ${libFolders})
                file(GLOB output ${output} ${folder}lib${libaryName}*.so*)
	endforeach()
        set(${returnValue} ${output} PARENT_SCOPE)
endfunction()



function(_add_cxxFlag flag)
    set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} " ${flag} " PARENT_SCOPE)
endfunction()


