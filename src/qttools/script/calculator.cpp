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

#include "calculator.h"

namespace qttools {

/*! \class Calculator
 *  \brief Provides evaluation of expressions
 *
 */

Calculator::Calculator() :
  _lastResult(0)
{
  _scriptEngine.evaluate(
        "var PI = Math.PI\n"
        "var RAD_TO_DEG = 180.0/PI\n"
        "var DEG_TO_RAD = PI/180.0\n"
        "function round_int(x)\n"
        "{ \n"
        "  if (Math.abs(Math.round(x)-x) < 1.0E-5)\n"
        "  {\n"
        "    return Math.round(x);"
        "  }\n"
        "  else\n"
        "  {\n"
        "    return x;"
        "  }\n"
        "}\n"
        "var abs    = new Function(\"x\", \"return round_int(Math.abs(x))\")\n"
        "var acos   = new Function(\"x\", \"return round_int(Math.acos(x))\")\n"
        "var asin   = new Function(\"x\", \"return round_int(Math.asin(x))\")\n"
        "var atan   = new Function(\"x\", \"return round_int(Math.atan(x))\")\n"
        "var atan2  = new Function(\"y\", \"x\", \"return round_int(Math.atan2(y, x))\")\n"
        "var ceil   = new Function(\"x\", \"return round_int(Math.ceil(x))\")\n"
        "var cos    = new Function(\"x\", \"return round_int(Math.cos(x))\")\n"
        "var exp    = new Function(\"x\", \"return round_int(Math.exp(x))\")\n"
        "var floor  = new Function(\"x\", \"return round_int(Math.floor(x))\")\n"
        "var log    = new Function(\"x\", \"return round_int(Math.log(x))\")\n"
        "var pow    = new Function(\"x\", \"y\", \"return round_int(Math.pow(x, y))\")\n"
        "var random = new Function(\"return Math.random()\")\n"
        "var round  = new Function(\"x\", \"return Math.round(x)\")\n"
        "var sin    = new Function(\"x\", \"return round_int(Math.sin(x))\")\n"
        "var sqrt   = new Function(\"x\", \"return round_int(Math.sqrt(x))\")\n"
        "var tan    = new Function(\"x\", \"return round_int(Math.tan(x))\")\n"
        );
}

void Calculator::evaluate(const QString& program)
{
  _lastResult = _scriptEngine.evaluate(program);
}

bool Calculator::hasResult() const
{
  return
      !_scriptEngine.hasUncaughtException() &&
      (_lastResult.isNumber() || _lastResult.isString());
}

QString Calculator::lastErrorText() const
{
  if (_scriptEngine.hasUncaughtException())
    return _scriptEngine.uncaughtException().toString();
  else if (!(_lastResult.isNumber() || _lastResult.isString()))
    return QString("?");
  return QString();
}

double Calculator::lastResult() const
{
  return _lastResult.toNumber();
}

QString Calculator::lastResultText() const
{
  return _lastResult.toString();
}

} // namespace qttools