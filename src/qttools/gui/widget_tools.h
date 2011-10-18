/****************************************************************************
**
**  FougTools
**  Copyright FougSys (1 Mar. 2011)
**  contact@fougsys.fr
**
** This software is a computer program whose purpose is to provide utility
** tools for the C++ language and the Qt toolkit.
**
** This software is governed by the CeCILL-C license under French law and
** abiding by the rules of distribution of free software.  You can  use,
** modify and/ or redistribute the software under the terms of the CeCILL-C
** license as circulated by CEA, CNRS and INRIA at the following URL
** "http://www.cecill.info".
**
** As a counterpart to the access to the source code and  rights to copy,
** modify and redistribute granted by the license, users are provided only
** with a limited warranty  and the software's author,  the holder of the
** economic rights,  and the successive licensors  have only  limited
** liability.
**
** In this respect, the user's attention is drawn to the risks associated
** with loading,  using,  modifying and/or developing or reproducing the
** software by the user in light of its specific status of free software,
** that may mean  that it is complicated to manipulate,  and  that  also
** therefore means  that it is reserved for developers  and  experienced
** professionals having in-depth computer knowledge. Users are therefore
** encouraged to load and test the software's suitability as regards their
** requirements in conditions enabling the security of their systems and/or
** data to be ensured and,  more generally, to use and operate it in the
** same conditions as regards security.
**
** The fact that you are presently reading this means that you have had
** knowledge of the CeCILL-C license and that you accept its terms.
**
****************************************************************************/

#ifndef QTTOOLS_WIDGET_TOOLS_H
#define QTTOOLS_WIDGET_TOOLS_H

#include "qttools/gui/gui.h"
#include <QtCore/QPair>
class QAbstractScrollArea;
class QWidget;

namespace qttools {

template<typename _PARENT_WIDGET_>
_PARENT_WIDGET_* findFirstParentWidget(QWidget* widget);

template<typename _PARENT_WIDGET_>
_PARENT_WIDGET_* findLastParentWidget(QWidget* widget);

QTTOOLS_GUI_EXPORT
void moveWidgetRightTo(QWidget* widget, const QWidget* nextTo);

QTTOOLS_GUI_EXPORT
void moveWidgetLeftTo(QWidget* widget, const QWidget* nextTo);

QTTOOLS_GUI_EXPORT
QPair<int, int> horizAndVertScrollValue(const QAbstractScrollArea* area);

QTTOOLS_GUI_EXPORT
void setHorizAndVertScrollValue(QAbstractScrollArea* area,
                                const QPair<int, int>& values);

} // namespace qttools



// --
// -- Implementation
// --

#include <QtGui/QWidget>

namespace qttools {

template<typename _PARENT_WIDGET_>
_PARENT_WIDGET_* findFirstParentWidget(QWidget* widget)
{
  _PARENT_WIDGET_* foundParentWidget = 0;
  QWidget* iteratorWidget = widget;
  while (iteratorWidget != 0 && foundParentWidget == 0)
  {
    iteratorWidget = iteratorWidget->parentWidget();
    foundParentWidget = qobject_cast<_PARENT_WIDGET_*>(iteratorWidget);
  }
  return foundParentWidget;
}

template<typename _PARENT_WIDGET_>
_PARENT_WIDGET_* findLastParentWidget(QWidget* widget)
{
  _PARENT_WIDGET_* foundParentWidget = 0;
  QWidget* iteratorWidget = widget;
  while (iteratorWidget != 0)
  {
    iteratorWidget = iteratorWidget->parentWidget();
    _PARENT_WIDGET_* currParentWidget =
        qobject_cast<_PARENT_WIDGET_*>(iteratorWidget);
    if (currParentWidget != 0)
      foundParentWidget = currParentWidget;
  }
  return foundParentWidget;
}

} // namespace qttools

#endif // QTTOOLS_WIDGET_TOOLS_H