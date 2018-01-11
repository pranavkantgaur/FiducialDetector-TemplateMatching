/* class SpectralClusteringTemplateSelection
 * \brief The class implements the spectral clustering approach for template selection.
*/
#include <vector>
#include <itkArray.h>
#include <itkAffinityClustering.h>
#include "Globals.h"

class SpectralClusteringTemplateSelection{
	public:
		void generateFiducialTemplates(float, std::vector<FiducialOrientation>&, itk::Array<unsigned int>&);		

};
