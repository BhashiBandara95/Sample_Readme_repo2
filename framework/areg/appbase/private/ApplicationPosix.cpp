/************************************************************************
* \file        areg/appbase/private/ApplicationWin.cpp
* \ingroup     AREG Asynchronous Event-Driven Communication Framework
* \author      Artak Avetyan (mailto:artak@aregtech.com)
* \brief       AREG Platform, Windows OS specific Application methods implementation
*              Windows apecifix API calls.
************************************************************************/
#include "areg/appbase/Application.hpp"

#ifdef _POSIX
#include <stdio.h>
#include <dirent.h>
#include <unistd.h>
#include "areg/base/File.hpp"
#include "areg/base/String.hpp"
#include "areg/base/Process.hpp"

static int _getProcIdByName(const char * procName)
{
    static const char * fmt = "/proc/%s/cmdline";
    int pid = -1;

    if (procName != NULL && *procName != '\0')
    {
        DIR * dir       = opendir("/proc");
        char * buffer   = DEBUG_NEW char[File::MAXIMUM_PATH + 1];
        if ((dir != NULL) && (buffer != NULL))
        {

            for (struct dirent * dirEntry = readdir(dir); (pid < 0) && (dirEntry != NULL); dirEntry = readdir(dir))
            {
                // skip non-numeric directories.
                if ((dirEntry->d_type == DT_REG) && (NEString::isNumeric<char>(dirEntry->d_name[0])))
                {
                    sprintf(buffer, fmt, dirEntry->d_name);
                    FILE * file = fopen(buffer, "r");
                    if (file != NULL)
                    {
                        if (fgets(buffer, File::MAXIMUM_PATH + 1, file) != NULL)
                        {
                            NEString::CharPos pos = NEString::findLastOf<char>(File::PATH_SEPARATOR, buffer);
                            if ( pos != NEString::InvalidPos )
                            {
                                char * name = buffer + pos + 1;
                                if (NEString::compareFastIgnoreCase<char, char>(procName, name) == 0)
                                {
                                    pid = NEString::makeInteger<char>(dirEntry->d_name, NULL);
                                }
                            }
                        }

                        fclose(file);
                    }

                }
            }
        }

        if (buffer != NULL)
            delete [] buffer;
    }

    return pid;
}

/**
* \brief   POSIX specific implementation of method.
**/
bool Application::_startRouterService( void )
{
    int pid = _getProcIdByName(NEApplication::DEFAULT_ROUTER_SERVICE_NAME);
    bool result = pid != -1;
    if (pid < 0)
    {
        String fileName = String::EmptyString;
        fileName += Process::getInstance().getPath();
        fileName += File::PATH_SEPARATOR;
        fileName += NEApplication::DEFAULT_ROUTER_SERVICE_NAME;
        const char * argv0 = "--console";
        result = execl(fileName.getString(), fileName.getString(), argv0, NULL_STRING) > 0;
    }

    return result;
}

#endif // _POSIX