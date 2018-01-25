/*Copyright 2017, 2018 Pranav Kant Gaur

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef INCLUDE_FIDUCIALDETECTION_H_
#define INCLUDE_FIDUCIALDETECTION_H_

#include <stdlib.h>
#include <string>
#include <vector>

#include <itkDOMNodeXMLReader.h>
#include <itkImage.h>
#include <itkGDCMImageIO.h>
#include <itkGDCMSeriesFileNames.h>
#include <itkImageSeriesReader.h>
#include <itkNrrdImageIOFactory.h>
#include <itkLabelGeometryImageFilter.h>


#include <itkArray.h>
#include <itkAffinityClustering.h>


/*! \struct FiducialOrientation
 * \brief Represents the orientation vector for a fiducial.
 */
struct FiducialOrientation {
  float alpha;
  float beta;
  float gamma;
};

/*! \class FiducialDetection
 * \brief The class modelling the concept of fiducial detector.
 */
class FiducialDetection {
 private:
  std::vector<FiducialOrientation> fiducial_orientation_vectors_;
  itk::Array<unsigned int> cluster_id_per_fiducial_;
  bool detector_configured_;
  std::string training_dataset_dir_;
  std::string test_dataset_dir_;
  size_t n_gaussians_;
  float percent_variation_eigen_vectors_;

  void GenerateTrainingData();
  void ExtractFiducialOrientationVector(itk::vnl_matrix<double>&,
                                        FiducialOrientation &);
  void GenerateFiducialTemplates(float, std::vector<FiducialOrientation>&,
                                 itk::Array<unsigned int>&);
  void GenerateFiducialCandidates(size_t);
  void MatchTemplatesWithCandidates();

 public:
  // Getters
  string GetTrainingDatasetDir();
  string GetTestDatasetDir();
  size_t GetNumberOfGaussians();
  float GetPercentVariationCoveredByEigenVectors();
  // Setters
  void SetTrainingDatasetDir(std::string);
  void GetTestDatasetDir(std::string);
  void GetNumberOfGaussians(size_t);
  void GetPercentVariationCoveredByEigenVectors(float);
  void Configure(std::string);  // Initializes the system,
                                //  with configuration read from
                                //  a system configuration file(assumes
                                //  XML)
  void Run();  // Orchestrates the fiducial detection process.
};
#endif  // INCLUDE_FIDUCIALDETECTION_H_
