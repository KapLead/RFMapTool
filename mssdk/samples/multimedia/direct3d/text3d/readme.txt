//-----------------------------------------------------------------------------
// Name: Text3D Direct3D Sample
// 
// Copyright (c) 1998-2000 Microsoft Corporation. All rights reserved.
//-----------------------------------------------------------------------------


Description
===========
   The Text3D sample shows how to draw 2D text in a 3D scene. This is most
   useful for display stats, in game menus, etc..


Path
====
   Source:     MSSDK\Samples\Multimedia\D3D\Text3D
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
   This sample uses the common class, CD3DFont, to display 2D text in a 3D
   scene. The souce code for the class can be found in the "common" directory
   (described below), and is of most interest to this sample. The class uses
   GDI to load a font and output each letter to a bitmap. That bitmap, in turn,
   is used to create a texture.

   When the CD3DFont class' DrawText() function is called, a vertex buffer is
   filled with polygons that are textured using the font texture created as 
   mentioned above. The polygons may be drawn as a 2D overlay (useful for
   printing stats, etc.) or truely integrated in the 3D scene.

   This sample makes use of common DirectX code (consisting of helper functions,
   etc.) that is shared with other samples on the DirectX SDK. All common
   headers and source code can be found in the following directory:
      Mssdk\Samples\Multimedia\Common
