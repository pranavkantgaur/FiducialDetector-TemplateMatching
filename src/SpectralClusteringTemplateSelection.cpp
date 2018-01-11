#include "SpectralClusteringTemplateSelection.h"


/* void SpectralClusteringTemplateSelection::generateFiducialTemplates(float percentVariationEigenVectors, vector<int>& clusterIDPerFiducial)
 * \brief generates templates modeling fiducials from training data
 * \param [in] percentVariationEigenVectors
 * \param [out] clusterIDPerFiducial
*/
void SpectralClusteringTemplateSelection::generateFiducialTemplates(float percentVariationEigenVectors, std::vector<FiducialOrientation>& fiducialOrientationVectors, itk::Array<unsigned int>& clusterIDPerFiducial)
{	
	/* Approach:
		spectral clustering in orientation space
			compute similarity matrix between contoured fiducials over their orientations
 			compute first k eigenvectors , build matrix U
			Perform k-means clustering on U 
		return k-disjoint clusters */

	// compute affinity matrix
	unsigned int affMatrixSize = fiducialOrientationVectors.size();
	typedef std::vector<std::vector<float> > Matrix;
	Matrix affinityMatrix(affMatrixSize, std::vector<float>(affMatrixSize));
/*	std::vector< FiducialOrientation > fiducialOneOrientationVector, fiducialTwoOrientationVector;
	for (unsigned int row = 0; row < simMatrixSize; row++)
		for (unsigned int col = 0; col < simMatrixSize; col++) // square matrix
		{
			fiducialOneOrientationVector.push(fiducialOrientationVectors[row]);
			fiducialTwoOrientationVector.push(fiducialOrientationVectors[col]);
			affinityMatrix[row][col] = fabs(fiducialOneOrientationVector.alpha - fiducialTwoOrientatioVector.alpha) + fabs(fiducialOneOrientationVector.beta - fiducialTwoOrientatioVector.beta) + fabs(fiducialOneOrientationVector.gamma - fiducialTwoOrientationVector.gamma);		 
		}

	// compute spectral clusters and return as output
	typedef itk::AffinityClustering AffinityClusteringFilterType;
	AffinityClusteringFilterType::Pointer affinityClusteringFilter = AffinityClusteringFilterType::New();
	affinityClustering->SetInput(affinityMatrix);
	affinityClustering->Update(); // the computation step		
	clusterIDPerFiducial = affinityClustering->GetOutputClusters(); // for every fiducial , we get the cluster id
	// TODO: how to use percentVariationEigenVectors??
	// there is no control how to discard clusters based on percentage confidence
	std::cout << "Number of fiducial template clusters: " << affinityClustering->GetNumberOfClusters() << std::endl;
*/	
}	

