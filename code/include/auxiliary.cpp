#ifndef AUXILIARY_CPP
#define AUXILIARY_CPP

#include <platform.h>



inline char* SkipToBlankSpace(char *Input) {
    while(*Input != ' ' && *Input != '\n' && *Input != '\0') Input++;
    return Input;
}

inline char* SkipToEndOfLine(char *Input) {
    while(*Input != '\n' && *Input != '\0') Input++;
    return Input;
}

// inline char* GenerateFileNameString(char *A, char *B) {
//     char *Result = (char *) malloc(sizeof(char) * (StringLength(A) + StringLength(B)));
//     ConcatenateString(Result, A, StringLength(A), B, StringLength(B));
//     return Result;
// }

void Win32WriteToLogFile() {
    HANDLE LogFileHandle =  CreateFileA("log.txt",
                                        FILE_APPEND_DATA ,
                                        FILE_SHARE_WRITE,
                                        0,
                                        OPEN_ALWAYS,
                                        0, 0);
    if(LogFileHandle != INVALID_HANDLE_VALUE) {
        WriteFile(LogFileHandle,
                  DebugInfo.GlobalLogBuffer,
                  StringLength(DebugInfo.GlobalLogBuffer),
                  0, 0);
    }
}

void Win32WriteToLogFile(char const *Input, ...) {
    HANDLE LogFileHandle = CreateFileA("log.txt",
                                        FILE_APPEND_DATA ,
                                        FILE_SHARE_WRITE,
                                        0,
                                        OPEN_ALWAYS,
                                        0, 0);
    if(LogFileHandle != INVALID_HANDLE_VALUE) {
        va_list VariableList;
        va_start(VariableList, Input);
        vsprintf_s(DebugInfo.GlobalLogBuffer,
                  MAX_LOGBUFFER_STRING_LENGTH,
                  Input, VariableList);
        va_end(VariableList);
        WriteFile(LogFileHandle,
                  DebugInfo.GlobalLogBuffer,
                  StringLength(DebugInfo.GlobalLogBuffer),
                  0, 0);
    }    
}

void Win32OutputToDebugConsole(char *Input, ...) {
    va_list VariableList;
    va_start(VariableList, Input);
    vsprintf_s(DebugInfo.ConsoleDebugBuffer,
               MAX_LOGBUFFER_STRING_LENGTH,
               Input, VariableList);
    va_end(VariableList);
    OutputDebugStringA((LPCSTR)DebugInfo.ConsoleDebugBuffer);
}

void* Win32ReadEntireFile(char *InputFileName) {
    void *Result;
    HANDLE FileHandle = CreateFileA(InputFileName,
                                    GENERIC_READ,
                                    FILE_SHARE_READ,
                                    0,
                                    OPEN_EXISTING,
                                    0, 0);
    if(FileHandle != INVALID_HANDLE_VALUE) {
        LARGE_INTEGER FileSize;
        GetFileSizeEx(FileHandle, &FileSize);
        u32 FileSize32 = FileSize.QuadPart;
        Result = malloc(FileSize32);
        DWORD BytesRead;
        if(!ReadFile(FileHandle, Result, FileSize32, &BytesRead, 0))
            Win32WriteToLogFile("ReadFile in function Win32ReadEntireFile failed.\n");
        return Result;
    }
    else {
        Win32WriteToLogFile("CreateFileA in function Win32ReadEntireFile returned INVALID_HANDLE_VALUE while reading file %s.\n", InputFileName);
        return 0;
    }
}

#endif AUXILIARY_CPP
