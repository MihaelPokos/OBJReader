#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>
#include <types.h>
#include <stdio.h>
#include <math.h>
#include <ourmath.h>
#include <strings.h>
#include <auxiliary.cpp>

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

inline f32 GetVertexValue(char *Input) {
    char TempValue[20];
    u32 i = 0;
    while(*Input != ' ' && *Input != '\n' && *Input != '\0') {
        TempValue[i] = *Input++;
        i++;
    }
    TempValue[i] = '\0';
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

b32 CompareVertices (vertex *InputA, vertex *InputB) {
    if((InputA->Position == InputB->Position) && 
       (InputA->Texture  == InputB->Texture)  &&
       (InputA->Normal   == InputB->Normal)) return 1;
    return 0;
}

u32 SearchForVertex(vertex *VertexArray,
                    u32 *NumberOfVertices,
                    vertex TempVertex) {
    for(int i = 0; i < *NumberOfVertices; i++) {
        if(CompareVertices(&TempVertex, &VertexArray[i])) return i; }
    (*NumberOfVertices)++;
    return *NumberOfVertices - 1;
}

vertex_index_data LoadOBJData(char *InputFileName) {
    void *ReadData = Win32ReadEntireFile(InputFileName);
    char *CurrentChar = (char *)ReadData;
    char *MtllibNameString = (char *)malloc(sizeof(char) * MAX_OBJECT_NAME_LENGTH);
    char *ObjectNameString = (char *)malloc(sizeof(char) * MAX_OBJECT_NAME_LENGTH);
    
    
    u32 VertexPositionCount = 0;
    u32 VertexTextureCount  = 0;
    u32 VertexNormalCount   = 0;
    u32 FacesCount = 0;
    
    v3 *VertexPositionArray = (v3 *) malloc(sizeof(v3) * 200000);
    v2 *VertexTextureArray  = (v2 *) malloc(sizeof(v2) * 200000);
    v3 *VertexNormalArray   = (v3 *) malloc(sizeof(v3) * 200000);
    face *FacesArray        = (face *) malloc(sizeof(face) * 300000);
    
    while(1) {
        if(*CurrentChar == '\0')
            break;
        else if(CompareString(CurrentChar, 7, "mtllib ", 7)) {
            CurrentChar = SkipToBlankSpace(CurrentChar);
            CurrentChar++;
            int i = 0;
            while(*CurrentChar != '\n') {
                MtllibNameString[i] = *CurrentChar;
                CurrentChar++;
                i++;
            }
        }
        else if(CompareString(CurrentChar, 2, "o ", 2)) {
            CurrentChar = SkipToBlankSpace(CurrentChar);
            CurrentChar++;
            int i = 0;
            while(*CurrentChar != '\n') {
                ObjectNameString[i] = *CurrentChar;
                CurrentChar++;
                i++;
            }
        }
        else if(CompareString(CurrentChar, 2, "v ", 2)) {
            v3 TempVertexPosition = {};
            CurrentChar++;
            CurrentChar++;                 //Skip the blank spaces
            TempVertexPosition.x = GetVertexValue(CurrentChar);
            CurrentChar = SkipToBlankSpace(CurrentChar);
            CurrentChar++;
            TempVertexPosition.y = GetVertexValue(CurrentChar);
            CurrentChar = SkipToBlankSpace(CurrentChar);
            CurrentChar++;
            TempVertexPosition.z = GetVertexValue(CurrentChar);
            CurrentChar = SkipToBlankSpace(CurrentChar);
            VertexPositionArray[VertexPositionCount] = TempVertexPosition;
            VertexPositionCount++;
        }
        else if(CompareString(CurrentChar, 3, "vt ", 3)) {
            v2 TempVertexTexture = {};
            CurrentChar++;                      //Skip the blank space
            CurrentChar++;
            CurrentChar++;
            TempVertexTexture.x = GetVertexValue(CurrentChar);
            CurrentChar = SkipToBlankSpace(CurrentChar);
            CurrentChar++;
            TempVertexTexture.y = GetVertexValue(CurrentChar);
            CurrentChar = SkipToBlankSpace(CurrentChar);
            VertexTextureArray[VertexTextureCount] = TempVertexTexture;
            VertexTextureCount++;
        }
        else if(CompareString(CurrentChar, 3, "vn ", 3)) {
            v3 TempVertexNormal = {};
            CurrentChar++;                      //Skip the blank space
            CurrentChar++;
            CurrentChar++;
            TempVertexNormal.x = GetVertexValue(CurrentChar);
            CurrentChar = SkipToBlankSpace(CurrentChar);
            CurrentChar++;
            TempVertexNormal.y = GetVertexValue(CurrentChar);
            CurrentChar = SkipToBlankSpace(CurrentChar);
            CurrentChar++;
            TempVertexNormal.z = GetVertexValue(CurrentChar);
            CurrentChar = SkipToBlankSpace(CurrentChar);
            VertexNormalArray[VertexNormalCount] = TempVertexNormal;
            VertexNormalCount++;
        }
        else if(CompareString(CurrentChar, 2, "f ", 2)) {
            CurrentChar++;
            char *VertexCounter = CurrentChar;
            u32 NumberOfVertices = 0;
            while(*VertexCounter != '\n' && *VertexCounter != '\0') {
                if(*VertexCounter == ' ') NumberOfVertices++;
                VertexCounter++;
            }
            face TempFace = {};
            TempFace.Vertex = (v3u32 *) malloc(sizeof(v3u32) * NumberOfVertices);
            TempFace.NumberOfVertices = NumberOfVertices;
            CurrentChar++;
            for(int i = 0; i < TempFace.NumberOfVertices; i++) {
                TempFace.Vertex[i] = GetFaceVertex(CurrentChar);
                CurrentChar = SkipToBlankSpace(CurrentChar);
                CurrentChar++;
            }
            FacesArray[FacesCount].Vertex = TempFace.Vertex;
            FacesArray[FacesCount].NumberOfVertices = TempFace.NumberOfVertices;
            FacesCount++;
        }
        else CurrentChar++;
    }
    
    u32 MaxFaceNumber = GetMaxFaceNumber(FacesArray, FacesCount);
    u32 *TempIndexArray = (u32 *)malloc(sizeof(u32) * MaxFaceNumber);
    u32 *IndexArray     = (u32 *)malloc(sizeof(u32) * 1000000);
    u32 NumberOfIndices = 0;
    
    vertex *VertexArray = (vertex *)malloc(sizeof(vertex) * 800000);
    u32    NumberOfVertices = 0;    
    
    for(int i = 0; i < FacesCount; i++) {        
        for(int j = 0; j < FacesArray[i].NumberOfVertices; j++) {
            vertex TempVertex = GetVertex(FacesArray[i].Vertex[j],
                                          VertexPositionArray,
                                          VertexTextureArray,
                                          VertexNormalArray);
            u32 TempVertexIndex = SearchForVertex(VertexArray,
                                                  &NumberOfVertices,
                                                  TempVertex);
            VertexArray[TempVertexIndex] = TempVertex;
            TempIndexArray[j] = TempVertexIndex;
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
    return {VertexArray, NumberOfVertices, IndexArray, NumberOfIndices};
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
    sprintf_s(LineBuffer, 256, "i ");   
    WriteFile(RMFFileHandle, LineBuffer, StringLength(LineBuffer), 0, 0);
    for(int j = 0; j < Input.NumberOfIndices; j++) {
        sprintf_s(LineBuffer, 256, "%d ", Input.IndexData[j]);
        WriteFile(RMFFileHandle, LineBuffer, StringLength(LineBuffer), 0, 0);
    }
    sprintf_s(LineBuffer, 256, "\0");   
    WriteFile(RMFFileHandle, LineBuffer, StringLength(LineBuffer), 0, 0);
    return;
}

int main() {
    vertex_index_data ToPrint = {};    
    ToPrint = LoadOBJData("kocka.obj");
    WriteRMF(ToPrint);
    return 0;
}