/*! void FiducialDetector::configure()
 *  \brief Reads project configuration file and initializes internal parameters.
*/
#include "FiducialDetection.h"




void FiducialDetection::configure(std::string fileName)
{
	// read project configuration file and initialize detector state
	itk::DOMNode::Pointer output_dom_object;
	itk::DOMNodeXMLReader::Pointer reader = DOMNodeXMLReader::New();
	reader->SetFileName(fileName);
	reader->Update();
	output_dom_object = reader->GetOutput();

	// dir location: training images
	itk::DOMNode::Pointer trainingDatasetDir = output_dom_object->Find("trainingDatasetDir"); // TODO check	
	// dir location for test images
	itk::DOMNode::Pointer testingDatasetDir = output_dom_object->Find("testDatasetDir");

	 // % of variation as a therehold for selecting k 
	itk::DOMNode::Pointer percentVariationEigenVectors = output_dom_object->Find("percentVariationEigenVectors");
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
/*		scts.generateFiducialTemplates();
		mmfs.generateFiducialCandidates();
		itr.matchTemplatesWithCandidates();
*/	}
	else
	{
		cout << "Please configure the detector!!";
		exit(1);
	}
}

