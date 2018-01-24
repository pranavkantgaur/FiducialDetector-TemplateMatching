#ifndef INCLUDE_FIDUCIALDETECTION_H
#define INCLUDE_FIDUCIALDETECTION_H

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

using namespace std;
using namespace itk;

/*! \struct FiducialOrientation
 * \brief Represents the orientation vector for a fiducial.
 */
struct FiducialOrientation {
	float alpha;
	float beta;
	float gamma;
};

/*! \class FiducialDetection
 * \brief The class modelling the concept of fiducial detector.
 */
class FiducialDetection {
	
	private:
		vector<FiducialOrientation> fiducial_orientation_vectors_;	
		Array<unsigned int> cluster_id_per_fiducial_; // the output of spectral clustering	
		bool detector_configured_;
		std::string training_dataset_dir_;
		std::string test_dataset_dir_;
		size_t n_gaussians_;
		float percent_variation_eigen_vectors_;

		void GenerateTrainingData();
		void ExtractFiducialOrientationVector(vnl_matrix<double>&, FiducialOrientation &);
		void GenerateFiducialTemplates(float, std::vector<FiducialOrientation>&, itk::Array<unsigned int>&);	
		
		void GenerateFiducialCandidates(size_t);
		void MatchTemplatesWithCandidates();

	public:
		// Getters
		string GetTrainingDatasetDir();
		string GetTestDatasetDir();
		size_t GetNumberOfGaussians();
		float GetPercentVariationCoveredByEigenVectors(); 	

		// Setters
		void SetTrainingDatasetDir(string);
		void GetTestDatasetDir(string);
		void GetNumberOfGaussians(size_t);
		void GetPercentVariationCoveredByEigenVectors(float); 	

		void Configure(std::string); /*!< Initializes the system, with configuration read from a system configuration file(assumes XML)*/ 
		void Run(); /*!< Orchestrates the fiducial detection process.*/
};
#endif
