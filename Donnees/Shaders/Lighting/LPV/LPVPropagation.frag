#version 130

#define DO_OCCLUSION
// Precision qualifier
precision highp float;

// Textures
uniform sampler2D LPVRed;
uniform sampler2D LPVGreen;
uniform sampler2D LPVBlue;
uniform sampler2D Occlusion;

// Parametre Grid
uniform vec4 LPVCellSize; // xyz = cell size, w = cell count
uniform vec4 LPVSize;
float RangeModifier = 1.0;

uniform bool DoOcclusion;

// Entree
smooth in vec2 outTexCoord;

// Sortie
out vec4 GridRed;
out vec4 GridGreen;
out vec4 GridBlue;

//solid Angles
// 0.4006696846f / 4Pi;
//#define directFaceSubtendedSolidAngle 0.03188428
// 0.4234413544f / 4Pi;
//#define sideFaceSubtendedSolidAngle 0.03369559

#define directFaceSubtendedSolidAngle 0.12753712
#define sideFaceSubtendedSolidAngle 0.13478556

#include <LPVSH.shadercode>

vec2 GetSamplePos2DOffset3D(in vec2 coords, in vec3 offset){
  vec2 c = coords * LPVSize.xy;
  float row = floor(c.y / LPVCellSize.w);
  float col = floor(c.x / LPVCellSize.w);
  c = clamp(vec2(c.x - col * LPVCellSize.w,c.y - row * LPVCellSize.w) + offset.xy,0.0,LPVCellSize.w-1.0);
  col += offset.z;
  float coldiff = min(col,0.0) + max(0.0,col-LPVSize.z+1.0);
  float newrow = clamp(row+coldiff,0.0,LPVSize.w-1.0);
  col += (min(coldiff,0.0) * (-LPVSize.z) + max(coldiff,0.0) * (-LPVSize.z)) * abs(newrow - row);
  col = clamp(col,0.0,LPVSize.z-1.0);
  vec2 spos = vec2(col * LPVCellSize.w + c.x,newrow * LPVCellSize.w + c.y) / LPVSize.xy;
  return spos;
}

vec4 Sample2DOffset3D(in sampler2D s, in vec2 coords, in vec3 offset){
  return texture2D(s,GetSamplePos2DOffset3D(coords,offset));
}

//1 / sqrt(5)
#define side1 0.447213595
//2 / sqrt(5)
#define side2 0.894427190

void propagate(in vec2 pos, in mat3 orientation, inout vec4 outputRed, inout vec4 outputGreen, inout vec4 outputBlue){
  //evaluate main direction
  vec3 MainDirection = orientation * vec3(0.0,0.0,1.0);
  vec4 MainDirectionSH = SH_evaluate(MainDirection * RangeModifier);
  vec4 MainDirectionHemi = SH_evaluateCosineLobe_direct(MainDirection);

  //Get neighbour SHs
  vec2 loadPos = GetSamplePos2DOffset3D(pos,-MainDirection);
  vec4 NeighbourRed = texture2D(LPVRed,loadPos);
  vec4 NeighbourGreen = texture2D(LPVGreen,loadPos);
  vec4 NeighbourBlue = texture2D(LPVBlue,loadPos);

  //Get Occlusion SH
  vec4 occlusionSH;
  if(DoOcclusion)
  {
  //vec4 occlusionSH = Load2DOffset3D(Occlusion,pos,vec3(0,0,0));
  occlusionSH = Sample2DOffset3D(Occlusion,pos,-MainDirection * 0.5);
  //vec4 occlusionSH = Sample2DOffset3D(Occlusion,pos,vec3(0,0,0));
  }

  float occlusionFactor = 1.0;

  //Evaluate main face
  float fluxRed = max(0.0,dot(MainDirectionSH,NeighbourRed));
  float fluxGreen = max(0.0,dot(MainDirectionSH,NeighbourGreen));
  float fluxBlue = max(0.0,dot(MainDirectionSH,NeighbourBlue));
  if(DoOcclusion)
  {
  occlusionFactor = 1.0 - min( SHDotAbs(SH_evaluate(-MainDirection),occlusionSH),1.0);
  occlusionFactor *= occlusionFactor;
  //occlusionFactor = 1.0 - clamp( dot(SH_evaluate(-MainDirection),occlusionSH),0.0,1.0);
  }
  vec4 resultRed = MainDirectionHemi * fluxRed * directFaceSubtendedSolidAngle * occlusionFactor;
  vec4 resultGreen = MainDirectionHemi * fluxGreen * directFaceSubtendedSolidAngle * occlusionFactor;
  vec4 resultBlue = MainDirectionHemi * fluxBlue * directFaceSubtendedSolidAngle * occlusionFactor;


  //evaluate side faces

  vec3 SideDirection = vec3(side1,0.0,side2) * orientation;
  vec3 ReproDirection = vec3(1.0,0.0,0.0) * orientation;
  vec4 SideDirectionSH = SH_evaluate(SideDirection * RangeModifier);
  vec4 SideDirectionHemi = SH_evaluateCosineLobe_direct(ReproDirection);
  fluxRed = max(0.0,dot(SideDirectionSH,NeighbourRed));
  fluxGreen = max(0.0,dot(SideDirectionSH,NeighbourGreen));
  fluxBlue = max(0.0,dot(SideDirectionSH,NeighbourBlue));
  if(DoOcclusion)
  {
  occlusionFactor = 1.0 - min( SHDotAbs(SH_evaluate(-SideDirection),occlusionSH),1.0);
  occlusionFactor *= occlusionFactor;
  //occlusionFactor = 1.0 - clamp( dot(SH_evaluate(-SideDirection),occlusionSH),0.0,1.0);
  }
  resultRed += SideDirectionHemi * fluxRed * directFaceSubtendedSolidAngle * occlusionFactor;
  resultGreen += SideDirectionHemi * fluxGreen * directFaceSubtendedSolidAngle * occlusionFactor;
  resultBlue += SideDirectionHemi * fluxBlue * directFaceSubtendedSolidAngle * occlusionFactor;

  SideDirection = vec3(-side1,0.0,side2) * orientation;
  ReproDirection = vec3(-1.0,0.0,0.0) * orientation;
  SideDirectionSH = SH_evaluate(SideDirection * RangeModifier);
  SideDirectionHemi = SH_evaluateCosineLobe_direct(ReproDirection);
  fluxRed = max(0.0,dot(SideDirectionSH,NeighbourRed));
  fluxGreen = max(0.0,dot(SideDirectionSH,NeighbourGreen));
  fluxBlue = max(0.0,dot(SideDirectionSH,NeighbourBlue));
  if(DoOcclusion)
  {
  occlusionFactor = 1.0 - min( SHDotAbs(SH_evaluate(-SideDirection),occlusionSH),1.0);
  occlusionFactor *= occlusionFactor;
  //occlusionFactor = 1.0 - clamp( dot(SH_evaluate(-SideDirection),occlusionSH),0.0,1.0);
  }
  resultRed += SideDirectionHemi * fluxRed * directFaceSubtendedSolidAngle * occlusionFactor;
  resultGreen += SideDirectionHemi * fluxGreen * directFaceSubtendedSolidAngle * occlusionFactor;
  resultBlue += SideDirectionHemi * fluxBlue * directFaceSubtendedSolidAngle * occlusionFactor;

  SideDirection = vec3(0.0,side1,side2) * orientation;
  ReproDirection = vec3(0.0,1.0,0.0) * orientation;
  SideDirectionSH = SH_evaluate(SideDirection * RangeModifier);
  SideDirectionHemi = SH_evaluateCosineLobe_direct(ReproDirection);
  fluxRed = max(0.0,dot(SideDirectionSH,NeighbourRed));
  fluxGreen = max(0.0,dot(SideDirectionSH,NeighbourGreen));
  fluxBlue = max(0.0,dot(SideDirectionSH,NeighbourBlue));
  if(DoOcclusion)
  {
  occlusionFactor = 1.0 - min( SHDotAbs(SH_evaluate(-SideDirection),occlusionSH),1.0);
  occlusionFactor *= occlusionFactor;
  //occlusionFactor = 1.0 - clamp( dot(SH_evaluate(-SideDirection),occlusionSH),0.0,1.0);
  }
  resultRed += SideDirectionHemi * fluxRed * directFaceSubtendedSolidAngle * occlusionFactor;
  resultGreen += SideDirectionHemi * fluxGreen * directFaceSubtendedSolidAngle * occlusionFactor;
  resultBlue += SideDirectionHemi * fluxBlue * directFaceSubtendedSolidAngle * occlusionFactor;

  SideDirection = vec3(0.0,-side1,side2) * orientation;
  ReproDirection = vec3(0.0,-1.0,0.0) * orientation;
  SideDirectionSH = SH_evaluate(SideDirection * RangeModifier);
  SideDirectionHemi = SH_evaluateCosineLobe_direct(ReproDirection);
  fluxRed = max(0.0,dot(SideDirectionSH,NeighbourRed));
  fluxGreen = max(0.0,dot(SideDirectionSH,NeighbourGreen));
  fluxBlue = max(0.0,dot(SideDirectionSH,NeighbourBlue));
  if(DoOcclusion)
  {
  occlusionFactor = 1.0 - min( SHDotAbs(SH_evaluate(-SideDirection),occlusionSH),1.0);
  occlusionFactor *= occlusionFactor;
  //occlusionFactor = 1.0 - clamp( dot(SH_evaluate(-SideDirection),occlusionSH),0.0,1.0);
  }
  resultRed += SideDirectionHemi * fluxRed * directFaceSubtendedSolidAngle * occlusionFactor;
  resultGreen += SideDirectionHemi * fluxGreen * directFaceSubtendedSolidAngle * occlusionFactor;
  resultBlue += SideDirectionHemi * fluxBlue * directFaceSubtendedSolidAngle * occlusionFactor;

  outputRed += resultRed;// * RangeModifier;
  outputGreen += resultGreen;// * RangeModifier;
  outputBlue += resultBlue;// * RangeModifier;
}

void main(){
  vec4 resultRed = vec4(0.0,0.0,0.0,0.0);
  vec4 resultGreen = vec4(0.0,0.0,0.0,0.0);
  vec4 resultBlue = vec4(0.0,0.0,0.0,0.0);
  //Z+
  propagate(outTexCoord,mat3( 1.0, 0.0, 0.0,
                            0.0, 1.0, 0.0,
                            0.0, 0.0, 1.0),resultRed,resultGreen,resultBlue);
  //Z-
  propagate(outTexCoord,mat3(-1.0, 0.0, 0.0,
                            0.0, 1.0, 0.0,
                            0.0, 0.0,-1.0),resultRed,resultGreen,resultBlue);
  //X+
  propagate(outTexCoord,mat3( 0.0, 0.0, 1.0,
                            0.0, 1.0, 0.0,
                           -1.0, 0.0, 0.0),resultRed,resultGreen,resultBlue);
  //X-
  propagate(outTexCoord,mat3( 0.0, 0.0,-1.0,
                            0.0, 1.0, 0.0,
                            1.0, 0.0, 0.0),resultRed,resultGreen,resultBlue);
  //Y+
  propagate(outTexCoord,mat3( 1.0, 0.0, 0.0,
                            0.0, 0.0, 1.0,
                            0.0,-1.0, 0.0),resultRed,resultGreen,resultBlue);
  //Y-
  propagate(outTexCoord,mat3( 1.0, 0.0, 0.0,
                            0.0, 0.0,-1.0,
                            0.0, 1.0, 0.0),resultRed,resultGreen,resultBlue);
  //Data for next propagation step
  GridRed = resultRed*2;
  GridGreen = resultGreen*2;
  GridBlue = resultBlue*2;

//  vec2 loadPos = GetSamplePos2DOffset3D(outTexCoord,vec3(0.0,0.0,0.0));
//  GridRed += texture2D(LPVRed, loadPos);
//  GridGreen += texture2D(LPVGreen, loadPos);
//  GridBlue += texture2D(LPVBlue, loadPos);
//  //Final data
//  #ifdef FIRST_STEP
//    //On first step add initial injected data
//    ivec2 loadPos = GetLoadPos2DOffset3D(TexCoords,vec3(0.0,0.0,0.0));
//    resultRed += texelFetch2D(LPVRed, loadPos, 0);
//    resultGreen += texelFetch2D(LPVRed, loadPos, 0);
//    resultBlue += texelFetch2D(LPVRed, loadPos, 0);
//  #endif
//  GridRed[3] = resultRed;
//  GridRed[4] = resultGreen;
//  GridRed[5] = resultBlue;
}
