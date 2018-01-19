#include <string>


#include <itkDOMNodeXMLReader.h>
#include <itkImage.h>
#include <itkGDCMImageIO.h>
#include <itkGDCMSeriesFileNames.h>
#include <itkImageSeriesReader.h>
#include <itkNrrdImageIOFactory.h>
#include <itkLabelGeometryImageFilter.h>



#include "Globals.h"
#include "SpectralClusteringTemplateSelection.h"
#include "MAPMARFiducialCandidateSelection.h"

//#include "ICPTemplateRegistration.h"

using namespace std;
using namespace itk;

class FiducialDetection{
	
	private:
		vector<FiducialOrientation> fiducialOrientationVectors;				       		Array<unsigned int> clusterIDPerFiducial; // the output of spectral clustering	
		bool detectorConfigured;
		std::string trainingDatasetDir;
		std::string testDatasetDir;
		size_t nGaussians;
		float percentVariationEigenVectors;
		SpectralClusteringTemplateSelection scts;
		MAPMARFiducialCandidateSelection mmfcs;
/*		ICPTemplateRegistration itr;
		*/
		void generateTrainingData();
		void extractFiducialOrientationVector(vnl_matrix<double>&, FiducialOrientation &);
	public:
		void configure(std::string); 
		void run();
};


