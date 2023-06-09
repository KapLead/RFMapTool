//-----------------------------------------------------------------------------
// Name: StencilDepth Direct3D Sample
// 
// Copyright (c) 1998-2000 Microsoft Corporation. All rights reserved.
//-----------------------------------------------------------------------------


Description
===========
   The StencilDepth sample uses stencil buffers to display the depth complexity
   of a scene. The depth complextity of a scene is defined as the average
   number of times each pixel is rendered to.
   
   Stencil buffers are a depth buffer technique that can be updated as
   geometry is rendered, and used again as a mask for drawing more geometry.
   Common effects include mirrors, shadows (an advanced technique), dissolves,
   etc..

   Note that not all cards support all features for all the various stencil
   buffer techniques (some hardware has no, or limited, stencil buffer
   support). For more information on stencil buffers, refer to the DirectX SDK
   documentation. 


Path
====
   Source:     MSSDK\Samples\Multimedia\D3D\StencilBuffer\StencilDepth
   Executable: MSSDK\Samples\Multimedia\D3D\Bin


User's Guide
============
   The following keys are implemented. The dropdown menus can be used for the
   same controls.
      <Enter>     Starts and stops the scene
      <Space>     Advances the scene by a small increment
      <F1>        Shows help or available commands.
      <F2>        Prompts user to select a new rendering device or display mode
      <Alt+Enter> Toggles between fullscreen and windowed modes
      <Esc>       Exits the app.


Programming Notes
=================
   Displaying depth complexity is a valuable tool to analyze the performance of
   a scene. Scenes with high amounts of overdraw could benefit from some scene
   optimization, such as sorting the geometry in a front-to-back order.

   Note that not all cards support all features for all the various stencil
   buffer techniques (some hardware has no, or limited, stencil buffer
   support). For more information on stencil buffers, refer to the DirectX SDK
   documentation. 

   This sample makes use of common DirectX code (consisting of helper functions,
   etc.) that is shared with other samples on the DirectX SDK. All common
   headers and source code can be found in the following directory:
      Mssdk\Samples\Multimedia\Common
