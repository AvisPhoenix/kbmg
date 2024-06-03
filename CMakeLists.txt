cmake_minimum_required(VERSION 3.5)

project(kbmg LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)


include(FetchContent)

FetchContent_Declare(
  yaml-cpp
  GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
  GIT_TAG master # Can be a tag (yaml-cpp-x.x.x), a commit hash, or a branch name (master)
)
FetchContent_GetProperties(yaml-cpp)

if(NOT yaml-cpp_POPULATED)
  message(STATUS "Fetching yaml-cpp...")
  FetchContent_Populate(yaml-cpp)
  add_subdirectory(${yaml-cpp_SOURCE_DIR} ${yaml-cpp_BINARY_DIR})
endif()

add_executable(kbmg main.cpp
    procrunner.h procrunner.cpp
    parsers/kubepodsparser.h parsers/kubepodsparser.cpp
    commands/kbgetpods.h commands/kbgetpods.cpp
    commands/kbsavelog.h commands/kbsavelog.cpp
    commands/builder/kbgobash.h
    commands/builder/kbgetlog.h
    commands/zipfiles.h
    commands/removefiles.h
    commands/builder/kbgoshell.h

    commands/kbgoenvironment.h
    parsers/configfile.h parsers/configfile.cpp
    logger.h logger.cpp
    kubernetesmanager.h kubernetesmanager.cpp
    commandlineinterface.h commandlineinterface.cpp

)

#If pstream does not found add the next line
#target_include_directories(kbmg PRIVATE include)


target_link_libraries(kbmg PRIVATE Threads::Threads)
target_link_libraries(kbmg PUBLIC yaml-cpp::yaml-cpp)

include(GNUInstallDirs)
install(TARGETS kbmg
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
