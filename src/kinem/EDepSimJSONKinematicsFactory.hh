#ifndef EDepSim_JSONKinematicsFactory_hh_seen
#define EDepSim_JSONKinematicsFactory_hh_seen

#include "kinem/EDepSimVKinematicsFactory.hh"

class G4VPrimaryGenerator;
namespace EDepSim {class JSONKinematicsGenerator;}

namespace EDepSim {class JSONKinematicsFactory;}
class EDepSim::JSONKinematicsFactory : public EDepSim::VKinematicsFactory {
public:
    JSONKinematicsFactory(EDepSim::UserPrimaryGeneratorMessenger* fParent);
    virtual ~JSONKinematicsFactory();

    /// Return a new generator enclosing the current factory state.  The new
    /// generator method is pure virtual so it must be implemented by derived
    /// classes.
    virtual EDepSim::VKinematicsGenerator* GetGenerator();

    /// Set the input file to read.
    virtual void SetInputFile(const G4String& name) {fInputFile=name;}

    /// Get the input file to read.
    virtual const G4String& GetInputFile() const {return fInputFile;}

    /// Set the amount of output from G4HEPEvtInterface.
    virtual void SetVerbose(int v) {fVerbosity = v;}

    /// Get the expected level of output from G4HEPEvtInterface.
    virtual int GetVerbose() {return fVerbosity;}

    /// Handle the macro command inputs.
    virtual void SetNewValue(G4UIcommand* command,G4String newValue);

private:

    /// The text file of JSON records.
    G4String fInputFile;

    /// The verbosity of the event reading.
    G4int fVerbosity;

    /// A command to get the file name to read for the JSON records.
    G4UIcmdWithAString* fInputFileCMD;

    /// A command to set the JSON verbosity.
    G4UIcmdWithAnInteger* fVerboseCMD;

};
#endif
