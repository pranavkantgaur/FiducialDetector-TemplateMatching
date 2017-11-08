#include "SpectralClusteringTemplateSelection.h"
#include "MAPMARCandidateSelection.h"
#include "ICPTemplateRegistration.h"

class FiducialDetector{
	
	private:
		generateTrainingData(); // reads input DICOM images and extracts fiducials
		generateFiducialTemplates(); // generates representative fiducial templates
		generateFiducialCandidates(); // determines fiducial candidates from testing data
		matchTemplatesWithCandidates(); // performs matching of candidates against all templates

	public:
		run(); 		
};


