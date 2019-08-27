#include <GU/GU_PrimPoly.h>
#include <PRM/PRM_Include.h>
#include <SIM/SIM_DataFilter.h>
#include <SIM/SIM_DopDescription.h>
#include <SIM/SIM_Engine.h>
#include <SIM/SIM_Force.h>
#include <SIM/SIM_GeometryCopy.h>
#include <SIM/SIM_Object.h>
#include <SIM/SIM_ObjectArray.h>

#include <dlfcn.h>

#include "CppDOPExecutor.h"
#include "utils/scope_guard.hpp"

SIM_CppDOPExecutor::SIM_CppDOPExecutor(const SIM_DataFactory *factory)
    : BaseClass(factory)
    , SIM_OptionsUser(this) {}

SIM_CppDOPExecutor::~SIM_CppDOPExecutor() {}

const SIM_DopDescription *
SIM_CppDOPExecutor::getCppDOPExecutorDopDescription() {

  static PRM_Name theCallbackLibrary("callback_library", "Callback Library");
  static PRM_Template theTemplates[] = {
      PRM_Template(PRM_FILE, 1, &theCallbackLibrary),
      PRM_Template(),
  };

  static SIM_DopDescription theDopDescription(
      true, "cpp_dop_executor", "Cpp DOP Executor", SIM_SOLVER_DATANAME,
      classname(), theTemplates);
  return &theDopDescription;
}

SIM_Solver::SIM_Result SIM_CppDOPExecutor::solveSingleObjectSubclass(
    SIM_Engine &engine, SIM_Object &object, SIM_ObjectArray &objArray,
    const SIM_Time &timestep, bool newobject) {

  // Load external library
  auto  callback_library = getCallbackLibrary();
  void *handle           = dlopen(callback_library.c_str(), RTLD_LAZY);

  // Check if loaded successfully
  if (!handle) {
    std::cout << "Error: Library failed to load!" << std::endl;
    std::cout << "Error Message:" << std::endl << dlerror() << std::endl;
  } else {

    auto guard = sg::make_scope_guard([handle]() { dlclose(handle); });

    // Get main call in the library
    auto callback =
        (void (*)(SIM_Engine &, SIM_Object &, SIM_ObjectArray &,
                  const SIM_Time &, bool))dlsym(handle, "dop_callback");

    if (!callback) {
      std::cout << "Error: Failed to load function callback!" << std::endl;
      std::cout << "Error Message:" << std::endl << dlerror() << std::endl;
    } else {
      callback(engine, object, objArray, timestep, newobject);
    }
  }

  return SIM_SOLVER_SUCCESS;
}
