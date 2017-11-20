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
	cout << "Percent variation for Eigen vector selection: " << percentVariationEigenVectors << endl;

	// number of gaussian distributions
	itk::DOMNode::Pointer nGaussiansDOMNode = output_dom_object->Find("nGaussians");
	textNodePtr = nGaussiansDOMNode->GetTextChild();
	nGaussians = stoi(textNodePtr->GetText());
	cout << "Number of gaussians: " << nGaussians << endl;
	
	// parameters for icp template registration parameters
	detectorConfigured = true;
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
	nameGenerator->SetUseSeriesDetails(true);
	nameGenerator->AddSeriesRestriction("0008|0021");
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
			cout << "Reading: ";
			typedef std::vector<std::string> FileNamesContainer;
			FileNamesContainer fileNames;
			fileNames = nameGenerator->GetFileNames(seriesIdentifier);
			ReaderType::Pointer reader = ReaderType::New();
			typedef itk::GDCMImageIO ImageIOType;
			ImageIOType::Pointer dicomIO = ImageIOType::New();
			reader->SetImageIO(dicomIO);
			reader->SetFileNames(fileNames);
		}
	}
	catch (itk::ExceptionObject &ex)
	{
		cout << ex << endl;
	}				
	FiducialOrientation fOrientVector;
/*	for (unsigned int sID = 0; sID < nTrainingSamples; sID++)
		for (unsigned int fID = 0; fID < nFiducialsPerSample; fID++)
		{
			fOrientVector.alpha = ;
			fOrientVector.beta = ;
			fOrientVector.gamma = ;
			fiducialOrientationVectors.push(fOrientVector);
		}
	*/	
}



/* void FiducialDetector::run()
 * \brief Public interface for running the system
 */
void FiducialDetection::run()
{
	if (detectorConfigured == true)
	{
		generateTrainingData();
		scts.generateFiducialTemplates(percentVariationEigenVectors);
/*		mmfs.generateFiducialCandidates(nGaussians);
		itr.matchTemplatesWithCandidates(testDatasetDir;
*/	}
	else
	{
		cout << "Please configure the detector!!";
		exit(1);
	}
}

