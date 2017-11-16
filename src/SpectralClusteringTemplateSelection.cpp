/* void FiducialDetector::readTrainingData()
 * \brief reads contoured fiducial samples, computes orientation of each sanple
 */
void SpectralClusteringTemplateSelection::generateTrainingData(std::string trainingDatasetDir, float nGaussians)
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
	
	
	FiducialOrientation fOrientVector;
	for (unsigned int sID = 0; sID < nTrainingSamples; sID++)
		for (unsigned int fID = 0; fID < nFiducialsPerSample; fID++)
		{
			fOrientVector.alpha = ;
			fOrientVector.beta = ;
			fOrientVector.gamma = ;
			fiducialOrientationVectors.push(fOrientVector);
		}
		
}

void computeSimilarityMatrix()
{
	simMatrixSize = fiducialOrientationVectors.size();
	similarityMatrix = new vector(simMatrixSize, vector<float>(simMatrixSize)>);
	for (unsigned int row = 0; row < simMatrixSize; row++)
		for (unsigned int col = 0; col < simMatrixSize; col++)
			similarityMatrix[][] = fabs() + fabs() + fabs();		 
}

void computeKEigenVectors()
{
	// compute U matrix
	// U =  first k eigen vectors of (D-1/2)W(D+1/2), D=diag(simMatrix)
	vector<vector<float> > DMatrix;
	DMatrix = similarityMatrix;
	DMatrix.erase();
	// zero out all elements except diagonals
	unsigned int similarityMatrixSize = fiducialOrientationVectors.size();
	for (unsigned int row = 0; row < similarityMatrixSize; row++)
		for (unsigned int col = 0; col < similarityMatrixSize; col++)
		{
			if (row == col)
				DMatrix[row][col] = similarityMatrix[row][col];
		}
	// D-1/2WD+1/2 TODO
	Dmatrix
}

void computeKMeansClustering()
{
}



/* void FiducialDetector::generateTemplates()
 * \brief generates templates modeling fiducials from training data
*/
void SpectralClusteringTemplateSelection::generateFiducialTemplates()
{	
	generateTrainingData();
	
	// spectral clustering in orientation space
		// compute similarity matrix between contoured fiducials over their orientations
		// compute first k eigenvectors , build matrix U
		// Perform k-means clustering on U 
		// return k-disjoint clusters
	computeSimilarityMatrix();
	computeKEigenVectors();
	computeKMeansClustering(); // returns disjoing clusters
	return;
}

