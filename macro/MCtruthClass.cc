#include "MCtruthClass.h"
/** \file MCtruthClass.cpp 
Written by Anthony Hillaret
*/

void MCtruthClass::Init( )
{	
}
/** This method is used to define the detector geometry in the
 class DetectorGeo.This method would eventually open a file
 or anything else that contains the relevant geometry
 information.*/
bool MCtruthClass::InitGeometry(ConfigFile &C)
{
	// geo = new DetectorGeo();
	// return geo->ReadGeometry( TmpFile, Log);
  return true;
}

/** This is the super important method of the class.When an entry of the TTree is loaded, this method is called.
 This is here that new variables can be filled according to
the entry. For example one can calculate the momentum from
the 3 momentum vector coordinates and store the value in a
 variable which will be accessible in the modules like if it 
 was in the tree.If the entry has a problem like something important missing,
just return false and the whole event will be skipped.
Use only this for sanity checks, not for physics.*/

bool MCtruthClass::Load( )
{
	return true;
}

