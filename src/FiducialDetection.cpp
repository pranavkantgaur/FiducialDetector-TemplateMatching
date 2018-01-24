#include "FiducialDetection.h"

std::string FiducialDetection::getTrainingDatasetDir() {
	return this->trainingDatasetDir;
}



/*! \fn FiducialDetection::getTestDatasetDir()
 *  \brief Returns the location of input test data directory.
 *  \return std::string Representing the location of input test data directory
 */
std::string FiducialDetection::getTestDatasetDir() {
	return this->testDatasetDir;
}

/*! \fn FiducialDetection::getPercentVariationEigenVector()
 *  \brief Returns percentage of variation that must be captured by the selected Eigen vectors(fixes 'k' in k-way clustering)
 *  \return float Percente variation that must be captured by the Eigen vectors selected for Fiducial candidate selection.
 */
float FiducialDetection::getPercentVariationEigenVector(){
	return this->percentVariationEigenVector;
}

/*! \fn FiducialDetection::getNumberOfGaussians()
 *  \brief Returns number of Gaussians for Fiducial candidate selection stage.
 *  \param [in] inputTrainingDatasetDir input directory
 *  \return size_t Number of Gaussians for Fiducial candidate selection
 */
size_t FiducialDetection::getNumberOfGaussians(){
	return this->nGaussians;	
}


/*! \fn FiducialDetection::setTrainingDatasetDir(std::string inputTrainingDatasetDir)
 *  \brief sets directory for input training data
 *  \param [in] inputTrainingDatasetDir input directory
 *  \return void
 */
void FiducialDetection::setTrainingDatasetDir(std::string inputTrainingDatasetDir) {
	this->trainingDatasetDir = inputTrainingDatasetDir;
}

/*! \fn FiducialDetection::setTestDatasetDir(std::string inputTestDatasetDir)
 *  \brief sets test dataset directory
 *  \param [in] Input test directory location
 *  \return void
*/
void FiducialDetection::setTestDatasetDir(std::string inputTestDatasetDir) {
	this->testDatasetDir = inputTestDatasetDir;
}


/*! \fn FiducialDetection:setPercentVariationEigenVector(float inputPercentVariationEigenVector)
 *  \brief Sets percentatge variation for Eigen vector
 * \param [in] inputPercentVariationEigenVector Input percent variation for Eigen value based clustering.
*   \return void
*/
void FiducialDetection::setPercentageVariationEigenVector(float inputPercentVariationEigenVector){
	this->percentVariationEigenVector = inputPercentVariationEigenVector;
}

/*! \fn FiducialDetection:setNumberOfGaussians(size_t inputNumberOfGaussians)
 * \brief Sets number of Gaussians for fiducial candidate selection.
 * \param [in] inputNumberOfGaussians Number of Gaussians
*/
void FiducialDetection::setNumberOfGaussians(size_t inputNumberOfGaussians) {
	this->nGaussians = inputNumberOfGaussians;
}



/* void FiducialDetector::configure()
 *  \brief Reads project configuration file and initializes internal parameters.
 */
void FiducialDetection::configure(std::string fileName)
{
	// read project configuration file and initialize detector state
	itk::DOMNode::Pointer output_dom_object;
	itk::DOMNodeXMLReader::Pointer reader = DOMNodeXMLReader::New();
	reader->SetFileName(fileName);
	reader->Update();
	output_dom_object = reader->GetOutput();
	
	// dir location: training images
	itk::DOMNode::Pointer trainingDatasetDirDOMNode = output_dom_object->Find("training_dataset_dir"); 
	itk::DOMTextNode::Pointer textNodePtr = trainingDatasetDirDOMNode->GetTextChild();
	
	this->setTrainDatasetDir(textNodePtr->GetText());
	// dir location for test images
	itk::DOMNode::Pointer testingDatasetDirDOMNode = output_dom_object->Find("test_dataset_dir");
	textNodePtr = testingDatasetDirDOMNode->GetTextChild();
	this->setTestDatasetDir(textNodePtr->GetText());

	 // % of variation as a therehold for selecting k 
	itk::DOMNode::Pointer percentVariationEigenVectorsDOMNode = output_dom_object->Find("percent_variation_eigen_vectors");
 	textNodePtr = percentVariationEigenVectorsDOMNode->GetTextChild();
	this->setPercentVariationEigenVector(stof(textNodePtr->GetText()));
	
	// number of gaussian distributions
	itk::DOMNode::Pointer nGaussiansDOMNode = output_dom_object->Find("nGaussians");
	textNodePtr = nGaussiansDOMNode->GetTextChild();
	this->setNumberOfGaussians(stoi(textNodePtr->GetText()));
		
	// parameters for icp template registration parameters
	detectorConfigured = true;
}

/* \fn FiducialDetector::extractFiducialOrientation(vnl_matrix<double>& rotationMatrix, vector<FiducialOrientation>& fiducialOrientationVector)
 * \brief Extracts (alpha, beta, gamma) from input rotation matrix. Computes axis-angle representation from rotation matrix. Compute alpha, beta, gamma from axis-angle representation.
 * \param [in] rotationMatrix the input rotation matrix returned by itkLabelGeometryFilter::GetRotationMatrix()
 * \param [out] fiducialOrientationVector the output orientation vectors as required by training phase, for computing similarity matrix.
*/
void FiducialDetection::extractFiducialOrientationVector(vnl_matrix<double>& rotationMatrix , FiducialOrientation & fiducialOrientationVector)
{
	// Reference: http://nghiaho.com/?page_id=846 
	// Rotation matrix to Euler angle conversion, assumed rotation sequence YXZ, a.k.a Yaw-Pitch-Roll
	fiducialOrientationVector.alpha = atan2(rotationMatrix(2, 1), rotationMatrix(2, 2));
	fiducialOrientationVector.beta = atan2(-1.0 * rotationMatrix(2, 0), sqrtf(pow(rotationMatrix(2, 1), 2) + pow(rotationMatrix(2, 2), 2)));
	fiducialOrientationVector.gamma = atan2(rotationMatrix(1, 0), rotationMatrix(0, 0));
}


/* void FiducialDetector::readTrainingData()
 * \brief reads contoured fiducial samples, computes orientation of each sanple
 */
void FiducialDetection::generateTrainingData()
{
	// read DICOM images
	typedef signed short PixelType;
	const unsigned int Dimension = 3;
	typedef itk::Image<PixelType, Dimension> ImageType;
	typedef itk::ImageSeriesReader<ImageType> ReaderType;
	string dirName = trainingDatasetDir;

	typedef itk::GDCMSeriesFileNames NamesGeneratorType;
	NamesGeneratorType::Pointer nameGenerator = NamesGeneratorType::New();
	nameGenerator->SetGlobalWarningDisplay(false);
	nameGenerator->SetDirectory(dirName);

	try
	{
		typedef vector<string> SeriesIdContainer;
		const SeriesIdContainer & seriesUID = nameGenerator->GetSeriesUIDs();
		SeriesIdContainer::const_iterator seriesItr = seriesUID.begin();
		SeriesIdContainer::const_iterator seriesEnd = seriesUID.end();
		if (seriesItr != seriesEnd)
		{
			cout << "The directory: ";
			cout << dirName << endl;
			cout << "Contains the following DICOM series: " << endl;
		}
		else
		{
			cout << "No DICOMs in: " << dirName << endl;
			exit(0);
		}
		while (seriesItr != seriesEnd)
		{
			cout << seriesItr->c_str() << endl;
			++seriesItr;
		}
		seriesItr = seriesUID.begin();
		while(seriesItr != seriesUID.end())
		{
			string seriesIdentifier;
			seriesIdentifier = seriesItr->c_str();
			seriesItr++;
			typedef std::vector<std::string> FileNamesContainer;
			FileNamesContainer fileNames;
			fileNames = nameGenerator->GetFileNames(seriesIdentifier);
			ReaderType::Pointer reader = ReaderType::New();
			typedef itk::GDCMImageIO ImageIOType;
			ImageIOType::Pointer dicomIO = ImageIOType::New();
			reader->SetImageIO(dicomIO);
			reader->SetFileNames(fileNames);
			reader->Update(); // read the series of DICOM images				
			cout << "Number of files in this series: " << fileNames.size() << endl;
			
			size_t nFilesInSeries = fileNames.size();	
			
			// write fiducial orientations as the training data for the approach 
			typedef LabelGeometryImageFilter<ImageType> LabelGeometryImageFilterType;
			LabelGeometryImageFilterType::Pointer fiducialOrientationEstimator = LabelGeometryImageFilterType::New();

			FiducialOrientation fiducialOrientVector; // represents the orientation of a fiducial
			vnl_matrix<double> fiducialRotationMatrix; // represents the rotation matrix corresponding to a fiducial
			
			for (unsigned int fileID = 0; fileID < nFilesInSeries; fileID++) // for every label image file				    	
			{
				fiducialOrientationEstimator->SetInput(reader->GetOutput()); // sets the next label image to be read
				fiducialOrientationEstimator->Update(); // reads label image

				cout << "Number of fiducials in this image: " << fiducialOrientationEstimator->GetNumberOfLabels() << endl;
				size_t nLabelsInImage = fiducialOrientationEstimator->GetNumberOfLabels();
				for (size_t labelID = 0; labelID < nLabelsInImage; labelID++) 				
				{
					fiducialRotationMatrix = fiducialOrientationEstimator->GetRotationMatrix(labelID); // get rotation matrix
					this->extractFiducialOrientationVector(fiducialRotationMatrix, fiducialOrientVector); // 'fOrientVector' now contains orientation information for the fiducial
					fiducialOrientationVectors.push_back(fiducialOrientVector); // insert orientation triplet for this fiducial into a global training array.
				}		
			}
		}
	}
	catch (itk::ExceptionObject &ex)
	{
		cout << ex << endl;
	}				
}





/* void SpectralClusteringTemplateSelection::generateFiducialTemplates(float percentVariationEigenVectors, vector<int>& clusterIDPerFiducial)
 * \brief generates templates modeling fiducials from training data
 * \param [in] percentVariationEigenVectors
 * \param [out] clusterIDPerFiducial
*/
void FiducialDetection::generateFiducialTemplates(float percentVariationEigenVectors, std::vector<FiducialOrientation>& fiducialOrientationVectors, itk::Array<unsigned int>& clusterIDPerFiducial)
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
	affinityMatrix.SetSize(affMatrixSize, affMatrixSize);

	
	FiducialOrientation fiducialOneOrientationVector, fiducialTwoOrientationVector;
	for (unsigned int row = 0; row < affMatrixSize; row++)
		for (unsigned int col = 0; col < affMatrixSize; col++) // square matrix
		{
			fiducialOneOrientationVector = fiducialOrientationVectors[row];
			fiducialTwoOrientationVector = fiducialOrientationVectors[col];
			affinityMatrix(row, col) = fabs(fiducialOneOrientationVector.alpha - fiducialTwoOrientationVector.alpha) + fabs(fiducialOneOrientationVector.beta - fiducialTwoOrientationVector.beta) + fabs(fiducialOneOrientationVector.gamma - fiducialTwoOrientationVector.gamma);		 
		}

	// compute spectral clusters and return as output
	affinityClusteringFilter->SetInput(affinityMatrix);
	affinityClusteringFilter->Update(); // the computation step		
	clusterIDPerFiducial = affinityClusteringFilter->GetOutputClusters(); // for every fiducial , we get the cluster id
	// TODO: how to use percentVariationEigenVectors??		
	// there is no control how to discard clusters based on percentage confidence
	// TODO: identify leader fiducial to act as a template for next stage
	std::cout << "Number of fiducial template clusters: " << affinityClusteringFilter->GetNumberOfClusters() << std::endl;
}	


/* void FiducialDetector::generateFiducialCandidates()
 * \brief Generates most probable candidates modeling fiducials, combines Gaussian mixture modeling with Markov random field
 */
#include "MAPMARFiducialCandidateSelection.h"

void MAPMARFiducialCandidateSelection::generateFiducialCandidates(size_t nGaussians)
{
	// Perform unit variance and zero mean normalization of test data
	// Perform k-means clustering of test data to get initial class neabs for k Gaussian distributions
	// EM-based clustering is used to determine the likelihood of a voxel belonging to a Gauissian distribution
	// A MAP estimate of the likelihood is used to assign a label to the voxel
	// Means of Gaussian distributions are sorted in ascending order, Gold fdcials are obtained 'consistently' in one Gaussian distribution
	// To assign spatial context, Markovian model is enforced , ensuring voxels with same neighborhood labels and similar likelihoods are clustered together
	// Generate distinct voxel labels from previous MRF step by combining voxels that are connected in a 3X3X3 voxel neighborhood
	
	// Discard false positives:
		// learn Gaussian volume space from training data
		// if volume of a candidate falls outside threshold, discard
}


/* void FiducialDetector::matchTemplatesWithCandidates()
 * \brief Matches fiducial candidates against templates
 */
void FiducialDetector::matchTemplatesWithCandidates()
{
	// Compares all prospective fiducial candidates 
		// convert fiducial candiates and templates into 3D volumes using Marching cube before registering with ICP
		// ICP minimizes the pose variations of the fiducial marker candidates with respect to templates
		// Assumption: Gold fiducial marker candidate will be similar to aleast one of the templates after rigid registration
}


/* void FiducialDetector::run()
 * \brief Public interface for running the system
 */
void FiducialDetection::run()
{
	if (detectorConfigured == true)
	{
		generateTrainingData();
		scts.generateFiducialTemplates(percentVariationEigenVectors, fiducialOrientationVectors, clusterIDPerFiducial);
		mmfcs.generateFiducialCandidates(nGaussians);
/*		itr.matchTemplatesWithCandidates(testDatasetDir;
*/	}
	else
	{
		cout << "Please configure the detector!!";
		exit(1);
	}
}

