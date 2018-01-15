#include "itkAffinityClustering.h"

int main()
{
  typedef itk::AffinityClustering AffinityClusteringFilterType;
  AffinityClusteringFilterType::Pointer filter = AffinityClusteringFilterType::New();

  typedef AffinityClusteringFilterType::AffinityMatrixType AffinityMatrixType;
  AffinityMatrixType affinityMatrix;
  affinityMatrix.SetSize( 4, 4 );
  affinityMatrix.Fill( 12.1 );
  
  filter->SetInput( affinityMatrix );
  
  filter->Print( std::cout );
  std::cout << (filter->GetAffinityMatrix()) << std::endl;
  std::cout << filter->GetOutputClusters() << std::endl;

  return EXIT_SUCCESS;
}

  
