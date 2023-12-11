#ifndef OURMATH_H
#define OURMATH_H

#define PI       3.14159265359
#define DEGTORAD 0.01745329251
#define RADTODEG 57.2957795131

union v2
{
    struct
    {
        f32 x, y;
    };
    struct
    {
        f32 u, v;
    };
    f32 E[2];
};

union v3
{
    struct
    {
        f32 x, y, z;
    };
    struct
    {
        f32 u, v, __;
    };
    struct
    {
        f32 r, g, b;
    };
    struct
    {
        v2 xy;
        f32 Ignored0_;
    };
    struct
    {
        f32 Ignored1_;
        v2 yz;
    };
    struct
    {
        v2 uv;
        f32 Ignored2_;
    };
    struct
    {
        f32 Ignored3_;
        v2 v__;
    };
    f32 E[3];
};

union v3u32
{
    struct
    {
        u32 x, y, z;
    };
    u32 E[3];
};

union v4
{
    struct
    {
        union
        {
            v3 xyz;
            struct
            {
                f32 x, y, z;
            };
        };
        
        f32 w;
    };
    struct
    {
        union
        {
            v3 rgb;
            struct
            {
                f32 r, g, b;
            };
        };
        
        f32 a;
    };
    struct
    {
        v2 xy;
        f32 Ignored0_;
        f32 Ignored1_;
    };
    struct
    {
        f32 Ignored2_;
        v2 yz;
        f32 Ignored3_;
    };
    struct
    {
        f32 Ignored4_;
        f32 Ignored5_;
        v2 zw;
    };
    f32 E[4];
};


v2 operator+(v2 X, v2 Y) {
    return {X.x + Y.x, X.y + Y.y};
}

v2 operator-(v2 X, v2 Y) {
    return {X.x - Y.x, X.y - Y.y};
}

v2 operator*(f32 a, v2 X) {
    return {a * X.x, a * X.y};
}

b32 operator!=(v2 A, v2 B) {
    return (A.x != B.x) || (A.y != B.y); 
}

b32 operator==(v2 A, v2 B) {
    return (A.x == B.x) && (A.y == B.y);
}

v3 operator+(v3 X, v3 Y) {
    return {X.x + Y.x, X.y + Y.y, X.z + Y.z};
}

v3 operator-(v3 X, v3 Y) {
    return {X.x - Y.x, X.y - Y.y, X.z - Y.z};
}

v3 operator-(v3 X) {
    return {-X.x, -X.y, -X.z};
}

v3 operator*(f32 a, v3 X) {
    return {a * X.x, a * X.y, a * X.z};
}

v3 operator*(v3 X, v3 Y) {
    return {X.x * Y.x, X.y * Y.y, X.z * Y.z};
}

b32 operator!=(v3 A, v3 B) {
    return (A.x != B.x) || (A.y != B.y) || (A.z != B.z); 
}

b32 operator==(v3 A, v3 B) {
    return (A.x == B.x) && (A.y == B.y) && (A.z == B.z);
}

inline v3 Normalize(v3 X) {
    f32 ReciprocalNorm = 1 / sqrt(X.x * X.x + X.y * X.y + X.z * X.z);
    return {X.x * ReciprocalNorm, X.y * ReciprocalNorm, X.z * ReciprocalNorm};
}

inline v3 CrossProduct(v3 X, v3 Y) {
    return {X.y * Y.z - X.z * Y.y,
            X.z * Y.x - X.x * Y.z,
            X.x * Y.y - X.y * Y.x};
}

inline f32 DotProduct(v3 X, v3 Y) {
    return X.x * Y.x + X.y * Y.y + X.z * Y.z;
}

//ROW MAJOR
struct m2x2 {
    f32 E[2][2];
};

inline m2x2 Identity2x2() {
    m2x2 Result = {};
    Result.E[0][0] = 1.0;
    Result.E[1][1] = 1.0;
    
    return Result;
};

struct m3x3 {
    f32 E[3][3];
};

inline m3x3 Identity3x3() {
    m3x3 Result = {};
    Result.E[0][0] = 1.0;
    Result.E[1][1] = 1.0;
    Result.E[2][2] = 1.0;
    
    return Result;
};

struct m4x4 {
    f32 E[4][4];
};

inline m4x4 Identity4x4() {
    m4x4 Result = {};
    Result.E[0][0] = 1.0;
    Result.E[1][1] = 1.0;
    Result.E[2][2] = 1.0;
    Result.E[3][3] = 1.0;
    
    return Result;
};

v3 operator*(m3x3 A, v3 b) {
    v3 Result = {};
    for(int i = 0; i < 3; i++)
        for(int j = 0; j < 3; j++)
            Result.E[i] += A.E[i][j] * b.E[j];
    return Result;
};

m3x3 operator*(m3x3 A, m3x3 B) {
    m3x3 Result = {};
    for(int i = 0; i < 3; i++)
        for(int j = 0; j < 3; j++)
            for(int k = 0; k < 3; k++)
              Result.E[i][j] += A.E[i][k] * B.E[k][j];
    
    return Result;
}

v4 operator*(m4x4 A, v4 b) {
    v4 Result = {};
    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
            Result.E[i] += A.E[i][j] * b.E[j];
    return Result;
};

m4x4 operator*(m4x4 A, m4x4 B) {
    m4x4 Result = {};
    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
            for(int k = 0; k < 4; k++)
              Result.E[i][j] += A.E[i][k] * B.E[k][j];
    
    return Result;
}

inline m4x4 Rotation4x4X(f32 Angle) {
    m4x4 Result = Identity4x4();
    Result.E[1][1] = cos(Angle);
    Result.E[1][2] = -sin(Angle);
    Result.E[2][1] = sin(Angle);
    Result.E[2][2] = cos(Angle);
    
    return Result;
}

inline m4x4 Rotation4x4Y(f32 Angle) {
    m4x4 Result = Identity4x4();
    Result.E[0][0] = cos(Angle);
    Result.E[0][2] = sin(Angle);
    Result.E[2][0] = -sin(Angle);
    Result.E[2][2] = cos(Angle);
    
    return Result;
}

inline m4x4 Rotation4x4Z(f32 Angle) {
    m4x4 Result = Identity4x4();
    Result.E[0][0] = cos(Angle);
    Result.E[0][1] = -sin(Angle);
    Result.E[1][0] = sin(Angle);
    Result.E[1][1] = cos(Angle);
    
    return Result;
}

inline m4x4 Scaling4x4(f32 ScalingFactor) {
    m4x4 Result = Identity4x4();
    Result.E[0][0] = ScalingFactor;
    Result.E[1][1] = ScalingFactor;
    Result.E[2][2] = ScalingFactor;
    
    return Result;
}

inline m4x4 Translation4x4(v3 Input) {
    m4x4 Result = Identity4x4();
    Result.E[0][3] = Input.x;
    Result.E[1][3] = Input.y;
    Result.E[2][3] = Input.z;
    
    return Result;
}

inline m3x3 Rotation3x3X(f32 Angle) {
    m3x3 Result = Identity3x3();
    Result.E[1][1] = cos(Angle);
    Result.E[1][2] = -sin(Angle);
    Result.E[2][1] = sin(Angle);
    Result.E[2][2] = cos(Angle);
    
    return Result;
}

inline m3x3 Rotation3x3Y(f32 Angle) {
    m3x3 Result = Identity3x3();
    Result.E[0][0] = cos(Angle);
    Result.E[0][2] = sin(Angle);
    Result.E[2][0] = -sin(Angle);
    Result.E[2][2] = cos(Angle);
    
    return Result;
}

inline m3x3 Rotation3x3Z(f32 Angle) {
    m3x3 Result = Identity3x3();
    Result.E[0][0] = cos(Angle);
    Result.E[0][1] = -sin(Angle);
    Result.E[1][0] = sin(Angle);
    Result.E[1][1] = cos(Angle);
    
    return Result;
}

inline m3x3 Scaling3x3(f32 ScalingFactor) {
    m3x3 Result = Identity3x3();
    Result.E[0][0] = ScalingFactor;
    Result.E[1][1] = ScalingFactor;
    Result.E[2][2] = ScalingFactor;
    
    return Result;
}

inline m3x3 Transpose(m3x3 Input) {
    return {Input.E[0][0], Input.E[1][0], Input.E[2][0],
            Input.E[0][1], Input.E[1][1], Input.E[2][1],
            Input.E[0][2], Input.E[1][2], Input.E[2][2]};
}

inline m4x4 Transpose(m4x4 Input) {
    return {Input.E[0][0], Input.E[1][0], Input.E[2][0], Input.E[3][0],
            Input.E[0][1], Input.E[1][1], Input.E[2][1], Input.E[3][1],
            Input.E[0][2], Input.E[1][2], Input.E[2][2], Input.E[3][2],
            Input.E[0][3], Input.E[1][3], Input.E[2][3], Input.E[3][3]};
}

inline m3x3 GetUpperLeftMatrix(m4x4 Input) {
    return {Input.E[0][0], Input.E[0][1], Input.E[0][2],
            Input.E[1][0], Input.E[1][1], Input.E[1][2],
            Input.E[2][0], Input.E[2][1], Input.E[2][2]};
}

inline f32 m2x2Determinant(m2x2 Input) {
    return Input.E[0][0] * Input.E[1][1] - Input.E[0][1] * Input.E[1][0];
}

inline f32 m3x3Determinant(m3x3 Input) {
    f32 Result = Input.E[0][0] * (Input.E[1][1] * Input.E[2][2] - Input.E[1][2] * Input.E[2][1]) -
                 Input.E[0][1] * (Input.E[1][0] * Input.E[2][2] - Input.E[1][2] * Input.E[2][0]) + 
                 Input.E[0][2] * (Input.E[1][0] * Input.E[2][1] - Input.E[1][1] * Input.E[2][0]);
    return Result;
}

inline f32 m4x4Determinant(m4x4 Input) {
    f32 Result = Input.E[0][0] * m3x3Determinant({Input.E[1][1], Input.E[1][2], Input.E[1][3],
                                                  Input.E[2][1], Input.E[2][2], Input.E[2][3],
                                                  Input.E[3][1], Input.E[3][2], Input.E[3][3]}) -
                 Input.E[0][1] * m3x3Determinant({Input.E[1][0], Input.E[1][2], Input.E[1][3],
                                                  Input.E[2][0], Input.E[2][2], Input.E[3][3],
                                                  Input.E[3][0], Input.E[2][2], Input.E[3][3]}) +
                 Input.E[0][2] * m3x3Determinant({Input.E[1][0], Input.E[1][1], Input.E[1][3],
                                                  Input.E[2][0], Input.E[2][1], Input.E[2][3],
                                                  Input.E[3][0], Input.E[3][1], Input.E[3][3]}) -
                 Input.E[0][3] * m3x3Determinant({Input.E[1][0], Input.E[1][1], Input.E[1][2],
                                                  Input.E[2][0], Input.E[2][1], Input.E[2][2],
                                                  Input.E[3][0], Input.E[3][1], Input.E[3][2]});
    return Result;
}

inline m2x2 m3x3Cofactor(m3x3 Input, int i, int j) {
    m2x2 Result = Identity2x2();
    int ResultRow = 0;
    for(int Row = 0; Row < 3; Row++) {
        int ResultCol = 0;
        if(Row == i) continue;
    
        for(int Col = 0; Col < 3; Col++) {
            if(Col == j) continue;
            Result.E[ResultRow][ResultCol] = Input.E[Row][Col];
            ResultCol++;
        }
        ResultRow++;
    }
    return Result;
}

inline m3x3 m4x4Cofactor(m4x4 Input, int i, int j) {
    m3x3 Result = Identity3x3();
    int ResultRow = 0;
    for(int Row = 0; Row < 4; Row++) {
        int ResultCol = 0;
        if(Row == i) continue;
        for(int Col = 0; Col < 4; Col++) {
            if(Col == j) continue;
            Result.E[ResultRow][ResultCol] = Input.E[Row][Col];
            ResultCol++;
        }
        ResultRow++;
    }
    return Result;
}

inline m4x4 Inverse(m4x4 Input) {
    m4x4 Result;
    f32 InputDetReciprocal = 1 / m4x4Determinant(Input);
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            Result.E[j][i] = pow(-1, i + j) * InputDetReciprocal * m3x3Determinant(m4x4Cofactor(Input, i, j));
        }
    }
    return Result;
}

inline m3x3 Inverse(m3x3 Input) {
    m3x3 Result;
    f32 InputDetReciprocal = 1 / m3x3Determinant(Input);
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            Result.E[j][i] = pow(-1, i + j) * InputDetReciprocal * m2x2Determinant(m3x3Cofactor(Input, i, j));
        }
    }
    return Result;
}

#endif OURMATH_H