cmake_minimum_required(VERSION 3.0.0)
project(compiler VERSION 0.1.0)

include(CTest)
enable_testing()

include_directories(
    "."
    "interface"
    "log"
    "src"
    "src/backend"
    "src/elements"
    "src/elements/2"
    "src/elements/3"
    "src/elements/4"
    "src/elements/5"
    "src/elements/7"
    "src/elements/8"
    "src/elements/10"
    "src/elements/11"
    "src/grammar"
    "src/lex"
    "src/modify"
    "src/modify/dag"
    "src/modify/flow"
    "src/semantics"
    "test"
    )

file(GLOB_RECURSE ALL_SRC
    *.cpp
)

add_executable(compiler ${ALL_SRC})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
