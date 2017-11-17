/* class SpectralClusteringTemplateSelection
 * \brief The class implements the spectral clustering approach for template selection.
*/
#include <vector>

class SpectralClusteringTemplateSelection{
	private:
		std::vector<std::vector<float> > similarityMatrix;
		unsigned int similarityMatrixSize;
		std::vector<std::vector<float> > UMatrix;
		void computeFiducialOrientations();
		void computeSimilarityMatrix();
		void computeEigenVectors();
		void computeKmeansClustering();
	
	public:
		void generateFiducialTemplates(float);		

};
