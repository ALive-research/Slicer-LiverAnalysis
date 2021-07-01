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

#include "vtkSlicerResectionSurfaceRepresentation3D.h"

#include "vtkMRMLLiverMarkupsBezierSurfaceNode.h"

// VTK includes
#include <vtkActor.h>
#include <vtkCutter.h>
#include <vtkPlane.h>
#include <vtkPolyDataMapper.h>
#include <vtkSphereSource.h>
#include <vtkRenderer.h>


#include "vtkBezierSurfaceSource.h"


//------------------------------------------------------------------------------
vtkStandardNewMacro(vtkSlicerResectionSurfaceRepresentation3D);

//------------------------------------------------------------------------------
vtkSlicerResectionSurfaceRepresentation3D::vtkSlicerResectionSurfaceRepresentation3D()
  :TargetOrgan(nullptr)
{

  this->SlicingPlane = vtkSmartPointer<vtkPlane>::New();

  this->Cutter =  vtkSmartPointer<vtkCutter>::New();
  this->Cutter->SetInputData(this->TargetOrgan);
  this->Cutter->SetCutFunction(this->SlicingPlane);
  this->Cutter->SetNumberOfContours(1);
  this->Cutter->GenerateTrianglesOn();
  this->Cutter->GenerateCutScalarsOff();

  this->ContourMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  this->ContourMapper->SetInputConnection(this->Cutter->GetOutputPort());

  this->ContourActor = vtkSmartPointer<vtkActor>::New();
  this->ContourActor->SetMapper(this->ContourMapper);

  this->MiddlePointSource = vtkSmartPointer<vtkSphereSource>::New();

  this->MiddlePointMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  this->MiddlePointMapper->SetInputConnection(this->MiddlePointSource->GetOutputPort());

  this->MiddlePointActor = vtkSmartPointer<vtkActor>::New();
  this->MiddlePointActor->SetMapper(this->MiddlePointMapper);

  this->BezierSurfaceActor->VisibilityOn();
}

//------------------------------------------------------------------------------
vtkSlicerResectionSurfaceRepresentation3D::~vtkSlicerResectionSurfaceRepresentation3D()
{

    this->Renderer->RemoveActor(this->BezierSurfaceActor);
}

//------------------------------------------------------------------------------
void vtkSlicerResectionSurfaceRepresentation3D::PrintSelf(ostream& os, vtkIndent indent)
{
  Superclass::PrintSelf(os, indent);
}

//------------------------------------------------------------------------------
void vtkSlicerResectionSurfaceRepresentation3D::GetActors(vtkPropCollection* pc)
{
  this->Superclass::GetActors(pc);

  if (this->TargetOrgan)
    {
    this->ContourActor->GetActors(pc);
    }

  this->MiddlePointActor->GetActors(pc);
}

//------------------------------------------------------------------------------
void vtkSlicerResectionSurfaceRepresentation3D::ReleaseGraphicsResources(vtkWindow* win)
{
  this->Superclass::ReleaseGraphicsResources(win);

  if (this->TargetOrgan)
    {
    this->ContourActor->ReleaseGraphicsResources(win);
    }

  this->MiddlePointActor->ReleaseGraphicsResources(win);
}

//------------------------------------------------------------------------------
int vtkSlicerResectionSurfaceRepresentation3D::RenderOverlay(vtkViewport* viewport)
{
  int count = this->Superclass::RenderOverlay(viewport);
  if (this->TargetOrgan && this->ContourActor->GetVisibility())
    {
    count += this->ContourActor->RenderOverlay(viewport);
    }

  if (this->MiddlePointActor->GetVisibility())
    {
    count += this->MiddlePointActor->RenderOverlay(viewport);
    }
  return count;
}

//------------------------------------------------------------------------------
int vtkSlicerResectionSurfaceRepresentation3D::RenderOpaqueGeometry(vtkViewport* viewport)
{
  int count = this->Superclass::RenderOpaqueGeometry(viewport);
  if (this->TargetOrgan && this->ContourActor->GetVisibility())
    {
    count += this->ContourActor->RenderOpaqueGeometry(viewport);
    }

  if (this->MiddlePointActor->GetVisibility())
    {
    count += this->MiddlePointActor->RenderOpaqueGeometry(viewport);
    }

  return count;
}

//------------------------------------------------------------------------------
int vtkSlicerResectionSurfaceRepresentation3D::RenderTranslucentPolygonalGeometry(vtkViewport* viewport)
{
  int count = this->Superclass::RenderTranslucentPolygonalGeometry(viewport);
  if (this->TargetOrgan && this->ContourActor->GetVisibility())
    {
    this->ContourActor->SetPropertyKeys(this->GetPropertyKeys());
    count += this->ContourActor->RenderTranslucentPolygonalGeometry(viewport);
    }

  if (this->MiddlePointActor->GetVisibility())
    {
    this->MiddlePointActor->SetPropertyKeys(this->GetPropertyKeys());
    count += this->MiddlePointActor->RenderTranslucentPolygonalGeometry(viewport);
    }

  return count;
}

//------------------------------------------------------------------------------
vtkTypeBool vtkSlicerResectionSurfaceRepresentation3D::HasTranslucentPolygonalGeometry()
{
  if (this->Superclass::HasTranslucentPolygonalGeometry())
    {
    return true;
    }

  if (this->TargetOrgan && this->ContourActor->GetVisibility() &&
      this->ContourActor->HasTranslucentPolygonalGeometry())
    {
    return true;
    }

  if (this->MiddlePointActor->GetVisibility() &&
      this->MiddlePointActor->HasTranslucentPolygonalGeometry())
    {
    return true;
    }

  return false;
}

//----------------------------------------------------------------------
void vtkSlicerResectionSurfaceRepresentation3D::UpdateFromMRML(vtkMRMLNode* caller,
                                                           unsigned long event,
                                                           void *callData /*=nullptr*/)
{
 this->Superclass::UpdateFromMRML(caller, event, callData);

 this->NeedToRenderOn();

 this->BuildMiddlePoint();
 this->BuildSlicingPlane();
 this->BuildBezierSurface();

 vtkMRMLLiverMarkupsBezierSurfaceNode* liverMarkupsResectionSurfaceNode=
   vtkMRMLLiverMarkupsBezierSurfaceNode::SafeDownCast(this->GetMarkupsNode());

 if (!liverMarkupsResectionSurfaceNode)
   {
   std::cout << "invalid resection surface node" << std::endl;
   return;
   }

 this->TargetOrgan = liverMarkupsResectionSurfaceNode->GetTargetOrgan();
 this->Cutter->SetInputData(this->TargetOrgan);
}

//------------------------------------------------------------------------------
void vtkSlicerResectionSurfaceRepresentation3D::BuildBezierSurface()
{
    vtkMRMLLiverMarkupsBezierSurfaceNode* liverMarkupsBezierSurfaceNode =
        vtkMRMLLiverMarkupsBezierSurfaceNode::SafeDownCast(this->GetMarkupsNode());
    if (!liverMarkupsBezierSurfaceNode)
    {
        return;
    }
    int numberOfControlPoints = liverMarkupsBezierSurfaceNode->GetNumberOfControlPoints();
    if (numberOfControlPoints < 16)
    {
        return;
    }
    this->BezierSurfaceSource->SetNumberOfControlPoints(4, 4);
    this->BezierSurfaceSource->SetResolution(40, 40);

    //vtkMRMLLiverMarkupsBezierSurfaceNode and vtkMRMLMarkupsNode got different set of control points
    //For now just use the points from vtkMRMLMarkupsNode, as the points in vtkMRMLLiverMarkupsBezierSurfaceNode
    //don't get updated from GUI
    vtkMRMLMarkupsNode* markupsNode = this->GetMarkupsNode();
    vtkSmartPointer<vtkPoints> points = liverMarkupsBezierSurfaceNode->GetControlPoints();
    std::vector< vtkMRMLMarkupsNode::ControlPoint* > *controlpoints = markupsNode->GetControlPoints();

    // Convert ControlPoints to vtkPoints
    for (int i = 0; i < numberOfControlPoints; ++i)
    {
        double* point = new double[3];
        point[0] = (*controlpoints)[i]->Position[0];
        point[1] = (*controlpoints)[i]->Position[1];
        point[2] = (*controlpoints)[i]->Position[2];
        points->SetPoint(i, point);
    }

    this->BezierSurfaceSource->SetControlPoints(points);
    this->BezierSurfaceMapper->SetInputConnection(this->BezierSurfaceSource->GetOutputPort());
    this->BezierSurfaceActor->SetMapper(this->BezierSurfaceMapper.GetPointer());
    //this->BezierSurfaceActor->SetProperty(this->BezierSurfaceProperty.GetPointer());

    this->Renderer->AddActor(this->BezierSurfaceActor);
}
//------------------------------------------------------------------------------
void vtkSlicerResectionSurfaceRepresentation3D::BuildMiddlePoint()
{
  vtkMRMLMarkupsNode* markupsNode = this->GetMarkupsNode();
  if (!markupsNode)
    {
    return;
    }

  if (markupsNode->GetNumberOfControlPoints() != 2)
    {
    return;
    }

  double p1[3] = { 0.0 };
  double p2[3] = { 0.0 };
  double center[3] = {0.0};
  markupsNode->GetNthControlPointPositionWorld(0, p1);
  markupsNode->GetNthControlPointPositionWorld(1, p2);
  center[0] = (p1[0] + p2[0]) / 2.0;
  center[1] = (p1[1] + p2[1]) / 2.0;
  center[2] = (p1[2] + p2[2]) / 2.0;

  this->MiddlePointSource->SetCenter(center);
  this->MiddlePointSource->SetRadius(this->ControlPointSize);
}

//------------------------------------------------------------------------------
void vtkSlicerResectionSurfaceRepresentation3D::BuildSlicingPlane()
{
  vtkMRMLMarkupsNode* markupsNode = this->GetMarkupsNode();
  if (!markupsNode)
    {
    return;
    }

  if (markupsNode->GetNumberOfControlPoints() != 2)
    {
    return;
    }

  double p1[3] = { 0.0 };
  double p2[3] = { 0.0 };
  double origin[3] = {0.0};
  double normal[3] = {0.0};

  markupsNode->GetNthControlPointPositionWorld(0, p1);
  markupsNode->GetNthControlPointPositionWorld(1, p2);

  origin[0] = (p1[0] + p2[0]) / 2.0;
  origin[1] = (p1[1] + p2[1]) / 2.0;
  origin[2] = (p1[2] + p2[2]) / 2.0;

  normal[0] = p2[0] - p1[0];
  normal[1] = p2[1] - p1[1];
  normal[2] = p2[2] - p1[2];

  this->SlicingPlane->SetOrigin(origin);
  this->SlicingPlane->SetNormal(normal);
}
