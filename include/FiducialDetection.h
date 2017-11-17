#include <string>


#include <itkDOMNodeXMLReader.h>
#include <itkImage.h>
#include <itkGDCMImageIO.h>
#include <itkGDCMSeriesFileNames.h>
#include <itkImageSeriesReader.h>
#include <itkNrrdImageIOFactory.h>
//#include <itkLabelGeometryImageFilter.h>




#include "SpectralClusteringTemplateSelection.h"
//#include "MAPMARCandidateSelection.h"
//#include "ICPTemplateRegistration.h"

using namespace std;
using namespace itk;

class FiducialDetection{
	
	private:
		struct FiducialOrientation{
			float alpha;
			float beta;
			float gamma;
		};
		std::vector<FiducialOrientation> fiducialOrientationVectors;			

		bool detectorConfigured;
		std::string trainingDatasetDir;
		std::string testDatasetDir;
		size_t nGaussians;
		float percentVariationEigenVectors;
		SpectralClusteringTemplateSelection scts;
/*		MAPMARFiducialCandidateSelection mmfcs;
		ICPTemplateRegistration itr;
		*/
		void generateTrainingData();
	public:
		void configure(std::string); 
		void run();
};


