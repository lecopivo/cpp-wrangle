#include "sop_node_template.h"

#include <GU/GU_Detail.h>
#include <OP/OP_AutoLockInputs.h>
#include <OP/OP_Operator.h>
#include <OP/OP_OperatorTable.h>
#include <PRM/PRM_Include.h>
#include <UT/UT_DSOVersion.h>
#include <stddef.h>

// CALLBACK CODE

void newSopOperator(OP_OperatorTable *table) {
  table->addOperator(new OP_Operator("T_NODE_NAME", "T_NODE_LABEL",
                                     SOP_T_NODE_CLASS_NAME::myConstructor,
                                     SOP_T_NODE_CLASS_NAME::myTemplateList, 1));
}

static PRM_Name prm_names[] = {
    // PRM_NAME_LIST
};

PRM_Template SOP_T_NODE_CLASS_NAME::myTemplateList[] = {
    // PRM_TEMPLATE_LIST
    PRM_Template(),
};

OP_Node *SOP_T_NODE_CLASS_NAME::myConstructor(OP_Network *net, const char *name,
                                              OP_Operator *op) {
  return new SOP_T_NODE_CLASS_NAME(net, name, op);
}

SOP_T_NODE_CLASS_NAME::SOP_T_NODE_CLASS_NAME(OP_Network *net, const char *name,
                                             OP_Operator *op)
    : SOP_Node(net, name, op) {
  // Right now I'm assuming that anything can change
  // In fugure, I might bump data IDs base on which attribute handles were
  // created with `getattrib` function

  // mySopFlags.setManagesDataIDs(true);
}

SOP_T_NODE_CLASS_NAME::~SOP_T_NODE_CLASS_NAME() {}

OP_ERROR
SOP_T_NODE_CLASS_NAME::cookMySop(OP_Context &context) {
  // We must lock our inputs before we try to access their geometry.
  // OP_AutoLockInputs will automatically unlock our inputs when we return.
  // NOTE: Don't call unlockInputs yourself when using this!
  OP_AutoLockInputs inputs(this);
  if (inputs.lock(context) >= UT_ERROR_ABORT)
    return error();

  // Duplicate incoming geometry
  duplicateSource(0, context);

  fpreal time = context.getTime();

  // Call function from CppWrangle
  callback(time, this, this, gdp);

  if (error() >= UT_ERROR_ABORT)
    return error();

  return error();
}
