#include "FiducialDetection.h"


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
	
	trainingDatasetDir = textNodePtr->GetText();
	// dir location for test images
	itk::DOMNode::Pointer testingDatasetDirDOMNode = output_dom_object->Find("test_dataset_dir");
	textNodePtr = testingDatasetDirDOMNode->GetTextChild();
	testDatasetDir = textNodePtr->GetText();

	 // % of variation as a therehold for selecting k 
	itk::DOMNode::Pointer percentVariationEigenVectorsDOMNode = output_dom_object->Find("percent_variation_eigen_vectors");
 	textNodePtr = percentVariationEigenVectorsDOMNode->GetTextChild();
	percentVariationEigenVectors = stof(textNodePtr->GetText());
	
	// number of gaussian distributions
	itk::DOMNode::Pointer nGaussiansDOMNode = output_dom_object->Find("nGaussians");
	textNodePtr = nGaussiansDOMNode->GetTextChild();
	nGaussians = stoi(textNodePtr->GetText());
		
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


/* void FiducialDetector::run()
 * \brief Public interface for running the system
 */
void FiducialDetection::run()
{
	if (detectorConfigured == true)
	{
		generateTrainingData();
/*		scts.generateFiducialTemplates(percentVariationEigenVectors, clusterIDPerFiducial);
		mmfs.generateFiducialCandidates(nGaussians);
		itr.matchTemplatesWithCandidates(testDatasetDir;
*/	}
	else
	{
		cout << "Please configure the detector!!";
		exit(1);
	}
}

