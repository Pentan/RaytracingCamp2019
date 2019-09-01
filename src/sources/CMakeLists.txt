# directories
set(PINKYPI_LINEARALGEBRA_DIR linearalgebra)
set(PINKYPI_CORE_DIR pinkycore)
set(PINKYPI_MAIN_DIR pinkypi)

# LinearAlgebra
set(LINEARALGEBRA_SRCS
    ${PINKYPI_LINEARALGEBRA_DIR}/linearalgebra.h
    ${PINKYPI_LINEARALGEBRA_DIR}/constants.h
    ${PINKYPI_LINEARALGEBRA_DIR}/vector3.h
    ${PINKYPI_LINEARALGEBRA_DIR}/vector4.h
    ${PINKYPI_LINEARALGEBRA_DIR}/matrix4.h
    ${PINKYPI_LINEARALGEBRA_DIR}/quaternion.h
    ${PINKYPI_LINEARALGEBRA_DIR}/vector3.inl
    ${PINKYPI_LINEARALGEBRA_DIR}/vector4.inl
    ${PINKYPI_LINEARALGEBRA_DIR}/matrix4.inl
    ${PINKYPI_LINEARALGEBRA_DIR}/quaternion.inl
)
add_library(LinearAlgebra INTERFACE)
# target_include_directories(LinearAlgebra INTERFACE ${PINKYPI_LINEARALGEBRA_DIR})
add_custom_target(LinearAlgebra_h SOURCES ${LINEARALGEBRA_SRCS})

# Core
list(APPEND PINKIPI_CORE_SRCS
    ${PINKYPI_CORE_DIR}/aabb.cc
    ${PINKYPI_CORE_DIR}/config.cc
    ${PINKYPI_CORE_DIR}/framebuffer.cc
    ${PINKYPI_CORE_DIR}/postprocessor.cc
    ${PINKYPI_CORE_DIR}/renderer.cc
    ${PINKYPI_CORE_DIR}/renderscheduler.cc
    ${PINKYPI_CORE_DIR}/scene.cc
    ${PINKYPI_CORE_DIR}/material.cc
    ${PINKYPI_CORE_DIR}/texture.cc
    ${PINKYPI_CORE_DIR}/mesh.cc
    ${PINKYPI_CORE_DIR}/light.cc
    ${PINKYPI_CORE_DIR}/camera.cc
    ${PINKYPI_CORE_DIR}/node.cc
    ${PINKYPI_CORE_DIR}/assetlibrary.cc
    ${PINKYPI_CORE_DIR}/bvh.cc
)
list(APPEND PINKIPI_CORE_SRCS
    ${PINKYPI_CORE_DIR}/pptypes.h
    ${PINKYPI_CORE_DIR}/ray.h
    ${PINKYPI_CORE_DIR}/aabb.h
    ${PINKYPI_CORE_DIR}/config.h
    ${PINKYPI_CORE_DIR}/framebuffer.h
    ${PINKYPI_CORE_DIR}/postprocessor.h
    ${PINKYPI_CORE_DIR}/renderer.h
    ${PINKYPI_CORE_DIR}/renderscheduler.h
    ${PINKYPI_CORE_DIR}/scene.h
    ${PINKYPI_CORE_DIR}/material.h
    ${PINKYPI_CORE_DIR}/texture.h
    ${PINKYPI_CORE_DIR}/mesh.h
    ${PINKYPI_CORE_DIR}/light.h
    ${PINKYPI_CORE_DIR}/camera.h
    ${PINKYPI_CORE_DIR}/node.h
    ${PINKYPI_CORE_DIR}/assetlibrary.h
    ${PINKYPI_CORE_DIR}/bvh.h
    ${PINKYPI_CORE_DIR}/random.h
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
list(APPEND PINLYPI_APP_SRCS
    ${PINKYPI_MAIN_DIR}/sceneloader.cc
)
list(APPEND PINLYPI_APP_SRCS
    ${PINKYPI_MAIN_DIR}/sceneloader.h
)
add_library(PinkyPiApp ${PINLYPI_APP_SRCS})
target_include_directories(PinkyPiApp
    PRIVATE
    ${PINKYPI_SOURCE_DIR}
    ${PINKYPI_CORE_DIR}
    ${PINKYPI_THIRDPARTY_DIR}
)

# executable settings
add_executable(PinkyPi
    ${PINKYPI_MAIN_DIR}/main.cc
)
target_link_libraries(PinkyPi PUBLIC
    PinkyPiApp
    PinkyCore
    LinearAlgebra
    ThirdpartyLibs
)
target_include_directories(PinkyPi PRIVATE
    ${PINKYPI_SOURCE_DIR}
    ${PINKYPI_THIRDPARTY_DIR}
    ${PINKYPI_MAIN_DIR}
)
    # ${PINKYPI_LINEARALGEBRA_DIR}
    # ${PINKYPI_CORE_DIR}

# Friendship is magic!
set(RRT_CORE_DIR rrtcore)
set(PWR_MAIN_DIR pinkywrrt)

list(APPEND RRT_CORE_SRCS
    ${RRT_CORE_DIR}/aabb.cc
    ${RRT_CORE_DIR}/aabb.h
    ${RRT_CORE_DIR}/abstracttonemapper.cc
    ${RRT_CORE_DIR}/abstracttonemapper.h
    ${RRT_CORE_DIR}/asset.h
    ${RRT_CORE_DIR}/bsdfs
    ${RRT_CORE_DIR}/bvhnode.cc
    ${RRT_CORE_DIR}/bvhnode.h
    ${RRT_CORE_DIR}/camera.cc
    ${RRT_CORE_DIR}/camera.h
    ${RRT_CORE_DIR}/commandqueue.cc
    ${RRT_CORE_DIR}/commandqueue.h
    ${RRT_CORE_DIR}/eduptscene.cc
    ${RRT_CORE_DIR}/eduptscene.h
    ${RRT_CORE_DIR}/framebuffer.cc
    ${RRT_CORE_DIR}/framebuffer.h
    ${RRT_CORE_DIR}/geometries
    ${RRT_CORE_DIR}/intersection.cc
    ${RRT_CORE_DIR}/intersection.h
    ${RRT_CORE_DIR}/materials
    ${RRT_CORE_DIR}/r1htypes.h
    ${RRT_CORE_DIR}/random.h
    ${RRT_CORE_DIR}/ray.h
    ${RRT_CORE_DIR}/renderer.cc
    ${RRT_CORE_DIR}/renderer.h
    ${RRT_CORE_DIR}/scene.cc
    ${RRT_CORE_DIR}/scene.h
    ${RRT_CORE_DIR}/sceneobject.cc
    ${RRT_CORE_DIR}/sceneobject.h
    ${RRT_CORE_DIR}/scenesupport.cc
    ${RRT_CORE_DIR}/scenesupport.h
    ${RRT_CORE_DIR}/tonemapper.cc
    ${RRT_CORE_DIR}/tonemapper.h
)
# ${RRT_CORE_DIR}/wavefrontobj.cc
# ${RRT_CORE_DIR}/wavefrontobj.h
# ${RRT_CORE_DIR}/xmlobjloader.cc
# ${RRT_CORE_DIR}/xmlobjloader.h
# ${RRT_CORE_DIR}/xmlscene.cc
# ${RRT_CORE_DIR}/xmlscene.h
list(APPEND RRT_CORE_SRCS
    ${RRT_CORE_DIR}/bsdfs/bsdf.cc
    ${RRT_CORE_DIR}/bsdfs/bsdf.h
    ${RRT_CORE_DIR}/bsdfs/bsdf_include.h
    ${RRT_CORE_DIR}/bsdfs/burley.cc
    ${RRT_CORE_DIR}/bsdfs/burley.h
    ${RRT_CORE_DIR}/bsdfs/fullreflection.cc
    ${RRT_CORE_DIR}/bsdfs/fullreflection.h
    ${RRT_CORE_DIR}/bsdfs/fullrefraction.cc
    ${RRT_CORE_DIR}/bsdfs/fullrefraction.h
    ${RRT_CORE_DIR}/bsdfs/ggx.cc
    ${RRT_CORE_DIR}/bsdfs/ggx.h
    ${RRT_CORE_DIR}/bsdfs/lambert.cc
    ${RRT_CORE_DIR}/bsdfs/lambert.h
    ${RRT_CORE_DIR}/bsdfs/translucent.cc
    ${RRT_CORE_DIR}/bsdfs/translucent.h
)
list(APPEND RRT_CORE_SRCS
    ${RRT_CORE_DIR}/geometries/geometry.h
    ${RRT_CORE_DIR}/geometries/geometry_include.h
    ${RRT_CORE_DIR}/geometries/mesh.cc
    ${RRT_CORE_DIR}/geometries/mesh.h
    ${RRT_CORE_DIR}/geometries/sphere.cc
    ${RRT_CORE_DIR}/geometries/sphere.h
)
# ${RRT_CORE_DIR}/geometries/aabbgeometry.cc
# ${RRT_CORE_DIR}/geometries/aabbgeometry.h
list(APPEND RRT_CORE_SRCS
    ${RRT_CORE_DIR}/materials/fineglass.cpp
    ${RRT_CORE_DIR}/materials/fineglass.h
    ${RRT_CORE_DIR}/materials/lightmaterial.cpp
    ${RRT_CORE_DIR}/materials/lightmaterial.h
    ${RRT_CORE_DIR}/materials/material.cpp
    ${RRT_CORE_DIR}/materials/material.h
    ${RRT_CORE_DIR}/materials/material_include.h
    ${RRT_CORE_DIR}/materials/singlebsdfmaterial.cpp
    ${RRT_CORE_DIR}/materials/singlebsdfmaterial.h
    ${RRT_CORE_DIR}/materials/skymaterial.cc
    ${RRT_CORE_DIR}/materials/skymaterial.h
    ${RRT_CORE_DIR}/materials/sppmaterial.cpp
    ${RRT_CORE_DIR}/materials/sppmaterial.h
)
list(APPEND RRT_CORE_SRCS
    ${RRT_CORE_DIR}/textures/imagetexture.cc
    ${RRT_CORE_DIR}/textures/imagetexture.h
    ${RRT_CORE_DIR}/textures/noisetextures.cc
    ${RRT_CORE_DIR}/textures/noisetextures.h
    ${RRT_CORE_DIR}/textures/texture.cc
    ${RRT_CORE_DIR}/textures/texture.h
    ${RRT_CORE_DIR}/textures/texture_include.h
)
list(APPEND RRT_CORE_SRCS
    ${RRT_CORE_DIR}/vqmmath/constants.h
    ${RRT_CORE_DIR}/vqmmath/matrix4.h
    ${RRT_CORE_DIR}/vqmmath/matrix4.inl
    ${RRT_CORE_DIR}/vqmmath/vector3.h
    ${RRT_CORE_DIR}/vqmmath/vector3.inl
    ${RRT_CORE_DIR}/vqmmath/vector4.h
    ${RRT_CORE_DIR}/vqmmath/vector4.inl
)

add_library(RrTCore ${RRT_CORE_SRCS})
target_include_directories(RrTCore
    PRIVATE
    ${RRT_CORE_DIR}
    ${PINKYPI_THIRDPARTY_DIR}
)
# ${PINKYPI_SOURCE_DIR}
# ${PINKYPI_CORE_DIR}

add_executable(PinkyWithRrT
    ${PWR_MAIN_DIR}/main.cc
)
target_link_libraries(PinkyWithRrT
    PUBLIC
    RrTCore
    ThirdpartyLibs
)
target_include_directories(PinkyWithRrT
    PRIVATE
    ${RRT_CORE_DIR}
    ${PINKYPI_THIRDPARTY_DIR}
)
# ${PINKYPI_SOURCE_DIR}
# ${PINKYPI_MAIN_DIR}
