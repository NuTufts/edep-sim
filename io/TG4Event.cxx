////////////////////////////////////////////////////////////
//
#include "TG4Event.h"

ClassImp(TG4Event)

TG4Event::~TG4Event() {}

int TG4Event::GetNumSegmentDetectors()
{
    return SegmentDetectors.size();
}

std::vector<std::string> TG4Event::GetSegmentDetectorNameList()
{
    std::vector<std::string> names;
    for ( auto it=SegmentDetectors.begin(); it!=SegmentDetectors.end(); it++ ) {
        names.push_back( it->first );
    }
    return names;
}
