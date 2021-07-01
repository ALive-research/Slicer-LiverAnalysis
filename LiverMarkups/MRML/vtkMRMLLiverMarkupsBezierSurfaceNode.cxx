/*=========================================================================

  Program: NorMIT-Plan
  Module: vtkMRMLBezierSurfaceNode.cxx

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

// This module includes
#include "vtkMRMLLiverMarkupsBezierSurfaceNode.h"

// MRML includes
#include <vtkMRMLModelNode.h>
#include <vtkMRMLScene.h>
#include "vtkMRMLMarkupsSlicingContourNode.h"

// VTK includes
#include <vtkObjectFactory.h>
#include <vtkPoints.h>
#include <vtkCommand.h>
#include <vtkPlaneSource.h>

#include <vtkEventBroker.h>

//------------------------------------------------------------------------------
vtkMRMLNodeNewMacro(vtkMRMLLiverMarkupsBezierSurfaceNode);

//------------------------------------------------------------------------------
vtkMRMLLiverMarkupsBezierSurfaceNode::vtkMRMLLiverMarkupsBezierSurfaceNode()
//  :ResectionMargin(DEFAULT_RESECTION_MARGIN)
{

  // Initialization of control points
  double startX = -100;
  double startY = -100;
  double endX = 100;
  double endY = 100;
  double incX = (endX - startX)/4.0;
  double incY = (endY - startY)/4.0;

  //Generate geometry;
  for(int i=0; i<4; ++i)
    {
    for(int j=0; j<4; ++j)
      {
      double point[3] = {startX+i*incX, startY+j*incY, 0.0};
      this->ControlPoints->InsertNextPoint(point);
      }
    }
  this->SetControlPointPositionsWorld(this->ControlPoints);
}

//------------------------------------------------------------------------------
vtkMRMLLiverMarkupsBezierSurfaceNode::~vtkMRMLLiverMarkupsBezierSurfaceNode()
{

}

//------------------------------------------------------------------------------
void vtkMRMLLiverMarkupsBezierSurfaceNode::PrintSelf(ostream &vtkNotUsed(os),
                                            vtkIndent vtkNotUsed(nIndent))
{

}

//------------------------------------------------------------------------------
vtkMRMLBezierSurfaceDisplayNode*
vtkMRMLLiverMarkupsBezierSurfaceNode::GetBezierSurfaceDisplayNode()
{
  vtkMRMLDisplayNode *displayNode = this->GetDisplayNode();
 // if (displayNode && displayNode->IsA("vtkMRMLBezierSurfaceDisplayNode"))
 //   {
 //   return vtkMRMLBezierSurfaceDisplayNode::SafeDownCast(displayNode);
 //   }
  return NULL;
}

//------------------------------------------------------------------------------
void vtkMRMLLiverMarkupsBezierSurfaceNode::AddTargetTumor(vtkMRMLModelNode *tumorNode)
{
  if (!tumorNode)
    {
    return;
    }

  if (this->TargetTumors->IsItemPresent(tumorNode))
    {
    return;
    }

  this->TargetTumors->AddItem(tumorNode);
  this->Modified();
}

//------------------------------------------------------------------------------
void vtkMRMLLiverMarkupsBezierSurfaceNode::RemoveTargetTumor(vtkMRMLModelNode *tumorNode)
{
  if (!tumorNode)
    {
    return;
    }

  if (!this->TargetTumors->IsItemPresent(tumorNode))
    {
    return;
    }

  this->TargetTumors->RemoveItem(tumorNode);
  this->Modified();
}

//------------------------------------------------------------------------------
int vtkMRMLLiverMarkupsBezierSurfaceNode::GetNumberOfTargetTumors() const
{
  return this->TargetTumors->GetNumberOfItems();
}

//------------------------------------------------------------------------------
void vtkMRMLLiverMarkupsBezierSurfaceNode::SetControlPoints(vtkPoints *points)
{
  if (!points)
    {
    vtkErrorMacro("No points provided.");
    return;
    }

  this->ControlPoints->DeepCopy(points);
  this->SetControlPointPositionsWorld(this->ControlPoints);
  this->Modified();
}

//------------------------------------------------------------------------------
void vtkMRMLLiverMarkupsBezierSurfaceNode::SetTargetParenchyma(vtkMRMLModelNode *node)
{
  this->TargetParenchyma = node;
  this->InvokeEvent(vtkCommand::ModifiedEvent);
}

//------------------------------------------------------------------------------
vtkMRMLModelNode* vtkMRMLLiverMarkupsBezierSurfaceNode::GetTargetParenchyma() const
{
  return this->TargetParenchyma;
}

//---------------------------------------------------------------------------
void vtkMRMLLiverMarkupsBezierSurfaceNode::ProcessMRMLEvents(vtkObject* caller,
    unsigned long event,
    void* callData)
{
    if (!caller)
    {
        std::cout << "No caller?" << std::endl;
        return;
    }

    //std::cout << "ProcessMRMLEvents: " << caller->GetClassName() << " " << event << std::endl;

    vtkEventBroker* broker = vtkEventBroker::GetInstance();
    if (!broker)
        return;

    if (!broker->GetObservationExist(this->GetScene(), vtkMRMLScene::NodeAddedEvent, this, this->MRMLCallbackCommand))
        broker->AddObservation(this->GetScene(), vtkMRMLScene::NodeAddedEvent, this, this->MRMLCallbackCommand);

    if (event == vtkMRMLScene::NodeAddedEvent)
    {
        vtkMRMLMarkupsSlicingContourNode* node = reinterpret_cast<vtkMRMLMarkupsSlicingContourNode*>(callData);
        //When vtkMRMLLiverMarkupsSlicingContourNode is added, start listening to DisplayModifiedEvent
        if (node)
        {
            //std::cout << "ProcessMRMLEvents got vtkMRMLLiverMarkupsSlicingContourNode" << std::endl;
            if (!broker->GetObservationExist(node, vtkMRMLMarkupsSlicingContourNode::DisplayModifiedEvent, this, this->MRMLCallbackCommand))
                broker->AddObservation(node, vtkMRMLMarkupsSlicingContourNode::DisplayModifiedEvent, this, this->MRMLCallbackCommand);
        }
    }
    /*else if (event == vtkMRMLMarkupsSlicingContourNode::DisplayModifiedEvent)
    {
        //std::cout << "ProcessMRMLEvents: " << caller->GetClassName() << " " << event << std::endl;
        vtkMRMLMarkupsSlicingContourNode* node = dynamic_cast<vtkMRMLMarkupsSlicingContourNode*>(caller);
        //When vtkMRMLLiverMarkupsSlicingContourNode is modified, update control points
        if (node)
        {
            vtkPoints* points = node->GetCurvePoints();
            if (points)
            {
                //std::cout << "init point 0: " << points->GetPoint(0)[0] << " " << points->GetPoint(0)[1] << points->GetPoint(0)[2] << std::endl;
                this->InitializeBezierSurface(points);
            }
        }
    }*/


    Superclass::ProcessMRMLEvents(caller, event, callData);
}
//------------------------------------------------------------------------------
void vtkMRMLLiverMarkupsBezierSurfaceNode::InitializeBezierSurface(vtkPoints* curve)
{
    std::cout << "InitializeBezierSurface" << std::endl;
    double point1[3];
    double point2[3];
    double midPoint[3];
    double normal[3];

    //initNode->GetPoint1(point1);
    //initNode->GetPoint2(point2);
    //vtkPoints* curve = contourNode->GetCurvePoints();
    if (curve->GetNumberOfPoints() < 2)
        return;
    curve->GetPoint(0, point1);
    curve->GetPoint(1, point2);

    midPoint[0] = (point1[0] + point2[0]) / 2.0;
    midPoint[1] = (point1[1] + point2[1]) / 2.0;
    midPoint[2] = (point1[2] + point2[2]) / 2.0;

    normal[0] = point2[0] - point1[0];
    normal[1] = point2[1] - point1[1];
    normal[2] = point2[2] - point1[2];

    // Cut the parenchyma (generate contour).
    /*vtkNew<vtkPlane> cuttingPlane;
    cuttingPlane->SetOrigin(midPoint);
    cuttingPlane->SetNormal(normal);
    vtkNew<vtkCutter> cutter;
    cutter->SetInputData(this->ParenchymaModelNode->GetPolyData());
    cutter->SetCutFunction(cuttingPlane.GetPointer());
    cutter->Update();

    vtkPolyData* contour = cutter->GetOutput();

    // Perform Principal Component Analysis
    vtkNew<vtkDoubleArray> xArray;
    xArray->SetNumberOfComponents(1);
    xArray->SetName("x");
    vtkNew<vtkDoubleArray> yArray;
    yArray->SetNumberOfComponents(1);
    yArray->SetName("y");
    vtkNew<vtkDoubleArray> zArray;
    zArray->SetNumberOfComponents(1);
    zArray->SetName("z");

    vtkNew<vtkCenterOfMass> centerOfMass;
    centerOfMass->SetInputData(contour);
    centerOfMass->Update();
    double com[3] = { 0 };
    centerOfMass->GetCenter(com);

    for (unsigned int i = 0; i < contour->GetNumberOfPoints(); i++)
    {
        double point[3];
        contour->GetPoint(i, point);
        xArray->InsertNextValue(point[0]);
        yArray->InsertNextValue(point[1]);
        zArray->InsertNextValue(point[2]);
    }

    vtkNew<vtkTable> dataTable;
    dataTable->AddColumn(xArray.GetPointer());
    dataTable->AddColumn(yArray.GetPointer());
    dataTable->AddColumn(zArray.GetPointer());

    vtkNew<vtkPCAStatistics> pcaStatistics;
    pcaStatistics->SetInputData(vtkStatisticsAlgorithm::INPUT_DATA,
        dataTable.GetPointer());
    pcaStatistics->SetColumnStatus("x", 1);
    pcaStatistics->SetColumnStatus("y", 1);
    pcaStatistics->SetColumnStatus("z", 1);
    pcaStatistics->RequestSelectedColumns();
    pcaStatistics->SetDeriveOption(true);
    pcaStatistics->SetFixedBasisSize(3);
    pcaStatistics->Update();

    vtkNew<vtkDoubleArray> eigenvalues;
    pcaStatistics->GetEigenvalues(eigenvalues.GetPointer());
    vtkNew<vtkDoubleArray> eigenvector1;
    pcaStatistics->GetEigenvector(0, eigenvector1.GetPointer());
    vtkNew<vtkDoubleArray> eigenvector2;
    pcaStatistics->GetEigenvector(1, eigenvector2.GetPointer());
    vtkNew<vtkDoubleArray> eigenvector3;
    pcaStatistics->GetEigenvector(2, eigenvector3.GetPointer());

    double length1 = 4.0 * sqrt(pcaStatistics->GetEigenvalue(0, 0));
    double length2 = 4.0 * sqrt(pcaStatistics->GetEigenvalue(0, 1));

    double v1[3] =
    {
      eigenvector1->GetValue(0),
      eigenvector1->GetValue(1),
      eigenvector1->GetValue(2)
    };

    double v2[3] =
    {
      eigenvector2->GetValue(0),
      eigenvector2->GetValue(1),
      eigenvector2->GetValue(2)
    };

    double origin[3] =
    {
      com[0] - v1[0] * length1 / 2.0 - v2[0] * length2 / 2.0,
      com[1] - v1[1] * length1 / 2.0 - v2[1] * length2 / 2.0,
      com[2] - v1[2] * length1 / 2.0 - v2[2] * length2 / 2.0,
    };

    point1[0] = origin[0] + v1[0] * length1;
    point1[1] = origin[1] + v1[1] * length1;
    point1[2] = origin[2] + v1[2] * length1;

    point2[0] = origin[0] + v2[0] * length2;
    point2[1] = origin[1] + v2[1] * length2;
    point2[2] = origin[2] + v2[2] * length2;*/

    //Create bezier surface according to initial plane
    vtkNew<vtkPlaneSource> planeSource;
    //planeSource->SetOrigin(origin);
    planeSource->SetOrigin(normal);
    planeSource->SetPoint1(point1);
    planeSource->SetPoint2(point2);
    planeSource->SetXResolution(3);
    planeSource->SetYResolution(3);
    planeSource->Update();

    vtkPoints* points = planeSource->GetOutput()->GetPoints();
    if (!points)
    {
        std::cout << "No points from planeSource" << std::endl;
        return;
    }
    this->SetControlPoints(points);
}
