/* void FiducialDetector::readTrainingData()
 * \brief reads contoured fiducial samples, computes orientation of each sanple
 */
void FiducialDetector::generateTrainingData()
{
	// read DICOM images
			
	// compute orientation of each training data fiducial
	
}

/* void FiducialDetector::generateTemplates()
 * \brief generates templates modeling fiducials from training data
*/
void FiducialDetector::generateFiducialTemplates()
{	
	// spectral clustering in orientation space
		// compute similarity matrix between contoured fiducials over their orientations
		// compute first k eigenvectors , build matrix U
		// Perform k-means clustering on U 
		// return k-disjoint clusters
}

