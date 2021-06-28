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

  * Neither the name of Kitware, Inc. nor the names of Contributors
    may be used to endorse or promote products derived from this
    software without specific prior written permission.

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

#include "qSlicerLiverMarkupsModule.h"
//#include "MRML/vtkMRMLLiverMarkupsSlicingContourNode.h"
#include "MRML/vtkMRMLLiverMarkupsResectionSurfaceNode.h"
#include "MRML/vtkMRMLMarkupsSlicingContourNode.h"

// Qt includes
#include <QDebug>

// Liver Markups Logic includes
#include "vtkSlicerLiverMarkupsLogic.h"

// Markups Logic includes
#include <vtkSlicerMarkupsLogic.h>

// Markups VTKWidgets includes
#include <vtkSlicerLineWidget.h>

// Liver Markups VTKWidgets includes
#include <vtkSlicerSlicingContourWidget.h>
#include <vtkSlicerResectionSurfaceWidget.h>

#include <qSlicerModuleManager.h>
#include <qSlicerCoreApplication.h>

// MRMLDisplayableManager includes
#include <vtkMRMLThreeDViewDisplayableManagerFactory.h>
#include <vtkMRMLSliceViewDisplayableManagerFactory.h>
#include <vtkMRMLResectionDisplayableManager3D.h>

// DisplayableManager initialization
#include <vtkAutoInit.h>
#include <vtkMRMLScene.h>
VTK_MODULE_INIT(vtkSlicerLiverMarkupsModuleMRMLDisplayableManager)

//-----------------------------------------------------------------------------
/// \ingroup Slicer_QtModules_ExtensionTemplate
class qSlicerLiverMarkupsModulePrivate
{
public:
  qSlicerLiverMarkupsModulePrivate();
};

//-----------------------------------------------------------------------------
// qSlicerLiverMarkupsModulePrivate methods

//-----------------------------------------------------------------------------
qSlicerLiverMarkupsModulePrivate::qSlicerLiverMarkupsModulePrivate()
{
}

//-----------------------------------------------------------------------------
// qSlicerLiverMarkupsModule methods

//-----------------------------------------------------------------------------
qSlicerLiverMarkupsModule::qSlicerLiverMarkupsModule(QObject* _parent)
  : Superclass(_parent)
  , d_ptr(new qSlicerLiverMarkupsModulePrivate)
  , resectionSurfaceNode(nullptr)
{
}

//-----------------------------------------------------------------------------
qSlicerLiverMarkupsModule::~qSlicerLiverMarkupsModule()
{
}

bool qSlicerLiverMarkupsModule::isHidden() const
{
    return true;
}

//-----------------------------------------------------------------------------
QString qSlicerLiverMarkupsModule::helpText() const
{
  return "This module contains fundamental markups to be used in the Slicer-Liver extension.";
}

//-----------------------------------------------------------------------------
QString qSlicerLiverMarkupsModule::acknowledgementText() const
{
  return "This work has been partially funded by The Research Council of Norway (grant nr. 311393)";
}

//-----------------------------------------------------------------------------
QStringList qSlicerLiverMarkupsModule::contributors() const
{
  QStringList moduleContributors;
  moduleContributors << QString("Rafael Palomar (Oslo University Hospital / NTNU) ");
  moduleContributors << QString("Ole Vegard Solberg (SINTEF) ");
  moduleContributors << QString("Geir Arne Tangen (SINTEF) ");
  return moduleContributors;
}

//-----------------------------------------------------------------------------
QIcon qSlicerLiverMarkupsModule::icon() const
{
  return QIcon(":/Icons/LiverMarkups.png");
}

//-----------------------------------------------------------------------------
QStringList qSlicerLiverMarkupsModule::categories() const
{
  return QStringList() << "Liver";
}

//-----------------------------------------------------------------------------
QStringList qSlicerLiverMarkupsModule::dependencies() const
{
  return QStringList() << "Markups";
}

//-----------------------------------------------------------------------------
void qSlicerLiverMarkupsModule::setup()
{
  this->Superclass::setup();

  // Register displayable managers 3D
  //Not working yet: "Error: vtkMRMLResectionDisplayableManager3D is not a displayable manager. Failed to register"
  // Use the displayable manager class to make sure the the containing library is loaded
  //vtkSmartPointer<vtkMRMLResectionDisplayableManager3D> displayableManager3D = vtkSmartPointer<vtkMRMLResectionDisplayableManager3D>::New();
  //vtkMRMLThreeDViewDisplayableManagerFactory::GetInstance()->GlobalWarningDisplayOn();
  /*vtkMRMLThreeDViewDisplayableManagerFactory* factory
      = vtkMRMLThreeDViewDisplayableManagerFactory::GetInstance();

  QStringList displayableManagers;
  displayableManagers << "vtkMRMLResectionDisplayableManager3D" 
                      << "vtkMRMLResectionInitializationDisplayableManager3D";
  foreach(const QString & displayableManager, displayableManagers)
  {
      if (!factory->IsDisplayableManagerRegistered(displayableManager.toUtf8()))
          factory->RegisterDisplayableManager(displayableManager.toUtf8());
  }*/

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
 vtkNew<vtkMRMLMarkupsSlicingContourNode> slicingContourNode;
 vtkNew<vtkSlicerSlicingContourWidget> slicingContourWidget;
 markupsLogic->RegisterMarkupsNode(slicingContourNode, slicingContourWidget);


 resectionSurfaceNode = vtkMRMLLiverMarkupsResectionSurfaceNode::New();
 markupsLogic->RegisterMarkupsNode(resectionSurfaceNode,
                                   vtkSlicerResectionSurfaceWidget::New());

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
qSlicerAbstractModuleRepresentation* qSlicerLiverMarkupsModule
::createWidgetRepresentation()
{
    return nullptr;
}

//-----------------------------------------------------------------------------
vtkMRMLAbstractLogic* qSlicerLiverMarkupsModule::createLogic()
{
  return vtkSlicerLiverMarkupsLogic::New();
}

//-----------------------------------------------------------------------------
QStringList qSlicerLiverMarkupsModule::associatedNodeTypes() const
{
 return QStringList() << "vtkMRMLMarkupsSlicingContourNode"
                      << "vtkMRMLMarkupsResectionSurfaceNode";
}

//-----------------------------------------------------------------------------
void qSlicerLiverMarkupsModule::setMRMLScene(vtkMRMLScene* scene)
{
  this->Superclass::setMRMLScene(scene);
  if(resectionSurfaceNode)
    this->mrmlScene()->AddNode(resectionSurfaceNode);
  vtkSlicerLiverMarkupsLogic* logic =
    vtkSlicerLiverMarkupsLogic::SafeDownCast(this->logic());
  if (!logic)
    {
    qCritical() << Q_FUNC_INFO << " failed: logic is invalid";
    return;
    }
}
