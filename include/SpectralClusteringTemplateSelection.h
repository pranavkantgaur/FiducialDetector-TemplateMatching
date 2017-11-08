/* class SpectralClusteringTemplateSelection
 * \brief The class implements the spectral clustering approach for template selection.
*/
class SpectralClusteringTemplateSelection{
	private:
		computeFiducialOrientations();
		computeSimilarityMatrix();
		computeEigenVectors();
		computeKmeansClustering();
	protected:
		generateFiducialTemplates();		
};
