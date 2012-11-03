/**
 * @file volDistanceTraversal.cpp
 * @ingroup Examples
 * @author Jacques-Olivier Lachaud (\c jacques-olivier.lachaud@univ-savoie.fr )
 * Laboratory of Mathematics (CNRS, UMR 5127), University of Savoie, France
 *
 * @date 2012/02/06
 *
 * An example file named qglViewer.
 *
 * This file is part of the DGtal library.
 */

///////////////////////////////////////////////////////////////////////////////
//! [volDistanceTraversal-basicIncludes]
#include <iostream>
#include <queue>
#include <QImageReader>
#include <QtGui/qapplication.h>
#include "DGtal/base/BasicFunctors.h"
#include "DGtal/base/Lambda2To1.h"
#include "DGtal/kernel/EuclideanDistance.h"
#include "DGtal/kernel/sets/SetPredicate.h"
#include "DGtal/kernel/CanonicSCellEmbedder.h"
#include "DGtal/io/readers/VolReader.h"
#include "DGtal/io/viewers/Viewer3D.h"
#include "DGtal/io/DrawWithDisplay3DModifier.h"
#include "DGtal/io/Color.h"
#include "DGtal/io/colormaps/HueShadeColorMap.h"
#include "DGtal/images/ImageSelector.h"
#include "DGtal/images/imagesSetsUtils/SetFromImage.h"
#include "DGtal/shapes/Shapes.h"
#include "DGtal/helpers/StdDefs.h"
#include "DGtal/topology/DigitalSurface.h"
#include "DGtal/topology/LightImplicitDigitalSurface.h"
#include "DGtal/topology/DistanceVisitor.h"
//! [volDistanceTraversal-basicIncludes]

///////////////////////////////////////////////////////////////////////////////

using namespace std;
using namespace DGtal;
using namespace Z3i;

///////////////////////////////////////////////////////////////////////////////

void usage( int, char** argv )
{
  std::cerr << "Usage: " << argv[ 0 ] << " <fileName.vol> <minT> <maxT>" << std::endl;
  std::cerr << "\t - displays the boundary of the shape stored in vol file <fileName.vol>." << std::endl;
  std::cerr << "\t - voxel v belongs to the shape iff its value I(v) follows minT <= I(v) <= maxT." << std::endl;
}

int main( int argc, char** argv )
{
  if ( argc < 4 )
    {
      usage( argc, argv );
      return 1;
    }
  std::string inputFilename = argv[ 1 ];
  unsigned int minThreshold = atoi( argv[ 2 ] );
  unsigned int maxThreshold = atoi( argv[ 3 ] );

  //! [volDistanceTraversal-readVol]
  trace.beginBlock( "Reading vol file into an image." );
  typedef ImageSelector < Domain, int>::Type Image;
  Image image = VolReader<Image>::importVol(inputFilename);
  DigitalSet set3d (image.domain());
  SetPredicate<DigitalSet> set3dPredicate( set3d );
  SetFromImage<DigitalSet>::append<Image>(set3d, image, 
                                          minThreshold, maxThreshold);
  trace.endBlock();
  //! [volDistanceTraversal-readVol]
  
  
  //! [volDistanceTraversal-KSpace]
  trace.beginBlock( "Construct the Khalimsky space from the image domain." );
  KSpace ks;
  bool space_ok = ks.init( image.domain().lowerBound(), 
                           image.domain().upperBound(), true );
  if (!space_ok)
    {
      trace.error() << "Error in the Khamisky space construction."<<std::endl;
      return 2;
    }
  trace.endBlock();
  //! [volDistanceTraversal-KSpace]

  //! [volDistanceTraversal-SurfelAdjacency]
  typedef SurfelAdjacency<KSpace::dimension> MySurfelAdjacency;
  MySurfelAdjacency surfAdj( true ); // interior in all directions.
  //! [volDistanceTraversal-SurfelAdjacency]

  //! [volDistanceTraversal-SetUpDigitalSurface]
  trace.beginBlock( "Set up digital surface." );
  typedef LightImplicitDigitalSurface<KSpace, SetPredicate<DigitalSet> > 
    MyDigitalSurfaceContainer;
  typedef DigitalSurface<MyDigitalSurfaceContainer> MyDigitalSurface;
  SCell bel = Surfaces<KSpace>::findABel( ks, set3dPredicate, 100000 );
  MyDigitalSurfaceContainer* ptrSurfContainer = 
    new MyDigitalSurfaceContainer( ks, set3dPredicate, surfAdj, bel );
  MyDigitalSurface digSurf( ptrSurfContainer ); // acquired
  trace.endBlock();
  //! [volDistanceTraversal-SetUpDigitalSurface]

  //! [volDistanceTraversal-ExtractingSurface]
  trace.beginBlock( "Extracting boundary by distance tracking from an initial bel." );
  typedef CanonicSCellEmbedder<KSpace> SCellEmbedder;
  typedef SCellEmbedder::Value RealPoint;
  typedef RealPoint::Coordinate Scalar;
  typedef EuclideanDistance<RealPoint> Distance;
  typedef Lambda2To1<Distance, RealPoint, RealPoint, Scalar> DistanceToPoint;
  typedef Composer<SCellEmbedder, DistanceToPoint, Scalar> VertexFunctor;
  typedef DistanceVisitor< MyDigitalSurface, VertexFunctor, std::set<SCell> > 
    MyDistanceVisitor;
  typedef MyDistanceVisitor::Node MyNode;
  typedef MyDistanceVisitor::Scalar MySize;

  SCellEmbedder embedder;
  Distance distance;
  DistanceToPoint distanceToPoint( distance, embedder( bel ) );
  VertexFunctor vfunctor( embedder, distanceToPoint );
  MyDistanceVisitor visitor( digSurf, vfunctor, bel );

  unsigned long nbSurfels = 0;
  MyNode node;
  while ( ! visitor.finished() )
    {
      node = visitor.current();
      ++nbSurfels;
      visitor.expand();
    }
  MySize maxDist = node.second;
  trace.endBlock();
  //! [volDistanceTraversal-ExtractingSurface]

  //! [volDistanceTraversal-DisplayingSurface]
  trace.beginBlock( "Displaying surface in Viewer3D." );
  QApplication application(argc,argv);
  Viewer3D viewer;
  viewer.show(); 
  HueShadeColorMap<MySize,1> hueShade( 0, maxDist );
  MyDistanceVisitor visitor2( digSurf, vfunctor, bel );
  viewer << CustomColors3D( Color::Black, Color::White )
         << ks.unsigns( bel );
  visitor2.expand();
  while ( ! visitor2.finished() )
    {
      node = visitor2.current();
      Color c = hueShade( node.second );
      viewer << CustomColors3D( Color::Red, c )
             << ks.unsigns( node.first );
      visitor2.expand();
    }
  viewer << Viewer3D::updateDisplay;
  trace.info() << "nb surfels = " << nbSurfels << std::endl;
  trace.endBlock();
  return application.exec();
  //! [volDistanceTraversal-DisplayingSurface]
}

