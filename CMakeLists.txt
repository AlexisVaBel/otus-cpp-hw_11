cmake_minimum_required(VERSION 3.5)


project(async)

if ($ENV{TRAVIS_BUILD_NUMBER})
 project(${PROJECT_NAME} VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})
else ($ENV{TRAVIS_BUILD_NUMBER})
 project(${PROJECT_NAME}  VERSION 0.0.1)
endif ($ENV{TRAVIS_BUILD_NUMBER})


find_package( Threads )



set(CMAKE_CXX_STANDARD 14)

add_library(${PROJECT_NAME} SHARED src_async/async.cpp src_async/async.h context/contextholder.h context/contextitm.h datahodor/bulkholder.h consumer/consumtofile.h consumer/consumtoterminal.h consumer/consumtofile.cpp consumer/consumtoterminal.cpp processor/cmdacceptor.h producer/bulkproducer.h producer/bulkproducer.cpp)

#add_executable(${PROJECT_NAME} main.cpp)


set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
    )

target_link_libraries(${PROJECT_NAME}
        ${CMAKE_THREAD_LIBS_INIT}
)

install (TARGETS ${PROJECT_NAME}
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    COMPONENT library)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "$ENV{TRAVIS_BUILD_NUMBER}")

set(CPACK_PACKAGE_CONTACT alexisvabel@gmail.com)

set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
include(CPack)
