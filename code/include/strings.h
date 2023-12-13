#ifndef STRINGS_H
#define STRINGS_H

#define MAX_TEMP_STRING_BUILDER_LENGTH 256

struct string {
    char *Data;
    u64  Length;
};

//NOTE: In C/C++, the string null termintator '\0' doesn't 
//      evaluate as a charater. For example, the string "Hello"
//      and the string "Hello\0" both return the value '5' while
//      using the StringLength function.
//      The following code copies that behaviour.

inline int StringLength(char *A) {
    int Result = 0;
    while(*A++) Result++;
    return Result;
}

inline string* StringBuilder(char *Input, ...) {
    char TempString[MAX_TEMP_STRING_BUILDER_LENGTH];
    va_list VariableList;
    va_start(VariableList, Input);
    vsprintf_s(TempString,
               MAX_TEMP_STRING_BUILDER_LENGTH,
               Input, VariableList);
    va_end(VariableList);
    string *ReturnString = (string *)malloc(sizeof(string));
    ReturnString->Length = StringLength(TempString);
    ReturnString->Data   = (char *)malloc(ReturnString->Length * 
                                          sizeof(char));
    for(int i = 0; i < ReturnString->Length; i++)
        ReturnString->Data[i] = TempString[i];
    return ReturnString;
}

inline void ConcatenateString(char *Dest,
                              char *InputA, int InputALength,
                              char *InputB, int InputBLength) {
    for(int i = 0; i < InputALength; ++i) *Dest++ = *InputA++;
    for(int i = 0; i < InputBLength; ++i) *Dest++ = *InputB++;
    *Dest++ = 0;
}

inline string* ConcatenateString(string InputA,
                                 string InputB) {
    string *Dest = (string *)malloc(sizeof(string));
    Dest->Length = InputA.Length + InputB.Length;
    Dest->Data = (char *)malloc(sizeof(Dest->Length) * sizeof(char));
    for(int i = 0; i < InputA.Length; ++i)
        Dest->Data = InputA.Data++;
    for(int i = 0; i < InputB.Length; ++i)
        Dest->Data = InputB.Data++;
    
    return Dest;
}

inline b32 CompareString(char *InputA, char *InputB) {
    b32 Result = 1;
    u32 StringLengthA = StringLength(InputA);
    u32 StringLengthB = StringLength(InputB);
    if(StringLengthA != StringLengthB) return 0;
    for(int i = 0; i < StringLengthA; i++)
        if(InputA[i] != InputB[i]) return 0;
    return 1;
}

inline b32 CompareString(char *InputA, int StringLengthA,
                         char *InputB, int StringLengthB) {
    b32 Result = 1;
    if(StringLengthA != StringLengthB) return 0;
    for(int i = 0; i < StringLengthA; i++)
        if(InputA[i] != InputB[i]) return 0;
    return 1;
}

#endif STRINGS_H