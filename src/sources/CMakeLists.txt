# directories
set(PINKYPI_LINEARALGEBRA_DIR linearalgebra)
set(PINKYPI_THIRDPARTY_DIR thirdparty)
set(PINKYPI_CORE_DIR pinkycore)
set(PINKYPI_MAIN_DIR pinkypi)

# thirdparty settings
set(THIRDPARTY_SRCS
    ${PINKYPI_THIRDPARTY_DIR}/thirdparty_impl.cc
)
add_library(ThirdpartyLibs ${THIRDPARTY_SRCS})
target_include_directories(ThirdpartyLibs
    PUBLIC
    ${PINKYPI_THIRDPARTY_DIR}/stb
    ${PINKYPI_THIRDPARTY_DIR}/json
    ${PINKYPI_THIRDPARTY_DIR}/tinygltf
)

# LinearAlgebra
set(LINEARALGEBRA_SRCS
    ${PINKYPI_LINEARALGEBRA_DIR}/linearalgebra.h
    ${PINKYPI_LINEARALGEBRA_DIR}/constants.h
    ${PINKYPI_LINEARALGEBRA_DIR}/vector3.h
    ${PINKYPI_LINEARALGEBRA_DIR}/vector4.h
    ${PINKYPI_LINEARALGEBRA_DIR}/matrix4.h
    ${PINKYPI_LINEARALGEBRA_DIR}/vector3.inl
    ${PINKYPI_LINEARALGEBRA_DIR}/vector4.inl
    ${PINKYPI_LINEARALGEBRA_DIR}/matrix4.inl
)
add_library(LinearAlgebra INTERFACE)
# target_include_directories(LinearAlgebra INTERFACE ${PINKYPI_LINEARALGEBRA_DIR})
add_custom_target(LinearAlgebra_h SOURCES ${LINEARALGEBRA_SRCS})

# Core
list(APPEND PINKIPI_CORE_SRCS
    ${PINKYPI_CORE_DIR}/aabb.cc
)
list(APPEND PINKIPI_CORE_SRCS
    ${PINKYPI_CORE_DIR}/pptypes.h
    ${PINKYPI_CORE_DIR}/ray.h
    ${PINKYPI_CORE_DIR}/aabb.h
)

add_library(PinkyCore ${PINKIPI_CORE_SRCS})
target_include_directories(PinkyCore
    PRIVATE
    ${PINKYPI_SOURCE_DIR}
    ${PINKYPI_CORE_DIR}
    ${PINKYPI_THIRDPARTY_DIR}
)
# ${PINKYPI_LINEARALGEBRA_DIR}

# Application
set(PINLYPI_MAIN_SRCS
    ${PINKYPI_MAIN_DIR}/main.cc
)

# executable settings
add_executable(Pinkypi ${PINLYPI_MAIN_SRCS})
target_link_libraries(Pinkypi PUBLIC
    PinkyCore
    LinearAlgebra
)
target_include_directories(Pinkypi PRIVATE
    ${PINKYPI_SOURCE_DIR}
    ${PINKYPI_THIRDPARTY_DIR}
    ${PINKYPI_MAIN_DIR}
)
    # ${PINKYPI_LINEARALGEBRA_DIR}
    # ${PINKYPI_CORE_DIR}
