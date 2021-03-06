/****************************************************************************
**  FougTools
**  Copyright Fougue (30 Mar. 2015)
**  contact@fougue.pro
**
** This software is a computer program whose purpose is to provide utility
** tools for the C++ language and the Qt toolkit.
**
** This software is governed by the CeCILL-C license under French law and
** abiding by the rules of distribution of free software.  You can  use,
** modify and/ or redistribute the software under the terms of the CeCILL-C
** license as circulated by CEA, CNRS and INRIA at the following URL
** "http://www.cecill.info".
****************************************************************************/

#include "qwidget_utils.h"

// QtWidgets
#include <QAbstractScrollArea>
#include <QBoxLayout>
#include <QDialog>
#include <QDialogButtonBox>
#include <QMenu>
#include <QScrollBar>
#include <QWidget>

namespace qtgui {

/*! \class QWidgetUtils
 *  \brief Provides a collection of tools around QWidget
 *
 *  \headerfile qwidget_utils.h <qttools/gui/qwidget_utils.h>
 *  \ingroup qttools_gui
 */

/*! \brief Make \p widget the central widget of \p dialog
 *
 *  \p dialog should be empty for this function to work.\n
 *  wrapWidgetInDialog() will try to find if \p widget contains a
 *  QDialogButtonBox, if so
 *  then it connects to \p dialog 's accept()/reject() slots.
 */
void QWidgetUtils::wrapWidgetInDialog(QWidget *widget, QDialog *dialog)
{
    if (widget != NULL && dialog != NULL) {
        dialog->setWindowTitle(widget->windowTitle());
        widget->setParent(dialog);
        if (dialog->layout() != NULL) {
            dialog->layout()->addWidget(widget);
        }
        else {
            auto layout = new QVBoxLayout;
            layout->addWidget(widget);
            dialog->setLayout(layout);
        }

        auto btnBox = widget->findChild<QDialogButtonBox*>();
        if (btnBox != NULL) {
            QObject::connect(
                        btnBox, &QDialogButtonBox::accepted,
                        dialog, &QDialog::accept);
            QObject::connect(
                        btnBox, &QDialogButtonBox::rejected,
                        dialog, &QDialog::reject);
        }
    }
}

/*! Add \p contentsWidget to \p containerWidget
 *
 *  If \p containerWidget is empty, a QBoxLayout is created to receive
 *  \p contentsWidget
 */
void QWidgetUtils::addContentsWidget(
        QWidget *containerWidget, QWidget *contentsWidget)
{
    if (containerWidget != NULL && contentsWidget != NULL) {
        if (containerWidget->layout() == NULL) {
            containerWidget->setLayout(new QVBoxLayout);
            containerWidget->layout()->setContentsMargins(0, 0, 0, 0);
        }
        contentsWidget->setParent(containerWidget);
        containerWidget->layout()->addWidget(contentsWidget);
    }
}

/*! Returns the global position of a widget corner
 *
 *  \returns Null point (ie. with coordinates (0, 0)) if \p widget is null
 */
QPoint QWidgetUtils::globalPos(const QWidget *widget, Qt::Corner widgetCorner)
{
    if (widget != NULL) {
        const QRect geom = widget->frameGeometry();
        switch (widgetCorner) {
        case Qt::TopLeftCorner:
            return widget->mapToGlobal(QPoint(-geom.width(), 0));
        case Qt::TopRightCorner:
            return widget->mapToGlobal(QPoint(geom.width(), 0));
        case Qt::BottomLeftCorner:
            return widget->mapToGlobal(QPoint(-geom.width(), geom.height()));
        case Qt::BottomRightCorner:
            return widget->mapToGlobal(QPoint(geom.width(), geom.height()));
        }
    }
    return QPoint(0, 0);
}

/*! Move position of \p widget so it's displayed stuck to the right of \p nextTo
 */
void QWidgetUtils::moveWidgetRightTo(QWidget* widget, const QWidget* nextTo)
{
    const QRect frameGeom = nextTo->frameGeometry();
    widget->move(nextTo->mapToGlobal(QPoint(frameGeom.width(), 0)));
}

/*! Move position of \p widget so it's displayed stuck to the left of \p nextTo
 */
void QWidgetUtils::moveWidgetLeftTo(QWidget* widget, const QWidget* nextTo)
{
    //const QRect nextToFrameGeom = nextTo->frameGeometry();
    const QRect widgetFrameGeom = widget->frameGeometry();
    widget->move(nextTo->mapToGlobal(QPoint(-widgetFrameGeom.width(), 0)));
}

/*! Current slide positions of the horizontal and vertical scroll bars
 */
QPair<int, int> QWidgetUtils::horizAndVertScrollValue(const QAbstractScrollArea* area)
{
    return qMakePair(area->horizontalScrollBar()->value(),
                     area->verticalScrollBar()->value());
}

/*! Set the current slide positions of the horizontal and vertical scroll bars
 *  to \p values
 */
void QWidgetUtils::setHorizAndVertScrollValue(
        QAbstractScrollArea* area, const QPair<int, int>& values)
{
    area->horizontalScrollBar()->setValue(values.first);
    area->verticalScrollBar()->setValue(values.second);
}

/*! Executes \p dialog asynchronously */
void QWidgetUtils::asyncDialogExec(QDialog *dialog)
{
    if (dialog != NULL) {
        dialog->setModal(true);
        QObject::connect(
                    dialog, &QDialog::finished,
                    dialog, &QObject::deleteLater,
                    Qt::UniqueConnection);
        dialog->show();
    }
}

/*! Executes \p menu asynchronously */
void QWidgetUtils::asyncMenuExec(QMenu *menu, const QPoint &pos)
{
    if (menu != NULL) {
        QObject::connect(menu, &QMenu::aboutToHide, menu, &QObject::deleteLater);
        menu->popup(pos);
    }
}

/*! Executes message box information asynchronously */
QMessageBox* QWidgetUtils::asyncMsgBoxInfo(
        QWidget* parent,
        const QString& title,
        const QString& text,
        QMessageBox::StandardButtons buttons)
{
    auto msgBox = new QMessageBox(QMessageBox::Information,
                                  title, text, buttons, parent);
    QWidgetUtils::asyncDialogExec(msgBox);
    return msgBox;
}

/*! Executes message box warning asynchronously */
QMessageBox* QWidgetUtils::asyncMsgBoxWarning(
        QWidget *parent,
        const QString &title,
        const QString &text,
        QMessageBox::StandardButtons buttons)
{
    auto msgBox = new QMessageBox(QMessageBox::Warning,
                                  title, text, buttons, parent);
    QWidgetUtils::asyncDialogExec(msgBox);
    return msgBox;
}

/*! Executes message box critical asynchronously */
QMessageBox* QWidgetUtils::asyncMsgBoxCritical(
        QWidget* parent,
        const QString& title,
        const QString& text,
        QMessageBox::StandardButtons buttons)
{
    auto msgBox = new QMessageBox(QMessageBox::Critical,
                                  title, text, buttons, parent);
    QWidgetUtils::asyncDialogExec(msgBox);
    return msgBox;
}

} // namespace qtgui
