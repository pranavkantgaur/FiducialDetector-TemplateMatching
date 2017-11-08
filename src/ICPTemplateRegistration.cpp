/* void FiducialDetector::matchTemplatesWithCandidates()
 * \brief Matches fiducial candidates against templates
 */
void FiducialDetector::matchTemplatesWithCandidates()
{
	// Compares all prospective fiducial candidates 
		// convert fiducial candiates and templates into 3D volumes using Marching cube before registering with ICP
		// ICP minimizes the pose variations of the fiducial marker candidates with respect to templates
		// Assumption: Gold fiducial marker candidate will be similar to aleast one of the templates after rigid registration
}

