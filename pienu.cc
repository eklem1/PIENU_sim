#ifndef WIN32
#include <unistd.h>
#endif

#include "Randomize.hh"

#include "G4RunManager.hh"
#include "G4UImanager.hh"

#include "PhysicsList.hh"
#include "ExtraPhysics.hh"

#include "G4OpticalPhysics.hh"
#include "G4PhysListFactory.hh"

#include "PhysicsList.hh"
#include "ExtraPhysics.hh"

#include "DetectorConstruction.hh"
#include "PrimaryGeneratorAction.hh"

#include "RunAction.hh"
#include "EventAction.hh"
#include "TrackingAction.hh"
#include "SteppingAction.hh"
#include "StackingAction.hh"

//#include "PersistencyManager.hh"
//#include "RootPersistencyManager.hh"

#include "G4UIterminal.hh"
#include "G4UItcsh.hh"

#ifdef G4VIS_USE
//#include "G4VisExecutive.hh"
#endif

#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif

// argc holds the number of arguments (including the name) on the command line
// -> it is ONE when only the name is  given !!!
// argv[0] is always the name of the program
// argv[1] points to the first argument, and so on

int main(int argc,char** argv)
{
  //  G4String physicsList = "QGSP_BERT";
   G4String physicsList = "FTFP_BERT_PEN";
  // G4String physicsList = "ShieldingLEND_PEN";

  G4int seed = 123;
  if (argc  > 2) seed = atoi(argv[argc-1]);

  // Choose the Random engine

  CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine);
  CLHEP::HepRandom::setTheSeed(seed);

#ifndef WIN32
  G4int c = 0;
  while ((c=getopt(argc,argv,"p")) != -1)
  {
     switch (c)
     {
       case 'p':
         physicsList = optarg;
         G4cout << "Physics List used is " <<  physicsList << G4endl;
         break;
       case ':':       /* -p without operand */
         fprintf(stderr,
                         "Option -%c requires an operand\n", optopt);
         break;
       case '?':
         fprintf(stderr,
                         "Unrecognised option: -%c\n", optopt);
     }
  }
#endif

  // Construct the default run manager

  G4RunManager * runManager = new G4RunManager;

  // Set mandatory initialization classes

  runManager->SetUserInitialization(new DetectorConstruction());
  runManager->SetUserInitialization(new PhysicsList(physicsList));

  // Set mandatory user action class

  runManager->SetUserAction( new PrimaryGeneratorAction() );

  // Set optional user action class

  RunAction*      runAction      = new RunAction();
  EventAction*    eventAction    = new EventAction(runAction);
  TrackingAction* trackingAction = new TrackingAction(runAction);
  SteppingAction* steppingAction = new SteppingAction(runAction);

  runManager->SetUserAction( runAction );
  runManager->SetUserAction( eventAction );
  runManager->SetUserAction( trackingAction );
  runManager->SetUserAction( steppingAction );
  runManager->SetUserAction( new StackingAction() );

  // Initialize G4 kernel
  //
  runManager->Initialize();

#ifdef G4VIS_USE
  // visualization manager

//  G4VisManager* visManager = new G4VisExecutive;
//  visManager->Initialize();
#endif

  // Create the Persistency Manager.
//  PersistencyManager* persistencyManager
//      = RootPersistencyManager::GetRootManager();

  // Get the pointer to the User Interface manager

  G4UImanager * UImanager = G4UImanager::GetUIpointer();

//  G4String outputFilename = "Output";

//  if (persistencyManager && outputFilename != "") {
//     G4String command = "/db/open ";
//     UImanager->ApplyCommand(command+outputFilename);
//  }

#ifndef WIN32
  G4int optmax = argc;
  if (argc > 2)  { optmax = optmax-1; }

  if (optind < optmax)
  {
     G4String command = "/control/execute ";
     for ( ; optind < optmax; optind++)
     {
         G4String macroFilename = argv[optind];
         UImanager->ApplyCommand(command+macroFilename);
     }
  }
#else  // Simple UI for Windows runs, no possibility of additional arguments
  if (argc!=1)
  {
     G4String command = "/control/execute ";
     G4String fileName = argv[1];
     UImanager->ApplyCommand(command+fileName);
  }
#endif
  else  {
     // Define (G)UI terminal for interactive mode
#ifdef G4UI_USE
     G4UIExecutive * ui = new G4UIExecutive(argc,argv);
#ifdef G4VIS_USE
//     UImanager->ApplyCommand("/control/execute vis.mac");
#endif
     ui->SessionStart();
     delete ui;
#endif
  }

  // job termination

#ifdef G4VIS_USE
//  delete visManager;
#endif

//  if (persistencyManager) {
//     persistencyManager->Close();
//     delete persistencyManager;
//  }

  delete runManager;

  return 0;
}
