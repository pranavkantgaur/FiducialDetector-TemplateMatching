#include "FiducialDetection.h"


/* void FiducialDetector::configure()
 *  \brief Reads project configuration file and initializes internal parameters.
 */
void FiducialDetection::configure(std::string fileName)
{
	// read project configuration file and initialize detector state
	itk::DOMNode::Pointer output_dom_object;
	itk::DOMNodeXMLReader::Pointer reader = DOMNodeXMLReader::New();
	reader->SetFileName(fileName);
	reader->Update();
	output_dom_object = reader->GetOutput();
	
	cout << "Name: " << output_dom_object->GetName() << endl;
	// dir location: training images
	itk::DOMNode::Pointer trainingDatasetDir = output_dom_object->Find("training_dataset_dir"); // TODO check
 	cout << "Name of training dataset tag: " << trainingDatasetDir->GetName() << endl;
	cout << "Dir location: " << trainingDatasetDir->GetAttribute("training_dataset_dir") << endl;	
	// dir location for test images
	itk::DOMNode::Pointer testingDatasetDir = output_dom_object->Find("test_dataset_dir");

	 // % of variation as a therehold for selecting k 
	itk::DOMNode::Pointer percentVariationEigenVectors = output_dom_object->Find("percent_variation_eigen_vectors");
 	// number of gaussian distributions
	itk::DOMNode::Pointer nGaussians = output_dom_object->Find("nGaussians");
	
	// parameters for icp template registration parameters
	detectorConfigured = true;
}


/* void FiducialDetector::run()
 * \brief Public interface for running the system
 */
void FiducialDetection::run()
{
	if (detectorConfigured == true)
	{
/*		scts.generateFiducialTemplates(trainingDatasetDir, percentConfidenceEigenVectors);
		mmfs.generateFiducialCandidates(nGaussians);
		itr.matchTemplatesWithCandidates(testDatasetDir;
*/	}
	else
	{
		cout << "Please configure the detector!!";
		exit(1);
	}
}

