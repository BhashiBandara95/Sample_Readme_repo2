//============================================================================
// Name        : test-areg-basic.cpp
// Author      : Artak Avetyan
// Version     :
// Copyright   : Aregtech � 2021
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "areg/base/RemoteMessage.hpp"
#include "areg/base/NEMemory.hpp"
#include "areg/component/NEService.hpp"
#include "areg/ipc/NEConnection.hpp"
#include "areg/component/StubAddress.hpp"

#include <iostream>

#ifdef WINDOWS
    #pragma comment(lib, "areg.lib")
#endif // WINDOWS

static void printBufferData(const NEMemory::sRemoteMessage & msg, const char * info);

void initTrims( char * trim1, char * trim2, char * trim3, char * trim4)
{
    *trim1 ++ = *trim2 ++ = ' ';
    *trim1 ++ = *trim2 ++ = ' ';
    *trim1 ++ = *trim2 ++ = *trim3 ++ = *trim4 ++ = '1';
    *trim1 ++ = *trim2 ++ = *trim3 ++ = *trim4 ++ = '2';
    *trim1 ++ = *trim2 ++ = *trim3 ++ = *trim4 ++ = '3';
    *trim1 ++ = *trim2 ++ = *trim3 ++ = *trim4 ++ = '4';
    *trim1 ++ = *trim2 ++ = *trim3 ++ = *trim4 ++ = '5';
    *trim1 ++ = *trim2 ++ = *trim3 ++ = *trim4 ++ = '6';
    *trim1 ++ = *trim2 ++ = *trim3 ++ = *trim4 ++ = '7';
    *trim1 ++ = *trim2 ++ = *trim3 ++ = *trim4 ++ = '8';
    *trim1 ++ = *trim2 ++ = *trim3 ++ = *trim4 ++ = '9';
    *trim1 ++ = *trim2 ++ = *trim3 ++ = *trim4 ++ = '0';
    *trim1 ++ = *trim3 ++ = ' ';
    *trim1 ++ = *trim3 ++ = ' ';
    *trim1 ++ = *trim2 ++ = *trim3 ++ = *trim4 ++ = '\0';
}

int main()
{
	std::cout << "!!!Hello World!!!" << std::endl; // prints !!!Hello World!!!

    char trim1[32]  = {0};
    char trim2[32]  = {0};
    char trim3[32]  = {0};
    char trim4[32]  = {0};
    char buf[32]    = {0};

    initTrims(trim1, trim2, trim3, trim4);
    NEString::trimLeft<char, char>(buf, 32, trim1, NEString::CountAll);
    NEString::trimLeft<char, char>(buf, 32, trim2, NEString::CountAll);
    NEString::trimLeft<char, char>(buf, 32, trim3, NEString::CountAll);
    NEString::trimLeft<char, char>(buf, 32, trim4, NEString::CountAll);

    NEString::trimLeft(trim1, NEString::CountAll);
    NEString::trimLeft(trim2, NEString::CountAll);
    NEString::trimLeft(trim3, NEString::CountAll);
    NEString::trimLeft(trim4, NEString::CountAll);

    initTrims(trim1, trim2, trim3, trim4);
    NEString::trimRight<char, char>(buf, 32, trim1, NEString::CountAll);
    NEString::trimRight<char, char>(buf, 32, trim2, NEString::CountAll);
    NEString::trimRight<char, char>(buf, 32, trim3, NEString::CountAll);
    NEString::trimRight<char, char>(buf, 32, trim4, NEString::CountAll);

    NEString::trimRight(trim1, NEString::CountAll);
    NEString::trimRight(trim2, NEString::CountAll);
    NEString::trimRight(trim3, NEString::CountAll);
    NEString::trimRight(trim4, NEString::CountAll);

    initTrims(trim1, trim2, trim3, trim4);
    NEString::trimAll<char, char>(buf, 32, trim1, NEString::CountAll);
    NEString::trimAll<char, char>(buf, 32, trim2, NEString::CountAll);
    NEString::trimAll<char, char>(buf, 32, trim3, NEString::CountAll);
    NEString::trimAll<char, char>(buf, 32, trim4, NEString::CountAll);

    NEString::trimAll(trim1, NEString::CountAll);
    NEString::trimAll(trim2, NEString::CountAll);
    NEString::trimAll(trim3, NEString::CountAll);
    NEString::trimAll(trim4, NEString::CountAll);

//////////////////////////////////////////////////////////////

    String str1, str2, str3, str4, str;

    initTrims(trim1, trim2, trim3, trim4);
    str1 = trim1;
    str2 = trim2;
    str3 = trim3;
    str4 = trim4;
   
    str1.trimLeft(str);
    str2.trimLeft(str);
    str3.trimLeft(str);
    str4.trimLeft(str);

    str1.trimLeft();
    str2.trimLeft();
    str3.trimLeft();
    str4.trimLeft();

    initTrims(trim1, trim2, trim3, trim4);
    str1 = trim1;
    str2 = trim2;
    str3 = trim3;
    str4 = trim4;

    str1.trimRight(str);
    str2.trimRight(str);
    str3.trimRight(str);
    str4.trimRight(str);

    str1.trimRight();
    str2.trimRight();
    str3.trimRight();
    str4.trimRight();

    initTrims(trim1, trim2, trim3, trim4);
    str1 = trim1;
    str2 = trim2;
    str3 = trim3;
    str4 = trim4;

    str1.trimAll(str);
    str2.trimAll(str);
    str3.trimAll(str);
    str4.trimAll(str);

    str1.trimAll();
    str2.trimAll();
    str3.trimAll();
    str4.trimAll();

    int pos = 0;

    initTrims(trim1, trim2, trim3, trim4);
    const char * next = trim1;
    pos = NEString::findFirstOf(' ', trim1, NEString::StartPos, &next);
    while (pos != NEString::InvalidPos)
    {
        pos = NEString::findFirstOf(' ', trim1, pos + 1, &next);
    }

    next = trim1;
    pos  = NEString::findFirstOf("  ", trim1, NEString::StartPos, &next);
    while (pos != NEString::InvalidPos)
    {
        pos = NEString::findFirstOf("  ", trim1, pos + 1, &next);
    }

    const char * prev = trim1;
    pos = NEString::findLastOf(' ', trim1, NEString::EndPos, &prev);
    while (pos != NEString::InvalidPos)
    {
        pos = NEString::findLastOf(' ', trim1, pos - 1, &prev);
    }

    prev = trim1;
    pos = NEString::findLastOf("  ", trim1, NEString::EndPos, &prev);
    while (pos != NEString::InvalidPos)
    {
        pos = NEString::findLastOf("  ", trim1, pos - 1, &prev);
    }

    if (sizeof(void *) == 4)
    {
        OUTPUT_DBG("32-bit system......");
    }
    else if (sizeof(void *) == 8)
    {
        OUTPUT_DBG("64-bit system......");
    }
    OUTPUT_DBG("sizeof(NEMemory::sBuferHeader) = %d;", sizeof(NEMemory::sBuferHeader));

    printBufferData(NEConnection::MessageHelloServer, "NEConnection::MessageHelloServer");


	return 0;
}

static void printBufferData(const NEMemory::sRemoteMessage & msg, const char * info)
{
    NEService::eServiceRequestType reqType = NEService::SERVICE_REQUEST_REGISTER_STUB;
    StubAddress stub("TestServiceName", Version(1, 1, 1), NEService::ServiceRemote, "TestRoleName", "TestThreadName");

    OUTPUT_DBG("Print ORIGIN of %s = {", info);
    OUTPUT_DBG("    {");
    OUTPUT_DBG("        {");
    OUTPUT_DBG("            , msg.rbHeader.rbhBufHeader.biBufSize = %d", msg.rbHeader.rbhBufHeader.biBufSize);
    OUTPUT_DBG("            , msg.rbHeader.rbhBufHeader.biLength  = %d", msg.rbHeader.rbhBufHeader.biLength);
    OUTPUT_DBG("            , msg.rbHeader.rbhBufHeader.biOffset  = %d", msg.rbHeader.rbhBufHeader.biOffset);
    OUTPUT_DBG("            , msg.rbHeader.rbhBufHeader.biBufType = %s", NEMemory::getString(static_cast<NEMemory::eBufferType>(msg.rbHeader.rbhBufHeader.biBufType)));
    OUTPUT_DBG("            , msg.rbHeader.rbhBufHeader.biUsed    = %d", msg.rbHeader.rbhBufHeader.biUsed);
    OUTPUT_DBG("            , msg.rbHeader.rbhBufHeader.biRefCount= %d", msg.rbHeader.rbhBufHeader.biRefCount);
    OUTPUT_DBG("        }");
    OUTPUT_DBG("        , msg.rbHeader.rbhTarget      = %d", msg.rbHeader.rbhTarget);
    OUTPUT_DBG("        , msg.rbHeader.rbhChecksum    = %d", msg.rbHeader.rbhChecksum);
    OUTPUT_DBG("        , - msg.rbHeader.rbhSource    = %d", msg.rbHeader.rbhSource);
    OUTPUT_DBG("        , msg.rbHeader.rbhMessageId   = %d", msg.rbHeader.rbhMessageId);
    OUTPUT_DBG("        , msg.rbHeader.rbhResult      = %s", NEMemory::getString( static_cast<NEMemory::eMessageResult>(msg.rbHeader.rbhResult)));
    OUTPUT_DBG("        , - msg.rbHeader.rbhSequenceNr= %d", msg.rbHeader.rbhSequenceNr);
    OUTPUT_DBG("    }");
    OUTPUT_DBG("    , msg.rbData[0]   = %d", static_cast<int>(msg.rbData[0]));
    OUTPUT_DBG("};");
    OUTPUT_DBG("ServiceRequestType  = %s;", NEService::getString(reqType));
    OUTPUT_DBG("StubAddress         = %s;", StubAddress::convAddressToPath(stub).getString());

    RemoteMessage remote;
    remote.initMessage(msg.rbHeader);
    remote.setSource( NEService::SOURCE_UNKNOWN );
    remote.setSequenceNr( NEService::SEQUENCE_NUMBER_NOTIFY );    
    remote << reqType;
    remote << stub;
    
    remote.bufferCompletionFix();
    remote.moveToBegin();

    const NEMemory::sRemoteMessage & rmt = remote.getRemoteMessage();
    NEService::eServiceRequestType cpyReqType;
    StubAddress cpyStub;
    remote >> cpyReqType;
    remote >> cpyStub;

    const unsigned int offset   = MACRO_OFFSETOF(NEMemory::sRemoteMessageHeader, rbhSource);
    const unsigned char * data  = reinterpret_cast<const unsigned char *>(&rmt.rbHeader.rbhSource);
    const unsigned int remain   = rmt.rbHeader.rbhBufHeader.biOffset - offset;
    const unsigned int used     = rmt.rbHeader.rbhBufHeader.biUsed;
    const unsigned int correct  = MACRO_MAX(used, sizeof(NEMemory::BufferData));
    const unsigned int length   = used + remain;

    OUTPUT_DBG("-------------------------------------------------------");
    OUTPUT_DBG("Print COPY of %s = {", info);
    OUTPUT_DBG("    {");
    OUTPUT_DBG("        {");
    OUTPUT_DBG("            , rmt.rbHeader.rbhBufHeader.biBufSize = %d", rmt.rbHeader.rbhBufHeader.biBufSize);
    OUTPUT_DBG("            , rmt.rbHeader.rbhBufHeader.biLength  = %d", rmt.rbHeader.rbhBufHeader.biLength);
    OUTPUT_DBG("            , rmt.rbHeader.rbhBufHeader.biOffset  = %d", rmt.rbHeader.rbhBufHeader.biOffset);
    OUTPUT_DBG("            , rmt.rbHeader.rbhBufHeader.biBufType = %s", NEMemory::getString(static_cast<NEMemory::eBufferType>(rmt.rbHeader.rbhBufHeader.biBufType)));
    OUTPUT_DBG("            , rmt.rbHeader.rbhBufHeader.biUsed    = %d", rmt.rbHeader.rbhBufHeader.biUsed);
    OUTPUT_DBG("            , rmt.rbHeader.rbhBufHeader.biRefCount= %d", rmt.rbHeader.rbhBufHeader.biRefCount);
    OUTPUT_DBG("        }");
    OUTPUT_DBG("        , rmt.rbHeader.rbhTarget      = %d", rmt.rbHeader.rbhTarget);
    OUTPUT_DBG("        , rmt.rbHeader.rbhChecksum    = %d", rmt.rbHeader.rbhChecksum);
    OUTPUT_DBG("        , + rmt.rbHeader.rbhSource    = %d", rmt.rbHeader.rbhSource);
    OUTPUT_DBG("        , rmt.rbHeader.rbhMessageId   = %d", rmt.rbHeader.rbhMessageId);
    OUTPUT_DBG("        , rmt.rbHeader.rbhResult      = %s", NEMemory::getString( static_cast<NEMemory::eMessageResult>(rmt.rbHeader.rbhResult)));
    OUTPUT_DBG("        , + rmt.rbHeader.rbhSequenceNr= %d", rmt.rbHeader.rbhSequenceNr);
    OUTPUT_DBG("    }");
    OUTPUT_DBG("    , rmt.rbData[0]   = %d", static_cast<int>(rmt.rbData[0]));
    OUTPUT_DBG("};");
    OUTPUT_DBG("ServiceRequestType  = %s;", NEService::getString(cpyReqType));
    OUTPUT_DBG("StubAddress         = %s;", StubAddress::convAddressToPath(cpyStub).getString());
    OUTPUT_DBG("-------------------------------------------------------");
    OUTPUT_DBG("CHECKSUM data:");
    OUTPUT_DBG("checksum offset = %d;", offset);
    //OUTPUT_DBG("checksum data   = %p;", data);
    OUTPUT_DBG("checksum remain = %d;", remain);
    OUTPUT_DBG("checksum used   = %d;", used);
    OUTPUT_DBG("checksum correct= %d;", correct);
    OUTPUT_DBG("checksum length = %d;", length);
    OUTPUT_MSG("checksum data       = {");
    String comma = "";
    unsigned int i = 0;
    for (i = 0; i < length; ++ i)
    {
        OUTPUT_MSG("%s0x%02x", comma.getString(), static_cast<unsigned int>(data[i]));
        comma = ", ";
    }
    OUTPUT_MSG("};\n");

    OUTPUT_DBG("-------------------------------------------------------");
    const unsigned char * buffer = remote.getBuffer();
    unsigned int len = remote.getSizeUsed();

    const unsigned char * rbhSource     = reinterpret_cast<const unsigned char *>(&rmt.rbHeader.rbhSource);
    const unsigned char * rbhMessageId  = reinterpret_cast<const unsigned char *>(&rmt.rbHeader.rbhMessageId);
    const unsigned char * rbhResult     = reinterpret_cast<const unsigned char *>(&rmt.rbHeader.rbhResult);
    const unsigned char * rbhSequenceNr = reinterpret_cast<const unsigned char *>(&rmt.rbHeader.rbhSequenceNr);

    OUTPUT_MSG("        rbhSource   = {0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x};\n", rbhSource[0], rbhSource[1], rbhSource[2], rbhSource[3], rbhSource[4], rbhSource[5], rbhSource[6], rbhSource[7]);
    OUTPUT_MSG("        rbhMessageId= {                                                0x%02x, 0x%02x, 0x%02x, 0x%02x};\n", rbhMessageId[0], rbhMessageId[1], rbhMessageId[2], rbhMessageId[3]);
    OUTPUT_MSG("        rbhResult   = {                                                                        0x%02x, 0x%02x, 0x%02x, 0x%02x};\n", rbhResult[0], rbhResult[1], rbhResult[2], rbhResult[3]);
    OUTPUT_MSG("        rbhSequence = {                                                                                                0x%02x, 0x%02x, 0x%02x, 0x%02x};\n", rbhSequenceNr[0], rbhSequenceNr[1], rbhSequenceNr[2], rbhSequenceNr[3]);
    OUTPUT_MSG("         buffer     = {");
    
    comma = "";
    for (i = 0; i < remain; ++ i)
    {
        OUTPUT_MSG("%s    ", comma.getString());
        comma = "  ";
    }

    comma = "  ";
    for (unsigned int i = 0; i < len; ++ i)
    {
        OUTPUT_MSG("%s0x%02x", comma.getString(), static_cast<unsigned int>(buffer[i]));
        comma = ", ";
    }
    OUTPUT_MSG("};\n");

    OUTPUT_DBG("-------------------------------------------------------");
    OUTPUT_DBG("*******************************************************");

}
