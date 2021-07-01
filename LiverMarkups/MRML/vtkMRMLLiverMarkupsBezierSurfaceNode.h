/*=========================================================================

  Program: NorMIT-Plan
  Module: vtkMRMLBezierSurfaceNode.h

  Copyright (c) 2017, The Intervention Centre, Oslo University Hospital

  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:

  1. Redistributions of source code must retain the above copyright notice,
  this list of conditions and the following disclaimer.

  2. Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

  3. Neither the name of the copyright holder nor the names of its contributors
  may be used to endorse or promote products derived from this software
  without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  =========================================================================*/

#ifndef __vtkMRMLLiverMarkupsBezierSurfaceNode_h
#define __vtkMRMLLiverMarkupsBezierSurfaceNode_h

// This module includes.
#include "vtkSlicerLiverMarkupsModuleMRMLExport.h"

// MRML includes
//#include <vtkMRMLModelNode.h>
#include <vtkMRMLMarkupsNode.h>

// VTK includes
#include <vtkNew.h>
#include <vtkWeakPointer.h>
#include <vtkCollection.h>

//------------------------------------------------------------------------------
class vtkMRMLBezierSurfaceDisplayNode;
class vtkMRMLModelNode;
class vtkCollection;
class vtkPoints;

//------------------------------------------------------------------------------

/**
 * \ingroup LiverMarkups
 *
 * \brief This class represents the data model employed for resection. It
 * contains all the information related to the resection like its relationship
 * to other elements in the MRML scene.
 */
class VTK_SLICER_LIVERMARKUPS_MODULE_MRML_EXPORT
    vtkMRMLLiverMarkupsBezierSurfaceNode : public vtkMRMLMarkupsNode
{

public:
    virtual const char* GetIcon()  {return ":/Icons/MarkupsGeneric.png";}
    virtual const char* GetAddIcon()  {return ":/Icons/MarkupsGenericMouseModePlace.png";}
    virtual const char* GetPlaceAddIcon()  {return ":/Icons/MarkupsGenericMouseModePlaceAdd.png";}

    virtual vtkMRMLNode* CreateNodeInstance() override;
    /// Get node XML tag name (like Volume, Model)
    ///
    virtual const char* GetNodeTagName() override {return "MarkupBezierSurface";}

    /// Get markup name
    virtual const char* GetMarkupType()  {return "BezierSurface";}

    /// Get markup short name
    virtual const char* GetDefaultNodeNamePrefix()  {return "RS";}

    /// \sa vtkMRMLNode::CopyContent
    vtkMRMLCopyContentDefaultMacro(vtkMRMLLiverMarkupsSlicingContourNode);

    vtkPolyData* GetTargetOrgan() const {return this->TargetOrgan;}
    void SetTargetOrgan(vtkPolyData* targetOrgan) {this->TargetOrgan = targetOrgan;}

    /// Alternative method to propagate events generated in Display nodes
    void ProcessMRMLEvents(vtkObject* /*caller*/,
        unsigned long /*event*/,
        void* /*callData*/) override;

    void InitializeBezierSurface(vtkPoints* curve);

//  protected:
//    vtkMRMLLiverMarkupsSlicingContourNode();
//    ~vtkMRMLLiverMarkupsSlicingContourNode() override;
//    vtkMRMLLiverMarkupsSlicingContourNode(const vtkMRMLLiverMarkupsSlicingContourNode&);
//    void operator=(const vtkMRMLLiverMarkupsSlicingContourNode&);

  private:
    vtkPolyData *TargetOrgan = nullptr;




public:

  /**
   * Standard vtk object instantiation method.
   *
   * @return a pointer to a newly created vtkMRMLBezierSurfaceNode.
   */
  static vtkMRMLLiverMarkupsBezierSurfaceNode *New();

  vtkTypeMacro(vtkMRMLLiverMarkupsBezierSurfaceNode, vtkMRMLMarkupsNode);

  /**
   * Standard print object information method.
   *
   * @param os output stream to print the information to.
   * @param indent indent value.
   */
  void PrintSelf(ostream &os, vtkIndent indent);

  /**
   * Standard MRML method to create the node instance.
   *
   * @return a pointer to the new created vtkMRMLNode.
   */
//  virtual vtkMRMLNode *CreateNodeInstance();

  /**
   * Get the tag name of the node.
   *
   *
   * @return string with the tag name of the node.
   */
//  virtual const char* GetNodeTagName() {return "BezierSurface";}

  /**
   * Get the icon associated to the node.
   *
   *
   * @return string pointing to the resource where the icon is located.
   */
//  virtual const char* GetIcon() {return "";}

  /**
   * Get the display node associated
   *
   *
   * @return pointer to the vtkMRMLBezierSurfaceDisplayNode associated with
   * this node.
   */
  vtkMRMLBezierSurfaceDisplayNode* GetBezierSurfaceDisplayNode();

  /**
   * Add target tumor.
   *
   * @param tumorModelNode pointer to vtkMRMLModelNode representing the target
   * tumor to be added. If the tumor is already in the internal collection, the
   * operation will not take place.
   */
  void AddTargetTumor(vtkMRMLModelNode *tumorModelNode);

  /**
   * Remove target tumor.
   *
   * @param tumorModelNode pointer to the vtkMRMLModelNode representing the
   * target tumor to remove.
   */
  void RemoveTargetTumor(vtkMRMLModelNode *tumorModelNode);

  /**
   * Get number of target tumors.
   *
   * @return number of target tumors associated to the resection.
   */
  int GetNumberOfTargetTumors() const;

  /**
   * Get the resection margin associated to the resection surface node
   *
   * @return resection margin.
   */
  vtkGetMacro(ResectionMargin, double);

  /**
   * Set the resection margin associated to the resection surface node
   *
   * @param ResectionMargin resection margin.
   *
   */
  vtkSetMacro(ResectionMargin, double);

  vtkGetNewMacro(ControlPoints, vtkPoints);

  /**
   * Set control points
   *
   * @param newControlPoints
   */
  void SetControlPoints(vtkPoints *newControlPoints);

  void SetTargetParenchyma(vtkMRMLModelNode *node);
  vtkMRMLModelNode *GetTargetParenchyma() const;

 protected:
  vtkMRMLLiverMarkupsBezierSurfaceNode();
  ~vtkMRMLLiverMarkupsBezierSurfaceNode();

  vtkMRMLLiverMarkupsBezierSurfaceNode(const vtkMRMLLiverMarkupsBezierSurfaceNode&);
  void operator=(const vtkMRMLLiverMarkupsBezierSurfaceNode&);

  vtkNew<vtkCollection> TargetTumors;
  vtkNew<vtkPoints> ControlPoints;
  vtkWeakPointer<vtkMRMLModelNode> TargetParenchyma;
  double ResectionMargin;
};

#endif
