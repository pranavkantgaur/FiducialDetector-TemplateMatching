#include "SpectralClusteringTemplateSelection.h"

/* void SpectralClusteringTemplateSelection::generateFiducialTemplates(float percentVariationEigenVectors, vector<int>& clusterIDPerFiducial)
 * \brief generates templates modeling fiducials from training data
 * \param [in] percentVariationEigenVectors
 * \param [out] clusterIDPerFiducial
*/
void SpectralClusteringTemplateSelection::generateFiducialTemplates(float percentVariationEigenVectors, vector<int>& clusterIDPerFiducial)
{	
	/* Approach:
		spectral clustering in orientation space
			compute similarity matrix between contoured fiducials over their orientations
 			compute first k eigenvectors , build matrix U
			Perform k-means clustering on U 
		return k-disjoint clusters */

	// compute affinity matrix
	affMatrixSize = fiducialOrientationVectors.size();
	affinityMatrix = new vector(affMatrixSize, vector<float>(affMatrixSize)>);
	vector< FiducialOrientation > fiducialOneOrientationVector, fiducialTwoOrientationVector;
	for (unsigned int row = 0; row < simMatrixSize; row++)
		for (unsigned int col = 0; col < simMatrixSize; col++) // square matrix
		{
			fiducialOneOrientationVector.push(fiducialOrientationVectors[row]);
			fiducialTwoOrientationVector.push(fiducialOrientationVectors[col]);
			affinityMatrix[row][col] = fabs(fiducialOneOrientationVector.alpha - fiducialTwoOrientatioVector.alpha) + fabs(fiducialOneOrientationVector.beta - fiducialTwoOrientatioVector.beta) + fabs(fiducialOneOrientationVector.gamma - fiducialTwoOrientationVector.gamma);		 
		}

	// compute spectral clusters and return as output
	itkAffinityClustering::Pointer affinityClustering = itkAffinityClustering::New();
	affinityClustering->SetInput();
	affinityClustering->Update(); // the computation step		
	clustrerIDPerFiducial = affinityClustering->GetOutput(); // TODO: how to use percentVariationEigenVectors??
}	

