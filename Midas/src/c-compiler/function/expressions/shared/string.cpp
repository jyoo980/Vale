#include "string.h"
#include "heap.h"

LLVMValueRef getInnerStrPtrFromWrapperPtr(
    LLVMBuilderRef builder,
    LLVMValueRef strWrapperPtrLE) {
  return LLVMBuildStructGEP(
      builder, strWrapperPtrLE, 1, "strInnerStructPtr");
}

LLVMValueRef getLenPtrFromStrWrapperPtr(
    LLVMBuilderRef builder,
    LLVMValueRef strWrapperPtrLE) {
  auto innerStringPtrLE =
      getInnerStrPtrFromWrapperPtr(builder, strWrapperPtrLE);
  auto lenPtrLE =
      LLVMBuildStructGEP(builder, innerStringPtrLE, 0, "lenPtr");
  return lenPtrLE;
}

LLVMValueRef getLenFromStrWrapperPtr(
    LLVMBuilderRef builder,
    LLVMValueRef strWrapperPtrLE) {
  return LLVMBuildLoad(builder, getLenPtrFromStrWrapperPtr(builder, strWrapperPtrLE), "len");
}

LLVMValueRef buildConstantVStr(
    GlobalState* globalState,
    LLVMBuilderRef builder,
    const std::string& contents) {

  auto lengthLE = constI64LE(contents.length());

  auto strWrapperPtrLE = mallocStr(globalState, builder, lengthLE);

  std::vector<LLVMValueRef> argsLE = {
      getInnerStrPtrFromWrapperPtr(builder, strWrapperPtrLE),
      globalState->getOrMakeStringConstant(contents)
  };
  LLVMBuildCall(builder, globalState->initStr, argsLE.data(), argsLE.size(), "");

  return strWrapperPtrLE;
}
