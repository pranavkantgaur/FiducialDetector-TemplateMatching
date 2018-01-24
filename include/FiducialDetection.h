#include <stdlib.h>
#include <string>
#include <vector>

#include <itkDOMNodeXMLReader.h>
#include <itkImage.h>
#include <itkGDCMImageIO.h>
#include <itkGDCMSeriesFileNames.h>
#include <itkImageSeriesReader.h>
#include <itkNrrdImageIOFactory.h>
#include <itkLabelGeometryImageFilter.h>


#include <itkArray.h>
#include <itkAffinityClustering.h>
#include "Globals.h"

using namespace std;
using namespace itk;

struct FiducialOrientation {
	float alpha;
	float beta;
	float gamma;
};

class FiducialDetection{
	
	private:
		vector<FiducialOrientation> fiducialOrientationVectors;	
		Array<unsigned int> clusterIDPerFiducial; // the output of spectral clustering	
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
		std::string getTrainingDir();
		std::string getTestDir();
		float getPercentVariationEigenVector();
		size_t getNumberOfGaussians();
		void setTrainingDir();
		void setTestDir();
		void setPercentageVariationEigenVector();
		void setNumberOfGaussians();

		void generateFiducialTemplates(float, std::vector<FiducialOrientation>&, itk::Array<unsigned int>&);	
		
		void generateFiducialCandidates(size_t nGaussians);

	public:
		void configure(std::string); 
		void run();
};


