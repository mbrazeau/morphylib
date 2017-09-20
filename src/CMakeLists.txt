file(GLOB LIBSRCS
    "*.h"
    "*.hpp"
    "*.c"
)
include_directories(../include)

add_library(morphy STATIC ${LIBSRCS})

install(TARGETS morphy DESTINATION lib)
install(DIRECTORY ../include/ DESTINATION include)

