//-----------------------------------------------------------------------------
// Name: Meshes Direct3D Tutorial
// 
// Copyright (c) 2000 Microsoft Corporation. All rights reserved.
//-----------------------------------------------------------------------------


Description
===========
   The Textures tutorial shows how to load and render file-based geometry
   meshes in Direct3D.

   
Path
====
   Source:     MSSDK\Samples\Multimedia\VBSamples\D3D\Tutorials\Tut06_Meshes


Programming Notes
=================
   Complicated geometry is usally modelled using 3D modelling software and 
   saved in a file, such as Microsoft's .x file format. Using meshes can be
   somewhat involved, but fortunately D3DX contains functions to help out. This
   tutorial shows how use the D3DX functions to load and render file-based 
   meshes. Note that we still have to handle materials and textures manually.

