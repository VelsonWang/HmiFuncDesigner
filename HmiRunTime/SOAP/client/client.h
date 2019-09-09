#ifndef QMLCLIENT_H
#define QMLCLIENT_H

// gSoap lib import
#include "../public/soapStub.h"
#include "../public/soapH.h"

// Qt lib import
#include <QtCore>

bool sendDatStream(const char *server, std::string szSendMsg, std::string &szRetMsg);

#endif // QMLCLIENT_H
