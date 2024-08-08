#ifndef __EDepSim_JSON_Kinematics_Generator_hh__seen
#define __EDepSim_JSON_Kinematics_Generator_hh__seen

#include <globals.hh>
#include <G4LorentzVector.hh>

#include "kinem/EDepSimVKinematicsGenerator.hh"

#include <string>
#include <fstream>

#ifdef EDEPSIM_HAS_JSON
#include "nlohmann/json.hpp" /// external dependency. allows us to parse json files.
#endif

class G4Event;
class G4VPrimaryGenerator;

/**
 * @class EDepSimJSONKinematicsGenerator.hh
 * @brief Class to read particles from json file
 *
 */
namespace EDepSim { class JSONKinematicsGenerator; };
class EDepSim::JSONKinematicsGenerator :
  public EDepSim::VKinematicsGenerator
{
public:
  JSONKinematicsGenerator(const G4String& name,
			  const G4String& fileName,
			  int verbosity = 0);
  virtual ~JSONKinematicsGenerator();

  /// Add a primary vertex to the event.
  virtual GeneratorStatus
  GeneratePrimaryVertex(G4Event* evt,
			const G4LorentzVector& position);

protected:

  int fverbosity;
  std::string fjson_input_path;
  size_t fcurrent_json_entry;
  size_t ftotal_json_entries;
#ifdef EDEPSIM_HAS_JSON
  nlohmann::json json_event_block; /// object for parseing the contents of block: "events":[{...},{...},...],                                                                               
#endif

};



  

#endif

