/*==============================================================================

  Program: 3D Slicer

  Copyright (c) Laboratory for Percutaneous Surgery (PerkLab)
  Queen's University, Kingston, ON, Canada. All Rights Reserved.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

  This file was originally developed by Csaba Pinter, PerkLab, Queen's University
  and was supported through the Applied Cancer Research Unit program of Cancer Care
  Ontario with funds provided by the Ontario Ministry of Health and Long-Term Care

==============================================================================*/

// SubjectHierarchy MRML includes
#include "vtkMRMLSubjectHierarchyNode.h"
#include "vtkMRMLSubjectHierarchyConstants.h"

// SubjectHierarchy Plugins includes
#include "qSlicerSubjectHierarchyPluginHandler.h"
#include "qSlicerSubjectHierarchyLiverAnalysisPlugin.h"
#include "qSlicerSubjectHierarchyDefaultPlugin.h"

// Terminologies includes
#include "qSlicerTerminologyItemDelegate.h"
#include "vtkSlicerTerminologiesModuleLogic.h"

// MRML widgets includes
#include "qMRMLNodeComboBox.h"

// MRML includes
#include <vtkMRMLDisplayableNode.h>
#include <vtkMRMLDisplayNode.h>
#include <vtkMRMLMarkupsDisplayNode.h>
#include <vtkMRMLMarkupsNode.h>
#include <vtkMRMLScene.h>

// vtkSegmentationCore includes
#include <vtkSegment.h>

// VTK includes
#include <vtkObjectFactory.h>
#include <vtkSmartPointer.h>

// Qt includes
#include <QDebug>
#include <QInputDialog>
#include <QStandardItem>
#include <QAction>

// Slicer includes
#include "qSlicerAbstractModuleWidget.h"

//-----------------------------------------------------------------------------
/// \ingroup Slicer_QtModules_SubjectHierarchy_Plugins
class qSlicerSubjectHierarchyLiverAnalysisPluginPrivate: public QObject
{
  Q_DECLARE_PUBLIC(qSlicerSubjectHierarchyLiverAnalysisPlugin);
protected:
  qSlicerSubjectHierarchyLiverAnalysisPlugin* const q_ptr;
public:
  qSlicerSubjectHierarchyLiverAnalysisPluginPrivate(qSlicerSubjectHierarchyLiverAnalysisPlugin& object);
  ~qSlicerSubjectHierarchyLiverAnalysisPluginPrivate() override;
  void init();

public:
  QAction* RenamePointAction;
  QAction* DeletePointAction;
  QAction* ToggleSelectPointAction;
  QAction* ToggleHandleInteractive;

  QVariantMap ViewMenuEventData;
};

//-----------------------------------------------------------------------------
// qSlicerSubjectHierarchyLiverAnalysisPluginPrivate methods

//-----------------------------------------------------------------------------
qSlicerSubjectHierarchyLiverAnalysisPluginPrivate::qSlicerSubjectHierarchyLiverAnalysisPluginPrivate(qSlicerSubjectHierarchyLiverAnalysisPlugin& object)
: q_ptr(&object)
, RenamePointAction(nullptr)
, DeletePointAction(nullptr)
, ToggleSelectPointAction(nullptr)
, ToggleHandleInteractive(nullptr)
{
}

//------------------------------------------------------------------------------
void qSlicerSubjectHierarchyLiverAnalysisPluginPrivate::init()
{
  Q_Q(qSlicerSubjectHierarchyLiverAnalysisPlugin);

  this->RenamePointAction = new QAction("Rename point...", q);
  this->RenamePointAction->setObjectName("RenamePointAction");
  QObject::connect(this->RenamePointAction, SIGNAL(triggered()), q, SLOT(renamePoint()));

  this->DeletePointAction = new QAction("Delete point", q);
  this->DeletePointAction->setObjectName("DeletePointAction");
  QObject::connect(this->DeletePointAction, SIGNAL(triggered()), q, SLOT(deletePoint()));

  this->ToggleSelectPointAction = new QAction("Toggle select point", q);
  this->ToggleSelectPointAction->setObjectName("ToggleSelectPointAction");
  QObject::connect(this->ToggleSelectPointAction, SIGNAL(triggered()), q, SLOT(toggleSelectPoint()));

  this->ToggleHandleInteractive = new QAction("Interaction handles visible");
  this->ToggleHandleInteractive->setObjectName("ToggleHandleInteractive");
  this->ToggleHandleInteractive->setCheckable(true);
  QObject::connect(this->ToggleHandleInteractive, SIGNAL(triggered()), q, SLOT(toggleHandleInteractive()));
}

//-----------------------------------------------------------------------------
qSlicerSubjectHierarchyLiverAnalysisPluginPrivate::~qSlicerSubjectHierarchyLiverAnalysisPluginPrivate() = default;

//-----------------------------------------------------------------------------
// qSlicerSubjectHierarchyLiverAnalysisPlugin methods

//-----------------------------------------------------------------------------
qSlicerSubjectHierarchyLiverAnalysisPlugin::qSlicerSubjectHierarchyLiverAnalysisPlugin(QObject* parent)
 : Superclass(parent)
 , d_ptr( new qSlicerSubjectHierarchyLiverAnalysisPluginPrivate(*this) )
{
  this->m_Name = QString("LiverAnalysis");

  Q_D(qSlicerSubjectHierarchyLiverAnalysisPlugin);
  d->init();
}

//-----------------------------------------------------------------------------
qSlicerSubjectHierarchyLiverAnalysisPlugin::~qSlicerSubjectHierarchyLiverAnalysisPlugin() = default;

//----------------------------------------------------------------------------
double qSlicerSubjectHierarchyLiverAnalysisPlugin::canAddNodeToSubjectHierarchy(
  vtkMRMLNode* node, vtkIdType parentItemID/*=vtkMRMLSubjectHierarchyNode::INVALID_ITEM_ID*/)const
{
  Q_UNUSED(parentItemID);
  if (!node)
    {
    qCritical() << Q_FUNC_INFO << ": Input node is NULL";
    return 0.0;
    }
  else if (node->IsA("vtkMRMLMarkupsBezierSurfaceNode"))
    {
    // Item is a markup
    return 0.5;
    }
  return 0.0;
}

//---------------------------------------------------------------------------
double qSlicerSubjectHierarchyLiverAnalysisPlugin::canOwnSubjectHierarchyItem(vtkIdType itemID)const
{
  if (itemID == vtkMRMLSubjectHierarchyNode::INVALID_ITEM_ID)
    {
    qCritical() << Q_FUNC_INFO << ": Invalid input item";
    return 0.0;
    }
  vtkMRMLSubjectHierarchyNode* shNode = qSlicerSubjectHierarchyPluginHandler::instance()->subjectHierarchyNode();
  if (!shNode)
    {
    qCritical() << Q_FUNC_INFO << ": Failed to access subject hierarchy node";
    return 0.0;
    }

  // Markup
  vtkMRMLNode* associatedNode = shNode->GetItemDataNode(itemID);
  if (associatedNode &&
      (associatedNode->IsA("vtkMRMLMarkupsBezierSurfaceNode"))
    )
    {
    // Item is a markup
    return 0.5;
    }

  return 0.0;
}

//---------------------------------------------------------------------------
const QString qSlicerSubjectHierarchyLiverAnalysisPlugin::roleForPlugin()const
{
  return "Markup";
}

//---------------------------------------------------------------------------
QIcon qSlicerSubjectHierarchyLiverAnalysisPlugin::icon(vtkIdType itemID)
{
  if (itemID == vtkMRMLSubjectHierarchyNode::INVALID_ITEM_ID)
    {
    qCritical() << Q_FUNC_INFO << ": Invalid input item";
    return QIcon();
    }

  Q_D(qSlicerSubjectHierarchyLiverAnalysisPlugin);

  if (!this->canOwnSubjectHierarchyItem(itemID))
    {
    // Item unknown by plugin
    return QIcon();
    }
  vtkMRMLSubjectHierarchyNode* shNode = qSlicerSubjectHierarchyPluginHandler::instance()->subjectHierarchyNode();
  if (!shNode)
    {
    return QIcon();
    }
  vtkMRMLNode* node = shNode->GetItemDataNode(itemID);
  if (!node)
    {
    return QIcon();
    }
  if (node->IsA("vtkMRMLMarkupsBezierSurfaceNode"))
    {
    return QIcon(":Icons/MarkupsFiducial.png");
    }
  return QIcon();
}

//---------------------------------------------------------------------------
QIcon qSlicerSubjectHierarchyLiverAnalysisPlugin::visibilityIcon(int visible)
{
  // Have the default plugin (which is not registered) take care of this
  return qSlicerSubjectHierarchyPluginHandler::instance()->defaultPlugin()->visibilityIcon(visible);
}

//-----------------------------------------------------------------------------
void qSlicerSubjectHierarchyLiverAnalysisPlugin::setDisplayColor(vtkIdType itemID, QColor color, QMap<int, QVariant> terminologyMetaData)
{
  if (itemID == vtkMRMLSubjectHierarchyNode::INVALID_ITEM_ID)
    {
    qCritical() << Q_FUNC_INFO << ": Invalid input item";
    return;
    }
  vtkMRMLSubjectHierarchyNode* shNode = qSlicerSubjectHierarchyPluginHandler::instance()->subjectHierarchyNode();
  if (!shNode)
    {
    qCritical() << Q_FUNC_INFO << ": Failed to access subject hierarchy node";
    return;
    }

  // Get display node
  vtkMRMLDisplayableNode* displayableNode = vtkMRMLDisplayableNode::SafeDownCast(shNode->GetItemDataNode(itemID));
  if (!displayableNode)
    {
    qCritical() << Q_FUNC_INFO << ": Unable to find node for subject hierarchy item " << shNode->GetItemName(itemID).c_str();
    return;
    }
  vtkMRMLDisplayNode* displayNode = displayableNode->GetDisplayNode();
  if (!displayNode)
    {
    qCritical() << Q_FUNC_INFO << ": No display node";
    return;
    }

  // Set terminology metadata
  if (terminologyMetaData.contains(qSlicerTerminologyItemDelegate::TerminologyRole))
    {
    displayableNode->SetAttribute(vtkSegment::GetTerminologyEntryTagName(),
      terminologyMetaData[qSlicerTerminologyItemDelegate::TerminologyRole].toString().toUtf8().constData() );
    }
  if (terminologyMetaData.contains(qSlicerTerminologyItemDelegate::NameRole))
    {
    displayableNode->SetName(
      terminologyMetaData[qSlicerTerminologyItemDelegate::NameRole].toString().toUtf8().constData() );
    }
  if (terminologyMetaData.contains(qSlicerTerminologyItemDelegate::NameAutoGeneratedRole))
    {
    displayableNode->SetAttribute( vtkSlicerTerminologiesModuleLogic::GetNameAutoGeneratedAttributeName(),
      terminologyMetaData[qSlicerTerminologyItemDelegate::NameAutoGeneratedRole].toString().toUtf8().constData() );
    }
  if (terminologyMetaData.contains(qSlicerTerminologyItemDelegate::ColorAutoGeneratedRole))
    {
    displayableNode->SetAttribute( vtkSlicerTerminologiesModuleLogic::GetColorAutoGeneratedAttributeName(),
      terminologyMetaData[qSlicerTerminologyItemDelegate::ColorAutoGeneratedRole].toString().toUtf8().constData() );
    }

  // Set color
  double* oldColorArray = displayNode->GetColor();
  QColor oldColor = QColor::fromRgbF(oldColorArray[0], oldColorArray[1], oldColorArray[2]);
  if (oldColor != color)
    {
    displayNode->SetSelectedColor(color.redF(), color.greenF(), color.blueF());

    // Trigger update of color swatch
    shNode->ItemModified(itemID);
    }
}

//-----------------------------------------------------------------------------
QColor qSlicerSubjectHierarchyLiverAnalysisPlugin::getDisplayColor(vtkIdType itemID, QMap<int, QVariant> &terminologyMetaData)const
{
  if (itemID == vtkMRMLSubjectHierarchyNode::INVALID_ITEM_ID)
    {
    qCritical() << Q_FUNC_INFO << ": Invalid input item";
    return QColor(0,0,0,0);
    }
  vtkMRMLSubjectHierarchyNode* shNode = qSlicerSubjectHierarchyPluginHandler::instance()->subjectHierarchyNode();
  if (!shNode)
    {
    qCritical() << Q_FUNC_INFO << ": Failed to access subject hierarchy node";
    return QColor(0,0,0,0);
    }
  vtkMRMLScene* scene = qSlicerSubjectHierarchyPluginHandler::instance()->mrmlScene();
  if (!scene)
    {
    qCritical() << Q_FUNC_INFO << ": Invalid MRML scene";
    return QColor(0,0,0,0);
    }

  if (scene->IsImporting())
    {
    // During import SH node may be created before the segmentation is read into the scene,
    // so don't attempt to access the segment yet
    return QColor(0,0,0,0);
    }

  // Get display node
  vtkMRMLDisplayableNode* displayableNode = vtkMRMLDisplayableNode::SafeDownCast(shNode->GetItemDataNode(itemID));
  if (!displayableNode)
    {
    qCritical() << Q_FUNC_INFO << ": Unable to find node for subject hierarchy item " << shNode->GetItemName(itemID).c_str();
    return QColor(0,0,0,0);
    }
  vtkMRMLDisplayNode* displayNode = displayableNode->GetDisplayNode();
  if (!displayNode)
    {
    // this is normal when the markups node is being created
    return QColor(0,0,0,0);
    }

  // Get terminology metadata
  terminologyMetaData.clear();
  terminologyMetaData[qSlicerTerminologyItemDelegate::TerminologyRole] =
    displayableNode->GetAttribute(vtkSegment::GetTerminologyEntryTagName());
  terminologyMetaData[qSlicerTerminologyItemDelegate::NameRole] = displayableNode->GetName();
  // If auto generated flags are not initialized, then set them to the default
  // (color: on, name: off - this way color will be set from the selector but name will not)
  bool nameAutoGenerated = false;
  if (displayableNode->GetAttribute(vtkSlicerTerminologiesModuleLogic::GetNameAutoGeneratedAttributeName()))
    {
    nameAutoGenerated = QVariant(displayableNode->GetAttribute(vtkSlicerTerminologiesModuleLogic::GetNameAutoGeneratedAttributeName())).toBool();
    }
  terminologyMetaData[qSlicerTerminologyItemDelegate::NameAutoGeneratedRole] = nameAutoGenerated;
  bool colorAutoGenerated = true;
  if (displayableNode->GetAttribute(vtkSlicerTerminologiesModuleLogic::GetColorAutoGeneratedAttributeName()))
    {
    colorAutoGenerated = QVariant(displayableNode->GetAttribute(vtkSlicerTerminologiesModuleLogic::GetColorAutoGeneratedAttributeName())).toBool();
    }
  terminologyMetaData[qSlicerTerminologyItemDelegate::ColorAutoGeneratedRole] = colorAutoGenerated;

  // Get and return color
  double* colorArray = displayNode->GetSelectedColor();
  return QColor::fromRgbF(colorArray[0], colorArray[1], colorArray[2]);
}

//-----------------------------------------------------------------------------
QList<QAction*> qSlicerSubjectHierarchyLiverAnalysisPlugin::viewContextMenuActions()const
{
  Q_D(const qSlicerSubjectHierarchyLiverAnalysisPlugin);

  QList<QAction*> actions;
  actions << d->RenamePointAction << d->DeletePointAction << d->ToggleSelectPointAction << d->ToggleHandleInteractive;
  return actions;
}

//-----------------------------------------------------------------------------
void qSlicerSubjectHierarchyLiverAnalysisPlugin::showViewContextMenuActionsForItem(vtkIdType itemID, QVariantMap eventData)
{
  Q_D(qSlicerSubjectHierarchyLiverAnalysisPlugin);

  if (itemID == vtkMRMLSubjectHierarchyNode::INVALID_ITEM_ID)
    {
    qCritical() << Q_FUNC_INFO << ": Invalid input item";
    return;
    }
  vtkMRMLSubjectHierarchyNode* shNode = qSlicerSubjectHierarchyPluginHandler::instance()->subjectHierarchyNode();
  if (!shNode)
    {
    qCritical() << Q_FUNC_INFO << ": Failed to access subject hierarchy node";
    return;
    }

  // Markup
  vtkMRMLMarkupsNode* associatedNode = vtkMRMLMarkupsNode::SafeDownCast(shNode->GetItemDataNode(itemID));
  if (associatedNode)
    {
    d->ViewMenuEventData = eventData;
    d->ViewMenuEventData["NodeID"] = QVariant(associatedNode->GetID());

    int componentType = d->ViewMenuEventData["ComponentType"].toInt();
    bool handlesSelected = componentType == vtkMRMLMarkupsDisplayNode::ComponentTranslationHandle ||
      componentType == vtkMRMLMarkupsDisplayNode::ComponentRotationHandle;

    d->RenamePointAction->setVisible(!handlesSelected);
    d->DeletePointAction->setVisible(!handlesSelected);
    d->ToggleSelectPointAction->setVisible(!handlesSelected);

    vtkMRMLMarkupsDisplayNode* displayNode = vtkMRMLMarkupsDisplayNode::SafeDownCast(associatedNode->GetDisplayNode());
    d->ToggleHandleInteractive->setVisible(displayNode != nullptr);
    if (displayNode)
      {
      d->ToggleHandleInteractive->setChecked(displayNode->GetHandlesInteractive());
      }
    }
}

//-----------------------------------------------------------------------------
void qSlicerSubjectHierarchyLiverAnalysisPlugin::renamePoint()
{
  Q_D(qSlicerSubjectHierarchyLiverAnalysisPlugin);

  if (d->ViewMenuEventData.find("NodeID") == d->ViewMenuEventData.end())
    {
    qCritical() << Q_FUNC_INFO << ": No node ID found in the view menu event data";
    return;
    }
  vtkMRMLScene* scene = qSlicerSubjectHierarchyPluginHandler::instance()->mrmlScene();
  if (!scene)
    {
    qCritical() << Q_FUNC_INFO << ": Failed to access MRML scene";
    return;
    }

  // Get markups node
  QString nodeID = d->ViewMenuEventData["NodeID"].toString();
  vtkMRMLNode* node = scene->GetNodeByID(nodeID.toUtf8().constData());
  vtkMRMLMarkupsNode* markupsNode = vtkMRMLMarkupsNode::SafeDownCast(node);
  if (!markupsNode)
    {
    qCritical() << Q_FUNC_INFO << ": Failed to get markups node by ID " << nodeID;
    return;
    }

  // Get point index
  int componentIndex = d->ViewMenuEventData["ComponentIndex"].toInt();

  // Pop up an entry box for the new name, with the old name as default
  QString oldName(markupsNode->GetNthControlPointLabel(componentIndex).c_str());

  bool ok = false;
  QString newName = QInputDialog::getText(nullptr, QString("Rename ") + oldName, "New name:", QLineEdit::Normal, oldName, &ok);
  if (!ok)
    {
    return;
    }

  markupsNode->SetNthControlPointLabel(componentIndex, newName.toUtf8().constData());
}

//-----------------------------------------------------------------------------
void qSlicerSubjectHierarchyLiverAnalysisPlugin::deletePoint()
{
  Q_D(qSlicerSubjectHierarchyLiverAnalysisPlugin);

  if (d->ViewMenuEventData.find("NodeID") == d->ViewMenuEventData.end())
    {
    qCritical() << Q_FUNC_INFO << ": No node ID found in the view menu event data";
    return;
    }
  vtkMRMLScene* scene = qSlicerSubjectHierarchyPluginHandler::instance()->mrmlScene();
  if (!scene)
    {
    qCritical() << Q_FUNC_INFO << ": Failed to access MRML scene";
    return;
    }

  // Get markups node
  QString nodeID = d->ViewMenuEventData["NodeID"].toString();
  vtkMRMLNode* node = scene->GetNodeByID(nodeID.toUtf8().constData());
  vtkMRMLMarkupsNode* markupsNode = vtkMRMLMarkupsNode::SafeDownCast(node);
  if (!markupsNode)
    {
    qCritical() << Q_FUNC_INFO << ": Failed to get markups node by ID " << nodeID;
    return;
    }

  // Get point index
  int componentIndex = d->ViewMenuEventData["ComponentIndex"].toInt();

  markupsNode->RemoveNthControlPoint(componentIndex);
}

//-----------------------------------------------------------------------------
void qSlicerSubjectHierarchyLiverAnalysisPlugin::toggleSelectPoint()
{
  Q_D(qSlicerSubjectHierarchyLiverAnalysisPlugin);

  if (d->ViewMenuEventData.find("NodeID") == d->ViewMenuEventData.end())
    {
    qCritical() << Q_FUNC_INFO << ": No node ID found in the view menu event data";
    return;
    }
  vtkMRMLScene* scene = qSlicerSubjectHierarchyPluginHandler::instance()->mrmlScene();
  if (!scene)
    {
    qCritical() << Q_FUNC_INFO << ": Failed to access MRML scene";
    return;
    }

  // Get markups node
  QString nodeID = d->ViewMenuEventData["NodeID"].toString();
  vtkMRMLNode* node = scene->GetNodeByID(nodeID.toUtf8().constData());
  vtkMRMLMarkupsNode* markupsNode = vtkMRMLMarkupsNode::SafeDownCast(node);
  if (!markupsNode)
    {
    qCritical() << Q_FUNC_INFO << ": Failed to get markups node by ID " << nodeID;
    return;
    }

  // Get point index
  int componentIndex = d->ViewMenuEventData["ComponentIndex"].toInt();

  markupsNode->SetNthControlPointSelected(componentIndex, !markupsNode->GetNthControlPointSelected(componentIndex));
}

//-----------------------------------------------------------------------------
void qSlicerSubjectHierarchyLiverAnalysisPlugin::toggleHandleInteractive()
{
  Q_D(qSlicerSubjectHierarchyLiverAnalysisPlugin);

  if (d->ViewMenuEventData.find("NodeID") == d->ViewMenuEventData.end())
    {
    qCritical() << Q_FUNC_INFO << ": No node ID found in the view menu event data";
    return;
    }
  vtkMRMLScene* scene = qSlicerSubjectHierarchyPluginHandler::instance()->mrmlScene();
  if (!scene)
    {
    qCritical() << Q_FUNC_INFO << ": Failed to access MRML scene";
    return;
    }

  // Get markups node
  QString nodeID = d->ViewMenuEventData["NodeID"].toString();
  vtkMRMLNode* node = scene->GetNodeByID(nodeID.toUtf8().constData());
  vtkMRMLMarkupsNode* markupsNode = vtkMRMLMarkupsNode::SafeDownCast(node);
  if (!markupsNode)
    {
    qCritical() << Q_FUNC_INFO << ": Failed to get markups node by ID " << nodeID;
    return;
    }

  vtkMRMLMarkupsDisplayNode* displayNode = vtkMRMLMarkupsDisplayNode::SafeDownCast(markupsNode->GetDisplayNode());
  if (!displayNode)
    {
    qCritical() << Q_FUNC_INFO << ": Failed to get display node for " << nodeID;
    return;
    }
  displayNode->SetHandlesInteractive(!displayNode->GetHandlesInteractive());
}
