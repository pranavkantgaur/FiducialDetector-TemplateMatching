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
/*	typedef std::vector<std::vector<float> > Matrix;
	Matrix affinityMatrix(affMatrixSize, std::vector<float>(affMatrixSize));
*/
	typedef itk::AffinityClustering AffinityClusteringFilterType;
	AffinityClusteringFilterType::Pointer affinityClusteringFilter = AffinityClusteringFilterType::New();
	typedef double AffinityMatrixValueType;
	typedef itk::VariableSizeMatrix< AffinityMatrixValueType > AffinityMatrixType;
	AffinityMatrixType affinityMatrix;

	FiducialOrientation fiducialOneOrientationVector, fiducialTwoOrientationVector;
	for (unsigned int row = 0; row < affMatrixSize; row++)
		for (unsigned int col = 0; col < affMatrixSize; col++) // square matrix
		{
			fiducialOneOrientationVector = fiducialOrientationVectors[row];
			fiducialTwoOrientationVector = fiducialOrientationVectors[col];
			affinityMatrix[row][col] = fabs(fiducialOneOrientationVector.alpha - fiducialTwoOrientationVector.alpha) + fabs(fiducialOneOrientationVector.beta - fiducialTwoOrientationVector.beta) + fabs(fiducialOneOrientationVector.gamma - fiducialTwoOrientationVector.gamma);		 
		}

	// compute spectral clusters and return as output
	affinityClusteringFilter->SetInput(affinityMatrix);
	affinityClusteringFilter->Update(); // the computation step		
	clusterIDPerFiducial = affinityClusteringFilter->GetOutputClusters(); // for every fiducial , we get the cluster id
	// TODO: how to use percentVariationEigenVectors??
	// there is no control how to discard clusters based on percentage confidence
	std::cout << "Number of fiducial template clusters: " << affinityClusteringFilter->GetNumberOfClusters() << std::endl;
	
}	

