vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO alandtse/CommonLibSSE-NG
    REF 39f9d07a6ffabea8fb559eee87ab7d27cd463e8a
    SHA512 712374255cf5da060715f24faabe07a581208f6ea5998ced00bbff340916d77f379441a1a0972388c4c7b0909bddbbf99cc5c2c99819e3d76fce8fe61270b579
)

vcpkg_from_github(
    OUT_SOURCE_PATH OPENVR_SOURCE_PATH
    REPO ValveSoftware/openvr
    REF 60eb187801956ad277f1cae6680e3a410ee0873b
    SHA512 bb85b4705e7095ac65df9969112b2df8930cee7917cc5f14231c5a0ffeed7a73ffa60727fd32f8786a403656f95a3ec0f80bf3ceabc5b8ede964aefb920bc718
)
file(COPY "${OPENVR_SOURCE_PATH}/headers" "${OPENVR_SOURCE_PATH}/lib" DESTINATION "${SOURCE_PATH}/extern/openvr")

vcpkg_cmake_configure(
    SOURCE_PATH "${SOURCE_PATH}"
    OPTIONS
        -DBUILD_TESTS=OFF
        -DENABLE_SKYRIM_SE=ON
        -DENABLE_SKYRIM_AE=ON
        -DENABLE_SKYRIM_VR=ON
        -DSKSE_SUPPORT_XBYAK=OFF
        -DSKSE_SUPPORT_PATCH_SAFETY=OFF
        -DCOMMONLIB_PREBUILT=OFF
        -DCMAKE_TRY_COMPILE_CONFIGURATION=Release
)
vcpkg_cmake_install()
vcpkg_cmake_config_fixup(CONFIG_PATH lib/cmake/CommonLibSSE)

file(INSTALL "${SOURCE_PATH}/cmake/CommonLibSSE.cmake" DESTINATION "${CURRENT_PACKAGES_DIR}/share/${PORT}")
file(APPEND "${CURRENT_PACKAGES_DIR}/share/${PORT}/CommonLibSSEConfig.cmake" "\nfind_dependency(directxtk CONFIG)\n")
file(INSTALL "${OPENVR_SOURCE_PATH}/headers/openvr.h" DESTINATION "${CURRENT_PACKAGES_DIR}/include")
file(INSTALL "${OPENVR_SOURCE_PATH}/lib/win64/openvr_api.lib" DESTINATION "${CURRENT_PACKAGES_DIR}/lib")
if(NOT VCPKG_BUILD_TYPE)
    file(INSTALL "${OPENVR_SOURCE_PATH}/lib/win64/openvr_api.lib" DESTINATION "${CURRENT_PACKAGES_DIR}/debug/lib")
endif()
vcpkg_replace_string(
    "${CURRENT_PACKAGES_DIR}/share/${PORT}/CommonLibSSE-targets.cmake"
    "${SOURCE_PATH}/extern/openvr/lib/win64/openvr_api.lib"
    "\${_IMPORT_PREFIX}/lib/openvr_api.lib"
)

file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug/include")
vcpkg_install_copyright(FILE_LIST "${SOURCE_PATH}/COPYING.txt" "${OPENVR_SOURCE_PATH}/LICENSE")
