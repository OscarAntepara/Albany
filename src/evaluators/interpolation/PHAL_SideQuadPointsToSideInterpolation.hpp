//*****************************************************************//
//    Albany 3.0:  Copyright 2016 Sandia Corporation               //
//    This Software is released under the BSD license detailed     //
//    in the file "license.txt" in the top-level Albany directory  //
//*****************************************************************//

#ifndef PHAL_SIDE_QUAD_POINT_TO_SIDE_INTERPOLATION_HPP
#define PHAL_SIDE_QUAD_POINT_TO_SIDE_INTERPOLATION_HPP

#include "Phalanx_config.hpp"
#include "Phalanx_Evaluator_WithBaseImpl.hpp"
#include "Phalanx_Evaluator_Derived.hpp"
#include "Phalanx_MDField.hpp"

#include "Albany_Layouts.hpp"
#include "Albany_SacadoTypes.hpp"
#include "PHAL_Utilities.hpp"

namespace PHAL
{
/** \brief Average from Qp to Side

    This evaluator averages the quadrature points values to
    obtain a single value for the whole cell

*/

template<typename EvalT, typename Traits, typename ScalarT>
class SideQuadPointsToSideInterpolationBase : public PHX::EvaluatorWithBaseImpl<Traits>,
                                              public PHX::EvaluatorDerived<EvalT, Traits>
{
public:

  SideQuadPointsToSideInterpolationBase (const Teuchos::ParameterList& p,
                                         const Teuchos::RCP<Albany::Layouts>& dl_side);

  void postRegistrationSetup (typename Traits::SetupData d,
                              PHX::FieldManager<Traits>& fm);

  void evaluateFields(typename Traits::EvalData d);

private:

  typedef typename EvalT::MeshScalarT MeshScalarT;
  typedef typename Albany::StrongestScalarType<ScalarT,MeshScalarT>::type OutputScalarT;

  int fieldDim;
  std::vector<int> dims;
  int dimsArray[5];

  bool useCollapsedSidesets;

  std::string sideSetName;

  MDFieldMemoizer<Traits> memoizer;

  Albany::LocalSideSetInfo sideSet;

  // Input:
  PHX::MDField<const ScalarT>     field_qp;
  PHX::MDField<const MeshScalarT> w_measure;

  // Output:
  PHX::MDField<OutputScalarT>     field_side;

public:

  typedef Kokkos::View<int***, PHX::Device>::execution_space ExecutionSpace;
  struct Dim0_Tag{};
  struct Dim1_Tag{};
  struct Dim2_Tag{};

  typedef Kokkos::RangePolicy<ExecutionSpace, Dim0_Tag> Dim0_Policy;
  typedef Kokkos::RangePolicy<ExecutionSpace, Dim1_Tag> Dim1_Policy;
  typedef Kokkos::RangePolicy<ExecutionSpace, Dim2_Tag> Dim2_Policy;

  KOKKOS_INLINE_FUNCTION
  void operator() (const Dim0_Tag& tag, const int& sideSet_idx) const;
  KOKKOS_INLINE_FUNCTION
  void operator() (const Dim1_Tag& tag, const int& sideSet_idx) const;
  KOKKOS_INLINE_FUNCTION
  void operator() (const Dim2_Tag& tag, const int& sideSet_idx) const;

};

// Some shortcut names
template<typename EvalT, typename Traits>
using SideQuadPointsToSideInterpolation = SideQuadPointsToSideInterpolationBase<EvalT,Traits,typename EvalT::ScalarT>;

template<typename EvalT, typename Traits>
using SideQuadPointsToSideInterpolationMesh = SideQuadPointsToSideInterpolationBase<EvalT,Traits,typename EvalT::MeshScalarT>;

template<typename EvalT, typename Traits>
using SideQuadPointsToSideInterpolationParam = SideQuadPointsToSideInterpolationBase<EvalT,Traits,typename EvalT::ParamScalarT>;

} // Namespace PHAL

#endif // PHAL_SIDE_QUAD_POINT_TO_SIDE_INTERPOLATION_HPP
