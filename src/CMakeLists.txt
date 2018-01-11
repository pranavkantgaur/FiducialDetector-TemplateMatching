add_executable (fiducial_detector FiducialDetection.cpp main.cpp) #MAPMARFiducialCandidateSelection.cxx ICPTemplateRegistration.cxx FiducialDetector.cxx main.cpp)

target_link_libraries (fiducial_detector ITKIOXML-4.12 ITKCommon-4.12 ITKIONRRD-4.12 ITKIOImageBase-4.12 ITKTransform-4.12 itksys-4.12 itkvnl-4.12 itkvnl_algo-4.12 ITKIOGDCM-4.12 itkgdcmMSFF-4.12)





