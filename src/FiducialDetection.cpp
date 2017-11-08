/*! void FiducialDetector::configure()
 *  \brief Reads project configuration file and initializes internal parameters.
*/
void FiducialDetector::configure()
{
	// read project configuration file and initialize detector state
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

