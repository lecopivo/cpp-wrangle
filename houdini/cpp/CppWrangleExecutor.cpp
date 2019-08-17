#include <iostream>
#include <mutex>
#include <string>

#include <dlfcn.h>
#include <stddef.h>

#include <GU/GU_Detail.h>
#include <OP/OP_AutoLockInputs.h>
#include <OP/OP_Director.h>
#include <OP/OP_Operator.h>
#include <OP/OP_OperatorTable.h>
#include <PRM/PRM_Include.h>
#include <UT/UT_DSOVersion.h>

#include "CppWrangleExecutor.h"
#include "utils/scope_guard.hpp"

void newSopOperator(OP_OperatorTable *table) {
  table->addOperator(
      new OP_Operator("cpp_wrangle_executor", "Cpp Wrangle Executor",
                      SOP_CppWrangleExecutor::myConstructor,
                      SOP_CppWrangleExecutor::myTemplateList, 1));
}

static PRM_Name prm_names[] = {
    PRM_Name("callback_library", "Callback Library"),
};

PRM_Template SOP_CppWrangleExecutor::myTemplateList[] = {
    PRM_Template(PRM_FILE, 1, &prm_names[0]),
    PRM_Template(),
};

OP_Node *SOP_CppWrangleExecutor::myConstructor(OP_Network * net,
                                               const char * name,
                                               OP_Operator *op) {
  return new SOP_CppWrangleExecutor(net, name, op);
}

SOP_CppWrangleExecutor::SOP_CppWrangleExecutor(OP_Network * net,
                                               const char * name,
                                               OP_Operator *op)
    : SOP_Node(net, name, op) {
  // Right now I'm assuming that anything can change
  // In fugure, I might bump data IDs base on which attribute handles were
  // created with `getattrib` function

  // mySopFlags.setManagesDataIDs(true);
}

SOP_CppWrangleExecutor::~SOP_CppWrangleExecutor() {}

OP_ERROR
SOP_CppWrangleExecutor::cookMySop(OP_Context &context) {
  // We must lock our inputs before we try to access their geometry.
  // OP_AutoLockInputs will automatically unlock our inputs when we return.
  // NOTE: Don't call unlockInputs yourself when using this!
  OP_AutoLockInputs input_lock_guard(this);
  if (input_lock_guard.lock(context) >= UT_ERROR_ABORT)
    return error();

  // Duplicate incoming geometry
  duplicateSource(0, context);

  flags().setTimeDep(true);
  fpreal time = context.getTime();

  // Load external library
  UT_String callback_library;
  evalString(callback_library, "callback_library", 0, time);
  void *handle = dlopen(callback_library.c_str(), RTLD_LAZY);

  // Check if loaded successfully
  if (!handle) {
    std::cout << "Error: Library failed to load!" << std::endl;
    std::cout << "Error Message:" << std::endl << dlerror() << std::endl;
  } else {
    // Unload library upon exiting
    auto guard = sg::make_scope_guard([handle]() { dlclose(handle); });

    // Get main call in the library
    auto callback = (void (*)(const float, SOP_Node *, GU_Detail *))dlsym(
        handle, "callback");

    if (!callback) {
      std::cout << "Error: Failed to load function callback!" << std::endl;
      std::cout << "Error Message:" << std::endl << dlerror() << std::endl;
    } else {
      callback(time, this, gdp);
    }
  }

  if (error() >= UT_ERROR_ABORT)
    return error();

  return error();
}
