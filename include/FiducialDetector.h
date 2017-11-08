#include "SpectralClusteringTemplateSelection.h"
#include "MAPMARCandidateSelection.h"
#include "ICPTemplateRegistration.h"

class FiducialDetector{
	
	private:
		SpectralClusteringTemplateSelection scts;
		MAPMARFiducialCandidateSelection mmfcs;
		ICPTemplateRegistration itr;
	public:
		configure(); // reads a config file defjning the project configuration
		run(); 		
};


