/*=========================================================================

  Program:   Visualization Toolkit

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

// all variables that represent positions or directions have a suffix
// indicating the coordinate system they are in. The possible values are
// MC - Model Coordinates
// WC - WC world coordinates
// VC - View Coordinates
// DC - Display Coordinates

attribute vec4 vertexWC;

// material property values
//VTK::Color::Dec
uniform float opacity;

uniform mat4 WCVCMatrix;  // World to view matrix

varying vec4 fcolor;

void main()
{
  gl_Position = WCVCMatrix*vertexWC;
  fcolor = vec4(diffuseColor.rgb, opacity);
}

