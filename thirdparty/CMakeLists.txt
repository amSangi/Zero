# Assimp
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Don't build Assimp as a shared lib" FORCE)
set(ASSIMP_NO_EXPORT ON CACHE BOOL "Don't build Assimp exporters" FORCE)
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "Don't build Assimp tools" FORCE)
set(ASSIMP_BUILD_ASSIMP_VIEW OFF CACHE BOOL "Don't build Assimp view" FORCE)
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "Don't build Assimp tests" FORCE)
set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT OFF CACHE BOOL "Explicitly set the importers to use" FORCE)
set(USED_MODEL_FORMATS
        ASSIMP_BUILD_OBJ_IMPORTER
        ASSIMP_BUILD_COLLADA_IMPORTER
        ASSIMP_BUILD_FBX_IMPORTER
        )
foreach(model_format ${USED_MODEL_FORMATS})
    set(${model_format} ON CACHE BOOL "Build used model formats" FORCE)
endforeach()
add_subdirectory(assimp)

# Entt
add_subdirectory(entt)

# GoogleTest
add_subdirectory(googletest)