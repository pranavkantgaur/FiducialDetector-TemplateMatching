/* class SpectralClusteringTemplateSelection
 * \brief The class implements the spectral clustering approach for template selection.
*/
class SpectralClusteringTemplateSelection{
	private:
		void computeFiducialOrientations();
		void computeSimilarityMatrix();
		void computeEigenVectors();
		void computeKmeansClustering();
		void generateFiducialTemplates();		

	protected:
		void generateTrainingData();
};
