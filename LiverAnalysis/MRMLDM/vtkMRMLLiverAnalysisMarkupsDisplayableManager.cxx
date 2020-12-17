#include "vtkMRMLLiverAnalysisMarkupsDisplayableManager.h"

#include <vtkMRMLAbstractSliceViewDisplayableManager.h>

// MarkupsModule/MRML includes
#include "vtkMRMLMarkupsBezierSurfaceNode.h"

// MarkupsModule/VTKWidgets includes
#include "vtkSlicerBezierSurfaceWidget.h"

//---------------------------------------------------------------------------
vtkStandardNewMacro (vtkMRMLLiverAnalysisMarkupsDisplayableManager);

//---------------------------------------------------------------------------
// vtkMRMLLiverAnalysisMarkupsDisplayableManager methods

//---------------------------------------------------------------------------
vtkMRMLLiverAnalysisMarkupsDisplayableManager::vtkMRMLLiverAnalysisMarkupsDisplayableManager()
{
  this->RegisterMarkup(vtkMRMLMarkupsBezierSurfaceNode::New(),
                       vtkSlicerBezierSurfaceWidget::New());
}

//---------------------------------------------------------------------------
vtkMRMLLiverAnalysisMarkupsDisplayableManager::~vtkMRMLLiverAnalysisMarkupsDisplayableManager()
{
}

//---------------------------------------------------------------------------
void vtkMRMLLiverAnalysisMarkupsDisplayableManager::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
}
