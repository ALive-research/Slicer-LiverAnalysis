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

// Qt includes
#include <QDebug>

// Liver Markups Logic includes
//#include "vtkSlicerLiverMarkupsLogic.h"
#include "vtkResectionSurfaceWidget.h"

#include "qSlicerCoreApplication.h"
#include "vtkMRMLResectionSurfaceNode.h"

// Markups Logic includes
#include <vtkSlicerMarkupsLogic.h>
#include <qSlicerModuleManager.h>

// LiverAnalysis Logic includes
#include <vtkSlicerLiverAnalysisLogic.h>

// LiverAnalysis includes
#include "qSlicerLiverAnalysisModule.h"
#include "qSlicerLiverAnalysisModuleWidget.h"

//-----------------------------------------------------------------------------
/// \ingroup Slicer_QtModules_ExtensionTemplate
class qSlicerLiverAnalysisModulePrivate
{
public:
  qSlicerLiverAnalysisModulePrivate();
};

//-----------------------------------------------------------------------------
// qSlicerLiverAnalysisModulePrivate methods

//-----------------------------------------------------------------------------
qSlicerLiverAnalysisModulePrivate::qSlicerLiverAnalysisModulePrivate()
{
}

//-----------------------------------------------------------------------------
// qSlicerLiverAnalysisModule methods

//-----------------------------------------------------------------------------
qSlicerLiverAnalysisModule::qSlicerLiverAnalysisModule(QObject* _parent)
  : Superclass(_parent)
  , d_ptr(new qSlicerLiverAnalysisModulePrivate)
{
}

//-----------------------------------------------------------------------------
qSlicerLiverAnalysisModule::~qSlicerLiverAnalysisModule()
{
}

//-----------------------------------------------------------------------------
QString qSlicerLiverAnalysisModule::helpText() const
{
  return "This is a loadable module that can be bundled in an extension";
}

//-----------------------------------------------------------------------------
QString qSlicerLiverAnalysisModule::acknowledgementText() const
{
  return "This work was partially funded by NIH grant NXNNXXNNNNNN-NNXN";
}

//-----------------------------------------------------------------------------
QStringList qSlicerLiverAnalysisModule::contributors() const
{
  QStringList moduleContributors;
  moduleContributors << QString("John Doe (AnyWare Corp.)");
  return moduleContributors;
}

//-----------------------------------------------------------------------------
QIcon qSlicerLiverAnalysisModule::icon() const
{
  return QIcon(":/Icons/LiverAnalysis.png");
}

//-----------------------------------------------------------------------------
QStringList qSlicerLiverAnalysisModule::categories() const
{
  return QStringList() << "Liver";
}

//-----------------------------------------------------------------------------
QStringList qSlicerLiverAnalysisModule::dependencies() const
{
  return QStringList() << "Markups";
}

//-----------------------------------------------------------------------------
void qSlicerLiverAnalysisModule::setup()
{
  this->Superclass::setup();
    vtkSlicerApplicationLogic* appLogic = this->appLogic();
    if (!appLogic)
      {
      qCritical() << Q_FUNC_INFO << " : invalid application logic.";
      return;
      }

    vtkSlicerMarkupsLogic* markupsLogic =
      vtkSlicerMarkupsLogic::SafeDownCast(appLogic->GetModuleLogic("Markups"));
    if (!markupsLogic)
      {
      qCritical() << Q_FUNC_INFO << " : invalid markups logic.";
      return;
      }

    // Register markups
    resectionSurfaceNode = vtkSmartPointer<vtkMRMLResectionSurfaceNode>::New();
    resectionSurfaceWidget = vtkSmartPointer<vtkResectionSurfaceWidget>::New();
    markupsLogic->RegisterMarkupsNode(resectionSurfaceNode, resectionSurfaceWidget);

    qSlicerModuleManager* moduleManager = qSlicerCoreApplication::application()->moduleManager();
    if (!moduleManager)
      {
      return;
      }

    qSlicerAbstractCoreModule* markupsModule = moduleManager->module("Markups");
    if(!markupsModule)
      {
      qCritical() << Q_FUNC_INFO << ": Could not get the Markups module.";
      return;
      }

}

//-----------------------------------------------------------------------------
qSlicerAbstractModuleRepresentation* qSlicerLiverAnalysisModule
::createWidgetRepresentation()
{
  return new qSlicerLiverAnalysisModuleWidget;
}

//-----------------------------------------------------------------------------
vtkMRMLAbstractLogic* qSlicerLiverAnalysisModule::createLogic()
{
  return vtkSlicerLiverAnalysisLogic::New();
}
