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
 * @file MeaningfulScaleAnalysis.h
 * @author Bertrand Kerautret (\c kerautre@loria.fr )
 * LORIA (CNRS, UMR 7503), University of Nancy, France
 *
 * @date 2015/12/27
 *
 * Header file for module MeaningfulScaleAnalysis.cpp
 *
 * This file is part of the DGtal library.
 */

#if defined(MeaningfulScaleAnalysis_RECURSES)
#error Recursive header files inclusion detected in MeaningfulScaleAnalysis.h
#else // defined(MeaningfulScaleAnalysis_RECURSES)
/** Prevents recursive inclusion of headers. */
#define MeaningfulScaleAnalysis_RECURSES

#if !defined MeaningfulScaleAnalysis_h
/** Prevents repeated inclusion of headers. */
#define MeaningfulScaleAnalysis_h

//////////////////////////////////////////////////////////////////////////////
// Inclusions
#include <iostream>
#include "DGtal/base/Common.h"
#include "DGtal/math/Profile.h"
//////////////////////////////////////////////////////////////////////////////

namespace DGtal
{

  /////////////////////////////////////////////////////////////////////////////
  // class MeaningfulScaleAnalysis
  /**
   * Description of class 'MeaningfulScaleAnalysis' <p>
   * \brief Aim:
   */
  template<typename TProfile>
  class MeaningfulScaleAnalysis
  {
    // ----------------------- Standard services ------------------------------
  public:

    typedef TProfile Profile;

    /**
     * Constructor
     */
    MeaningfulScaleAnalysis(const Profile &aProfile): myProfile(aProfile) {
       
    }

    /**
     * Destructor.
     */
    ~MeaningfulScaleAnalysis(){};

    // ----------------------- Interface --------------------------------------
  public:


  
    /**
     * A meaningful scale is an interval of scales of length no
     * smaller than [min_width] and in which the profile has slopes
     * below [max_slope] and above [min_slope]. This method computes
     * the sequence of meaningful scales for surfel [idx].
     *
     * @param[out] intervals (returns) a list of meaningful scales.
     * @param[in] minSize the minimum length for the meaningful scales.
     * @param[in] maxSlope the maximum allowed slope for length evolution.
     * @param[in] minSlope the minimum allowed slope for length evolution.
     */
    void 
    computeMeaningfulScales( std::vector< std::pair< unsigned int, unsigned int > > & intervals,
                             const unsigned int minSize = 1,
                             const double maxSlope = -0.2,
                             const double minSlope = -1e10 ) const;


    /**
     *  Compute the profile slope of the first meaningful scale
     *  interval computed by a simple linear regression model.
     *
     * @return a pair<bool, double> giving the slope and indicating if
     * a meaningful scale was  found or not. If no meaningful
     * scale interval was found, it simply return the slope obtained
     * from the linear regression. 
     *
     * @param[in] maxSlope the  maximum allowed slope for length evolution.
     * @param[in] minSlope the  minimum allowed slope for length evolution.  
     * @param[in] minSize the minimum length for the meaningful scales.
     * 
     **/
    std::pair<bool, double> 
    getSlopeFromMeaningfulScales(const double maxSlope=-0.2,
                                 const double minSlope=-1e10,
                                 const unsigned int minSize=2) const ;
    
    
    
    /**
     * The noise level is the first scale of the first meaningful
     * scale. A meaningful scale is an interval of scales of length no
     * smaller than [min_width] and in which the profile has slopes
     * below [max_slope]. 
     *
     * @param[in] minSize the minimum length for the meaningful scales.
     * @param[in] maxSlope the maximum allowed slope for length evolution.
     * @param[in] minSlope the minimum allowed slope for length evolution.
     * @return the noise level or zero is none was found.
     * @see meaningfulScales
     */
    unsigned int
    noiseLevel( const unsigned int minSize = 1,
		const double maxSlope = -0.2,
		const double minSlope = -1e10 ) const;



    /**
     * The noise level is the first scale of the first meaningful
     * scale. A meaningful scale is an interval of scales of length no
     * smaller than [minWidth] and in which the profile has slopes
     * below [maxSlope]. The lower bounded noise level also requires
     * minimum lenghs for different scales. Therefore the profile must
     * be greater that
     * [lower_bound_at_scale_1]+[lower_bound_slope]*scale.
     *
     * @param[in] minSize the minimum length for the meaningful scales.
     * @param[in] maxSlope the maximum allowed slope for length evolution.
     * @param[in] minSlope the minimum allowed slope for length evolution.
     * @param[in] lowerBoundAtScale1 the lower bound for the profile at scale 1.
     * @param[in] lowerBoundSlope the slope of the lower bound for the profile (for instance -1 for digital contours, -3 for digital image graphs since area values are divided by (scale)^3.
     * @return the noise level or zero is none was found.
     * @see meaningfulScales
     */
    unsigned int
    lowerBoundedNoiseLevel( const unsigned int minSize = 1,
			    const double maxSlope = -0.2,
			    const double minSlope = -1e10,
			    const double lowerBoundAtScale1 = 1.0,
			    const double lowerBoundSlope = -2.0 ) const;


    

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
    const Profile  &myProfile;
  
    // ------------------------- Private Datas --------------------------------
  private:

    // ------------------------- Hidden services ------------------------------
  protected:

    /**
     * Constructor.
     * Forbidden by default (protected to avoid g++ warnings).
     */
    MeaningfulScaleAnalysis();

  private:

    /**
     * Copy constructor.
     * @param other the object to clone.
     * Forbidden by default.
     */
    MeaningfulScaleAnalysis ( const MeaningfulScaleAnalysis & other );

    /**
     * Assignment.
     * @param other the object to copy.
     * @return a reference on 'this'.
     * Forbidden by default.
     */
    MeaningfulScaleAnalysis & operator= ( const MeaningfulScaleAnalysis & other );

    // ------------------------- Internals ------------------------------------
  private:

  }; // end of class MeaningfulScaleAnalysis


  /**
   * Overloads 'operator<<' for displaying objects of class 'MeaningfulScaleAnalysis'.
   * @param out the output stream where the object is written.
   * @param object the object of class 'MeaningfulScaleAnalysis' to write.
   * @return the output stream after the writing.
   */
  template<typename TProfile>
  std::ostream&
  operator<< ( std::ostream & out, const MeaningfulScaleAnalysis<TProfile> & object );


} // namespace DGtal


///////////////////////////////////////////////////////////////////////////////
// Includes inline functions.
#if !defined(BUILD_INLINE)
#include "DGtal/math/MeaningfulScaleAnalysis.ih"
#endif


//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined MeaningfulScaleAnalysis_h

#undef MeaningfulScaleAnalysis_RECURSES
#endif // else defined(MeaningfulScaleAnalysis_RECURSES)
