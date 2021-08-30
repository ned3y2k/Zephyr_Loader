#include "pch.h"
#include "file.util.h"
#include <stdlib.h>

bool CompareExtensionName(LPCSTR lpFileName, char const* extName) {
    size_t extNameLen = strlen(extName);
    char* ext = (char*)calloc(extNameLen + 1, sizeof(char));

    LPCSTR source = lpFileName + (strlen(lpFileName) - extNameLen);
    memcpy_s(ext, extNameLen + 1, (char*)source, extNameLen);

    for (unsigned int i = 0; i < extNameLen; i++) {
        ext[i] = tolower(ext[i]);
    }

    bool isEqualExtName = _strcmpi(ext, extName) == 0;
    free(ext);

    return isEqualExtName;
}

bool Dirname(LPCSTR lpFileName, LPCSTR destination, size_t destinationSize) {
    int dirPos = 0;
    
    for (int i = strlen(lpFileName); i > 0; i--) {
        if (lpFileName[i] == '\\') {
            dirPos = i;
            break;
        }
    }

    if (dirPos == 0)
        return false;

    memcpy_s((char*)destination, destinationSize, lpFileName, dirPos);

    return true;
}