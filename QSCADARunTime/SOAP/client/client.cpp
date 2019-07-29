#include "client.h"

#include "../public/soap.nsmap"

bool sendDatStream(const char *server, std::string szSendMsg, std::string &szRetMsg)
{
    soap add_soap;
    soap_init(&add_soap);

    soap_call_dataStream(&add_soap, server, "", szSendMsg, szRetMsg);

    if(add_soap.error)
    {
        qDebug("soap error:%d,%s,%s", add_soap.error, *soap_faultcode(&add_soap), *soap_faultstring(&add_soap) );
        soap_end(&add_soap);
        return false;
    }

    soap_end(&add_soap);
    return true;
}
