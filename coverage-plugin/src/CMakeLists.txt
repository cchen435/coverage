# If we don't need RTTI or EH, there's no reason to export anything
# from the plugin.
cmake_minimum_required(VERSION 3.4.3)

find_package(LLVM REQUIRED CONFIG)
include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})
link_directories(${LLVM_LIBRARY_DIRS})

add_library(CoverageAST MODULE CoverageAST.cpp)

target_compile_features(CoverageAST PRIVATE cxx_range_for cxx_auto_type)

set_target_properties(CoverageAST PROPERTIES
    COMPILE_FLAGS "-fno-rtti"
)

if(APPLE)
    set_target_properties(CoverageAST PROPERTIES
        LINK_FLAGS "-undefined dynamic_lookup")
    set_target_properties(CoverageAST PROPERTIES
        COMPILE_FLAGS "-fno-rtti -D__STDC_LIMIT_MACROS -D__STDC_CONSTANT_MACROS")

endif(APPLE)

#if( NOT MSVC ) # MSVC mangles symbols differently, and
#                # CoverageAST.export contains C++ symbols.
#  if( NOT LLVM_REQUIRES_RTTI )
#    if( NOT LLVM_REQUIRES_EH )
#      set(LLVM_EXPORTED_SYMBOL_FILE ${CMAKE_CURRENT_SOURCE_DIR}/CoverageAST.exports)
#    endif()
#  endif()
#endif()
#
#if(LLVM_ENABLE_PLUGINS AND (WIN32 OR CYGWIN))
#  target_link_libraries(CoverageAST ${cmake_2_8_12_PRIVATE}
#    clangAST
#    clangBasic
#    clangFrontend
#    LLVMSupport
#    )
#endif()
