file(GLOB LIBSRCS
    "*.h"
    "*.c"
)
include_directories(../../include ${R_INCLUDE_DIR})
add_library(RMorphyex ${LIBSRCS})
target_link_libraries(RMorphyex morphy)

install(TARGETS RMorphyex DESTINATION lib)
