#pragma once
#include "shaderClass.h"
#include "Buffer.h"
#include "VertexArray.h"

// This file is meant to be the only file you need to include to use GalaxyDraw.
// It should mostly contain typedefs and macros for ease of use outside of GalaxyDraw.
/////////////////////////////////////////////////////

#define GD_ GalaxyDraw


//Defined in Buffer.h
/////////////////////////////////////////////////////
typedef GalaxyDraw::VertexBuffer GD_VBO;
typedef GalaxyDraw::IndexBuffer GD_EBO;
typedef GalaxyDraw::ShaderDataType GD_ShaderDataType;
typedef GalaxyDraw::BufferLayout GD_BufferLayout;
/////////////////////////////////////////////////////


//Defined in VertexArray.h
/////////////////////////////////////////////////////
typedef GalaxyDraw::VertexArray GD_VAO;
/////////////////////////////////////////////////////