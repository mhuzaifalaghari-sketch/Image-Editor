# Qt Image Editor with Python Backend

A image editing application with a C++ Qt GUI frontend and Python image processing backend. Adjust brightness, contrast, and apply black & white filters in real-time with smooth slider controls.

## Features

  **Real-time Image Adjustments**
   Brightness control (-100 to +100)
   Contrast control (-100 to +100)
   Black & White filter
 ** Qt GUI**
   Multi page interface 
   Dockable side menu panel
   Maximized window layout
   Responsive image display with auto-scaling

 **Python Backend**
   NumPy-based image processing
   PIL/Pillow for image manipulation
   Throttled processing for smooth slider experience

## File Paths
**Always use forward slashes (`/`) in image paths**

correct : `C:/Users/Name/photo.jpg`  
wrong : `C:\Users\Name\photo.jpg`


## Prerequisites
- Visual Studio 2022 with C++
- Qt 6
- Python 3 with NumPy & Pillow

## Setup
```bash
pip install numpy pillow
