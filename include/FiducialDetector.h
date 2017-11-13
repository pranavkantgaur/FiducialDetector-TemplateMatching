#include <itkDOMNodeXMLReader.h>
#include <itkImage.h>
#include <itkGDCMImageIO.h>
#include <itkGDCMFileSeriesReader.h>
#include <itkImageFileWriter.h>
#include <itkNrrdImageIOFactory.h>

#include "SpectralClusteringTemplateSelection.h"
#include "MAPMARCandidateSelection.h"
#include "ICPTemplateRegistration.h"

using namespace std;
using namespace itk;

class FiducialDetector{
	
	private:
		SpectralClusteringTemplateSelection scts;
		MAPMARFiducialCandidateSelection mmfcs;
		ICPTemplateRegistration itr;
	public:
		configure(); // reads a config file defjning the project configuration
		run(); 		
};


