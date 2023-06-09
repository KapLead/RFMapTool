DirectShow Editing Sample - Still Frame Capture
------------------------------------------------

This C++ app uses the ISampleGrabber interface to capture still images
to a .bmp file on disk from a live capture stream. It demonstrates
how to put the sample grabber into the graph, and how to get the
bits back from it in real time via the callback.  

StillCap starts in preview mode, displaying input from the system's
default video capture device.  Normally, this will be an attached video
camera, like a USB WebCam.  When you click the "Snap Still" button,
a bitmap will be captured and written to disk at the location specified
in the Capture Directory field.  If you enable the "Increment filename"
option, then a new file will be written each time with a new name.
Otherwise, the same filename will be used for each captured bitmap.

You may also capture video files to disk if you select Video Capture
on the Capture Type group box.  Note that video capture will start
automatically when you select this option.  Click "Start Playback"
to end the capture session.  The recorded file will automatically
be played back to the screen.

NOTE: For simplicity, this sample uses RGB24 format for capturing data 
and writing bitmaps.  If your video driver is at 16-bit depth, you may 
notice flicker when running this application.  To resolve this problem, 
set your display's bit depth to 24-bit or 32-bit with the Display Properties
control panel application.

The size of the buffer allocated for storing captured bitmap data is
defined with the BITMAP_DATA_SIZE constant.  It is currently set to 400Kb, 
which is an appropriate size for a bitmap generated from a USB Web camera.
If your camera generates larger images, you will need to increase the
size of the bitmap buffer.  A future version of this sample will 
dynamically determine the appropriate bitmap size to allocate.
