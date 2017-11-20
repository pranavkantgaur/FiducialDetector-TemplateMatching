#include "FiducialDetection.h"

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		std::cerr << "Usage: " << std::endl;
		std::cerr << argv[0] << "[Detector Configuration File name]" << endl;
		exit(1);	
	}
	FiducialDetection detector;
	detector.configure(argv[1]);
	detector.run();
	return 0;
}
