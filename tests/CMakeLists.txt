file(GLOB SRCS
    "*.h"
    "*.c"
    "ctreelib/*.h"
    "ctreelib/*.c"
)
include_directories(../include ../src)

add_executable(morphytest ${SRCS})
target_link_libraries(morphytest morphy)
target_link_libraries(morphytest m)

install(TARGETS morphytest DESTINATION bin)

