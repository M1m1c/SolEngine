#pragma once
#include "Interfaces/Shader.h"
#include "Interfaces/Buffer.h"
#include "Interfaces/VertexArray.h"
#include "Interfaces/Renderer.h"
#include "Interfaces/RenderCommand.h"
#include "Interfaces/RenderingContext.h"
#include "Interfaces/Texture.h"
#include "Interfaces/OrthoCamera.h"
#include "Interfaces/Framebuffer.h"
#include "RendererCamera.h"
#include "SceneCamera.h"

// This file is meant to be the only file you need to include to use GalaxyDraw.
// It should mostly contain typedefs and macros for ease of use outside of GalaxyDraw.
/////////////////////////////////////////////////////

#define GD_ GalaxyDraw

//Defined in Shader.h
/////////////////////////////////////////////////////
typedef GalaxyDraw::Shader GD_Shader;
typedef GalaxyDraw::ShaderLibrary GD_ShaderLibrary;
/////////////////////////////////////////////////////


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


//Defined in Renderer.h
/////////////////////////////////////////////////////
typedef GalaxyDraw::Renderer GD_Renderer;
/////////////////////////////////////////////////////


//Defined in RenderCommand.h
/////////////////////////////////////////////////////
typedef GalaxyDraw::RenderCommand GD_RenderCommand;
/////////////////////////////////////////////////////

//Defined in RenderingContext.h
/////////////////////////////////////////////////////
typedef GalaxyDraw::RenderingContext GD_RenderingContext;
/////////////////////////////////////////////////////

//Defined in Texture.h
/////////////////////////////////////////////////////
typedef GalaxyDraw::Texture GD_Texture;
typedef GalaxyDraw::Texture2D GD_Texture2D;
/////////////////////////////////////////////////////

//Defined in OrthoCamera.h
/////////////////////////////////////////////////////
typedef GalaxyDraw::OrthoCamera GD_OrthoCamera;
/////////////////////////////////////////////////////

//Defined in RendererCamera.h
/////////////////////////////////////////////////////
typedef GalaxyDraw::RendererCamera GD_RendererCamera;
/////////////////////////////////////////////////////

//Defined in SceneCamera.h
/////////////////////////////////////////////////////
typedef GalaxyDraw::SceneCamera GD_SceneCamera;
/////////////////////////////////////////////////////

//Defined in Framebuffer.h
/////////////////////////////////////////////////////
typedef GalaxyDraw::Framebuffer GD_Framebuffer;
typedef GalaxyDraw::FramebufferProperties GD_FramebufferProps;
/////////////////////////////////////////////////////