#pragma once

#include <SIM/SIM_OptionsUser.h>
#include <SIM/SIM_SingleSolver.h>
#include <SIM/SIM_Utils.h>

#define SIM_NAME_SOURCEOBJECTS "sourceobjects"

class SIM_ObjectArray;
class SIM_GeometryCopy;

// This class implemented a computational fluid dynamics solver.
class SIM_CppDOPExecutor : public SIM_SingleSolver, public SIM_OptionsUser {

public:
  GETSET_DATA_FUNCS_S("callback_library", CallbackLibrary);

protected:
  explicit SIM_CppDOPExecutor(const SIM_DataFactory *factory);
  virtual ~SIM_CppDOPExecutor();
  virtual SIM_Result
  solveSingleObjectSubclass(SIM_Engine &engine, SIM_Object &object,
                            SIM_ObjectArray &feedbacktoobjects,
                            const SIM_Time &timestep, bool newobject);

private:
  static const SIM_DopDescription *getCppDOPExecutorDopDescription();
  DECLARE_STANDARD_GETCASTTOTYPE();
  DECLARE_DATAFACTORY(SIM_CppDOPExecutor, SIM_SingleSolver, "Cpp DOP Executor",
                      getCppDOPExecutorDopDescription());
};
