# tests/CMakeLists.txt

# add_subdirectory(coreTests)
# add_subdirectory(linearalgebraTests)
# add_subdirectory(pinkypiTests)

# Linear Algebra
set(LINEARALGEBRA_TEST_DIR linearalgebraTests)

set(LINEARALGEBRA_TESTS_SRCS
    ${LINEARALGEBRA_TEST_DIR}/Vector3Tests.cc
    ${LINEARALGEBRA_TEST_DIR}/Vector4Tests.cc
    ${LINEARALGEBRA_TEST_DIR}/Matrix4Tests.cc
)
source_group(linearalgebraTests FILES ${LINEARALGEBRA_TESTS_SRCS})

# Core
set(PINKYCORE_TEST_DIR coreTests)

set(PINKYCORE_TESTS_SRCS 
    ${PINKYCORE_TEST_DIR}/aabbTests.cc
)

source_group(coreTests FILES ${PINKYCORE_TESTS_SRCS})

# exectable
add_executable(PinkyPiTests
    testmain.cc
    testsupport.h
    ${LINEARALGEBRA_TESTS_SRCS}
    ${PINKYCORE_TESTS_SRCS}
)
target_link_libraries(PinkyPiTests PRIVATE
    LinearAlgebra
    PinkyCore
)
target_include_directories(PinkyPiTests PRIVATE
    ${PINKYPI_SOURCE_DIR}
    ${PINKYPI_THIRDPARTY_DIR}/Catch2/single_include
)

# source_group(linearalgebraTests REGULAR_EXPRESSION ${LINEARALGEBRA_TEST_DIR}/*)
