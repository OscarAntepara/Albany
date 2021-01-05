//*****************************************************************//
//    Albany 2.0:  Copyright 2012 Sandia Corporation               //
//    This Software is released under the BSD license detailed     //
//    in the file "license.txt" in the top-level Albany directory  //
//*****************************************************************//

#include "Teuchos_TestForException.hpp"
#include "Phalanx_DataLayout.hpp"

namespace PHAL
{

//**********************************************************************
template<typename EvalT, typename Traits, typename ScalarT>
DOFVecGradInterpolationSideBase<EvalT, Traits, ScalarT>::
DOFVecGradInterpolationSideBase(const Teuchos::ParameterList& p,
                            const Teuchos::RCP<Albany::Layouts>& dl_side) :
  sideSetName (p.get<std::string> ("Side Set Name")),
  val_node    (p.get<std::string> ("Variable Name"), (dl_side->useCollapsedSidesets) ? dl_side->node_vector_sideset : dl_side->node_vector),
  gradBF      (p.get<std::string> ("Gradient BF Name"), (dl_side->useCollapsedSidesets) ? dl_side->node_qp_gradient_sideset : dl_side->node_qp_gradient),
  grad_qp     (p.get<std::string> ("Gradient Variable Name"), (dl_side->useCollapsedSidesets) ? dl_side->qp_vecgradient_sideset : dl_side->qp_vecgradient)
{
  TEUCHOS_TEST_FOR_EXCEPTION (!dl_side->isSideLayouts, Teuchos::Exceptions::InvalidParameter,
                              "Error! The layouts structure does not appear to be that of a side set.\n");

  this->addDependentField(val_node.fieldTag());
  this->addDependentField(gradBF.fieldTag());
  this->addEvaluatedField(grad_qp);

  this->setName("DOFVecGradInterpolationSideBase" );

  useCollapsedSidesets = dl_side->useCollapsedSidesets;

  numSideNodes = dl_side->node_qp_gradient->extent(2);
  numSideQPs   = dl_side->node_qp_gradient->extent(3);
  numDims      = dl_side->node_qp_gradient->extent(4);
  vecDim       = dl_side->node_vector->extent(3);
}

//**********************************************************************
template<typename EvalT, typename Traits, typename ScalarT>
void DOFVecGradInterpolationSideBase<EvalT, Traits, ScalarT>::
postRegistrationSetup(typename Traits::SetupData d,
                      PHX::FieldManager<Traits>& fm)
{
  this->utils.setFieldData(val_node,fm);
  this->utils.setFieldData(gradBF,fm);
  this->utils.setFieldData(grad_qp,fm);
}

// *********************************************************************************
template<typename EvalT, typename Traits, typename ScalarT>
void DOFVecGradInterpolationSideBase<EvalT, Traits, ScalarT>::
evaluateFields(typename Traits::EvalData workset)
{
  if (workset.sideSetViews->find(sideSetName)==workset.sideSetViews->end()) return;

  sideSet = workset.sideSetViews->at(sideSetName);
  if (useCollapsedSidesets) {
    for (int sideSet_idx = 0; sideSet_idx < sideSet.size; ++sideSet_idx)
    {
      // Get the data that corresponds to the side
      const int cell = sideSet.elem_LID(sideSet_idx);
      const int side = sideSet.side_local_id(sideSet_idx);

      for (int qp=0; qp<numSideQPs; ++qp)
      {
        for (int comp=0; comp<vecDim; ++comp)
        {
          for (int dim=0; dim<numDims; ++dim)
          {
            grad_qp(sideSet_idx,qp,comp,dim) = 0.;
            for (int node=0; node<numSideNodes; ++node)
            {
              grad_qp(sideSet_idx,qp,comp,dim) += val_node(sideSet_idx,node,comp) * gradBF(sideSet_idx,node,qp,dim);
            }
          }
        }
      }
    }
  } else {
    for (int sideSet_idx = 0; sideSet_idx < sideSet.size; ++sideSet_idx)
    {
      // Get the data that corresponds to the side
      const int cell = sideSet.elem_LID(sideSet_idx);
      const int side = sideSet.side_local_id(sideSet_idx);

      for (int qp=0; qp<numSideQPs; ++qp)
      {
        for (int comp=0; comp<vecDim; ++comp)
        {
          for (int dim=0; dim<numDims; ++dim)
          {
            grad_qp(cell,side,qp,comp,dim) = 0.;
            for (int node=0; node<numSideNodes; ++node)
            {
              grad_qp(cell,side,qp,comp,dim) += val_node(cell,side,node,comp) * gradBF(cell,side,node,qp,dim);
            }
          }
        }
      }
    }
  }
}

} // Namespace PHAL
