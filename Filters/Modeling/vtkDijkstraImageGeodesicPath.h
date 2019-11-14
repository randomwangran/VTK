/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkDijkstraImageGeodesicPath.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
/**
 * @class   vtkDijkstraImageGeodesicPath
 * @brief   Dijkstra algorithm to compute the graph geodesic.
 *
 * Takes as input a polyline and an image representing a 2D cost function
 * and performs a single source shortest path calculation.
 * Dijkstra's algorithm is used. The implementation is
 * similar to the one described in Introduction to Algorithms (Second Edition)
 * by Thomas H. Cormen, Charles E. Leiserson, Ronald L. Rivest, and
 * Cliff Stein, published by MIT Press and McGraw-Hill. Some minor
 * enhancement are added though. All vertices are not pushed on the heap
 * at start, instead a front set is maintained. The heap is implemented as
 * a binary heap. The output of the filter is a set of lines describing
 * the shortest path from StartVertex to EndVertex.  See parent class
 * vtkDijkstraGraphGeodesicPath for the implementation.
 *
 * @warning
 * The input cost image must have only VTK_PIXEL cells: i.e., a 2D image or
 * slice of a 3D volume. A cost function for a gray scale image might
 * be generated by the following pipeline:
 * vtkImageData->vtkImageGradientMagnitude->vtkImageShiftScale
 * wherein the gradient magnitude image is inverted so that strong edges
 * have low cost value.  Costs in moving from a vertex v to a vertex u
 * are calculated using a weighted additive scheme:
 * cost = Iw*f(I) + Ew*f(u,v) + Cw*f(t,u,v)
 * where Iw is the weight associated with f(I): the normalized image cost,
 * Ew is the weight associated with f(u,v): the normalized distance between
 * vertices u and v, and Cw is the weight associated with f(t,u,v):
 * the normalized curvature calculated from the vertex t which precedes
 * vertex u, and vertices u and v.  All weights range from 0 to 1.
 *
 * @par Thanks:
 * The class was contributed by Dean Inglis.
*/

#ifndef vtkDijkstraImageGeodesicPath_h
#define vtkDijkstraImageGeodesicPath_h

#include "vtkDijkstraGraphGeodesicPath.h"
#include "vtkFiltersModelingModule.h" // For export macro

class vtkImageData;

class VTKFILTERSMODELING_EXPORT vtkDijkstraImageGeodesicPath :
                           public vtkDijkstraGraphGeodesicPath
{
public:

  /**
   * Instantiate the class
   */
  static vtkDijkstraImageGeodesicPath *New();

  //@{
  /**
   * Standard methods for printing and determining type information.
   */
  vtkTypeMacro( vtkDijkstraImageGeodesicPath, vtkDijkstraGraphGeodesicPath );
  void PrintSelf( ostream& os, vtkIndent indent ) override;
  //@}

  //@{
  /**
   * Specify the image object which is used as a cost function.
   */
  void SetInputData( vtkDataObject* );
  vtkImageData* GetInputAsImageData();
  //@}

  //@{
  /**
   * Image cost weight.
   */
  void SetImageWeight( double );
  vtkGetMacro( ImageWeight, double );
  //@}

  //@{
  /**
   * Edge length cost weight.
   */
  void SetEdgeLengthWeight( double );
  vtkGetMacro( EdgeLengthWeight, double );
  //@}

  //@{
  /**
   * Curvature cost weight.
   */
  vtkSetClampMacro( CurvatureWeight, double, 0.0, 1.0 );
  vtkGetMacro( CurvatureWeight, double );
  //@}

protected:
  vtkDijkstraImageGeodesicPath();
  ~vtkDijkstraImageGeodesicPath() override;

  int FillInputPortInformation(int port, vtkInformation *info) override;
  int RequestData(vtkInformation *, vtkInformationVector **,
                          vtkInformationVector *) override;

  // Build a graph description of the image
  void BuildAdjacency( vtkDataSet *inData ) override;

  // Update static costs without rebuilding adjacencyh when static weights change
  void UpdateStaticCosts( vtkImageData *image );

  // Override parent class methods.
  double CalculateStaticEdgeCost( vtkDataSet *inData , vtkIdType u, vtkIdType v) override;
  double CalculateDynamicEdgeCost( vtkDataSet *inData , vtkIdType u, vtkIdType v) override;

  double PixelSize;
  double ImageWeight;
  double EdgeLengthWeight;
  double CurvatureWeight;
  bool RebuildStaticCosts;

private:
  vtkDijkstraImageGeodesicPath(const vtkDijkstraImageGeodesicPath&) = delete;
  void operator=(const vtkDijkstraImageGeodesicPath&) = delete;

};

#endif

