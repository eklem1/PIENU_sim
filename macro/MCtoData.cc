////////////////////////////////////////////////////////////
//
// MCtoData
//
// Program for creating a data tree from the 
// Geant4 Pienu simulation
//
// Authors:
// L.Doria, A.Sher (2011)
//
////////////////////////////////////////////////////////////

#include "MCtree.h"



int main(int argc, char *argv[])
{

  //Help
  if (argc<=2){
    cout << endl << "Usage:" << endl;
    cout << "MCtoData <outputfile> <list of input files>" << endl;
    cout << "Example: MCtoData out.root 1.root 2.root 3.root ..." << endl;
  }

  //If the inputs are OK:
  if (argc>2){
    cout << endl << "Output File: " << argv[1] << endl;
    for (int i=1;i<argc;i++){
      cout << "Input File: " << argv[i] << endl;
    }

    //Create the main object:
    //Specify the tree names in the MC tree (hits and tree)
    MCtree *mctree = new MCtree("hits","tree");
     
    //Set the input MC file(s)
    for (int i=2;i<argc;i++){
      mctree->SetInputFile(argv[i]); 
    }
    
    //Set the output file and the tree name
    mctree->SetOutputFile(argv[1],"T");
    
    //Loop!
    mctree->Loop();
    
    //Save
    mctree->SaveOutputTree();
  }

}
