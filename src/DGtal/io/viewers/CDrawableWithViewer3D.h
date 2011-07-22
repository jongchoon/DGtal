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
 * @file CDrawableWithViewer3D.h
 * @author David Coeurjolly (\c david.coeurjolly@liris.cnrs.fr )
 * Laboratoire d'InfoRmatique en Image et Systèmes d'information - LIRIS (CNRS, UMR 5205), CNRS, France
 * @author Jacques-Olivier Lachaud (\c jacques-olivier.lachaud@univ-savoie.fr )
 * Laboratory of Mathematics (CNRS, UMR 5807), University of Savoie, France
 *
 * @date 2010/10/21
 *
 * Header file for concept CDrawableWithViewer3D.cpp
 *
 * This file is part of the DGtal library.
 */

#if defined(CDrawableWithViewer3D_RECURSES)
#error Recursive header files inclusion detected in CDrawableWithViewer3D.h
#else // defined(CDrawableWithViewer3D_RECURSES)
/** Prevents recursive inclusion of headers. */
#define CDrawableWithViewer3D_RECURSES

#if !defined CDrawableWithViewer3D_h
/** Prevents repeated inclusion of headers. */
#define CDrawableWithViewer3D_h

//////////////////////////////////////////////////////////////////////////////

// Inclusions

#include <iostream>
#include "boost/concept_check.hpp"
#include "DGtal/base/Common.h"











//////////////////////////////////////////////////////////////////////////////

namespace DGtal
{





  /////////////////////////////////////////////////////////////////////////////
  // class CDrawableWithViewer3D
  /**
   * Description of \b concept '\b CDrawableWithViewer3D' <p>
   * @ingroup Concepts
   * Aim:  The concept CDrawableWithViewer3D specifies what are the classes
   * that admit an export with Board2D.
   * An object x satisfying this concept may then be used as:
   * 
   \code
   Board2D board;
   board << CustomStyle( x.styleName(), x.defaultStyle() )
         << x;
   \endcode 
   *
   * <p> Refinement of
   *
   * <p> Associated types :
   *
   * <p> Notation
   * - \t X : A type that is a model of CDrawableWithViewer3D
   * - \t x, \t y	: Object of type X
   * - \t m	: a string of characters
   *
   * <p> Definitions
   *
   * <p> Valid expressions and semantics <br>
   * <table> <tr> <td> \b Name </td> <td> \b Expression </td>
   * <td> \b Type requirements </td> <td> \b Return type </td>
   * <td> \b Precondition </td> <td> \b Semantics </td>
   * <td> \b Postcondition </td> <td> \b Complexity </td>
   * </tr>
   * <tr>
   * <td> the default draw style</td> <td> x.defaultStyle( m = "" ) </td> <td> \t mode : \c std::string</td><td> DrawableWithBoard2D * </td> <td> </td> <td> returns a dynamic allocation of the default style for the model \t X in mode \t m</td><td> </td>
   *  <td> </td>
   * </tr>
   * <tr>
   * <td> the name of the model X</td> <td> x.styleName() </td> <td></td><td> std::string </td> <td> </td> <td> returns a string telling the name of the model X </td><td> </td>
   *  <td> </td>
   * </tr>
   * <tr>
   * <td> the way the object \t x is drawn</td> <td> x.selfDraw(Board2D &board) </td> <td></td> <td> </td> <td> </td> <td> draws on the \c board stream the object \c x </td><td> </td>
   *  <td> </td>
   * </tr>
   * </table>
   *
   * <p> Invariants <br>
   *
   * <p> Models <br>
   * ArimeticalDSS, FreemanChain, HyperRectDomain, ImageContainerByHashTree, ImageContainerBySTLVector, PointVector, DigitalSetBySTLSet,DigitalSetBySTLVector, Object
   *
   * <p> Notes <br>
   * @todo ImageContainerByHashTree does not implement selfDraw(Board2D &).
   * @todo ImageContainerByHashTree does not implement defaultStyle(std::string&)const.
   */
  template <typename T>
  struct CDrawableWithViewer3D
  {
    // ----------------------- Concept checks ------------------------------
  public:
    BOOST_CONCEPT_USAGE( CDrawableWithViewer3D )
    {
      //Drawable model should have a defaultStyle() returning a DrawableWithBoard2D*
      ConceptUtils::sameType( myD, myT.defaultStyleViewer3D() );
      //Drawable model should have a defaultStyle( string ) returning a DrawableWithBoard2D*
      ConceptUtils::sameType( myD, myT.defaultStyleViewer3D( myS ) );
      //Drawable model should have a styleName() returning a string
      ConceptUtils::sameType( myS, myT.styleName() );
      //Drawable model should have a selfDraw()
      ///@todo FIXME: si on décommente ça plante
      myT.selfDrawViewer3D( myB );
    }

    // ------------------------- Private Datas --------------------------------
  private:
    T myT;
    DrawableWithViewer3D *myD;

    ///@todo FIXME: si on décommente ça plante
    Viewer3D myB;
    std::string myS;

    // ------------------------- Internals ------------------------------------
  private:

  }; // end of concept CDrawableWithViewer3D

} // namespace DGtal


//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined CDrawableWithViewer3D_h

#undef CDrawableWithViewer3D_RECURSES
#endif // else defined(CDrawableWithViewer3D_RECURSES)