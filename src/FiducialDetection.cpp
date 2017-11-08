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


/* void FiducialDetector::generateFiducialCandidates()
 * \brief Generates most probable candidates modeling fiducials, combines Gaussian mixture modeling with Markov random field
 */
void FiducialDetector::generateFiducialCandidates()
{
	// Perform unit variance and zero mean normalization of test data
	// Perform k-means clustering of test data to get initial class neabs for k Gaussian distributions
	// EM-based clustering is used to determine the likelihood of a voxel belonging to a Gauissian distribution
	// A MAP estimate of the likelihood is used to assign a label to the voxel
	// Means of Gaussian distributions are sorted in ascending order, Gold fdcials are obtained 'consistently' in one Gaussian distribution
	// To assign spatial context, Markovian model is enforced , ensuring voxels with same neighborhood labels and similar likelihoods are clustered together
	// Generate distinct voxel labels from previous MRF step by combining voxels that are connected in a 3X3X3 voxel neighborhood
	
	// Discard false positives:
		// learn Gaussian volume space from training data
		// if volume of a candidate falls outside threshold, discard
}


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



/* void FiducialDetector::run()
 * \brief Public interface for running the system
 */
void FiducialDetector::run()
{
	generateTrainingData();
	generateFiducialTemplates();
	generateFiducialCandidates();
	matchTemplatesWithCandidates();
}

