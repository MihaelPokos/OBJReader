#ifndef STRINGS_H
#define STRINGS_H

#define MAX_TEMP_STRING_BUILDER_LENGTH 256
#define MAX_NUMBER_OF_SPLITS 256

struct string {
    char *Data;
    u32  Length;
} typedef string_inplace;

struct split_string_return {
    string_inplace *Data;
    u32 Length;
};

struct split_string_return_dummy {
    string_inplace Data[MAX_NUMBER_OF_SPLITS + 1];
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
    while(*A != '\n' && *A != '\0') {
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

/*
inline string_inplace* StringBuilderStatic(char *Input, u32 Length) {
    string_inplace *ReturnString = (string_inplace *)malloc(sizeof(string));
    ReturnString->Data = Input;
    ReturnString->Length = Length;
    return ReturnString;
}
*/


//There exists a C operator '##' for combining two strings!
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
    int MaxCount = Input->Length - SplitTarget->Length;
    if(MaxCount < 0) return {};
    
    //TODO: The future behaviour should be that if there are multiple
    //successive split targets (eg. "A_B__C"), that chunk
    //should be counted as only one split.
    //(It should split into "A", "B", "C".)
    //This should be implemented along with memory arenas.
    
    //TODO: An interesting fail case is the string "......" with a split of "..".
    //With a single "." split, it returns an empty array, but with a double it doesn't!
    
    int SplitPositions[MAX_NUMBER_OF_SPLITS] = {};
    SplitPositions[0] = 0;
    int SplitCount = 1;
    
    for(int i = 0; i < MaxCount; i++) {
        if(CompareString(&Input->Data[i], SplitTarget->Length,
                         SplitTarget->Data, SplitTarget->Length))
            SplitPositions[SplitCount++] = i + SplitTarget->Length;
    }
    SplitPositions[SplitCount++] = (Input->Length + 1);
    
    split_string_return_dummy ReturnStrings;
    int MaxSplitCount = SplitCount - 1;
    for(int i = 0; i < MaxSplitCount; i++) {
        int TargetLength = (SplitPositions[i + 1] - SplitPositions[i]) - 1;
        ReturnStrings.Data[i].Data   = &Input->Data[SplitPositions[i]];
        ReturnStrings.Data[i].Length = TargetLength;
    }
    ReturnStrings.Data[MaxSplitCount] = {};
    ReturnStrings.Length = SplitCount - 1;
    
    for(int i = 0; i < ReturnStrings.Length; i++) {
        if(ReturnStrings.Data[i].Length == (SplitTarget->Length - 1)) {
            for(int j = i; j < ReturnStrings.Length - 1; j++) {
                ReturnStrings.Data[j] = ReturnStrings.Data[j + 1];
            }
            i--;
            ReturnStrings.Length--;
        }
    }
    
    split_string_return *ReturnStringsPtr =
        (split_string_return *)malloc(sizeof(split_string_return));
    ReturnStringsPtr->Data = (string_inplace *) malloc(sizeof(string_inplace) * ReturnStrings.Length);
    ReturnStringsPtr->Length = ReturnStrings.Length;
    for(int i = 0; i < ReturnStrings.Length; i++) {
        ReturnStringsPtr->Data[i].Data   = ReturnStrings.Data[i].Data;
        ReturnStringsPtr->Data[i].Length = ReturnStrings.Data[i].Length;
    }
    return ReturnStringsPtr;
}

//The guiding principle for GetLine() implementation is that
//upon reading the file we wish to do our string operations
//in place, without additional allocations.
inline string_inplace GetLine(char *Input) {
    return {Input, LineLength(Input)};
}

#endif STRINGS_H