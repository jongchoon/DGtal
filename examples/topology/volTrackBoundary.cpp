/**
 * @file volTrackBoundary.cpp
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
//! [volTrackBoundary-basicIncludes]
#include <iostream>
#include <queue>
#include <QImageReader>
#include <QtGui/qapplication.h>
#include "DGtal/io/viewers/Viewer3D.h"
#include "DGtal/io/readers/VolReader.h"
#include "DGtal/io/DrawWithDisplay3DModifier.h"
#include "DGtal/io/Color.h"
#include "DGtal/images/ImageSelector.h"
#include "DGtal/images/imagesSetsUtils/SetFromImage.h"
#include "DGtal/shapes/Shapes.h"
#include "DGtal/helpers/StdDefs.h"
#include "DGtal/topology/helpers/Surfaces.h"
//! [volTrackBoundary-basicIncludes]

///////////////////////////////////////////////////////////////////////////////

using namespace std;
using namespace DGtal;
using namespace Z3i;

///////////////////////////////////////////////////////////////////////////////

void usage( int /*argc*/, char** argv )
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

  //! [volTrackBoundary-readVol]
  trace.beginBlock( "Reading vol file into an image." );
  typedef ImageSelector < Domain, int>::Type Image;
  Image image = VolReader<Image>::importVol(inputFilename);
  DigitalSet set3d (image.domain());
  SetFromImage<DigitalSet>::append<Image>(set3d, image, 
                                          minThreshold, maxThreshold);
  trace.endBlock();
  //! [volTrackBoundary-readVol]
  
  
  //! [volTrackBoundary-KSpace]
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
  //! [volTrackBoundary-KSpace]

  //! [volTrackBoundary-SurfelAdjacency]
  typedef SurfelAdjacency<KSpace::dimension> MySurfelAdjacency;
  MySurfelAdjacency surfAdj( true ); // interior in all directions.
  //! [volTrackBoundary-SurfelAdjacency]

  //! [volTrackBoundary-ExtractingSurface]
  trace.beginBlock( "Extracting boundary by tracking from an initial bel." );
  KSpace::SCellSet boundary;
  SCell bel = Surfaces<KSpace>::findABel( ks, set3d, 100000 );
  Surfaces<KSpace>::trackBoundary( boundary, ks, 
                                   surfAdj,
                                   set3d, bel );
  trace.endBlock();
  //! [volTrackBoundary-ExtractingSurface]

  //! [volTrackBoundary-DisplayingSurface]
  trace.beginBlock( "Displaying surface in Viewer3D." );
  QApplication application(argc,argv);
  Viewer3D<Z3i::Space, Z3i::KSpace> viewer;
  viewer.show(); 
  viewer << CustomColors3D(Color(250, 0, 0 ), Color( 128, 128, 128 ) );
  unsigned long nbSurfels = 0;
  for ( KSpace::SCellSet::const_iterator it = boundary.begin(),
          it_end = boundary.end(); it != it_end; ++it, ++nbSurfels )
    viewer << *it;
  viewer << Viewer3D<Z3i::Space, Z3i::KSpace>::updateDisplay;
  trace.info() << "nb surfels = " << nbSurfels << std::endl;
  trace.endBlock();
  return application.exec();
  //! [volTrackBoundary-DisplayingSurface]
}

