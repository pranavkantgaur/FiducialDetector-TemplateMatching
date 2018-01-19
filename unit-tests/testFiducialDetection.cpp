
namespace {

// The fixture for testing class Foo.
class FiducialDetectionTest : public ::testing::Test {
	protected:
	// You can remove any or all of the following functionsf its body is empty
	FiducialDetectionTest() {
	}

	virtual ~FiducialDetectionTest() {
		// You can do the clean-up work that doesn't throw exceptions here.
	//If the constructor and destructor are not enough for setting up
	// and cleaning up each test, you can define the following methods;
	virtual void setUp() {
	// Code here will be called immidiately after the constructor(right before each test)
	}
	virtual void TearDown() {
		// Code here will be called immidietly after each test(right before the destructor)
	}

};

TEST_f(FiducialDetectionTest, testConfigureInitializion) {
	FiducialDetection f;
	f.configure();
	ASSERT_EQ("trainingDir", );
	ASSERT_EQ("testDir", );
	ASSERT_EQ("percentVariationEigenVectors", );
	ASSERT_EQ("nGaussians");
}

TEST_f(FiducialDetectionTest, testGenerateTrainingData) {
	//TODO
}

TEST_f(test_system_initialization, test_extract_fiducial_orientation_vector) {
	// TODO
}


TEST_f(test_system_initialization, test_generate_training_data) {
	// TODO
}
 
};
} // namespace




