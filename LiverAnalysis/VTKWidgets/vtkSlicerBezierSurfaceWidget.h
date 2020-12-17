/*==============================================================================

  Copyright (c) Laboratory for Percutaneous Surgery (PerkLab)
  Queen's University, Kingston, ON, Canada. All Rights Reserved.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

  This file was originally developed by Kyle Sunderland, PerkLab, Queen's University
  and was supported through CANARIE's Research Software Program, Cancer
  Care Ontario, OpenAnatomy, and Brigham and Women’s Hospital through NIH grant R01MH112748.

==============================================================================*/

/**
 * @class   vtkSlicerBezierSurfaceWidget
 * @brief   create a plane with a set of 3 points
 *
 * The vtkSlicerBezierSurfaceWidget is used to create a plane widget with a set of 3 points.
 *
*/

#ifndef vtkSlicerBezierSurfaceWidget_h
#define vtkSlicerBezierSurfaceWidget_h

#include "vtkSlicerLiverAnalysisModuleVTKWidgetsExport.h"
#include "vtkSlicerMarkupsWidget.h"

class vtkSlicerMarkupsWidgetRepresentation;
class vtkPolyData;
class vtkIdList;

class VTK_SLICER_LIVERANALYSIS_MODULE_VTKWIDGETS_EXPORT vtkSlicerBezierSurfaceWidget :
public vtkSlicerMarkupsWidget
{
public:
  /// Instantiate this class.
  static vtkSlicerBezierSurfaceWidget *New();

  /// Standard methods for a VTK class.
  vtkTypeMacro(vtkSlicerBezierSurfaceWidget,vtkSlicerMarkupsWidget);

  vtkSlicerMarkupsWidget* CreateInstance() const override;

  /// Widget states
  enum
  {
    WidgetStateDefine = WidgetStateUser + 50, // click in empty area will place a new point
    WidgetStateTranslatePlane, // translating the plane
  };

  /// Widget events
  enum
  {
    WidgetEventControlPointPlace = WidgetEventUser + 50,
    WidgetEventPlaneMoveStart,
    WidgetEventPlaneMoveEnd,
  };

  /// Create the default widget representation and initializes the widget and representation.
  void CreateDefaultRepresentation(vtkMRMLMarkupsDisplayNode* markupsDisplayNode, vtkMRMLAbstractViewNode* viewNode, vtkRenderer* renderer) override;

protected:
  vtkSlicerBezierSurfaceWidget();
  ~vtkSlicerBezierSurfaceWidget() override;

  bool CanProcessInteractionEvent(vtkMRMLInteractionEventData* eventData, double& distance2) override;
  bool ProcessInteractionEvent(vtkMRMLInteractionEventData* eventData) override;
  bool ProcessPlaneMoveStart(vtkMRMLInteractionEventData* event);
  bool ProcessPlaneMoveEnd(vtkMRMLInteractionEventData* event);
  bool ProcessMouseMove(vtkMRMLInteractionEventData* eventData) override;
  bool ProcessPlaneTranslate(vtkMRMLInteractionEventData* event);

private:
  vtkSlicerBezierSurfaceWidget(const vtkSlicerBezierSurfaceWidget&) = delete;
  void operator=(const vtkSlicerBezierSurfaceWidget&) = delete;
};

#endif
