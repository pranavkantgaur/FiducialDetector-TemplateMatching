/*! void FiducialDetector::configure()
 *  \brief Reads project configuration file and initializes internal parameters.
*/
void FiducialDetector::configure()
{
	// read project configuration file and initialize detector state
	DOMNode::Pointer putput_dom_object;
	DOMNodeXMLReader::Pointer reader = DOMNodeXMLReader::New();
	reader->SetFileName();
	reader->Update();
	output_dom_object = reader->GetOutput();

	// dir location: training images
	string trainingDatasetDir = output_dom_object->find("trainingDatasetDir"); // TODO check	
	// dir location for test images
	string testingDatasetDir = output_dom_object->find("testDatasetDir");

	 // % of variation as a therehold for selecting k 
	size_t percentVariationEigenVectors = output_dom_object->find("percentVariationEigenVectors");
 	// number of gaussian distributions
	size_t nGaussians = output_dom_object->find("nGaussians");
	
	// icp template registration parameters

}


/* void FiducialDetector::run()
 * \brief Public interface for running the system
 */
void FiducialDetector::run()
{
	if (detectorConfigured == true)
	{
		scts.generateFiducialTemplates();
		mmfs.generateFiducialCandidates();
		itr.matchTemplatesWithCandidates();
	}
	else
	{
		cout << "Please configure the detector!!";
		exit();
	}
}

