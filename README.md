## RenderGLB+ (Experimental)
### An open-source project by Vixlatio PTY LTD

RenderGLB+ is a program built in C++ for rendering 3D objects from GLB files (and other file types soon!) into stunning image renders. This tool leverages OpenGL (GLUT) for rendering and supports various image formats including PNG, JPG, and JPEG. This program supports, both, GNU/LINUX and Microsoft Windows.

### Features

- Renders 3D objects from GLB files, rendering with the embedded textures, colors, etc.
- Supports rendering into multiple image formats including PNG, JPG, and JPEG.
- Simple command-line interface for ease of use.
- Detailed help command for quick reference.
- Highly performant.
- Cross-platform, runs on GNU/Linux as well as on Microsoft Windows.
- Automatic camera view framing.
- Custom output folder paths.
- Random file name generation.
- NVIDIA GPU and AMD GPU support.
- Threading.

### Planned Features
- Custom scene backgrounds.
- Custom camera positioning, overriding automatic camera view framing.
- Importing multiple GLB files into the scene and rendering them all as one image.
- CPU rendering.

### Usage

To render a 3D object from a GLB file, use the following command:

#### GNU/Linux:

```
./renderglbplus -i <GLB_File_Path> [-o <Output_File_Name>] -d <Output_Folder_Path> -f <File_Type> [-m <Render_Mode>]
```

* [] wrapping indicates that the parameter is not required.

#### Microsoft Windows systems:

```
renderglbplus.exe -i <GLB_File_Path> [-o <Output_File_Name>] -d <Output_Folder_Path> -f <File_Type> [-m <Render_Mode>]
```

* [] wrapping indicates that the parameter is not required.

### Command-line Options

- **Required:** `-i`, `--input`: Path to the GLB file.
- **Optional:** `-o`, `--output`: Name of the output image file (without the extension).
   - **Note:** If not specified, the program will generate a random string of alphanumeric characters and will use that as the file's name.
- **Required:** `-f`, `--format`: Desired output file type - this determines the output file's extension.
   - **Options:** `PNG`, `JPG`, `JPEG`
- **Optional:** `-h`, `--help`: Display detailed usage information.
   - **Note:** *Ignored if other command-line parameters are specified.*
- **Required:** `-d`, `--directory`: Path to the output folder.
   - **Note:** *Ensure that you have the system user, under which the program will be running, has the correct read/write permissions to the output directory.*
- **Optional:** `-m`, `--mode`: Rendering mode (GPU or CPU).
   - **Default:** `GPU` (Recommended).
   - **Options:** `GPU`, `CPU`.
   - **Note:** *The program will fail to render and will output an error if your system does not have a proper GPU.*

### Examples

Render a GLB file named `model.glb` into a PNG image on a GNU/Linux system using the GPU, where the GLB file is in the same path as the RenderGLB+ program:
```
./renderglbplus -i model.glb -o test_render -d C:\Users\Username\Documents\Renders -f PNG -m GPU
```

Render a GLB file named `model.glb` into a PNG image on a Microsoft Windows system using the GPU, where the GLB file is in the same path as the RenderGLB+ program:
```
renderglbplus.exe -i model.glb -o test_render -d C:\Users\Username\Documents\Renders -f PNG -m GPU
```

### Manual complilation

1. Clone the repository:
   ```
   git clone https://github.com/Vixlatio/renderglbplus.git
   ```

2. Navigate to the project directory:
   ```
   cd renderglbplus
   ```

3. Compile the source code
     Option 1 - Using g++ to build for GNU/Linux:
     ```
     g++ -o renderglbplus main.cpp -lglut -lpng -ljpeg -lm
     ```
     Option 2 - Using g++ to build for Microsoft Windows systems:
     ```
     g++ -o renderglbplus.exe main.cpp -lglut32 -lpng -ljpeg -lm
     ```

### Using a prebuilt version

Optionally, you may download and use a prebuilt version of the program directly from the Releases section of this repository.

### Requirements

- OpenGL (GLUT)
- CUDA Toolkit
- libpng
- libjpeg

#### GNU/Linux installation

1. **Install OpenGL (GLUT) via apt:**

   `sudo apt-get install freeglut3-dev libglfw3-dev libgl1-mesa-dev libglu1-mesa-dev libgl-dev`

   For more information about OpenGL (GLUT), visit their website: [https://www.opengl.org/resources/libraries/glut/glut_downloads.php](https://www.opengl.org/resources/libraries/glut/glut_downloads.php)
2. **Install libpng via apt:**

   `sudo apt-get install libpng-dev`
3. **Install libjpeg via apt:**

   `sudo apt-get install libjpeg-dev`

#### Microsoft Windows installation

1. **Install OpenGL (GLUT):**

   Instructions coming soon.

   For more information about OpenGL (GLUT), visit their website: [https://www.opengl.org/resources/libraries/glut/glut_downloads.php](https://www.opengl.org/resources/libraries/glut/glut_downloads.php)
2. **Install libpng:**

   Instructions coming soon.
3. **Install libjpeg:**

   Instructions coming soon.

### Contributing

Contributions are welcome! If you encounter any issues or have suggestions for improvements, feel free to open an issue or submit a pull request.

### License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
