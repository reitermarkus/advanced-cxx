execute_process(
  COMMAND brew --prefix llvm
  RESULT_VARIABLE BREW_LLVM_STATUS
  OUTPUT_VARIABLE BREW_LLVM_PREFIX
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

if(BREW_LLVM_STATUS EQUAL 0)
list(APPEND CLANG_TIDY_PATHS "${BREW_LLVM_PREFIX}/bin")
endif()

find_program(CLANG_TIDY clang-tidy PATHS ${CLANG_TIDY_PATHS})
set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY};-header-filter=.;-checks=*;-warnings-as-errors=*;-fix")
