//*****************************************************************//
//    Albany 3.0:  Copyright 2016 Sandia Corporation               //
//    This Software is released under the BSD license detailed     //
//    in the file "license.txt" in the top-level Albany directory  //
//*****************************************************************//


#ifndef ALBANY_STK3DPOINTSTRUCT_HPP
#define ALBANY_STK3DPOINTSTRUCT_HPP

#include "Albany_GenericSTKMeshStruct.hpp"

namespace Albany {

  /*!
   * \brief A specific mesh class for a 3D Point
   */

  class STK3DPointStruct : public GenericSTKMeshStruct {

  public:

    //! Default constructor
    STK3DPointStruct(const Teuchos::RCP<Teuchos::ParameterList>& params,
                     const Teuchos::RCP<const Teuchos_Comm>& comm,
                     const int numParams);

    ~STK3DPointStruct() = default;

    void setBulkData (const Teuchos::RCP<const Teuchos_Comm>& comm);

    //! Flag if solution has a restart values -- used in Init Cond
    bool hasRestartSolution() const {return false; }

    //! If restarting, convenience function to return restart data time
    double restartDataTime() const {return -1.0; }

  private:

    //! Build the mesh
    void buildMesh(const Teuchos::RCP<const Teuchos_Comm>& comm);

    //! Build a parameter list that contains valid input parameters
    Teuchos::RCP<const Teuchos::ParameterList>
    getValidDiscretizationParameters() const;
  };
}
#endif
