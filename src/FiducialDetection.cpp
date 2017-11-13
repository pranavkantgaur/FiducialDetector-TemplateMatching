/*! void FiducialDetector::configure()
 *  \brief Reads project configuration file and initializes internal parameters.
*/
void FiducialDetector::configure()
{
	// read project configuration file and initialize detector state
	itk::DOMNode::Pointer putput_dom_object;
	itk::DOMNodeXMLReader::Pointer reader = DOMNodeXMLReader::New();
	reader->SetFileName();
	reader->Update();
	output_dom_object = reader->GetOutput();

	// dir location: training images
	std::string trainingDatasetDir = output_dom_object->find("trainingDatasetDir"); // TODO check	
	// dir location for test images
	std::string testingDatasetDir = output_dom_object->find("testDatasetDir");

	 // % of variation as a therehold for selecting k 
	size_t percentVariationEigenVectors = output_dom_object->find("percentVariationEigenVectors");
 	// number of gaussian distributions
	size_t nGaussians = output_dom_object->find("nGaussians");
	
	// parameters for icp template registration parameters
	detectorConfigured = true;
}


/* void FiducialDetector::run()
 * \brief Public interface for running the system
 */
int FiducialDetection::run()
{
	if (detectorConfigured == true)
	{
		scts.generateFiducialTemplates();
//		mmfs.generateFiducialCandidates();
//		itr.matchTemplatesWithCandidates();
	}
	else
	{
		cout << "Please configure the detector!!";
		exit();
	}
	return 0;
}

