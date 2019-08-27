#include <OP/OP_Operator.h>
#include <OP/OP_OperatorTable.h>

// This include is critical! If not included, Houdini won't load the plugin even
// thou the code compiles
#include <UT/UT_DSOVersion.h>

#include "CppDOPExecutor.h"
#include "CppWrangleExecutor.h"

void newSopOperator(OP_OperatorTable *table) {
  table->addOperator(
      new OP_Operator("cpp_wrangle_executor", "Cpp Wrangle Executor",
                      SOP_CppWrangleExecutor::myConstructor,
                      SOP_CppWrangleExecutor::myTemplateList, 1));
}

void initializeSIM(void *) { IMPLEMENT_DATAFACTORY(SIM_CppDOPExecutor); }
