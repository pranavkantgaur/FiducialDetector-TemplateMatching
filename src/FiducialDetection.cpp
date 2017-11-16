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
	
	// dir location: training images
	itk::DOMNode::Pointer trainingDatasetDirDOMNode = output_dom_object->Find("training_dataset_dir"); 
	itk::DOMTextNode::Pointer textNodePtr = trainingDatasetDirDOMNode->GetTextChild();
	
	cout << "Training dataset dir: " << textNodePtr->GetText() << endl;
	// dir location for test images
	itk::DOMNode::Pointer testingDatasetDirDOMNode = output_dom_object->Find("test_dataset_dir");
	textNodePtr = testingDatasetDirDOMNode->GetTextChild();
	cout << "Test dataset dir: " << textNodePtr->GetText() << endl;

	 // % of variation as a therehold for selecting k 
	itk::DOMNode::Pointer percentVariationEigenVectorsDOMNode = output_dom_object->Find("percent_variation_eigen_vectors");
 	textNodePtr = percentVariationEigenVectorsDOMNode->GetTextChild();
	cout << "Percent variation for eigen vectors: " << textNodePtr->GetText() << endl;



	// number of gaussian distributions
	itk::DOMNode::Pointer nGaussiansDOMNode = output_dom_object->Find("nGaussians");
	textNodePtr = nGaussiansDOMNode->GetTextChild();
	cout << "nGaussians: " << textNodePtr->GetText() << endl;

	
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

