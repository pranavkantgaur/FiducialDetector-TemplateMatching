#include "FiducialDetection.h"

/*! \fn FiducialDetection::GetTrainingDatasetDir()
 *  \brief Returns the location of input training data directory.
 *  \return string Representing the location of input training data directory
 */
string FiducialDetection::GetTrainingDatasetDir() {
	return this->training_dataset_dir_;
}

/*! \fn FiducialDetection::GetTestDatasetDir()
 *  \brief Returns the location of input test data directory.
 *  \return string Representing the location of input test data directory
 */
string FiducialDetection::GetTestDatasetDir() {
	return this->test_dataset_dir_;
}

/*! \fn FiducialDetection::GetPercentVariationEigenVector()
 *  \brief Returns percentage of variation that must be captured by the selected Eigen vectors(fixes 'k' in k-way clustering)
 *  \return float Percente variation that must be captured by the Eigen vectors selected for Fiducial candidate selection.
 */
float FiducialDetection::GetPercentVariationEigenVector(){
	return this->percent_variation_eigen_vectors_;
}

/*! \fn FiducialDetection::GetNumberOfGaussians()
 *  \brief Returns number of Gaussians for Fiducial candidate selection stage.
 *  \param [in] inputTrainingDatasetDir input directory
 *  \return size_t Number of Gaussians for Fiducial candidate selection
 */
size_t FiducialDetection::GetNumberOfGaussians(){
	return this->n_gaussians_;	
}

/*! \fn FiducialDetection::SetTrainingDatasetDir(string inputTrainingDatasetDir)
 *  \brief sets directory for input training data
 *  \param [in] inputTrainingDatasetDir input directory
 *  \return void
 */
void FiducialDetection::SetTrainingDatasetDir(string input_training_dataset_dir) {
	this->training_dataset_dir_ = input_training_dataset_dir;
}

/*! \fn FiducialDetection::SetTestDatasetDir(string inputTestDatasetDir)
 *  \brief sets test dataset directory
 *  \param [in] Input test directory location
 *  \return void
*/
void FiducialDetection::SetTestDatasetDir(string input_test_dataset_dir) {
	this->test_dataset_dir_ = input_test_dataset_dir;
}

/*! \fn FiducialDetection:SetPercentVariationEigenVector(float inputPercentVariationEigenVector)
 *  \brief Sets percentatge variation for Eigen vector
 * \param [in] inputPercentVariationEigenVector Input percent variation for Eigen value based clustering.
*   \return void
*/
void FiducialDetection::SetPercentageVariationEigenVector(float input_percent_variation_eigen_vector){
	this->percent_variation_eigen_vector_ = input_percent_variation_eigen_vector;
}

/*! \fn FiducialDetection:SetNumberOfGaussians(size_t inputNumberOfGaussians)
 * \brief Sets number of Gaussians for fiducial candidate selection.
 * \param [in] inputNumberOfGaussians Number of Gaussians
*/
void FiducialDetection::SetNumberOfGaussians(size_t input_number_of_gaussians) {
	this->n_gaussians_ = input_number_of_gaussians;
}

/* void FiducialDetection::Configure()
 *  \brief Reads project configuration file and initializes internal parameters.
 */
void FiducialDetection::Configure(string file_name)
{
	// read project configuration file and initialize detector state
	itk::DOMNode::Pointer output_dom_object;
	itk::DOMNodeXMLReader::Pointer reader = DOMNodeXMLReader::New();
	reader->SetFileName(file_name);
	reader->Update();
	output_dom_object = reader->GetOutput();
	
	// dir location: training images
	itk::DOMNode::Pointer training_dataset_dir_dom_node = output_dom_object->Find("training_dataset_dir"); 
	itk::DOMTextNode::Pointer text_node_ptr = training_dataset_dir_dom_node->GetTextChild();
	
	this->SetTrainDatasetDir(text_node_ptr->GetText());
	// dir location for test images
	itk::DOMNode::Pointer testing_dataset_dir_dom_node = output_dom_object->Find("test_dataset_dir");
	text_node_ptr = testing_dataset_dir_dom_node->GetTextChild();
	this->SetTestDatasetDir(text_node_ptr->GetText());

	 // % of variation as a therehold for selecting k 
	itk::DOMNode::Pointer percent_variation_eigen_vectors_dom_node = output_dom_object->Find("percent_variation_eigen_vectors");
 	text_node_ptr = percent_variation_eigen_vectors_dom_node->GetTextChild();
	this->SetPercentVariationCoveredByEigenVector(stof(text_node_ptr->GetText()));
	
	// number of gaussian distributions
	itk::DOMNode::Pointer n_gaussians_dom_node = output_dom_object->Find("nGaussians");
	text_node_ptr = n_gaussians_dom_node->GetTextChild();
	this->SetNumberOfGaussians(stoi(text_node_ptr->GetText()));
		
	// parameters for icp template registration parameters
	detector_configured_ = true;
}

/* \fn FiducialDetection::extractFiducialOrientation(vnl_matrix<double>& rotationMatrix, vector<FiducialOrientation>& fiducialOrientationVector)
 * \brief Extracts (alpha, beta, gamma) from input rotation matrix. Computes axis-angle representation from rotation matrix. Compute alpha, beta, gamma from axis-angle representation.
 * \param [in] rotationMatrix the input rotation matrix returned by itkLabelGeometryFilter::GetRotationMatrix()
 * \param [out] fiducialOrientationVector the output orientation vectors as required by training phase, for computing similarity matrix.
*/
void FiducialDetection::ExtractFiducialOrientationVector(vnl_matrix<double>& rotation_matrix , FiducialOrientation & fiducial_orientation_vector)
{
	// Reference: http://nghiaho.com/?page_id=846 
	// Rotation matrix to Euler angle conversion, assumed rotation sequence YXZ, a.k.a Yaw-Pitch-Roll
	fiducial_orientation_vector.alpha = atan2(rotation_matrix(2, 1), rotation_matrix(2, 2));
	fiducial_orientation_vector.beta = atan2(-1.0 * rotation_matrix(2, 0), sqrtf(pow(rotation_matrix(2, 1), 2) + pow(rotation_matrix(2, 2), 2)));
	fiducial_orientation_vector.gamma = atan2(rotation_matrix(1, 0), rotation_matrix(0, 0));
}


/* void FiducialDetection::readTrainingData()
 * \brief reads contoured fiducial samples, computes orientation of each sanple
 */
void FiducialDetection::GenerateTrainingData()
{
	// read DICOM images
	typedef signed short PixelType;
	const unsigned int kDimension = 3;
	typedef itk::Image<PixelType, kDimension> ImageType;
	typedef itk::ImageSeriesReader<ImageType> ReaderType;
	string dir_name = training_dataset_dir_;

	typedef itk::GDCMSeriesFileNames NamesGeneratorType;
	NamesGeneratorType::Pointer name_generator = NamesGeneratorType::New();
	name_generator->SetGlobalWarningDisplay(false);
	name_generator->SetDirectory(dir_name);

	try
	{
		typedef vector<string> SeriesIdContainer;
		const SeriesIdContainer & series_uid = name_generator->GetSeriesUIDs();
		SeriesIdContainer::const_iterator series_itr = series_uid.begin();
		SeriesIdContainer::const_iterator series_end = series_uid.end();
		if (series_itr != series_end)
		{
			cout << "The directory: ";
			cout << dir_name << endl;
			cout << "Contains the following DICOM series: " << endl;
		}
		else
		{
			cout << "No DICOMs in: " << dir_name << endl;
			exit(0);
		}
		while (series_itr != series_end)
		{
			cout << series_itr->c_str() << endl;
			++series_itr;
		}
		series_itr = series_uid.begin();
		while(series_itr != series_uid.end())
		{
			string series_identifier;
			series_identifier = series_itr->c_str();
			series_itr++;
			typedef vector<string> FileNamesContainer;
			FileNamesContainer file_names;
			file_names = name_generator->GetFileNames(series_identifier);
			ReaderType::Pointer reader = ReaderType::New();
			typedef itk::GDCMImageIO ImageIOType;
			ImageIOType::Pointer dicom_io = ImageIOType::New();
			reader->SetImageIO(dicom_io);
			reader->SetFileNames(file_names);
			reader->Update(); // read the series of DICOM images				
			cout << "Number of files in this series: " << file_names.size() << endl;
			
			size_t n_files_in_series = file_names.size();	
			
			// write fiducial orientations as the training data for the approach 
			typedef LabelGeometryImageFilter<ImageType> LabelGeometryImageFilterType;
			LabelGeometryImageFilterType::Pointer fiducial_orientation_estimator = LabelGeometryImageFilterType::New();

			FiducialOrientation fiducial_orientation_vector; // represents the orientation of a fiducial
			vnl_matrix<double> fiducial_rotation_matrix; // represents the rotation matrix corresponding to a fiducial
			
			for (unsigned int file_id = 0; file_id < n_files_in_series; file_id++) // for every label image file				    	
			{
				fiducial_orientation_estimator->SetInput(reader->GetOutput()); // sets the next label image to be read
				fiducial_orientation_estimator->Update(); // reads label image

				cout << "Number of fiducials in this image: " << fiducial_orientation_estimator->GetNumberOfLabels() << endl;
				size_t n_labels_in_image = fiducial_orientation_estimator->GetNumberOfLabels();
				for (size_t label_id = 0; label_id < n_labels_in_image; label_id++) 				
				{
					fiducial_rotation_matrix = fiducial_orientation_estimator->GetRotationMatrix(label_id); // get rotation matrix
					this->ExtractFiducialOrientationVector(fiducial_rotation_matrix, fiducial_orientation_vector); // 'fOrientVector' now contains orientation information for the fiducial
					fiducial_orientation_vectors_.push_back(fiducial_orientation_vector); // insert orientation triplet for this fiducial into a global training array.
				}		
			}
		}
	}
	catch (itk::ExceptionObject &ex)
	{
		cout << ex << endl;
	}				
}

/* void FiducialDetection::GenerateFiducialTemplates(float percentVariationEigenVectors, vector<int>& cluster_id_per_fiducial)
 * \brief generates templates modeling fiducials from training data
 * \param [in] percentVariationEigenVectors
 * \param [out] cluster_id_per_fiducial
*/
void FiducialDetection::GenerateFiducialTemplates()
{	
	/* Approach:
		spectral clustering in orientation space
			compute similarity matrix between contoured fiducials over their orientations
 			compute first k eigenvectors , build matrix U
			Perform k-means clustering on U 
		return k-disjoint clusters */

	// compute affinity matrix
	unsigned int affinity_matrix_size = fiducial_orientation_vectors_.size();
/*	typedef vector<vector<float> > Matrix;
	Matrix affinity_matrix(affinity_matrix_size, vector<float>(affinity_matrix_size));
*/
	typedef itk::AffinityClustering AffinityClusteringFilterType;
	AffinityClusteringFilterType::Pointer affinity_clustering_filter = AffinityClusteringFilterType::New();
	typedef double AffinityMatrixValueType;
	typedef itk::VariableSizeMatrix< AffinityMatrixValueType > AffinityMatrixType;
	AffinityMatrixType affinity_matrix;
	affinity_matrix.SetSize(affinity_matrix_size, affinity_matrix_size);

	
	FiducialOrientation fiducial_one_orientation_vector, fiducial_two_orientation_vector;
	for (unsigned int row = 0; row < affinity_matrix_size; row++)
		for (unsigned int col = 0; col < affinity_matrix_size; col++) // square matrix
		{
			fiducial_one_orientation_vector = fiducial_orientation_vectors_[row];
			fiducial_two_orientation_vector = fiducial_orientation_vectors_[col];
			affinity_matrix(row, col) = fabs(fiducial_one_orientation_vector.alpha - fiducial_two_orientation_vector.alpha) + fabs(fiducial_one_orientation_vector.beta - fiducial_two_orientation_vector.beta) + fabs(fiducial_one_orientation_vector.gamma - fiducial_two_orientation_vector.gamma);		 
		}

	// compute spectral clusters and return as output
	affinity_clustering_filter->SetInput(affinity_matrix);
	affinity_clustering_filter->Update(); // the computation step		
	cluster_id_per_fiducial = affinity_clustering_filter->GetOutputClusters(); // for every fiducial , we get the cluster id
	// TODO: how to use percentVariationEigenVectors??		
	// there is no control how to discard clusters based on percentage confidence
	// TODO: identify leader fiducial to act as a template for next stage
	cout << "Number of fiducial template clusters: " << affinity_clustering_filter->GetNumberOfClusters() << endl;
}	

/* void FiducialDetection::GenerateFiducialCandidates()
 * \brief Generates most probable candidates modeling fiducials, combines Gaussian mixture modeling with Markov random field
 */
void FiducialDetection::GenerateFiducialCandidates()
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


/* void FiducialDetection::matchTemplatesWithCandidates()
 * \brief Matches fiducial candidates against templates
 */
void FiducialDetector::MatchTemplatesWithCandidates()
{
	// Compares all prospective fiducial candidates 
		// convert fiducial candiates and templates into 3D volumes using Marching cube before registering with ICP
		// ICP minimizes the pose variations of the fiducial marker candidates with respect to templates
		// Assumption: Gold fiducial marker candidate will be similar to aleast one of the templates after rigid registration
}

/* void FiducialDetection::run()
 * \brief Public interface for running the system
 */
void FiducialDetection::Run()
{
	if (detector_configured_ == true)
	{
		GenerateTrainingData();
		GenerateFiducialTemplates();
		GenerateFiducialCandidates();
		MatchTemplatesWithCandidates();
	}
	else
	{
		cout << "Please configure the detector!!";
		exit(1);
	}
}

