## RenderGLB+ (NOT READY FOR USE, WILL NOT WORK AS INTENDED)
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

- **Required:** `-i`: Path to the GLB file.
- **Optional:** `-f`: Name of the output image file (without the extension).
   - **Note:** If not specified, the program will generate a random string of alphanumeric characters and will use that as the file's name.
- **Required:** `-t`: Desired output file type - this determines the output file's extension.
   - **Options:** `png`, `jpg`, `jpeg`
- **Optional:** `-h`, `--help`: Display detailed usage information.
   - **Note:** *Ignored if other command-line parameters are specified.*
- **Required:** `-o`: Path to the output folder.
   - **Note:** *Ensure that you have the system user, under which the program will be running, has the correct read/write permissions to the output directory.*
- **Optional:** `-m`: Rendering mode (GPU or CPU).
   - **Default:** `gpu` (Recommended).
   - **Options:** `gpu`, `cpu`.
   - **Note:** *The program will fail to render in GPU mode and will output an error if your system does not have a proper GPU.*

### Examples

Render a GLB file named `model.glb` into a PNG image on a GNU/Linux system using the GPU, where the GLB file is in the same path as the RenderGLB+ program:
```
./renderglbplus -i model.glb -f test_render -o ./ -t PNG -m gpu
```

Render a GLB file named `model.glb` into a PNG image on a Microsoft Windows system using the GPU, where the GLB file is in the same path as the RenderGLB+ program:
```
renderglbplus.exe -i model.glb -f test_render -o ./ -t PNG -m gpu
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
     mkdir build && cd build && cmake .. && make
     ```
     Option 2 - Using g++ to build for Microsoft Windows systems:
     ```
     g++ -o renderglbplus.exe main.cpp stb/stb_image_write.h tinygltf/tiny_gltf.cc -lglut -lGL -lGLU -lpng -ljpeg -lm
     ```

### Using a prebuilt version

Optionally, you may download and use a prebuilt version of the program directly from the Releases section of this repository.

### Requirements

- g++ compiler (Tested with v11.4.0, whilst also using 17 spec)
- libpng
- libjpeg
- libgl-dev
- libglu1-mesa-dev
- libgl1-mesa-dev
- libglfw3-dev
- freeglut3-dev (Linux) __OR__ OpenGL (Windows)

### Prerequisites

- g++ compiler (v11.4.0)
- cmake (v3.29.0)
- libpng
- libjpeg
- libgl-dev
- libglu1-mesa-dev
- libgl1-mesa-dev
- libglfw3-dev
- freeglut3-dev (Linux) __OR__ OpenGL (Windows)

#### GNU/Linux installation

1. **Install prerequisites via apt:**

   `sudo apt-get install freeglut3-dev libglfw3-dev libgl1-mesa-dev libglu1-mesa-dev libgl-dev libpng-dev libjpeg-dev`

   For more information about OpenGL (GLUT), visit their website: [https://www.opengl.org/resources/libraries/glut/glut_downloads.php](https://www.opengl.org/resources/libraries/glut/glut_downloads.php)

#### Microsoft Windows installation

Instructions coming soon.

### Contributing

Contributions are welcome! If you encounter any issues or have suggestions for improvements, feel free to open an issue or submit a pull request.

### License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
