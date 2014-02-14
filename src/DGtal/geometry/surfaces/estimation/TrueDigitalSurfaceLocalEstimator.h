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
 * @file TrueDigitalSurfaceLocalEstimator.h
 * @author Jacques-Olivier Lachaud (\c jacques-olivier.lachaud@univ-savoie.fr )
 * Laboratory of Mathematics (CNRS, UMR 5127), University of Savoie, France
 *
 * @date 2014/02/14
 *
 * Header file for module TrueDigitalSurfaceLocalEstimator.cpp
 *
 * This file is part of the DGtal library.
 */

#if defined(TrueDigitalSurfaceLocalEstimator_RECURSES)
#error Recursive header files inclusion detected in TrueDigitalSurfaceLocalEstimator.h
#else // defined(TrueDigitalSurfaceLocalEstimator_RECURSES)
/** Prevents recursive inclusion of headers. */
#define TrueDigitalSurfaceLocalEstimator_RECURSES

#if !defined TrueDigitalSurfaceLocalEstimator_h
/** Prevents repeated inclusion of headers. */
#define TrueDigitalSurfaceLocalEstimator_h

//////////////////////////////////////////////////////////////////////////////
// Inclusions
#include <iostream>
#include "DGtal/base/Common.h"
#include "DGtal/base/ConstAlias.h"
#include "DGtal/base/CountedConstPtrOrConstPtr.h"
#include "DGtal/topology/CanonicSCellEmbedder.h"
//////////////////////////////////////////////////////////////////////////////

namespace DGtal
{

  /////////////////////////////////////////////////////////////////////////////
  // template class TrueDigitalSurfaceLocalEstimator
  /**
   * Description of template class 'TrueDigitalSurfaceLocalEstimator'
   * <p> \brief Aim: An estimator on digital surfaces that returns the
   * reference local geometric quantity. This is used for comparing
   * estimators.
   *
   * @tparam TKSpace the type of cellular grid space, a model of
   * CCellularGridSpaceND.
   *
   * @tparam TShape the type of the shape where geometric estimation
   * are made. It must have methods \a gradient, \a meanCurvature, etc
   * depending on the chosen functor.
   *
   * @tparam TGeometricFunctor the type of the function RealPoint ->
   * Quantity, generally one of
   * ShapeGeometricFunctors::ShapePositionFunctor,
   * ShapeGeometricFunctors::ShapeNormalVectorFunctor,
   * ShapeGeometricFunctors::ShapeMeanCurvatureFunctor,
   * ShapeGeometricFunctors::ShapeGaussianCurvatureFunctor,
   * ShapeGeometricFunctors::ShapeFirstPrincipalCurvatureFunctor,
   * ShapeGeometricFunctors::ShapeSecondPrincipalCurvatureFunctor,
   * ShapeGeometricFunctors::ShapeCurvatureTensorFunctor.
   *
   * @todo add precise concept CShape
   * @todo for now we use a CanonicSCellEmbedder times the gridstep to embed surfels.
   */
  template <typename TKSpace, typename TShape, typename TGeometricFunctor>
  class TrueDigitalSurfaceLocalEstimator
  {
    BOOST_CONCEPT_ASSERT(( CCellularGridSpaceND< TKSpace > ));

  public:
    typedef TrueDigitalSurfaceLocalEstimator<TKSpace,TShape,TGeometricFunctor> Self;
    typedef TKSpace KSpace;
    typedef TShape Shape;
    typedef TGeometricFunctor GeometricFunctor;
    typedef typename KSpace::Space Space;
    typedef typename KSpace::SCell SCell;
    typedef typename KSpace::Surfel Surfel;
    typedef typename Space::RealPoint RealPoint;
    typedef typename RealPoint::Coordinate Scalar;
    typedef typename GeometricFunctor::Quantity Quantity;
    typedef CanonicSCellEmbedder<KSpace> SCellEmbedder;
    BOOST_CONCEPT_ASSERT(( CUnaryFunctor< GeometricFunctor, RealPoint, Quantity > ));

    // ----------------------- Standard services ------------------------------
  public:

    /**
     * Destructor.
     */
    ~TrueDigitalSurfaceLocalEstimator();

    /**
     * Constructor.
     *
     * @param ks the cellular grid space.  The alias can be secured if
     * a some counted pointer is handed.
     *
     * @param fct the functor RealPoint -> Quantity returning some geometric quantity.
     */
    TrueDigitalSurfaceLocalEstimator( ConstAlias<KSpace> ks, 
                                      Clone<GeometricFunctor> fct );

    /**
     * Copy constructor.
     * @param other the object to clone.
     */
    TrueDigitalSurfaceLocalEstimator( const Self & other );

    /**
     * Assignment.
     * @param other the object to copy.
     * @return a reference on 'this'.
     */
    Self& operator=( const Self & other );

    /**
     * Attach a shape.
     *
     * @param aShape the shape of interest. The alias can be secured
     * if a some counted pointer is handed.
     */
    void attach( ConstAlias<Shape> aShape );

    /**
     * Initialisation. The three last parameters specify how the
     * nearest point on the surface is approached.
     *
     * @param h the grid step size (must be >0).
     * @param maxIter fixes the maximum number of steps (0: takes the point as is).
     * @param accuracy distance criterion to stop the descent (proximity of implicit function).
     * @param gamma coefficient associated with the gradient (size of each step).
     */
    void init( const Scalar h,
               const int maxIter = 0, 
               const Scalar accuracy = 0.1, 
               const Scalar gamma = 0.01 );
    
    /**
     * @return the estimated quantity at *it
     * @param [in] it the surfel iterator at which we evaluate the quantity.
     */
    template <typename SurfelConstIterator>
    Quantity eval( SurfelConstIterator it ) const;

    /**
     * @return the estimated quantity in the range [itb,ite)
     * @param [in] itb starting surfel iterator.
     * @param [in] ite end surfel iterator.
     * @param [in,out] result resulting output iterator
     *
     */
    template <typename OutputIterator, typename SurfelConstIterator>
    OutputIterator eval( SurfelConstIterator itb,
                         SurfelConstIterator ite,
                         OutputIterator result ) const;

    // ----------------------- Interface --------------------------------------
  public:

    /**
     * Writes/Displays the object on an output stream.
     * @param out the output stream where the object is written.
     */
    void selfDisplay ( std::ostream & out ) const;

    /**
     * Checks the validity/consistency of the object.
     * @return 'true' if the object is valid, 'false' otherwise.
     */
    bool isValid() const;

    // ------------------------- Protected Datas ------------------------------
  protected:
    /// The cellular space
    CountedConstPtrOrConstPtr<KSpace> myKSpace;
    /// The functor RealPoint -> Quantity returning some geometric quantity.
    GeometricFunctor myFct;
    /// The surfel embedder. /!\ For now, it is multiplied by myH.
    SCellEmbedder myEmbedder;
    /// The shape of interest.
    CountedConstPtrOrConstPtr<Shape> myShape;
    /// The gridstep.
    Scalar myH;
    /// Tells if we look for nearest point
    bool myNearest;

    /// fixes the maximum number of steps (0: takes the point as is).
    int myMaxIter;
    /// distance criterion to stop the descent (proximity of implicit function).
    Scalar myAccuracy;
    /// coefficient associated with the gradient (size of each step).
    Scalar myGamma;

    // ------------------------- Private Datas --------------------------------
  private:

    // ------------------------- Hidden services ------------------------------
  protected:

    /**
     * Constructor.
     * Forbidden by default (protected to avoid g++ warnings).
     */
    TrueDigitalSurfaceLocalEstimator();

  private:


    // ------------------------- Internals ------------------------------------
  private:

  }; // end of class TrueDigitalSurfaceLocalEstimator


  /**
   * Overloads 'operator<<' for displaying objects of class 'TrueDigitalSurfaceLocalEstimator'.
   * @param out the output stream where the object is written.
   * @param object the object of class 'TrueDigitalSurfaceLocalEstimator' to write.
   * @return the output stream after the writing.
   */
  template <typename TKSpace, typename TShape, typename TGeometricFunctor>
  std::ostream&
  operator<< ( std::ostream & out, const TrueDigitalSurfaceLocalEstimator<TKSpace, TShape, TGeometricFunctor> & object );

} // namespace DGtal


///////////////////////////////////////////////////////////////////////////////
// Includes inline functions.
#include "DGtal/geometry/surfaces/estimation/TrueDigitalSurfaceLocalEstimator.ih"

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined TrueDigitalSurfaceLocalEstimator_h

#undef TrueDigitalSurfaceLocalEstimator_RECURSES
#endif // else defined(TrueDigitalSurfaceLocalEstimator_RECURSES)
