#include "kinem/EDepSimJSONKinematicsGenerator.hh"

#include "EDepSimVertexInfo.hh"
#include "G4PrimaryVertex.hh"
#include "G4Event.hh"

#include <fstream>
#include <sstream>

EDepSim::JSONKinematicsGenerator::JSONKinematicsGenerator(const G4String& name,
							  const G4String& fileName,
							  int verbosity)
  : EDepSim::VKinematicsGenerator(name),
    fverbosity(verbosity),    
    fjson_input_path( fileName ),
    fcurrent_json_entry(0),
    ftotal_json_entries(0)
{

  G4cout << "[EDepSim::JSONKinematicsGenerator] Initializing json file input." << G4endl;
  G4cout << "input file: " << fjson_input_path << G4endl;

  std::ifstream i( fjson_input_path.c_str() );
  
  // load json object
  // why do we have a second implementation of json?
  // This one (nlohmann) has a very standard library-like interface, which is nice.
  // Marley had their own. oh well.
#ifdef EDEPSIM_HAS_JSON
  nlohmann::json j;
  try {
    i >> j;
  }
  catch (const std::exception& e ) {
    std::stringstream msg;
    msg << "[" << __FUNCTION__ << ".L" << __LINE__ << "] error parsing file: " << e.what() << std::endl;
    throw std::runtime_error(msg.str());
  }

  // get event list
  if ( j.find("events")==j.end() ) {
    std::string msg = "Did not find 'events' key in JSON file";
    throw std::runtime_error(msg);
  }

  json_event_block = j["events"];
  ftotal_json_entries = json_event_block.size();
  fcurrent_json_entry = 0;
  G4cout << "number of events specified in json file: " << ftotal_json_entries << G4endl;
#endif
  
}

EDepSim::JSONKinematicsGenerator::~JSONKinematicsGenerator()
{
  
}

/**
 * @brief Generates primary particles at the given vertex location
 *
 * Note: This will be called by EDepSim::PrimaryGenerator::GeneratePrimaryVertex(...)
 * A position for a vertex is first generated and passed to this location.
 *
 */
EDepSim::VKinematicsGenerator::GeneratorStatus
EDepSim::JSONKinematicsGenerator::GeneratePrimaryVertex(
   G4Event* evt, const G4LorentzVector& vertex_position)
{

  if ( fcurrent_json_entry >= ftotal_json_entries ) {
    std::string msg = "We have run out of json entries";
    return EDepSim::VKinematicsGenerator::kFail;
  }

#ifdef EDEPSIM_HAS_JSON
  nlohmann::json jevent = json_event_block.at( fcurrent_json_entry );
  // Get Pariticle List
  if ( jevent.find("final_particles")==jevent.end() ) {
    std::string msg = "did not find key 'final_particles' in 'event' block";
    throw std::runtime_error(msg);
  }
  
  size_t num_particles = jevent["final_particles"].size();
  G4cout << "[CENNSPrimaryGeneratorAction::" << __FUNCTION__<< "]  Number of particles: " << num_particles << G4endl;
  
  G4double vtxX = vertex_position.X;
  G4double vtxY = vertex_position.Y;
  G4double vtxZ = vertex_position.Z;
  G4double vtxT = vertex_position.T;
  
  std::unique_ptr<G4PrimaryVertex> vertex(new G4PrimaryVertex(vtxX,vtxY,vtxZ,vtxT));
  // Add vertex ID                                                                                                                                                                          
  EDepSim::VertexInfo *vertexInfo = new EDepSim::VertexInfo;
  vertex->SetUserInformation(vertexInfo);
  vertexInfo->SetInteractionNumber((int)fcurrent_json_entry);

  for ( size_t iparticle=0; iparticle<num_particles; iparticle++ ) {

    nlohmann::json jparticle = jevent["final_particles"].at(iparticle);
    
    // get the particle info
    int pdg = jparticle["pdg"].get<int>();
    G4double energy = jparticle["E"].get<double>()*MeV;
    G4double mass   = jparticle["mass"].get<double>()*MeV;
    G4double px     = jparticle["px"].get<double>()*MeV;
    G4double py     = jparticle["py"].get<double>()*MeV;
    G4double pz     = jparticle["pz"].get<double>()*MeV;

    G4double ke = (energy-mass);
    std::cout << " [" << iparticle << "] pdg=" << pdg << " KE=" << ke << std::endl;

    std::unique_ptr<G4PrimaryParticle> part(new G4PrimaryParticle(pdg, px, py, pz));
    part->SetMass(mass);
       
    // if ( pdg>1000000000 ) {
    //   // nuclei
    //   int A = pdg/10%1000;
    //   int Z = pdg/10000%1000;
    //   G4ParticleDefinition* recoil_nucleus = G4IonTable::GetIonTable()->GetIon(Z, A, 0.0*keV);
    //   fPartGenerator->SetParticleDefinition(recoil_nucleus);
    // }
    
    // add particle to vertex
    vertex->SetPrimary( part.release() );
  }

  evt->AddPrimaryVertex(vertex.release());
  
  // Print status messages about simulation progress after every 100
  // events have been generated
  if ( fcurrent_json_entry>0 && fcurrent_json_entry % 100 == 1 ) {
    // Print a status message showing the current number of events
    G4cout << "[EDepSim::JSONKinematicsGenerator::" << __FUNCTION__ << "] Event Count = " << fcurrent_json_entry << G4endl;
  }

  fcurrent_json_entry++;

  return kSuccess;
#else
  std::stringstream msg;
  msg << "JSONKinematicsGenerator not working. Must build with nlohmann::json package found." << std::endl;
  throw std::runtime_error( msg.str() );
  return kFail;
#endif

}


