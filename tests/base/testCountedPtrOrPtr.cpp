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

/**
 * @file testCountedPtrOrPtr.cpp
 * @ingroup Tests
 * @author Jacques-Olivier Lachaud (\c jacques-olivier.lachaud@univ-savoie.fr )
 * Laboratory of Mathematics (CNRS, UMR 5807), University of Savoie, France
 *
 * @date 2014/07/22
 *
 * Functions for testing class CountedPtrOrPtr.
 *
 * This file is part of the DGtal library.
 */

///////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include "DGtal/base/Common.h"
#include "DGtal/base/CountedPtrOrPtr.h"
///////////////////////////////////////////////////////////////////////////////

using namespace std;
using namespace DGtal;

///////////////////////////////////////////////////////////////////////////////
// Functions for testing class CountedPtrOrPtr.
///////////////////////////////////////////////////////////////////////////////

struct A {
  A( int _a ) : a( _a ) 
  {
    ++nb;
    trace.info() << "#" << nb << " A::A( int ), a is " << a << std::endl;
  }
  A( const A& other ) : a( other.a ) 
  {
    ++nb;
    trace.info() << "#" << nb << " A::A( const A& ), a is " << a << std::endl;
  }
  A& operator=( const A& other )
  {
    if ( this != &other )
      a = other.a;
    trace.info() << "#" << nb << " A::op=( const A& ), a is " << a << std::endl;
    return *this;
  }
  ~A()
  {
    --nb;
    trace.info() << "#" << nb << " A::~A(), a was " << a << std::endl;
  }
  static int nb;
  int a;
};

int A::nb = 0;

bool testCountedPtrOrPtrMemory()
{
  unsigned int nbok = 0;
  unsigned int nb = 0;
  trace.beginBlock ( "Testing CountedPtrOrPtr memory managment..." );

  trace.beginBlock ( "An invalid CountedPtrOrPtr does not create any instance." );
  {
    CountedPtrOrPtr<A> cptr;
  }
  ++nb, nbok += A::nb == 0 ? 1 : 0;
  trace.info() << "(" << nbok << "/" << nb << ") " << "A::nb == 0" << std::endl;
  trace.endBlock();

  trace.beginBlock ( "A CountedPtrOrPtr can be used as a simple pointer on an object without acquiring it." );
  {
    A a( 17 );
    ++nb, nbok += A::nb == 1 ? 1 : 0;
    trace.info() << "(" << nbok << "/" << nb << ") " << "A::nb == 1" << std::endl;
    {
      CountedPtrOrPtr<A> cptr( &a, false );
      ++nb, nbok += A::nb == 1 ? 1 : 0;
      trace.info() << "(" << nbok << "/" << nb << ") " << "A::nb == 1" << std::endl;
      ++nb, nbok += cptr.isSimple() ? 1 : 0;
      trace.info() << "(" << nbok << "/" << nb << ") " << "cptr.isSimple()" << std::endl;
    }
    ++nb, nbok += A::nb == 1 ? 1 : 0;
    trace.info() << "(" << nbok << "/" << nb << ") " << "A::nb == 1" << std::endl;
  }
  ++nb, nbok += A::nb == 0 ? 1 : 0;
  trace.info() << "(" << nbok << "/" << nb << ") " << "A::nb == 0" << std::endl;
  trace.endBlock();

  trace.beginBlock ( "CountedPtrOrPtr can be used as a smart pointer with acquisition and automatic deallocation." );
  {
    CountedPtrOrPtr<A> cptr( new A( 10 ) );
    ++nb, nbok += A::nb == 1 ? 1 : 0;
    trace.info() << "(" << nbok << "/" << nb << ") " << "A::nb == 1" << std::endl;
    ++nb, nbok += cptr.isSmart() ? 1 : 0;
    trace.info() << "(" << nbok << "/" << nb << ") " << "cptr.isSmart()" << std::endl;
  }
  ++nb, nbok += A::nb == 0 ? 1 : 0;
  trace.info() << "(" << nbok << "/" << nb << ") " << "A::nb == 0" << std::endl;
  trace.endBlock();

  trace.beginBlock ( "CountedPtrOrPtr can be initialized with = CountedPtrOrPtr<A>( pointer )." );
  {
    CountedPtrOrPtr<A> cptr =  CountedPtrOrPtr<A>( new A( 5 ) );
    ++nb, nbok += A::nb == 1 ? 1 : 0;
    trace.info() << "(" << nbok << "/" << nb << ") " << "A::nb == 1" << std::endl;
    ++nb, nbok += cptr.isSmart() ? 1 : 0;
    trace.info() << "(" << nbok << "/" << nb << ") " << "cptr.isSmart()" << std::endl;
  }
  ++nb, nbok += A::nb == 0 ? 1 : 0;
  trace.info() << "(" << nbok << "/" << nb << ") " << "A::nb == 0" << std::endl;
  trace.endBlock();

  trace.beginBlock ( "CountedPtrOrPtr can be initialized with = CountedPtr<A>( pointer )." );
  {
    CountedPtrOrPtr<A> cptr =  CountedPtr<A>( new A( 5 ) );
    ++nb, nbok += A::nb == 1 ? 1 : 0;
    trace.info() << "(" << nbok << "/" << nb << ") " << "A::nb == 1" << std::endl;
    ++nb, nbok += cptr.isSmart() ? 1 : 0;
    trace.info() << "(" << nbok << "/" << nb << ") " << "cptr.isSmart()" << std::endl;
  }
  ++nb, nbok += A::nb == 0 ? 1 : 0;
  trace.info() << "(" << nbok << "/" << nb << ") " << "A::nb == 0" << std::endl;
  trace.endBlock();

  trace.beginBlock ( "CountedPtrOrPtr allows to share objects." );
  {
    CountedPtrOrPtr<A> cptr( new A( 7 ) );
    CountedPtrOrPtr<A> cptr2 = cptr;
    ++nb, nbok += A::nb == 1 ? 1 : 0;
    trace.info() << "(" << nbok << "/" << nb << ") " << "A::nb == 1" << std::endl;
    ++nb, nbok += cptr.get() == cptr2.get() ? 1 : 0;
    trace.info() << "(" << nbok << "/" << nb << ") " << "cptr.get() == cptr2.get()" << std::endl;
    ++nb, nbok += cptr.count() == 2 ? 1 : 0;
    trace.info() << "(" << nbok << "/" << nb << ") " << "cptr.count() == 2" << std::endl;
    ++nb, nbok += cptr2.count() == 2 ? 1 : 0;
    trace.info() << "(" << nbok << "/" << nb << ") " << "cptr2.count() == 2" << std::endl;
  }
  ++nb, nbok += A::nb == 0 ? 1 : 0;
  trace.info() << "(" << nbok << "/" << nb << ") " << "A::nb == 0" << std::endl;
  trace.endBlock();

  trace.beginBlock ( "CountedPtrOrPtr allows to share objects with CountedPtr." );
  {
    CountedPtr<A> cptr( new A( 7 ) );
    CountedPtrOrPtr<A> cptr2 = cptr;
    ++nb, nbok += A::nb == 1 ? 1 : 0;
    trace.info() << "(" << nbok << "/" << nb << ") " << "A::nb == 1" << std::endl;
    ++nb, nbok += cptr.get() == cptr2.get() ? 1 : 0;
    trace.info() << "(" << nbok << "/" << nb << ") " << "cptr.get() == cptr2.get()" << std::endl;
    ++nb, nbok += cptr.count() == 2 ? 1 : 0;
    trace.info() << "(" << nbok << "/" << nb << ") " << "cptr.count() == 2" << std::endl;
    ++nb, nbok += cptr2.count() == 2 ? 1 : 0;
    trace.info() << "(" << nbok << "/" << nb << ") " << "cptr2.count() == 2" << std::endl;
  }
  ++nb, nbok += A::nb == 0 ? 1 : 0;
  trace.info() << "(" << nbok << "/" << nb << ") " << "A::nb == 0" << std::endl;
  trace.endBlock();

  trace.beginBlock ( "CountedPtrOrPtr are smart wrt assignment." );
  {
    CountedPtrOrPtr<A> cptr( new A( 3 ) );
    CountedPtrOrPtr<A> cptr2( new A( 12 ) );
    ++nb, nbok += A::nb == 2 ? 1 : 0;
    trace.info() << "(" << nbok << "/" << nb << ") " << "A::nb == 2" << std::endl;
    ++nb, nbok += cptr.get() != cptr2.get() ? 1 : 0;
    trace.info() << "(" << nbok << "/" << nb << ") " << "cptr.get() != cptr2.get()" << std::endl;
    cptr = cptr2;
    ++nb, nbok += A::nb == 1 ? 1 : 0;
    trace.info() << "(" << nbok << "/" << nb << ") " << "A::nb == 1" << std::endl;
    ++nb, nbok += cptr.get()->a == 12 ? 1 : 0;
    trace.info() << "(" << nbok << "/" << nb << ") " << "cptr.get()->a == 12" << std::endl;
    ++nb, nbok += cptr.get() == cptr2.get() ? 1 : 0;
    trace.info() << "(" << nbok << "/" << nb << ") " << "cptr.get() == cptr2.get()" << std::endl;
    ++nb, nbok += cptr.count() == 2 ? 1 : 0;
    trace.info() << "(" << nbok << "/" << nb << ") " << "cptr.count() == 2" << std::endl;
    ++nb, nbok += cptr2.count() == 2 ? 1 : 0;
    trace.info() << "(" << nbok << "/" << nb << ") " << "cptr2.count() == 2" << std::endl;
  }
  ++nb, nbok += A::nb == 0 ? 1 : 0;
  trace.info() << "(" << nbok << "/" << nb << ") " << "A::nb == 0" << std::endl;
  trace.endBlock();

  trace.beginBlock ( "CountedPtrOrPtr and CountedPtr are smart wrt assignment." );
  {
    CountedPtrOrPtr<A> cptr( new A( 3 ) );
    CountedPtr<A> cptr2( new A( 12 ) );
    ++nb, nbok += A::nb == 2 ? 1 : 0;
    trace.info() << "(" << nbok << "/" << nb << ") " << "A::nb == 2" << std::endl;
    ++nb, nbok += cptr.get() != cptr2.get() ? 1 : 0;
    trace.info() << "(" << nbok << "/" << nb << ") " << "cptr.get() != cptr2.get()" << std::endl;
    cptr = cptr2;
    ++nb, nbok += A::nb == 1 ? 1 : 0;
    trace.info() << "(" << nbok << "/" << nb << ") " << "A::nb == 1" << std::endl;
    ++nb, nbok += cptr.get()->a == 12 ? 1 : 0;
    trace.info() << "(" << nbok << "/" << nb << ") " << "cptr.get()->a == 12" << std::endl;
    ++nb, nbok += cptr.get() == cptr2.get() ? 1 : 0;
    trace.info() << "(" << nbok << "/" << nb << ") " << "cptr.get() == cptr2.get()" << std::endl;
    ++nb, nbok += cptr.count() == 2 ? 1 : 0;
    trace.info() << "(" << nbok << "/" << nb << ") " << "cptr.count() == 2" << std::endl;
    ++nb, nbok += cptr2.count() == 2 ? 1 : 0;
    trace.info() << "(" << nbok << "/" << nb << ") " << "cptr2.count() == 2" << std::endl;
  }
  ++nb, nbok += A::nb == 0 ? 1 : 0;
  trace.info() << "(" << nbok << "/" << nb << ") " << "A::nb == 0" << std::endl;
  trace.endBlock();

  trace.endBlock();
  return nb == nbok;
}


///////////////////////////////////////////////////////////////////////////////
// Standard services - public :

int main( int argc, char** argv )
{
  trace.beginBlock ( "Testing class CountedPtrOrPtr" );
  trace.info() << "Args:";
  for ( int i = 0; i < argc; ++i )
    trace.info() << " " << argv[ i ];
  trace.info() << endl;

  bool res = testCountedPtrOrPtrMemory();
  trace.emphase() << ( res ? "Passed." : "Error." ) << endl;
  trace.endBlock();
  return res ? 0 : 1;
}
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
