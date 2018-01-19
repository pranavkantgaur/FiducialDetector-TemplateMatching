/* void FiducialDetector::generateFiducialCandidates()
 * \brief Generates most probable candidates modeling fiducials, combines Gaussian mixture modeling with Markov random field
 */
#include "MAPMARFiducialCandidateSelection.h"

void MAPMARFiducialCandidateSelection::generateFiducialCandidates(size_t nGaussians)
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

