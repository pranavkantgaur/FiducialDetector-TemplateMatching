/*=========================================================================

Program:   Insight Segmentation & Registration Toolkit
Module:    $RCSfile: itkAffinityClustering.cxx,v $
Language:  C++
Date:      $Date: 2005/07/27 15:46:31 $
Version:   $Revision: 1.5 $

Copyright (c) Insight Software Consortium. All rights reserved.
See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

This software is distributed WITHOUT ANY WARRANTY; without even 
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "itkAffinityClustering.h"

namespace itk
{

AffinityClustering
::AffinityClustering() : m_NumberOfClusters( 0 ) 
{
  this->ProcessObject::SetNumberOfRequiredOutputs(1);
  this->ProcessObject::SetNumberOfRequiredInputs(1);

  OutputObjectType::Pointer output = static_cast< OutputObjectType * >(
      this->MakeOutput(0).GetPointer());
  this->ProcessObject::SetNthOutput(0, output.GetPointer());
}

AffinityClustering
::~AffinityClustering() 
{
}

DataObject::Pointer
AffinityClustering
::MakeOutput( unsigned int )
{
  OutputObjectType::Pointer output = OutputObjectType::New();
  OutputType outputArray = OutputType(m_NumberOfClusters);
  output->Set( outputArray );
  return static_cast< DataObject * >( output.GetPointer() );
}

/** Set the input affinity matrix */
void AffinityClustering
::SetInput( const AffinityMatrixType &affinityMatrix )
{
  InputAffinityMatrixObjectType::Pointer affinityMatrixObject;
  if( this->GetNumberOfInputs() == 0 )
    {
    affinityMatrixObject = InputAffinityMatrixObjectType::New();
    }
  else
    {
    const InputAffinityMatrixObjectType * currentAffinityMatrixObject = dynamic_cast< 
      const InputAffinityMatrixObjectType * >(this->ProcessObject::GetInput(0));
    affinityMatrixObject = const_cast< InputAffinityMatrixObjectType * >(
                                                  currentAffinityMatrixObject );
    }
  
  affinityMatrixObject->Set( affinityMatrix );
  
  this->ProcessObject::SetNthInput(0,  affinityMatrixObject  );
  
  // By default, set the length of the output array to rows in the affinityMatrix
  this->GetOutputArray().SetSize( this->GetNumberOfRowsInAffinityMatrix() );
} 

void AffinityClustering
::SetInput( const InputAffinityMatrixObjectType *affinityMatrixObject )
{
  // If input is the same return
  if ((this->GetNumberOfInputs() > 0) 
       && (this->ProcessObject::GetInput(0) == (itk::DataObject *)affinityMatrixObject))
    {
    return;
    }

  // Process object is not const-correct so the const_cast is required here
  this->ProcessObject::SetNthInput(0, 
       const_cast< InputAffinityMatrixObjectType* >( affinityMatrixObject ) );
  
  // By default, set the length of the output array to rows in the affinityMatrix
  this->GetOutputArray().SetSize( this->GetNumberOfRowsInAffinityMatrix() );
}

AffinityClustering::AffinityMatrixType &AffinityClustering
::GetAffinityMatrix() 
{
  if (this->GetNumberOfInputs() < 1)
    {
    itkExceptionMacro( << "Affinity matrix not set" );
    }
  
  const InputAffinityMatrixObjectType *object = 
    static_cast< const InputAffinityMatrixObjectType * >(
                                 this->ProcessObject::GetInput(0) );
  return( (const_cast< InputAffinityMatrixObjectType *>(object))->Get() ); 
} 

const AffinityClustering::InputAffinityMatrixObjectType * AffinityClustering
::GetInput()
{
  if (this->GetNumberOfInputs() < 1)
    {
    return 0;
    }
  
  return static_cast<const InputAffinityMatrixObjectType * >
    (this->ProcessObject::GetInput(0) );
}

unsigned int AffinityClustering
::GetNumberOfRowsInAffinityMatrix()
{
  if (this->GetNumberOfInputs() < 1)
    {
    return 0;
    }
  
  return (static_cast< InputAffinityMatrixObjectType *>(
        this->ProcessObject::GetInput(0)))->Get().Rows();
}

unsigned int AffinityClustering
::GetNumberOfColsInAffinityMatrix()
{
  if (this->GetNumberOfInputs() < 1)
    {
    return 0;
    }
  
  return (static_cast< InputAffinityMatrixObjectType *>(
        this->ProcessObject::GetInput(0)))->Get().Cols();
}

const AffinityClustering::OutputType AffinityClustering
::GetOutputClusters()
{
  return (static_cast< OutputObjectType * >(
        this->ProcessObject::GetOutput(0)))->Get();
}

AffinityClustering::OutputType& AffinityClustering
::GetOutputArray()
{
  return (static_cast< OutputObjectType * >(
        this->ProcessObject::GetOutput(0)))->Get();
}

void AffinityClustering
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "Number of output clusters to produce : " << m_NumberOfClusters << std::endl;
} 

} // end namespace itk
