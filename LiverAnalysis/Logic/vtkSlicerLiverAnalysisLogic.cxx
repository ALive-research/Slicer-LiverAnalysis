/*==============================================================================

  Program: 3D Slicer

  Portions (c) Copyright Brigham and Women's Hospital (BWH) All Rights Reserved.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

==============================================================================*/

// LiverAnalysis Logic includes
#include "vtkSlicerLiverAnalysisLogic.h"

// LiverAnalysis MRML includes
#include "vtkMRMLMarkupsBezierSurfaceNode.h"

// MRML includes
#include <vtkMRMLScene.h>
#include <vtkMRMLSelectionNode.h>

// VTK includes
#include <vtkIntArray.h>
#include <vtkNew.h>
#include <vtkObjectFactory.h>
#include <vtkSmartPointer.h>

// STD includes
#include <cassert>

//----------------------------------------------------------------------------
vtkStandardNewMacro(vtkSlicerLiverAnalysisLogic);

//----------------------------------------------------------------------------
vtkSlicerLiverAnalysisLogic::vtkSlicerLiverAnalysisLogic()
  :vtkSlicerMarkupsLogic()
{
}

//----------------------------------------------------------------------------
vtkSlicerLiverAnalysisLogic::~vtkSlicerLiverAnalysisLogic()
{
}

//----------------------------------------------------------------------------
void vtkSlicerLiverAnalysisLogic::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
}

//-----------------------------------------------------------------------------
void vtkSlicerLiverAnalysisLogic::RegisterNodes()
{
  assert(this->GetMRMLScene() != nullptr);

  vtkMRMLScene *scene = this->GetMRMLScene();

  // MRML Nodes
  scene->RegisterNodeClass(vtkSmartPointer<vtkMRMLMarkupsBezierSurfaceNode>::New());
}

//-----------------------------------------------------------------------------
void vtkSlicerLiverAnalysisLogic::ObserveMRMLScene()
{
  if (!this->GetMRMLScene())
    {
    return;
    }
  // add known markup types to the selection node
  vtkMRMLSelectionNode *selectionNode = vtkMRMLSelectionNode::SafeDownCast(this->GetMRMLScene()->GetNodeByID(this->GetSelectionNodeID().c_str()));
  if (selectionNode)
    {
    // got into batch process mode so that an update on the mouse mode tool
    // bar is triggered when leave it
    this->GetMRMLScene()->StartState(vtkMRMLScene::BatchProcessState);

    selectionNode->AddNewPlaceNodeClassNameToList("vtkMRMLMarkupsBezierSurfaceNode", ":/Icons/MarkupsPlaneMouseModePlace.png", "BezierSurface");

    // trigger an update on the mouse mode toolbar
    this->GetMRMLScene()->EndState(vtkMRMLScene::BatchProcessState);
    }

 this->Superclass::ObserveMRMLScene();
}
