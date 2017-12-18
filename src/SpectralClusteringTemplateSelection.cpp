#include "SpectralClusteringTemplateSelection.h"

void computeSimilarityMatrix()
{
	simMatrixSize = fiducialOrientationVectors.size();
	similarityMatrix = new vector(simMatrixSize, vector<float>(simMatrixSize)>);
	for (unsigned int row = 0; row < simMatrixSize; row++)
		for (unsigned int col = 0; col < simMatrixSize; col++)
		{
			fiducialOneOrientationVector = fiducialOrientationVectors[row];
			fiducialTwoOrientationVector = fiducialOrientationVectors[col];
			similarityMatrix[row][col] = fabs(fiducialOneOrientationVector.alpha - fiducialTwoOrientatioVector.alpha) + fabs(fiducialOneOrientationVector.beta - fiducialTwoOrientatioVector.beta) + fabs(fiducialOneOrientationVector.gamma - fiducialTwoOrientationVector.gamma);		 
		}
}
/*
void computeKEigenVectors()
{
	// compute U matrix
	// U =  first k eigen vectors of (D-1/2)W(D+1/2), D=diag(simMatrix)
	vector<vector<float> > DMatrix;
	DMatrix = similarityMatrix;
	DMatrix.erase();
	// zero out all elements except diagonals
	unsigned int similarityMatrixSize = fiducialOrientationVectors.size();
	for (unsigned int row = 0; row < similarityMatrixSize; row++)
		for (unsigned int col = 0; col < similarityMatrixSize; col++)
		{
			if (row == col)
				DMatrix[row][col] = similarityMatrix[row][col];
		}
	// D-1/2WD+1/2 TODO
	Dmatrix
}

void computeKMeansClustering()
{
}
*/


/* void FiducialDetector::generateTemplates()
 * \brief generates templates modeling fiducials from training data
*/
void SpectralClusteringTemplateSelection::generateFiducialTemplates(float percentVariationEigenVectors)
{	
	/* Approach:
		spectral clustering in orientation space
			compute similarity matrix between contoured fiducials over their orientations
 			compute first k eigenvectors , build matrix U
			Perform k-means clustering on U 
		return k-disjoint clusters */
	computeSimilarityMatrix();
/*	computeKEigenVectors();
	computeKMeansClustering(); // returns disjoing clusters
*/
}

