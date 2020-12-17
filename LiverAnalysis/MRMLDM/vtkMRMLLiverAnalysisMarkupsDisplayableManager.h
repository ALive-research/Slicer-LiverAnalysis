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

#ifndef __vtkMRMLLiverAnalysisMarkupsDisplayableManager_h
#define __vtkMRMLLiverAnalysisMarkupsDisplayableManager_h

// MarkupsModule includes
#include "vtkSlicerMarkupsModuleMRMLDisplayableManagerExport.h"


// MRMLDisplayableManager includes
#include <vtkMRMLMarkupsDisplayableManager.h>

// VTK includes
#include <vtkSlicerMarkupsWidget.h>

// STD includes
#include <map>

class vtkMRMLMarkupsNode;
class vtkSlicerViewerWidget;
class vtkMRMLMarkupsDisplayNode;
class vtkAbstractWidget;

/// \ingroup Slicer_QtModules_Markups
class  VTK_SLICER_MARKUPS_MODULE_MRMLDISPLAYABLEMANAGER_EXPORT vtkMRMLLiverAnalysisMarkupsDisplayableManager :
    public vtkMRMLMarkupsDisplayableManager
{
public:

  static vtkMRMLLiverAnalysisMarkupsDisplayableManager *New();
  vtkTypeMacro(vtkMRMLLiverAnalysisMarkupsDisplayableManager, vtkMRMLMarkupsDisplayableManager);
  void PrintSelf(ostream& os, vtkIndent indent) override;

protected:

  vtkMRMLLiverAnalysisMarkupsDisplayableManager();
  ~vtkMRMLLiverAnalysisMarkupsDisplayableManager() override;

private:
  vtkMRMLLiverAnalysisMarkupsDisplayableManager(const vtkMRMLLiverAnalysisMarkupsDisplayableManager&) = delete;
  void operator=(const vtkMRMLLiverAnalysisMarkupsDisplayableManager&) = delete;
};

#endif
