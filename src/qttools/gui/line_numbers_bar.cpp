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

#include "line_numbers_bar.h"

#include <QtCore/QRect>
#include <QtGui/QAbstractTextDocumentLayout>
#include <QtGui/QPainter>
#include <QtGui/QPixmap>
#include <QtGui/QTextBlock>
#include <QtGui/QTextDocument>
// QtWidgets
#include <QScrollBar>
#include <QTextEdit>
#include <QToolTip>

namespace qtgui {

/*! \class LineNumbersBar::Private
 *  \brief Internal (pimpl of LineNumbersBar)
 */
class LineNumbersBar::Private
{
public:
    Private()
        : m_edit(NULL),
          m_stopLine(-1),
          m_currentLine(-1),
          m_bugLine(-1)
    {
    }

    QTextEdit* m_edit;
    QRect m_stopRect;
    QRect m_currentRect;
    QRect m_bugRect;
    int m_stopLine;
    int m_currentLine;
    int m_bugLine;
    QPixmap m_stopMarker;
    QPixmap m_currentMarker;
    QPixmap m_bugMarker;
};

/*!
 * \class LineNumbersBar
 * \brief Provides numbering of the lines of a QTextEdit as a vertical bar
 *
 * \headerfile line_numbers_bar.h <qttools/gui/line_numbers_bar.h>
 * \ingroup qttools_gui
 */

LineNumbersBar::LineNumbersBar(QWidget *parent)
    : QWidget(parent),
      d(new Private)
{
    // Make room for 4 digits and the breakpoint icon
    this->setFixedWidth(this->fontMetrics().width(QLatin1String("000") + 10 + 32));
    //  d->m_stopMarker = QPixmap( "images/no.png" );
    //   currentMarker = QPixmap( "images/next.png" );
    //   bugMarker = QPixmap( "images/bug.png" );
}

LineNumbersBar::~LineNumbersBar()
{
    delete d;
}

void LineNumbersBar::setCurrentLine(int lineno)
{
    d->m_currentLine = lineno;
}

void LineNumbersBar::setStopLine(int lineno)
{
    d->m_stopLine = lineno;
}

void LineNumbersBar::setBugLine(int lineno)
{
    d->m_bugLine = lineno;
}

void LineNumbersBar::setTextEdit(QTextEdit* edit)
{
    auto docLayout = edit->document()->documentLayout();
    if (d->m_edit != NULL) {
        QObject::disconnect(
                    docLayout, &QAbstractTextDocumentLayout::update,
                    this, &LineNumbersBar::updateWidget);
        QObject::disconnect(
                    edit->verticalScrollBar(), &QScrollBar::valueChanged,
                    this, &LineNumbersBar::updateWidget);
    }
    d->m_edit = edit;
    if (edit != NULL) {
        QObject::connect(
                    docLayout, &QAbstractTextDocumentLayout::update,
                    this, &LineNumbersBar::updateWidget);
        QObject::connect(
                    edit->verticalScrollBar(), &QScrollBar::valueChanged,
                    this, &LineNumbersBar::updateWidget);
    }
}

void LineNumbersBar::paintEvent(QPaintEvent* event)
{
    if (d->m_edit == NULL) {
        QWidget::paintEvent(event);
        return;
    }

    auto layout = d->m_edit->document()->documentLayout();
    const int contentsY = d->m_edit->verticalScrollBar()->value();
    const qreal pageBottom = contentsY + d->m_edit->viewport()->height();
    const QFontMetrics fm = this->fontMetrics();
    const int ascent = this->fontMetrics().ascent() + 1; // height = ascent + descent + 1
    int lineCount = 1;
    QPainter p(this);

    d->m_bugRect = QRect();
    d->m_stopRect = QRect();
    d->m_currentRect = QRect();

    for (QTextBlock block = d->m_edit->document()->begin();
         block.isValid();
         block = block.next(), ++lineCount)
    {
        const QRectF boundingRect(layout->blockBoundingRect(block));
        QPointF position(boundingRect.topLeft());

        if (position.y() + boundingRect.height() < contentsY)
            continue;

        if (position.y() > pageBottom)
            break;

        const QString txt(QString::number(lineCount));
        p.drawText(this->width() - fm.width(txt), qRound(position.y() ) - contentsY + ascent, txt);

        // Bug marker
        if (d->m_bugLine == lineCount) {
            p.drawPixmap(1, qRound(position.y() ) - contentsY, d->m_bugMarker);
            d->m_bugRect = QRect(1, qRound(position.y()) - contentsY,
                                 d->m_bugMarker.width(),
                                 d->m_bugMarker.height());
        }

        // Stop marker
        if (d->m_stopLine == lineCount) {
            p.drawPixmap(19, qRound(position.y()) - contentsY, d->m_stopMarker);
            d->m_stopRect = QRect(19, qRound(position.y()) - contentsY,
                                  d->m_stopMarker.width(),
                                  d->m_stopMarker.height());
        }

        // Current line marker
        if (d->m_currentLine == lineCount) {
            p.drawPixmap(19, qRound(position.y()) - contentsY, d->m_currentMarker);
            d->m_currentRect = QRect(19, qRound(position.y()) - contentsY,
                                     d->m_currentMarker.width(),
                                     d->m_currentMarker.height());
        }
    }
}

bool LineNumbersBar::event(QEvent* event)
{
    if (event->type() == QEvent::ToolTip) {
        QHelpEvent *helpEvent = static_cast<QHelpEvent *>(event);
        if (d->m_stopRect.contains(helpEvent->pos()))
            QToolTip::showText(helpEvent->globalPos(), tr("Stop Here"));
        else if (d->m_currentRect.contains(helpEvent->pos()))
            QToolTip::showText(helpEvent->globalPos(), tr("Current Line"));
        else if (d->m_bugRect.contains(helpEvent->pos()))
            QToolTip::showText(helpEvent->globalPos(), tr("Error Line" ));
    }
    return QWidget::event(event);
}

void LineNumbersBar::updateWidget()
{
    QWidget::update();
}

} // namespace qtgui
