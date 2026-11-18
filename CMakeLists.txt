cmake_minimum_required(VERSION 3.25)

# vcpkg reads the toolchain and triplets during project().
if(NOT CMAKE_TOOLCHAIN_FILE)
    file(TO_CMAKE_PATH "$ENV{VCPKG_ROOT}" vcpkg_root)
    if(NOT vcpkg_root OR NOT EXISTS "${vcpkg_root}/scripts/buildsystems/vcpkg.cmake")
        message(FATAL_ERROR "Set VCPKG_ROOT to your bootstrapped vcpkg directory. See README.md.")
    endif()
    set(CMAKE_TOOLCHAIN_FILE "${vcpkg_root}/scripts/buildsystems/vcpkg.cmake"
        CACHE FILEPATH "vcpkg toolchain")
endif()

if(NOT DEFINED VCPKG_TARGET_TRIPLET)
    if(CMAKE_HOST_WIN32)
        set(VCPKG_TARGET_TRIPLET x64-windows-static-md CACHE STRING "vcpkg target triplet")
        set(VCPKG_HOST_TRIPLET "${VCPKG_TARGET_TRIPLET}" CACHE STRING "vcpkg host triplet")
    elseif(CMAKE_HOST_APPLE)
        cmake_host_system_information(RESULT host_arch QUERY OS_PLATFORM)
        if(host_arch STREQUAL "arm64")
            set(VCPKG_TARGET_TRIPLET arm64-osx CACHE STRING "vcpkg target triplet")
        elseif(host_arch STREQUAL "x86_64")
            set(VCPKG_TARGET_TRIPLET x64-osx CACHE STRING "vcpkg target triplet")
        endif()
    endif()
endif()

project(StardewBirthdayNetworking LANGUAGES C CXX)

# clangd uses this database for include paths, defines, and C++ standard flags.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(gns_source_dir "${CMAKE_CURRENT_SOURCE_DIR}/external/GameNetworkingSockets")
if(NOT EXISTS "${gns_source_dir}/CMakeLists.txt")
    message(FATAL_ERROR "Run git submodule update --init -- external/GameNetworkingSockets")
endif()

set(BUILD_STATIC_LIB ON)
set(BUILD_SHARED_LIB OFF)
set(BUILD_EXAMPLES OFF)
set(BUILD_TESTS OFF)
set(BUILD_TOOLS OFF)
set(ENABLE_ICE OFF)
set(USE_STEAMWEBRTC OFF)
set(USE_CRYPTO OpenSSL CACHE STRING "GameNetworkingSockets crypto backend")
set(Protobuf_USE_STATIC_LIBS ON)
set(OPENSSL_USE_STATIC_LIBS TRUE)
add_subdirectory("${gns_source_dir}" EXCLUDE_FROM_ALL)

find_package(spdlog CONFIG REQUIRED)

add_executable(stardew_bday_networking src/main.cpp)
set_target_properties(stardew_bday_networking PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
    RUNTIME_OUTPUT_DIRECTORY "$<1:${CMAKE_CURRENT_BINARY_DIR}>"
)
target_link_libraries(stardew_bday_networking PRIVATE
    GameNetworkingSockets::static
    spdlog::spdlog
)
