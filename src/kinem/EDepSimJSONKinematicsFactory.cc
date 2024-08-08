#include "kinem/EDepSimJSONKinematicsFactory.hh"
#include "kinem/EDepSimJSONKinematicsGenerator.hh"

EDepSim::JSONKinematicsFactory::JSONKinematicsFactory(
    EDepSim::UserPrimaryGeneratorMessenger* parent)
    : EDepSim::VKinematicsFactory("json",parent,false),
      fInputFile("not-open"),
      fVerbosity(0),
      fInputFileCMD(NULL),
      fVerboseCMD(NULL) {

    fInputFileCMD = new G4UIcmdWithAString(CommandName("input"),this);
    fInputFileCMD->SetGuidance("Set the input file.");
    fInputFileCMD->SetParameterName("name",false);

    fVerboseCMD = new G4UIcmdWithAnInteger(CommandName("verbose"),this);
    fVerboseCMD->SetGuidance("Set verbosity level (0 is default, 2 is max).");
    fVerboseCMD->SetParameterName("number",false);
}

EDepSim::JSONKinematicsFactory::~JSONKinematicsFactory() {
    if (fInputFileCMD) delete fInputFileCMD;
    if (fVerboseCMD)   delete fVerboseCMD;
}

EDepSim::VKinematicsGenerator*
EDepSim::JSONKinematicsFactory::GetGenerator() {
    EDepSim::VKinematicsGenerator* kine
        = new EDepSim::JSONKinematicsGenerator(GetName(),
					       GetInputFile(),
					       GetVerbose());
    return kine;
}

void EDepSim::JSONKinematicsFactory::SetNewValue(G4UIcommand* command,
                                                 G4String newValue) {
    if (command == fInputFileCMD) {
        SetInputFile(newValue);
    }
    else if (command == fVerboseCMD) {
        SetVerbose(fVerboseCMD->GetNewIntValue(newValue));
    }
    else{
        EDepSimError("Nothing to set the value.");
        EDepSimThrow("EDepSim::JSONKinematicsFactory::SetNewValue(): Error");
    }
}
