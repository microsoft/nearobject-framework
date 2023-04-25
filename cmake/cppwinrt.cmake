
include(CMakeParseArguments)

# Common variables. These dont technically need to be cache variables, but they
# are defined this way in case other project codes wants to use them. 
set(CPPWINRT_ROOT ${CMAKE_BINARY_DIR}/packages/Microsoft.Windows.CppWinRT.${VERSION_CPPWINRT} CACHE INTERNAL PATH)
set(CPPWINRT_BUILD ${CPPWINRT_ROOT}/build/native CACHE INTERNAL PATH)
set(CPPWINRT_BIN ${CPPWINRT_ROOT}/bin CACHE INTERNAL PATH)
set(CPPWINRT_TOOL ${CPPWINRT_BIN}/cppwinrt.exe CACHE INTERNAL PATH)
set(CPPWINRT_PROPS_FILE ${CPPWINRT_BUILD}/Microsoft.Windows.CppWinRT.props CACHE INTERNAL PATH)
set(CPPWINRT_TARGETS_FILE ${CPPWINRT_BUILD}/Microsoft.Windows.CppWinRT.targets CACHE INTERNAL PATH)

# Common argument prefixe name for all functions below
set(CPPWINRT_FUNCTION_PREFIX CPPWINRT)

# Add a C++/WinRT project import to the target.
function(cppwinrt_add_project_import)
  set(OPTION_ARGS EXCLUDE_FROM_SOURCE)
  set(SINGLE_VALUE_ARGS TARGET IMPORT)
  set(MULTI_VALUE_ARGS "")

  cmake_parse_arguments(
    ${CPPWINRT_FUNCTION_PREFIX}
    "${OPTION_ARGS}"
    "${SINGLE_VALUE_ARGS}"
    "${MULTI_VALUE_ARGS}"
    ${ARGN}
  )

  set(VS_PROJECT_IMPORT_PROPERTY "")
  get_target_property(VS_PROJECT_IMPORT_PROPERTY ${CPPWINRT_TARGET} VS_PROJECT_IMPORT)
  if (${VS_PROJECT_IMPORT_PROPERTY} STREQUAL VS_PROJECT_IMPORT_PROPERTY-NOTFOUND)
    set(VS_PROJECT_IMPORT_PROPERTY "${CPPWINRT_IMPORT}")
  else()
    list(APPEND VS_PROJECT_IMPORT_PROPERTY ${CPPWINRT_IMPORT})
    list(REMOVE_DUPLICATES VS_PROJECT_IMPORT_PROPERTY)
  endif()
  set_target_properties(${CPPWINRT_TARGET} PROPERTIES VS_PROJECT_IMPORT "${VS_PROJECT_IMPORT_PROPERTY}")

  if (NOT EXCLUDE_FROM_SOURCE)
    target_sources(${CPPWINRT_TARGET} PRIVATE ${CPPWINRT_IMPORT})
  endif()
endfunction()

# Enable C++/WinRT on a target.
function(cppwinrt_enable)
  set(OPTION_ARGS "")
  set(SINGLE_VALUE_ARGS TARGET)
  set(MULIT_VALUE_ARGS "")

  cmake_parse_arguments(
    ${CPPWINRT_FUNCTION_PREFIX}
    "${OPTION_ARGS}"
    "${SINGLE_VALUE_ARGS}"
    "${MULIT_VALUE_ARGS}"
    ${ARGN}
  )

  # Add the C++/WinRT properties to the target.
  cppwinrt_add_project_import(TARGET ${CPPWINRT_TARGET} IMPORT ${CPPWINRT_PROPS_FILE} EXCLUDE_FROM_SOURCE)

  # Add the C++/WinRT targets file to the target.
  target_link_libraries(${CPPWINRT_TARGET} PRIVATE ${CPPWINRT_TARGETS_FILE})

endfunction()
