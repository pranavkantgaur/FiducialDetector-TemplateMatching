/*! void FiducialDetector::configure()
 *  \brief Reads project configuration file and initializes internal parameters.
*/
void FiducialDetector::configure()
{
}


/* void FiducialDetector::run()
 * \brief Public interface for running the system
 */
void FiducialDetector::run()
{
	if (detectorConfigured == true)
	{
		generateTrainingData();
		generateFiducialTemplates();
		generateFiducialCandidates();
		matchTemplatesWithCandidates();
	}
	else
	{
		cout << "Please configure the detector!!";
		exit();
	}
}

