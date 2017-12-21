/* class SpectralClusteringTemplateSelection
 * \brief The class implements the spectral clustering approach for template selection.
*/
#include <vector>
#include <itkAffinityClustering.h>

class SpectralClusteringTemplateSelection{
	private:
		std::vector<std::vector<float> > affinityMatrix;
		unsigned int affinityMatrixSize;
	public:
		void generateFiducialTemplates(float, vector<int>);		

};
