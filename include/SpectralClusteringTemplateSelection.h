/* class SpectralClusteringTemplateSelection
 * \brief The class implements the spectral clustering approach for template selection.
*/
class SpectralClusteringTemplateSelection{
	private:
		class FiducialOrientation{
			float alpha;
			float beta;
			float gamma;
		};
		vector<FiducialOrientation> fiducialOrientationVectors;
		vector<vector<float> > similarityMatrix;
		unsigned int similarityMatrixSize;
		vector<vector<float> > UMatrix;
		void computeFiducialOrientations();
		void computeSimilarityMatrix();
		void computeEigenVectors();
		void computeKmeansClustering();
		void generateFiducialTemplates();		

	protected:
		void generateTrainingData(std::string, float);
};
