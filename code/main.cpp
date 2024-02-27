#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>
#include "types.h"
#include <stdio.h>
#include <math.h>
#include "math_custom.h"
#include "strings.h"
#include "auxiliary.cpp"

#define MAX_OBJECT_NAME_LENGTH 32
#define MAX_MATERIAL_NAME_LENGTH 64
#define MAX_OBJECT_ARRAY_LENGTH 256

struct vertex {
    v3 Position;
    v2 Texture;
    v3 Normal;
};

struct face {
    v3u32 *Vertex;
    u32 NumberOfVertices;
};

struct vertex_index_data {
    vertex *VertexData;
    u32    NumberOfVertices;
    u32    *IndexData;
    u32    NumberOfIndices;
    u32    *PrunedVertices;
    u32    NumberOfPrunedVertices;
};

struct vertex_sort {
    vertex *VertexPtr;
    u32 *OldValue;
    u32 *NewValue;
};

struct object {
    char Name[MAX_OBJECT_NAME_LENGTH];
    char Material[MAX_MATERIAL_NAME_LENGTH];
    vertex_index_data *VertexIndexData;
};

struct object_array {
    object *Data[MAX_OBJECT_ARRAY_LENGTH];
    u32 Length;
};

inline vertex GetVertex(v3u32 InputVertex,
                        v3 *Positions,
                        v2 *Textures,
                        v3 *Normals) {
    return {Positions[InputVertex.x - 1],
            Textures[InputVertex.y - 1],
            Normals[InputVertex.z - 1]};
}

inline f32 GetVertexValue(string_inplace *Input) {
    char TempValue[20];
    for(int i = 0; i < 20 && i < Input->Length; i++)
        TempValue[i] = Input->Data[i];
    return atof(TempValue);
}

inline v3u32 GetFaceVertex(char *Input) {
    v3u32 Vertex = {};
    u32 BackslashCounter = 0;
    char TempValue[20];
    u32 TempValueCounter = 0;
    while(*Input != ' ' && *Input != '\n' && *Input != '\0') {
        if(*Input == '/') {
            TempValue[TempValueCounter] = '\0';
            Vertex.E[BackslashCounter] = strtol(TempValue, '\0', 10);
            BackslashCounter++;
            TempValueCounter = 0;
            Input++;
        }
        TempValue[TempValueCounter] = *Input++;
        TempValueCounter++;
    }
    TempValue[TempValueCounter] = '\0';
    Vertex.E[BackslashCounter] = strtol(TempValue, '\0', 10);
    return Vertex;
}

u32 GetMaxFaceNumber(face *InputFacesArray,
                     u32 InputFacesCount) {
    u32 Result = 0;
    u32 Max = InputFacesCount + 1;
    for(int i = 0; i < Max; i++) {
        if(InputFacesArray->NumberOfVertices > Result)
            Result = InputFacesArray->NumberOfVertices;
    }
    return Result;
}

//Sorting functions
b32 CompareFirstVertexValue(vertex *InputA, vertex *InputB) {
    if((InputA->Position.x >= InputB->Position.x)) return 1;
    return 0;
}

b32 CompareVertices(vertex *InputA, vertex *InputB) {
    if((InputA->Position == InputB->Position) &&
       (InputA->Texture == InputB->Texture) &&
       (InputA->Normal == InputB->Normal))
        return 1;
    return 0;
}

void Swap(vertex_sort *InputA, int CountA,
          vertex_sort *InputB, int CountB) {
    vertex Temp = InputA->VertexPtr[CountA];
    u32 TempValue = InputA->NewValue[CountA];
    InputA->VertexPtr[CountA] = InputB->VertexPtr[CountB];
    InputA->NewValue[CountA]  = InputB->NewValue[CountB];
    InputB->VertexPtr[CountB] = Temp;
    InputB->NewValue[CountB]  = TempValue;
}

int Partition(vertex_sort *Input, int Low, int High) {
    vertex Pivot = Input->VertexPtr[High];
    int i = Low - 1;
    for(int j = i + 1; j < High; j++)
        if(CompareFirstVertexValue(&Input->VertexPtr[j], &Pivot)) {
            i++;
            Swap(Input, i, Input, j);
        }
    i++;
    Swap(Input, i, Input, High);
    return i;
}

void QuickSort(vertex_sort *Input, int Low, int High) {
    if(Low < High) {
        int Pivot = Partition(Input, Low, High);
        QuickSort(Input, Low, Pivot - 1);
        QuickSort(Input, Pivot + 1, High);
    }
}

void SwitchFaceValues(vertex_sort *Input,
                      u32 *IndexArray,
                      u32 NumberOfIndices) {
    for(int i = 0; i < NumberOfIndices; i++) {
        for(int j = 0; j < NumberOfIndices; j++) {
            if(IndexArray[i] == Input->OldValue[j]) {
                IndexArray[i] = Input->NewValue[j];
                break;
            }
        }
    }
}


void PruneSortedArray(vertex_sort *Input, int Length,
                      u32 *IndexArray,
                      u32 *PrunedVertices, u32 *NumberOfPrunedVertices) {
    int MaxLength = Length - 1;
    for(int i = 0; i < MaxLength; i++) {
        int j = i + 1;
        while(CompareFirstVertexValue(&Input->VertexPtr[j], &Input->VertexPtr[i]) && j < Length) {
            if(CompareVertices(&Input->VertexPtr[j], &Input->VertexPtr[i])) {
                IndexArray[j] = Input->NewValue[i];
                PrunedVertices[*NumberOfPrunedVertices] = j;
                (*NumberOfPrunedVertices)++;
            }
            j++;
        }
        i = j;
    }
}



vertex_index_data LoadOBJData(char *InputFileName) {
    void *ReadData = Win32ReadEntireFile(InputFileName);
    char *CurrentChar = (char *)ReadData;
    string_inplace *CurrentLine = (string_inplace *) malloc(sizeof(string_inplace*));
    split_string_return *SplitLine;
    
    string *MtllibNameString = (string *)malloc(sizeof(string));
    MtllibNameString->Data = (char *)malloc(sizeof(char) * MAX_MATERIAL_NAME_LENGTH);
    string *ObjectNameString = (string *)malloc(sizeof(string));
    ObjectNameString->Data = (char *)malloc(sizeof(char) * MAX_OBJECT_NAME_LENGTH);
    
    string BlankSpaceSplit = {" ", 1};
    string SlashSplit = {"/", 1};
    
    u32 VertexPositionCount = 0;
    u32 VertexTextureCount  = 0;
    u32 VertexNormalCount   = 0;
    u32 FacesCount          = 0;
    
    v3 *VertexPositionArray = (v3 *) malloc(sizeof(v3) * 200000);
    v2 *VertexTextureArray  = (v2 *) malloc(sizeof(v2) * 200000);
    v3 *VertexNormalArray   = (v3 *) malloc(sizeof(v3) * 200000);
    face *FacesArray        = (face *) malloc(sizeof(face) * 300000);
    
    v3 TempVertexPosition = {0.0f, 0.0f, 0.0f};
    v2 TempVertexTexture  = {0.0f, 0.0f};
    v3 TempVertexNormal   = {0.0f, 0.0f, 0.0f};
    
    while(1) {
        *CurrentLine = GetLine(CurrentChar);
        SplitLine = SplitStringInplace(CurrentLine, &BlankSpaceSplit);
        if(CurrentLine->Data[0] == '\0')
            break;
        if(CurrentLine->Data[0] == '\n')
            CurrentChar++;
        else if(CompareString(SplitLine->Data[0].Data, SplitLine->Data[0].Length,
                              "mtllib", 6)) {
            MtllibNameString = &SplitLine->Data[1];
        }
        else if(CompareString(SplitLine->Data[0].Data, SplitLine->Data[0].Length,
                              "o", 1)) {
            ObjectNameString = &SplitLine->Data[1];
        }
        else if(CompareString(SplitLine->Data[0].Data, SplitLine->Data[0].Length,
                              "v", 1)) {
            TempVertexPosition.x = GetVertexValue(&SplitLine->Data[1]);
            TempVertexPosition.y = GetVertexValue(&SplitLine->Data[2]);
            TempVertexPosition.z = GetVertexValue(&SplitLine->Data[3]);
            VertexPositionArray[VertexPositionCount] = TempVertexPosition;
            VertexPositionCount++;
        }
        else if(CompareString(SplitLine->Data[0].Data, SplitLine->Data[0].Length,
                              "vt", 2)) {
            TempVertexTexture.x = GetVertexValue(&SplitLine->Data[1]);
            TempVertexTexture.y = GetVertexValue(&SplitLine->Data[2]);
            VertexTextureArray[VertexTextureCount] = TempVertexTexture;
            VertexTextureCount++;
        }
        else if(CompareString(SplitLine->Data[0].Data, SplitLine->Data[0].Length,
                              "vn", 2)) {
            TempVertexNormal.x = GetVertexValue(&SplitLine->Data[1]);
            TempVertexNormal.y = GetVertexValue(&SplitLine->Data[2]);
            TempVertexNormal.z = GetVertexValue(&SplitLine->Data[3]);
            VertexNormalArray[VertexNormalCount] = TempVertexNormal;
            VertexNormalCount++;
        }
        else if(CompareString(SplitLine->Data[0].Data, SplitLine->Data[0].Length,
                              "f", 1)) {
            u32 NumberOfVertices = SplitLine->Length - 1;
            face TempFace = {};
            TempFace.Vertex = (v3u32 *) malloc(sizeof(v3u32) * NumberOfVertices);
            TempFace.NumberOfVertices = NumberOfVertices;
            for(int i = 0; i < TempFace.NumberOfVertices; i++) {
                TempFace.Vertex[i] = GetFaceVertex(SplitLine->Data[i + 1].Data);
            }
            FacesArray[FacesCount].Vertex = TempFace.Vertex;
            FacesArray[FacesCount].NumberOfVertices = TempFace.NumberOfVertices;
            FacesCount++;
        }
        CurrentChar += CurrentLine->Length;
    }
    
    u32 MaxFaceNumber   = GetMaxFaceNumber(FacesArray, FacesCount);
    u32 *TempIndexArray = (u32 *)malloc(sizeof(u32) * MaxFaceNumber);
    u32 *IndexArray     = (u32 *)malloc(sizeof(u32) * 1000000);
    u32 NumberOfIndices = 0;
    
    vertex_sort *VertexSort = (vertex_sort *)malloc(sizeof(vertex_sort));
    VertexSort->VertexPtr  = (vertex *)malloc(sizeof(vertex) * 800000);
    VertexSort->OldValue   = (u32 *)malloc(sizeof(u32) * 800000);
    VertexSort->NewValue   = (u32 *)malloc(sizeof(u32) * 800000);
    u32 NumberOfVertices = 0;    
    u32 TempVertexIndex = 0;
    
    for(int i = 0; i < FacesCount; i++) {        
        for(int j = 0; j < FacesArray[i].NumberOfVertices; j++) {
            vertex TempVertex = GetVertex(FacesArray[i].Vertex[j],
                                          VertexPositionArray,
                                          VertexTextureArray,
                                          VertexNormalArray);
            VertexSort->VertexPtr[TempVertexIndex] = TempVertex;
            VertexSort->OldValue[TempVertexIndex]  = TempVertexIndex; 
            VertexSort->NewValue[TempVertexIndex]  = TempVertexIndex;
            TempIndexArray[j] = TempVertexIndex;
            TempVertexIndex++;
        }
        for(int k = 0; k < (FacesArray[i].NumberOfVertices - 2); k++) {
            IndexArray[NumberOfIndices] = TempIndexArray[0];
            NumberOfIndices++;
            IndexArray[NumberOfIndices] = TempIndexArray[k + 1];
            NumberOfIndices++;
            IndexArray[NumberOfIndices] = TempIndexArray[k + 2];
            NumberOfIndices++;
        }
    }
    
    NumberOfVertices = TempVertexIndex;    
    vertex *VertexArray = VertexSort->VertexPtr;
    u32 NumberOfPrunedVertices = 0;
    u32 *PrunedVertices = (u32 *)malloc(sizeof(u32) * 40000);
    #if 0
    QuickSort(VertexSort, 0, NumberOfVertices - 1);
    SwitchFaceValues(VertexSort, IndexArray, NumberOfIndices);
    PruneSortedArray(VertexSort, NumberOfVertices, IndexArray,
                     PrunedVertices, &NumberOfPrunedVertices);
    #endif
    return {VertexArray, NumberOfVertices,
            IndexArray, NumberOfIndices,
            PrunedVertices, NumberOfPrunedVertices};
}

void WriteRMF(vertex_index_data Input) {
    HANDLE RMFFileHandle  =  CreateFileA("output.rmf",
                                         FILE_APPEND_DATA ,
                                         FILE_SHARE_WRITE,
                                         0,
                                         OPEN_ALWAYS,
                                         0, 0);
    char LineBuffer[256];
    for(int i = 0; i < Input.NumberOfVertices; i++) {
        int j = 0;
        for(; j < Input.NumberOfPrunedVertices; j++)
            if(i == Input.PrunedVertices[j]) break;
        if(j == Input.NumberOfPrunedVertices) {
            sprintf_s(LineBuffer, 256,
                       "v %f %f %f / %f %f %f / %f %f\n",
                       Input.VertexData[i].Position.x,
                       Input.VertexData[i].Position.y,
                       Input.VertexData[i].Position.z,
                       Input.VertexData[i].Normal.x,
                       Input.VertexData[i].Normal.y,
                       Input.VertexData[i].Normal.z,
                       Input.VertexData[i].Texture.x,
                       Input.VertexData[i].Texture.y);
            WriteFile(RMFFileHandle, LineBuffer, StringLength(LineBuffer), 0, 0);
        }
    }
    sprintf_s(LineBuffer, 256, "i ");
    WriteFile(RMFFileHandle, LineBuffer, StringLength(LineBuffer), 0, 0);
    for(int j = 0; j < Input.NumberOfIndices; j++) {
        sprintf_s(LineBuffer, 256, "%d ", Input.IndexData[j]);
        WriteFile(RMFFileHandle, LineBuffer, StringLength(LineBuffer), 0, 0);
    }
    sprintf_s(LineBuffer, 256, "\n\0");
    WriteFile(RMFFileHandle, LineBuffer, StringLength(LineBuffer), 0, 0);
    return;
}

int main() {
    vertex_index_data ToPrint = {};  
    ToPrint = LoadOBJData("sponza.obj");
    WriteRMF(ToPrint);
    
    return 0;   
}