#include <itkDOMNodeXMLReader.h>
#include <itkImage.h>
#include <itkGDCMImageIO.h>
#include <itkGDCMSeriesFileNames.h>
#include <itkImageFileReader.h>
#include <itkNrrdImageIOFactory.h>
//#include <itkLabelGeometryImageFilter.h>




//#include "SpectralClusteringTemplateSelection.h"
//#include "MAPMARCandidateSelection.h"
//#include "ICPTemplateRegistration.h"

using namespace std;
using namespace itk;

class FiducialDetection{
	
	private:
		bool detectorConfigured;
		std::string trainingDatasetDir;
		std::string testDatasetDir;
		size_t nGaussians;
		float percentVariationEigenVectors;
/*		SpectralClusteringTemplateSelection scts;
		MAPMARFiducialCandidateSelection mmfcs;
		ICPTemplateRegistration itr;
*/	public:
		void configure(std::string); 
		void run();
};


