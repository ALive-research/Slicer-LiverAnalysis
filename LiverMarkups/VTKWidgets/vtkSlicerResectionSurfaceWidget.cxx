/*==============================================================================

 Distributed under the OSI-approved BSD 3-Clause License.

  Copyright (c) Oslo University Hospital. All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions
  are met:

  * Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.

  * Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.

  * Neither the name of Oslo University Hospital nor the names
    of Contributors may be used to endorse or promote products derived
    from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
  HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

  This file was originally developed by Rafael Palomar (The Intervention Centre,
  Oslo University Hospital) and was supported by The Research Council of Norway
  through the ALive project (grant nr. 311393).

  ==============================================================================*/

#include "vtkSlicerResectionSurfaceWidget.h"

// Liver Markups VTKWidgets include
//#include "vtkSlicerSlicingContourRepresentation3D.h"

// VTK includes
#include <vtkObjectFactory.h>

// Markups VTKWidgets includes
#include <vtkSlicerLineRepresentation2D.h>
#include <vtkSlicerLineRepresentation3D.h>
#include <vtkSlicerResectionSurfaceRepresentation3D.h>

#include "vtkBezierSurfaceWidget.h"
#include "vtkMRMLLiverMarkupsResectionSurfaceNode.h"

//------------------------------------------------------------------------------
vtkStandardNewMacro(vtkSlicerResectionSurfaceWidget);

//------------------------------------------------------------------------------
vtkSlicerResectionSurfaceWidget::vtkSlicerResectionSurfaceWidget()
{
    BezierSurfaceWidget = vtkSmartPointer<vtkBezierSurfaceWidget>::New();
    //BezierSurfaceWidget->SetInteractor(this->GetInteractor());
    //BezierSurfaceWidget->SetCurrentRenderer(this->GetRenderer());
    BezierSurfaceWidget->SetHandleSizeFactor(0.8);
    BezierSurfaceWidget->On();
    BezierSurfaceWidget->ComputeNormalsOn();
}

//------------------------------------------------------------------------------
vtkSlicerResectionSurfaceWidget::~vtkSlicerResectionSurfaceWidget() = default;

//------------------------------------------------------------------------------
void vtkSlicerResectionSurfaceWidget::CreateDefaultRepresentation(vtkMRMLMarkupsDisplayNode* markupsDisplayNode,
                                                                vtkMRMLAbstractViewNode* viewNode,
                                                                vtkRenderer* renderer)
{
  vtkSmartPointer<vtkSlicerMarkupsWidgetRepresentation> rep = nullptr;
  if (vtkMRMLSliceNode::SafeDownCast(viewNode))
    {
    rep = vtkSmartPointer<vtkSlicerLineRepresentation2D>::New();
    }
  else
    {
    rep = vtkSmartPointer<vtkSlicerResectionSurfaceRepresentation3D>::New();
    }
  this->SetRenderer(renderer);
  this->SetRepresentation(rep);
  rep->SetViewNode(viewNode);
  rep->SetMarkupsDisplayNode(markupsDisplayNode);
  rep->UpdateFromMRML(nullptr, 0); // full update
  
}

//------------------------------------------------------------------------------
vtkSlicerMarkupsWidget* vtkSlicerResectionSurfaceWidget::CreateInstance() const
{
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkSlicerResectionSurfaceWidget");
  if(ret)
    {
    return static_cast<vtkSlicerResectionSurfaceWidget*>(ret);
    }

  vtkSlicerResectionSurfaceWidget* result = new vtkSlicerResectionSurfaceWidget;
#ifdef VTK_HAS_INITIALIZE_OBJECT_BASE
  result->InitializeObjectBase();
#endif
  return result;
}


//-------------------------------------------------------------------------
void vtkSlicerResectionSurfaceWidget::UpdateFromMRML(vtkMRMLNode* caller, unsigned long event, void* callData/*=nullptr*/)
{
    if (!this->WidgetRep)
    {
        return;
    }

    vtkMRMLLiverMarkupsResectionSurfaceNode* liverMarkupsResectionSurfaceNode =
        vtkMRMLLiverMarkupsResectionSurfaceNode::SafeDownCast(caller);

    //BezierSurfaceWidget->SetControlPoints(liverMarkupsResectionSurfaceNode->GetControlPoints());
    this->WidgetRep->UpdateFromMRML(caller, event, callData);
}