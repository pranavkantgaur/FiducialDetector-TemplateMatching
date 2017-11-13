add_executable (fiducial_detector SpectralClusteringFiducialTemaplateSelection.cxx MAPMARFiducialCandidateSelection.cxx ICPTemplateRegistration.cxx FiducialDetector.cxx)

target_link_libraries (fiducial_detector ITKCommon-4.12 ITKIONRRD-4.12 -ITKIOImageBase-4.12 itksys-4.12 itkvnl-4.12 itkvnl_algo-4.12 ITKIOGDCM-4.12 itkgdcmMSFF-4.12)





