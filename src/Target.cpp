//############################################################################//
//                                UNCLASSIFIED                                //
//############################################################################//
/*
  Target.cpp

  Overview:
  Target model

  Author:
  David Bruce

*/
//############################################################################//

#include "Target.hpp"

//
// C/C++ includes and namespaces
//
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#include "MathUtils.hpp"

//
// Sim includes and namespaces
//
//#include "genMath.h"
//#include "genMathConstants.h"
//using namespace genMath;


//
// Reference objects
//
#include "SimExec.hpp"



//############################################################################//
//
// Function:  Target()
//                                
// Purpose:   Constructor
//            Set default values of input variables.
//                                
//############################################################################//
Target::Target() :
MdlBase()
{
  //
  // Default initialization of input variables
  //
  frameTimeStep = 999.0e9;
  initFrameTime = 999.0e9;

//  initVxb = 1.0e3;
  initVxb = 270.0;
  weaveAmpl = 4.0 * GRAVITY;
  weavePeriod = 10;

  weaveStart = 999.e9;
  weaveEnd = -1.0;

  //
  // Register states for integration
  //
  RegisterState( pxf, vxf);
  RegisterState( pyf, vyf);

  RegisterState( vxf, axf);
  RegisterState( vyf, ayf);

}  // end Target()

//############################################################################//
//
// Function: </b>  ~Target()
//
// Purpose: </b>   Destructor
//
//############################################################################//
Target::~Target()
{
}

//############################################################################//
//
// Function: Initialize()
//                                
// Purpose:  Initialize the model 
//                                
//############################################################################//
void Target::Initialize()
{
  double cth;
  double sth;

  ResetAll();

  simTime = GetSimTime();
  
  nextFrameTime = initFrameTime;

  omega = TWOPI / weavePeriod;

  //
  // Crossing angle and flight path angle are supplements of each other.
  //
  theta = PI - pExec->xa;

  cth = cos( theta );
  sth = sin( theta );

  vxb = initVxb;
  vyb = 0.0;

  vxf = vxb * cth - vyb * sth;
  vyf = vxb * sth + vyb * cth;;

  pxf = -pExec->tFinal * vxf;
  pyf = -pExec->tFinal * vyf;

  weavePhase = uniform( -PI, PI, &pExec->seed );

}  // End Initialize()

//############################################################################//
//
// Function: FrameUpdate
//
// Purpose:  Perform frame update
//
//############################################################################//
void Target::FrameUpdate()
{
  simTime = GetSimTime();
}

//############################################################################//
//
// Function: Update
//
// Purpose:  Update the state
//
//############################################################################//
void Target::Update()
{
  double tempAxf;
  double tempAyf;

  simTime = GetSimTime();
  
  if ( (simTime < weaveStart) || (simTime > weaveEnd) )
  {
    axb = 0.0;
    ayb = 0.0;
  }
  else
  {
    if ( !weaveInit )
    {
      weaveInit = true;

      vyf = -( weaveAmpl / omega ) * cos( weavePhase );

      pyf = -( weaveAmpl / SQ(omega) ) * sin( weavePhase );
    }

    tempAxf = 0.0;
    tempAyf = weaveAmpl * sin( simTime * omega + weavePhase );

    axb =  tempAxf * cos(theta) + tempAyf * sin(theta);
    ayb = -tempAxf * sin(theta) + tempAyf * cos(theta);

    jxf = 0.0;
    jyf = weaveAmpl * omega * cos( simTime * omega );
  }
}  // end Update()

//############################################################################//
//
// Function: UpdateDerivatives()
//
// Purpose:  Update the state
//
//############################################################################//
void Target::UpdateDerivatives()
{
  simTime = GetSimTime();

  theta = atan2( vyf, vxf );

  axf = axb * cos(theta) - ayb * sin(theta);
  ayf = axb * sin(theta) + ayb * cos(theta);

}  // end UpdateDerivatives()

//############################################################################//
//
// Function: Finalize()
//
// Purpose:  Close out model
//
//############################################################################//
void Target::Finalize()
{
  simTime = GetSimTime();
} 

//############################################################################//
//
// Function: Output()
//
// Purpose:  Print selected variables
//
//############################################################################//
void Target::Output( bool printHeader,
                     FILE *pOutFile )
{
  simTime = GetSimTime();

  if ( printHeader )
  {
    fprintf( pOutFile, "%20s", "tgt.axb" );
    fprintf( pOutFile, "%20s", "tgt.axf" );
    fprintf( pOutFile, "%20s", "tgt.ayb" );
    fprintf( pOutFile, "%20s", "tgt.ayf" );
    fprintf( pOutFile, "%20s", "tgt.omega" );
    fprintf( pOutFile, "%20s", "tgt.pxf" );
    fprintf( pOutFile, "%20s", "tgt.pyf" );
    fprintf( pOutFile, "%20s", "tgt.theta" );
    fprintf( pOutFile, "%20s", "tgt.vxb" );
    fprintf( pOutFile, "%20s", "tgt.vxf" );
    fprintf( pOutFile, "%20s", "tgt.vyb" );
    fprintf( pOutFile, "%20s", "tgt.vyf" );
    fprintf( pOutFile, "%20s", "tgt.weavePhase" );
  }
  else
  {
    fprintf(pOutFile, "%20.6e", axb);
    fprintf(pOutFile, "%20.6e", axf);
    fprintf(pOutFile, "%20.6e", ayb);
    fprintf(pOutFile, "%20.6e", ayf);
    fprintf(pOutFile, "%20.6e", omega);
    fprintf(pOutFile, "%20.6e", pxf);
    fprintf(pOutFile, "%20.6e", pyf);
    fprintf(pOutFile, "%20.6e", theta);
    fprintf(pOutFile, "%20.6e", vxb);
    fprintf(pOutFile, "%20.6e", vxf);
    fprintf(pOutFile, "%20.6e", vyb);
    fprintf(pOutFile, "%20.6e", vyf);
    fprintf(pOutFile, "%20.6e", weavePhase );

  } // end if ( printHeader ) else
}  // end Output()

//############################################################################//
//
// Function: ResetAll()
//
// Purpose:  Clear selected variables
//
//############################################################################//
void Target::ResetAll()
{
  axb       = 0.0;
  axf       = 0.0;
  ayb       = 0.0;
  ayf       = 0.0;
  omega     = 0.0;
  pxf       = 0.0;
  pyf       = 0.0;
  simTime   = 0.0;
  theta     = 0.0;
  vxb       = 0.0;
  vxf       = 0.0;
  vyb       = 0.0;
  vyf       = 0.0;
  weaveInit = false;

}  // end ResetAll()


//##############################################################################
//                              UNCLASSIFIED
//##############################################################################
