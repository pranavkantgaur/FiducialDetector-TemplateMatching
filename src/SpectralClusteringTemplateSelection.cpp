/* void FiducialDetector::readTrainingData()
 * \brief reads contoured fiducial samples, computes orientation of each sanple
 */
void FiducialDetector::generateTrainingData()
{
	// read DICOM images
	typedef signed short PixelType;
	const unsigned int Dimension = 3;
	typedef itk::Image<PixelType, Dimension> ImageType;
	typedef itk::ImageSeriesReader<ImageType> ReaderType;
	string dirName = "."; // initialization
	// TODO set directory name from configuration data

	typedef itk::GDCMSeriesFileNames NamesGeneratorType;
	NamaesGeneratorType::Pointer nameGenerator = NamesGeneratorTyppe::New();
	nameGenerator->SetUseSeriesDetails(true);
	nameGenerator->AddSeriesRestriction();
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
		typedef vector<string> FileNamesContainer;
		FileNamesConstainer fileNames;
		fileNames = namesGenerator-.GetFileNames(seriesIdentifier);
		ReaderType::Pointer reader = ReaderType::New();
		typedef itk::GDCMImageIO ImageIOType;
		ImageIOType::Pointer dicomIO = ImageIOType::New();
		reader->SetImageIO(dicomIO);
		reader->SetFilename(filenames);
	}
	}
	catch (itk::ExeceptionObject &ex)
	{
		cout << ex << endl;
		return EXIT_FAILURE;
	}				
	// compute orientation of each training data fiducial
	
}

/* void FiducialDetector::generateTemplates()
 * \brief generates templates modeling fiducials from training data
*/
void FiducialDetector::generateFiducialTemplates()
{	
	// spectral clustering in orientation space
		// compute similarity matrix between contoured fiducials over their orientations
		// compute first k eigenvectors , build matrix U
		// Perform k-means clustering on U 
		// return k-disjoint clusters
}

