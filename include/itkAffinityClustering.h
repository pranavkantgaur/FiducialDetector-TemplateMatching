/*=========================================================================

Program:   Insight Segmentation & Registration Toolkit
Module:    $RCSfile: itkAffinityClustering.h,v $
Language:  C++
Date:      $Date: 2005/07/27 15:46:31 $
Version:   $Revision: 1.5 $

Copyright (c) Insight Software Consortium. All rights reserved.
See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

This software is distributed WITHOUT ANY WARRANTY; without even 
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _itkAffinityClustering_h
#define _itkAffinityClustering_h

#include "itkVariableSizeMatrix.h"
#include "itkArray.h"
#include "itkSimpleDataObjectDecorator.h"
#include "itkProcessObject.h"
#include "itkObjectFactory.h"
#include "itkMacro.h"

namespace itk
{

/** \class AffinityClustering
 * \brief Serves as a base class for Affinity based clustering methods. These
 * classes take an affinity matrix as input and return an Array as output. The
 * array contains an integer referring to the clusters that each of the n elements
 * of the n x n affinity correspond to.
 *
 * The affinity matrix may be set via the SetInput( VariableSizeMatrix * )
 * (or via SetInput( InputAffinityMatrixObjectType * )
 *
 * The output cluster may be accessed via the GetOutputClusters() method. 
 * (or wrapped in a data object via GetOutput()).
 * 
 * Subclasses may access/modify the Array via reference using GetOutputArray()
 * They must provide an implementation of the GenerateData() method.
 */

class AffinityClustering : public ProcessObject
{
public:
  /** Standard class typedefs */
  typedef AffinityClustering                  Self;
  typedef ProcessObject                       Superclass;

  /** Run-time type information (and related methods). */
  typedef SmartPointer<Self>                   Pointer;
  typedef SmartPointer<const Self>             ConstPointer;
  
  itkTypeMacro( AffinityClustering, ProcessObject );

  /** Method for creation through the object factory. */
  itkNewMacro(Self);
  
  typedef double                              AffinityMatrixValueType;
  typedef VariableSizeMatrix< AffinityMatrixValueType > AffinityMatrixType;

  /** Array containing the output classification. The m'th element contains an 
   * integer indicating the cluster that the m'th element belongs to. */
  typedef Array< unsigned int >               OutputType;
  
  /** Since VariableSizeMatrix are not dataobjects, we use the decorator to push
   *  them down the pipeline */ 
  typedef SimpleDataObjectDecorator< AffinityMatrixType > InputAffinityMatrixObjectType;

  /** OutputType returned by the filter, this is an Array decorated with a 
   * SimpleDataObjectDecorator, so it can be pushed down the pipeline. You could
   * also try the GetOutputClusters(). */
  typedef SimpleDataObjectDecorator< OutputType > OutputObjectType;

  /** Set method for the affinity matrix */
  virtual void SetInput( const AffinityMatrixType &affinityMatrix );

  /** Overloaded Set method for the affinity matrix in case it is released in
   * a data object decorator from another filter */
  virtual void SetInput( const InputAffinityMatrixObjectType *affinityMatrixObject );
  
  /** Get the affinity matrix set as input. Returned via reference. Affinity 
   * matrices can be large and we do not want to return them by value and spend
   * resources copying them. To be syntactically correct, this function should be
   * const AffinityMatrixType &GetAffinityMatrix() const; 
   * For some reason, the itkSimpleDataObjectDecorator.h has a function that returns
   * by value and should be replaced by an equivalent one that returns the reference.
   * I am convinced that it is a bug. We will wait until JimMiller confirms this. */
  AffinityMatrixType &GetAffinityMatrix();

  /** Get the output cluster. Const method for the user */
  const OutputType GetOutputClusters();

  /** Create an Array as the output and wrap it around a 
   * SimpleDataObjectDecorator. In future, if more outputs are produced, add them 
   * here. */
  virtual DataObjectPointer MakeOutput(unsigned int itkNotUsed(idx));
  
  /** Get the affinity matrix decorated using a SimpleDataObjectDecorator */
  const InputAffinityMatrixObjectType *GetInput();

  unsigned int GetNumberOfRowsInAffinityMatrix();
  unsigned int GetNumberOfColsInAffinityMatrix();

  /** Set/Get macros for the number of clusters */
  itkSetMacro(      NumberOfClusters, unsigned int ); 
  itkGetConstMacro( NumberOfClusters, unsigned int );

protected:
  AffinityClustering();
  ~AffinityClustering();

  /** To be provided by derived classes */
  virtual void GenerateData() {};

  /** Get the output via reference, so the derived classes can access the output
   * array, resize it or fill in contents */
  OutputType &GetOutputArray();

  virtual void PrintSelf(std::ostream& os, Indent indent) const;

private:
  AffinityClustering( const Self& ); // purposely not implemented
  void     operator=( const Self& ); // purposely not implemented
  
  unsigned int   m_NumberOfClusters;
};

} // end namespace itk

#endif
