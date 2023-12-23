#ifndef STRINGS_H
#define STRINGS_H

#define MAX_TEMP_STRING_BUILDER_LENGTH 256
#define MAX_NUMBER_OF_SPLITS 256


//NOTE: The intended use for string_static is to be used
//      with GetLine() on loaded files to mark the first
//      char of the string along with the desired length.
//      It doesn't contain the data of the whole string.
struct string {
    char *Data;
    u32  Length;
} typedef string_inplace;

struct split_string_return {
    string_inplace *Data;
    u32 Length;
};

//NOTE: In C/C++, the string null termintator '\0' doesn't 
//      evaluate as a character. For example, the string "Hello"
//      and the string "Hello\0" both return the value '5' while
//      using the StringLength function.
//      The following code copies that behaviour.

inline u32 StringLength(char *A) {
    u32 Result = 0;
    while(*A++) Result++;
    return Result;
}

inline u32 LineLength(char *A) {
    u32 Result = 0;
    while(*A != '\n' || *A != '\0') {
        Result++;
        A++;
    }
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
    //The '+1' is for the manually added '\0'
    ReturnString->Data   = (char *)malloc((ReturnString->Length + 1) * 
                                          sizeof(char));
    for(int i = 0; i < ReturnString->Length; i++)
        ReturnString->Data[i] = TempString[i];
    ReturnString->Data[ReturnString->Length] = '\0';
    return ReturnString;
}

inline string_inplace* StringBuilderStatic(char *Input, u32 Length) {
    string_inplace *ReturnString = (string_inplace *)malloc(sizeof(string));
    ReturnString->Data = Input;
    ReturnString->Length = Length;
    return ReturnString;
}

inline string* ConcatenateString(char *InputA,
                                 char *InputB) {
    return StringBuilder("%s%s", InputA, InputB);
}

inline b32 CompareString(char *InputA, char *InputB) {
    u32 StringLengthA = StringLength(InputA);
    u32 StringLengthB = StringLength(InputB);
    if(StringLengthA != StringLengthB) return 0;
    for(int i = 0; i < StringLengthA; i++)
        if(InputA[i] != InputB[i]) return 0;
    return 1;
}

inline b32 CompareString(string *InputA,
                         string *InputB) {
    if(InputA->Length != InputB->Length) return 0;
    for(int i = 0; i < InputA->Length; i++)
        if(InputA->Data[i] != InputB->Data[i]) return 0;
    return 1;
}

inline b32 CompareString(char *InputA, int LengthA,
                         char *InputB, int LengthB) {
    if(LengthA != LengthB) return 0;
    for(int i = 0; i < LengthA; i++)
        if(InputA[i] != InputB[i]) return 0;
    return 1;
}

inline split_string_return* SplitStringInplace(string *Input, string *SplitTarget) {
    u32 MaxCount = Input->Length - SplitTarget->Length;
    u32 SplitPositions[MAX_NUMBER_OF_SPLITS] = {};
    if(MaxCount < 0) return {};
    
    //The intended behaviour is that if there are multiple
    //successive split targets (eg. "A_B__C"), that chunk
    //will be counted as only one split.
    //(It will split into "A", "B", "C".)
    //NOTE: Not sure about this and the results of it,
    //      could change in the future.
    
    u32 SplitCount = 0;
    for(int i = 0; i < MaxCount; i++)
        if(CompareString(&Input->Data[i], SplitTarget->Length,
                         SplitTarget->Data, SplitTarget->Length));
            SplitPositions[SplitCount++] = i;
    
    split_string_return *ReturnStrings =
        (split_string_return *)malloc(sizeof(split_string_return));

    
    return {};    
}

//The guiding principle for GetLine() implementation is that
//upon reading the file we wish to do our string operations
//in place, without additional allocations.

inline string_inplace GetLine(char *Input) {
    return {Input, LineLength(Input)};
}

#endif STRINGS_H