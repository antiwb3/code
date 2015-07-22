#if 0
//
// Generated by Microsoft (R) HLSL Shader Compiler 9.29.952.3111
//
//
//   fxc /T ps_4_0 /Vn QuadPixelShader /Fh
//    FxPlayerSWFToTextureD3D1x_quadshader.psh.h
//    FxPlayerSWFToTextureD3D1x_quadshader.psh
//
//
// Resource Bindings:
//
// Name                                 Type  Format         Dim Slot Elements
// ------------------------------ ---------- ------- ----------- ---- --------
// RenderedTextureSampler            sampler      NA          NA    0        1
// RenderedTexture                   texture  float4          2d    0        1
//
//
//
// Input signature:
//
// Name                 Index   Mask Register SysValue Format   Used
// -------------------- ----- ------ -------- -------- ------ ------
// TEXCOORD                 0   xy          0     NONE  float   xy  
//
//
// Output signature:
//
// Name                 Index   Mask Register SysValue Format   Used
// -------------------- ----- ------ -------- -------- ------ ------
// SV_Target                0   xyzw        0   TARGET  float   xyzw
//
ps_4_0
    dcl_sampler s0, mode_default
    dcl_resource_texture2d (float,float,float,float) t0
    dcl_input_ps linear v0.xy
    dcl_output o0.xyzw
    sample o0.xyzw, v0.xyxx, t0.xyzw, s0
    ret 
    // Approximately 2 instruction slots used
#endif

const byte QuadPixelShader[] =
{
    68,  88,  66,  67, 102, 151, 
    172,  75,  43,   5, 162, 229, 
    209,  72,  19,  54, 147, 138, 
    138,  72,   1,   0,   0,   0, 
    64,   2,   0,   0,   5,   0, 
    0,   0,  52,   0,   0,   0, 
    240,   0,   0,   0,  36,   1, 
    0,   0,  88,   1,   0,   0, 
    196,   1,   0,   0,  82,  68, 
    69,  70, 180,   0,   0,   0, 
    0,   0,   0,   0,   0,   0, 
    0,   0,   2,   0,   0,   0, 
    28,   0,   0,   0,   0,   4, 
    255, 255,   0,   1,   0,   0, 
    131,   0,   0,   0,  92,   0, 
    0,   0,   3,   0,   0,   0, 
    0,   0,   0,   0,   0,   0, 
    0,   0,   0,   0,   0,   0, 
    0,   0,   0,   0,   1,   0, 
    0,   0,   0,   0,   0,   0, 
    115,   0,   0,   0,   2,   0, 
    0,   0,   5,   0,   0,   0, 
    4,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    1,   0,   0,   0,  13,   0, 
    0,   0,  82, 101, 110, 100, 
    101, 114, 101, 100,  84, 101, 
    120, 116, 117, 114, 101,  83, 
    97, 109, 112, 108, 101, 114, 
    0,  82, 101, 110, 100, 101, 
    114, 101, 100,  84, 101, 120, 
    116, 117, 114, 101,   0,  77, 
    105,  99, 114, 111, 115, 111, 
    102, 116,  32,  40,  82,  41, 
    32,  72,  76,  83,  76,  32, 
    83, 104,  97, 100, 101, 114, 
    32,  67, 111, 109, 112, 105, 
    108, 101, 114,  32,  57,  46, 
    50,  57,  46,  57,  53,  50, 
    46,  51,  49,  49,  49,   0, 
    73,  83,  71,  78,  44,   0, 
    0,   0,   1,   0,   0,   0, 
    8,   0,   0,   0,  32,   0, 
    0,   0,   0,   0,   0,   0, 
    0,   0,   0,   0,   3,   0, 
    0,   0,   0,   0,   0,   0, 
    3,   3,   0,   0,  84,  69, 
    88,  67,  79,  79,  82,  68, 
    0, 171, 171, 171,  79,  83, 
    71,  78,  44,   0,   0,   0, 
    1,   0,   0,   0,   8,   0, 
    0,   0,  32,   0,   0,   0, 
    0,   0,   0,   0,   0,   0, 
    0,   0,   3,   0,   0,   0, 
    0,   0,   0,   0,  15,   0, 
    0,   0,  83,  86,  95,  84, 
    97, 114, 103, 101, 116,   0, 
    171, 171,  83,  72,  68,  82, 
    100,   0,   0,   0,  64,   0, 
    0,   0,  25,   0,   0,   0, 
    90,   0,   0,   3,   0,  96, 
    16,   0,   0,   0,   0,   0, 
    88,  24,   0,   4,   0, 112, 
    16,   0,   0,   0,   0,   0, 
    85,  85,   0,   0,  98,  16, 
    0,   3,  50,  16,  16,   0, 
    0,   0,   0,   0, 101,   0, 
    0,   3, 242,  32,  16,   0, 
    0,   0,   0,   0,  69,   0, 
    0,   9, 242,  32,  16,   0, 
    0,   0,   0,   0,  70,  16, 
    16,   0,   0,   0,   0,   0, 
    70, 126,  16,   0,   0,   0, 
    0,   0,   0,  96,  16,   0, 
    0,   0,   0,   0,  62,   0, 
    0,   1,  83,  84,  65,  84, 
    116,   0,   0,   0,   2,   0, 
    0,   0,   0,   0,   0,   0, 
    0,   0,   0,   0,   2,   0, 
    0,   0,   0,   0,   0,   0, 
    0,   0,   0,   0,   0,   0, 
    0,   0,   1,   0,   0,   0, 
    0,   0,   0,   0,   0,   0, 
    0,   0,   0,   0,   0,   0, 
    0,   0,   0,   0,   0,   0, 
    0,   0,   0,   0,   0,   0, 
    1,   0,   0,   0,   0,   0, 
    0,   0,   0,   0,   0,   0, 
    0,   0,   0,   0,   0,   0, 
    0,   0,   0,   0,   0,   0, 
    0,   0,   0,   0,   0,   0, 
    0,   0,   0,   0,   0,   0, 
    0,   0,   0,   0,   0,   0, 
    0,   0,   0,   0,   0,   0, 
    0,   0,   0,   0,   0,   0, 
    0,   0,   0,   0,   0,   0
};
#if 0
//
// Generated by Microsoft (R) HLSL Shader Compiler 9.29.952.3111
//
//
//   fxc /T vs_4_0 /Vn QuadVertexShader /Fh
//    FxPlayerSWFToTextureD3D1x_quadshader.vsh.h
//    FxPlayerSWFToTextureD3D1x_quadshader.vsh
//
//
// Buffer Definitions: 
//
// cbuffer VSConstants
// {
//
//   float4x4 WorldViewProjMatrix;      // Offset:    0 Size:    64
//
// }
//
//
// Resource Bindings:
//
// Name                                 Type  Format         Dim Slot Elements
// ------------------------------ ---------- ------- ----------- ---- --------
// VSConstants                       cbuffer      NA          NA    0        1
//
//
//
// Input signature:
//
// Name                 Index   Mask Register SysValue Format   Used
// -------------------- ----- ------ -------- -------- ------ ------
// POSITION                 0   xyz         0     NONE  float   xyz 
// TEXCOORD                 0   xy          1     NONE  float   xy  
//
//
// Output signature:
//
// Name                 Index   Mask Register SysValue Format   Used
// -------------------- ----- ------ -------- -------- ------ ------
// TEXCOORD                 0   xy          0     NONE  float   xy  
// SV_Position              0   xyzw        1      POS  float   xyzw
//
vs_4_0
    dcl_constantbuffer cb0[4], immediateIndexed
    dcl_input v0.xyz
    dcl_input v1.xy
    dcl_output o0.xy
    dcl_output_siv o1.xyzw, position
    dcl_temps 1
    mov o0.xy, v1.xyxx
    mul r0.xyzw, v0.yyyy, cb0[1].xyzw
    mad r0.xyzw, cb0[0].xyzw, v0.xxxx, r0.xyzw
    mad r0.xyzw, cb0[2].xyzw, v0.zzzz, r0.xyzw
    add o1.xyzw, r0.xyzw, cb0[3].xyzw
    ret 
    // Approximately 6 instruction slots used
#endif

const byte QuadVertexShader[] =
{
    68,  88,  66,  67,  97, 204, 
    109, 130, 157, 151, 208, 108, 
    178, 172,  93, 121, 111, 211, 
    61, 236,   1,   0,   0,   0, 
    56,   3,   0,   0,   5,   0, 
    0,   0,  52,   0,   0,   0, 
    12,   1,   0,   0,  96,   1, 
    0,   0, 184,   1,   0,   0, 
    188,   2,   0,   0,  82,  68, 
    69,  70, 208,   0,   0,   0, 
    1,   0,   0,   0,  72,   0, 
    0,   0,   1,   0,   0,   0, 
    28,   0,   0,   0,   0,   4, 
    254, 255,   0,   1,   0,   0, 
    156,   0,   0,   0,  60,   0, 
    0,   0,   0,   0,   0,   0, 
    0,   0,   0,   0,   0,   0, 
    0,   0,   0,   0,   0,   0, 
    0,   0,   0,   0,   1,   0, 
    0,   0,   0,   0,   0,   0, 
    86,  83,  67, 111, 110, 115, 
    116,  97, 110, 116, 115,   0, 
    60,   0,   0,   0,   1,   0, 
    0,   0,  96,   0,   0,   0, 
    64,   0,   0,   0,   0,   0, 
    0,   0,   0,   0,   0,   0, 
    120,   0,   0,   0,   0,   0, 
    0,   0,  64,   0,   0,   0, 
    2,   0,   0,   0, 140,   0, 
    0,   0,   0,   0,   0,   0, 
    87, 111, 114, 108, 100,  86, 
    105, 101, 119,  80, 114, 111, 
    106,  77,  97, 116, 114, 105, 
    120,   0,   3,   0,   3,   0, 
    4,   0,   4,   0,   0,   0, 
    0,   0,   0,   0,   0,   0, 
    77, 105,  99, 114, 111, 115, 
    111, 102, 116,  32,  40,  82, 
    41,  32,  72,  76,  83,  76, 
    32,  83, 104,  97, 100, 101, 
    114,  32,  67, 111, 109, 112, 
    105, 108, 101, 114,  32,  57, 
    46,  50,  57,  46,  57,  53, 
    50,  46,  51,  49,  49,  49, 
    0, 171, 171, 171,  73,  83, 
    71,  78,  76,   0,   0,   0, 
    2,   0,   0,   0,   8,   0, 
    0,   0,  56,   0,   0,   0, 
    0,   0,   0,   0,   0,   0, 
    0,   0,   3,   0,   0,   0, 
    0,   0,   0,   0,   7,   7, 
    0,   0,  65,   0,   0,   0, 
    0,   0,   0,   0,   0,   0, 
    0,   0,   3,   0,   0,   0, 
    1,   0,   0,   0,   3,   3, 
    0,   0,  80,  79,  83,  73, 
    84,  73,  79,  78,   0,  84, 
    69,  88,  67,  79,  79,  82, 
    68,   0, 171, 171,  79,  83, 
    71,  78,  80,   0,   0,   0, 
    2,   0,   0,   0,   8,   0, 
    0,   0,  56,   0,   0,   0, 
    0,   0,   0,   0,   0,   0, 
    0,   0,   3,   0,   0,   0, 
    0,   0,   0,   0,   3,  12, 
    0,   0,  65,   0,   0,   0, 
    0,   0,   0,   0,   1,   0, 
    0,   0,   3,   0,   0,   0, 
    1,   0,   0,   0,  15,   0, 
    0,   0,  84,  69,  88,  67, 
    79,  79,  82,  68,   0,  83, 
    86,  95,  80, 111, 115, 105, 
    116, 105, 111, 110,   0, 171, 
    171, 171,  83,  72,  68,  82, 
    252,   0,   0,   0,  64,   0, 
    1,   0,  63,   0,   0,   0, 
    89,   0,   0,   4,  70, 142, 
    32,   0,   0,   0,   0,   0, 
    4,   0,   0,   0,  95,   0, 
    0,   3, 114,  16,  16,   0, 
    0,   0,   0,   0,  95,   0, 
    0,   3,  50,  16,  16,   0, 
    1,   0,   0,   0, 101,   0, 
    0,   3,  50,  32,  16,   0, 
    0,   0,   0,   0, 103,   0, 
    0,   4, 242,  32,  16,   0, 
    1,   0,   0,   0,   1,   0, 
    0,   0, 104,   0,   0,   2, 
    1,   0,   0,   0,  54,   0, 
    0,   5,  50,  32,  16,   0, 
    0,   0,   0,   0,  70,  16, 
    16,   0,   1,   0,   0,   0, 
    56,   0,   0,   8, 242,   0, 
    16,   0,   0,   0,   0,   0, 
    86,  21,  16,   0,   0,   0, 
    0,   0,  70, 142,  32,   0, 
    0,   0,   0,   0,   1,   0, 
    0,   0,  50,   0,   0,  10, 
    242,   0,  16,   0,   0,   0, 
    0,   0,  70, 142,  32,   0, 
    0,   0,   0,   0,   0,   0, 
    0,   0,   6,  16,  16,   0, 
    0,   0,   0,   0,  70,  14, 
    16,   0,   0,   0,   0,   0, 
    50,   0,   0,  10, 242,   0, 
    16,   0,   0,   0,   0,   0, 
    70, 142,  32,   0,   0,   0, 
    0,   0,   2,   0,   0,   0, 
    166,  26,  16,   0,   0,   0, 
    0,   0,  70,  14,  16,   0, 
    0,   0,   0,   0,   0,   0, 
    0,   8, 242,  32,  16,   0, 
    1,   0,   0,   0,  70,  14, 
    16,   0,   0,   0,   0,   0, 
    70, 142,  32,   0,   0,   0, 
    0,   0,   3,   0,   0,   0, 
    62,   0,   0,   1,  83,  84, 
    65,  84, 116,   0,   0,   0, 
    6,   0,   0,   0,   1,   0, 
    0,   0,   0,   0,   0,   0, 
    4,   0,   0,   0,   2,   0, 
    0,   0,   0,   0,   0,   0, 
    0,   0,   0,   0,   1,   0, 
    0,   0,   0,   0,   0,   0, 
    0,   0,   0,   0,   0,   0, 
    0,   0,   0,   0,   0,   0, 
    0,   0,   0,   0,   0,   0, 
    0,   0,   0,   0,   0,   0, 
    0,   0,   0,   0,   0,   0, 
    0,   0,   0,   0,   0,   0, 
    0,   0,   0,   0,   1,   0, 
    0,   0,   0,   0,   0,   0, 
    0,   0,   0,   0,   0,   0, 
    0,   0,   0,   0,   0,   0, 
    0,   0,   0,   0,   0,   0, 
    0,   0,   0,   0,   0,   0, 
    0,   0,   0,   0,   0,   0, 
    0,   0
};
