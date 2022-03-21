/**
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as
 *  published by the Free Software Foundation, either version 3 of the
 *  License, or  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 **/

#pragma once

/**
 * @file
 * @author David Coeurjolly (\c david.coeurjolly@liris.cnrs.fr )
 * Laboratoire d'InfoRmatique en Image et Systemes d'information - LIRIS (CNRS, UMR 5205), CNRS, France
 *
 * @date 2021/09/21
 *
 * Header file for module GeodesicsInHeat.cpp
 *
 * This file is part of the DGtal library.
 */

#if defined(GeodesicsInHeat_RECURSES)
#error Recursive header files inclusion detected in GeodesicsInHeat.h
#else // defined(GeodesicsInHeat_RECURSES)
/** Prevents recursive inclusion of headers. */
#define GeodesicsInHeat_RECURSES

#if !defined GeodesicsInHeat_h
/** Prevents repeated inclusion of headers. */
#define GeodesicsInHeat_h

//////////////////////////////////////////////////////////////////////////////
// Inclusions
#include <iostream>
#include "DGtal/base/Common.h"
#include "DGtal/base/ConstAlias.h"
//////////////////////////////////////////////////////////////////////////////

namespace DGtal
{
/////////////////////////////////////////////////////////////////////////////
// template class GeodesicsInHeat
/**
 * Description of template class 'GeodesicsInHeat' <p>
 * \brief This class implements @cite Crane2013 on polygonal surfaces  (using @ref modulePolygonalCalculus).
 *
 * see @ref moduleGeodesicsInHeat for details and examples.
 *
 * @tparam a model of PolygonalCalculus.
 */
template <typename TPolygonalCalculus>
class GeodesicsInHeat
{
    // ----------------------- Standard services ------------------------------
  public:

    typedef TPolygonalCalculus PolygonalCalculus;
    typedef typename PolygonalCalculus::SparseMatrix SparseMatrix;
    typedef typename PolygonalCalculus::DenseMatrix DenseMatrix;
    typedef typename PolygonalCalculus::Solver Solver;
    typedef typename PolygonalCalculus::Vector Vector;
    typedef typename PolygonalCalculus::Vertex Vertex;

    /**
     * Default constructor.
     */
    GeodesicsInHeat() = delete;
    
    /// Constructor from an existing polygonal calculus. T
    /// @param calculus a instance of PolygonalCalculus
    GeodesicsInHeat(ConstAlias<PolygonalCalculus> calculus): myCalculus(&calculus)
    {
      myIsInit=false;
    }
    
    /**
     * Destructor.
     */
    ~GeodesicsInHeat() = default;

    /**
     * Copy constructor.
     * @param other the object to clone.
     */
    GeodesicsInHeat ( const GeodesicsInHeat & other ) = delete;

    /**
     * Move constructor.
     * @param other the object to move.
     */
    GeodesicsInHeat ( GeodesicsInHeat && other ) = delete;

    /**
     * Copy assignment operator.
     * @param other the object to copy.
     * @return a reference on 'this'.
     */
    GeodesicsInHeat & operator= ( const GeodesicsInHeat & other ) = delete;

    /**
     * Move assignment operator.
     * @param other the object to move.
     * @return a reference on 'this'.
     */
    GeodesicsInHeat & operator= ( GeodesicsInHeat && other ) = delete;


    
    // ----------------------- Interface --------------------------------------
   
    /// Initialize the solvers with @a dt as timestep for the
    /// heat diffusion
    /// @param dt timestep
    void init(double dt)
    {
      myIsInit=true;
      
      //As the LB is PSD, the identity matrix shouldn't be necessary. However, some solvers
      //may have issues with positive semi-definite matrix.
      SparseMatrix I(myCalculus->nbVertices(),myCalculus->nbVertices());
      I.setIdentity();
      SparseMatrix laplacian = myCalculus->globalLaplaceBeltrami() + 1e-6 * I;
      SparseMatrix mass      = myCalculus->globalLumpedMassMatrix();
      SparseMatrix heatOpe   = mass + dt*laplacian;
      
      //Prefactorizing
      myPoissonSolver.compute(laplacian);
      myHeatSolver.compute(heatOpe);
      
      //empty source
      mySource    = Vector::Zero(myCalculus->nbVertices());
    }
    
    /** Adds a source point at a vertex @e aV
    * @param aV the Vertex
     **/
    void addSource(const Vertex aV)
    {
      ASSERT_MSG(aV < myCalculus->nbVertices(), "Vertex is not in the surface mesh vertex range");
      myLastSourceIndex = aV;
      mySource( aV ) = 1.0;
    }
    
    /**
     * @returns the source point vector.
     **/
    Vector source() const
    {
      FATAL_ERROR_MSG(myIsInit, "init() method must be called first");
      return mySource;
    }
    
    
    /// Main computation of the Geodesic In Heat
    /// @returns the estimated geodesic distances from the sources.
    Vector compute() const
    {
      FATAL_ERROR_MSG(myIsInit, "init() method must be called first");
      //Heat diffusion
      Vector heatDiffusion = myHeatSolver.solve(mySource);
      Vector divergence    = Vector::Zero(myCalculus->nbVertices());
      auto cpt=0;
      auto surfmesh = myCalculus->getSurfaceMeshPtr();
      
      // Heat, normalization and divergence per face
      for(auto f=0; f< myCalculus->nbFaces(); ++f)
      {
        Vector faceHeat( myCalculus->degree(f));
        cpt=0;
        auto vertices = surfmesh->incidentVertices(f);
        for(auto v: vertices)
        {
          faceHeat(cpt) = heatDiffusion( v );
          ++cpt;
        }
        // ∇heat / ∣∣∇heat∣∣
        Vector grad = -myCalculus->gradient(f) * faceHeat;
        grad.normalize();
      
        // div
        DenseMatrix oneForm = myCalculus->flat(f)*grad;
        Vector divergenceFace = myCalculus->divergence(f) * oneForm;
        cpt=0;
        for(auto v: vertices)
        {
          divergence(v) += divergenceFace(cpt);
          ++cpt;
        }
      }
      
      // Last Poisson solve
      Vector distVec = myPoissonSolver.solve(divergence);

      //Source val
      auto sourceval = distVec(myLastSourceIndex);
      
      //shifting the distances to get 0 at sources
      return distVec - sourceval*Vector::Ones(myCalculus->nbVertices());
    }
    
    
    /// @return true if the calculus is valid.
    bool isValid() const
    {
      return myIsInit && myCalculus->isValid();
    }
    
    // ----------------------- Private --------------------------------------

  private:
    
    ///The underlying PolygonalCalculus instance
    const PolygonalCalculus *myCalculus;

    ///Poisson solver
    Solver myPoissonSolver;

    ///Heat solver
    Solver myHeatSolver;

    ///Source vector
    Vector mySource;

    ///Vertex index to the last source point (to shift the distances)
    Vertex myLastSourceIndex;
  
    ///Validitate flag
    bool myIsInit;
    
  }; // end of class GeodesicsInHeat
} // namespace DGtal

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined GeodesicsInHeat_h

#undef GeodesicsInHeat_RECURSES
#endif // else defined(GeodesicsInHeat_RECURSES)