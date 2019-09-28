if(CMAKE_COMPILER_IS_GNUCXX)
  execute_process(COMMAND ${CMAKE_C_COMPILER} -dumpversion OUTPUT_VARIABLE GCC_VERSION)
  message(STATUS "Using GNU C++ compiler, version ${GCC_VERSION}")
  if(GCC_VERSION VERSION_GREATER 8.0 OR GCC_VERSION VERSION_EQUAL 8.0)
    if(GCC_VERSION VERSION_LESS 8.2)
      # NOTE: workaround for a problem similar to https://issues.apache.org/jira/browse/THRIFT-4584
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-tree-vrp -fno-inline -fno-tree-fre")
      set(CMAKE_C_FLAGS_RELEASE "-g -O0")
      set(CMAKE_CXX_FLAGS_RELEASE "-g -O0")
      set(CMAKE_C_FLAGS_RELWITHDEBINFO "-g -O0")
      set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-g -O0")
    endif()
  endif()
  if(BUILD_SHARED)
    set(CMAKE_CXX_FLAGS "-fvisibility=hidden ${CMAKE_CXX_FLAGS}")
  endif()
  if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    set(CMAKE_CXX_FLAGS "-fPIC -rdynamic ${CMAKE_CXX_FLAGS}")
  endif()
  if(NOT ${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    set(CMAKE_CXX_FLAGS "-ldl ${CMAKE_CXX_FLAGS}")
  endif()
elseif(${CMAKE_CXX_COMPILER_ID} MATCHES "Clang")
  execute_process(COMMAND ${CMAKE_C_COMPILER} --version OUTPUT_VARIABLE CLANG_VERSION)
  message(STATUS "Using LLVM/Clang C++ compiler, version info: ${CLANG_VERSION}")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
  if(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin" OR USE_LIBCPP)
    find_library(LIBCPP NAMES libc++.so libc++.so.1.0 libc++.dylib OPTIONAL)
    if(LIBCPP)
      message(STATUS "Using native Clang's C++ standard library: ${LIBCPP}")
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
      add_definitions("-DHAVELIBCPP")
    endif()
  endif()
elseif(MSVC)
  message(STATUS "Using VC++ compiler: ${CMAKE_CXX_COMPILER_VERSION}")
  set(CMAKE_CXX_FLAGS "-D_SCL_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_WARNINGS ${CMAKE_CXX_FLAGS}")
endif()
