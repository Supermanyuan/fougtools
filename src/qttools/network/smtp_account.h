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

#ifndef QTTOOLS_SMTP_ACCOUNT_H
#define QTTOOLS_SMTP_ACCOUNT_H

#include "qttools/network/network.h"
#include <QtCore/QString>

namespace qttools {

class QTTOOLS_NETWORK_EXPORT SmtpAccount
{
public:
  enum AuthenticationMethod
  {
    NoAuthentication, // Anonymous
    LoginAuthentication,
    PlainAuthentication,
    CramMd5Authentication
  };

  enum ConnectionSecurity
  {
    NoSecurity,
    StartTlsSecurity,
    SslTlsSecurity
  };

  SmtpAccount();

  QString host() const;
  void setHost(const QString& name);

  int port() const;
  void setPort(int p);

  AuthenticationMethod authenticationMethod() const;
  void setAuthenticationMethod(AuthenticationMethod method);

  ConnectionSecurity connectionSecurity() const;
  void setConnectionSecurity(ConnectionSecurity connSecurity);

  QString userName() const;
  void setUserName(const QString& uname);

  QString password() const;
  void setPassword(const QString& pwd);

  static AuthenticationMethod toAuthenticationMethod(int id, bool* ok = 0);
  static ConnectionSecurity toConnectionSecurity(int id, bool* ok = 0);

private:
  QString _host;
  int _port;
  AuthenticationMethod _authMethod;
  ConnectionSecurity _connSecurity;
  QString _userName;
  QString _password;
};

} // namespace qttools

#endif // QTTOOLS_SMTP_ACCOUNT_H