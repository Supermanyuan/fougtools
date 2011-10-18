/****************************************************************************
**
**  FougTools
**  Copyright FougSys (1 Mar. 2011)
**  contact@fougsys.fr
**
** This software is a computer program whose purpose is to provide utility
** tools for the C++ language, the Qt and Open Cascade toolkits.
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

#ifndef OCC_AIS_TEXT_H
#define OCC_AIS_TEXT_H

#include "occtools/occtools.h"
#include <AIS_InteractiveObject.hxx>
#include <gp_Pnt.hxx>
#include <Handle_Graphic2d_GraphicObject.hxx>
#include <Handle_Prs3d_Presentation.hxx>
#include <Handle_Prs3d_Projector.hxx>
#include <Handle_PrsMgr_PresentationManager2d.hxx>
#include <Handle_PrsMgr_PresentationManager3d.hxx>
#include <Handle_SelectMgr_Selection.hxx>
#include <QtCore/QString>
#include <QtGui/QColor>
#include <QtGui/QFont>
#include <Standard_DefineHandle.hxx>
#include <Standard_Macro.hxx>

DEFINE_STANDARD_HANDLE(AIS_Text, AIS_InteractiveObject)

class OCCTOOLS_EXPORT AIS_Text : public AIS_InteractiveObject
{
public:
  enum TextDisplayMode
  {
    TextOnlyDisplay,
    BackgroundDisplay,
    Style3dDisplay,
    XorTextDisplay
  };

  enum TextStyle
  {
    NormalStyle,
    AnnotationStyle
  };

  AIS_Text();
  AIS_Text(const QString& text, const gp_Pnt& pos);
  virtual ~AIS_Text();

//  DEFINE_STANDARD_RTTI(AIS_Text)

  QColor color(unsigned i = 0) const;
  void setColor(const QColor& c, unsigned i = 0);
  void setDefaultColor(const QColor& c);

  QFont font(unsigned i = 0) const;
  void setFont(const QFont& f, unsigned i = 0);
  void setDefaultFont(const QFont& f);

  const gp_Pnt& position(unsigned i = 0) const;
  void setPosition(const gp_Pnt& pos, unsigned i = 0);

  QString text(unsigned i = 0) const;
  void setText(const QString& v, unsigned i = 0);
  bool isValidTextIndex(unsigned i) const;

  void setTextBackgroundColor(const QColor& color, unsigned i = 0);
  void setDefaultTextBackgroundColor(const QColor& c);

  void setTextDisplayMode(TextDisplayMode mode, unsigned i = 0);
  void setDefaultTextDisplayMode(TextDisplayMode mode);

  void setTextStyle(TextStyle style, unsigned i = 0);
  void setDefaultTextStyle(TextStyle style);

  unsigned textsCount() const;
  void addText(const QString& text, const gp_Pnt& pos);

  // --- Implementation
protected:
  void Compute(const Handle_PrsMgr_PresentationManager3d& pm,
               const Handle_Prs3d_Presentation& pres,
               const Standard_Integer mode);
  void Compute(const Handle_Prs3d_Projector& proj,
               const Handle_Prs3d_Presentation& pres);
  void Compute(const Handle_PrsMgr_PresentationManager2d& pres,
               const Handle_Graphic2d_GraphicObject& grObj,
               const Standard_Integer mode);
  void ComputeSelection(const Handle_SelectMgr_Selection& sel,
                        const Standard_Integer mode);

private:
  class AIS_TextPrivate* _d;
};

#endif // AIS_TEXT_H