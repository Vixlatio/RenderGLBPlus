## glb-renderer

glb-renderer is a program built in C for rendering 3D objects from GLB files into stunning image renders. This tool leverages OpenGL for rendering and supports various image formats including PNG, JPG, SVG, and JPEG.

### Features

- Renders 3D objects from GLB files, rendering with the embedded textures, colors, etc.
- Supports rendering into multiple image formats including PNG, JPG, SVG, and JPEG.
- Simple command-line interface for ease of use.
- Detailed help command for quick reference.
- Highly performant.
- Cross-platform, runs on Unix-based systems, such as GNU/Linux, macOS, as well as on Windows.
- Automatic camera view framing.
- Custom output folder paths.
- GPU or CPU rendering options.
- Random file name generation.

### Planned Features
- Custom scene backgrounds.
- Custom camera positioning, overriding automatic camera view framing.

### Usage

To render a 3D object from a GLB file, use the following command:

#### Unix-like systems (GNU/Linux, macOS):

```
./glb-renderer -i <GLB_File_Path> [-o <Output_File_Name>] -d <Output_Folder_Path> -f <File_Type> [-m <Render_Mode>]
```

* [] wrapping indicates that the parameter is not required.

#### Windows systems:

```
glb-render.exe -i <GLB_File_Path> [-o <Output_File_Name>] -d <Output_Folder_Path> -f <File_Type> [-m <Render_Mode>]
```

* [] wrapping indicates that the parameter is not required.

### Command-line Options

- **Required:** `-i`, `--input`: Path to the GLB file.
- **Optional:** `-o`, `--output`: Name of the output image file (without the extension). *If not specified, generates a random name.*
- **Required:** `-f`, `--format`: Desired output file type (PNG, JPG, SVG, or JPEG), this determines the output file's extension.
- **Optional:** `-h`, `--help`: Display detailed usage information. *Ignored if other command-line parameters are specified.*
- **Required:** `-d`, `--directory`: Path to the output folder.
- **Optional:** `-m`, `--mode`: Rendering mode (GPU or CPU). *Default = CPU (Recommended).*
   - **Note:** *The program will fail to render and will output an error if your system does not have a proper GPU.*

### Examples

Render a GLB file named `model.glb` into a PNG image on a Unix-like system (GNU/Linux, macOS) using the CPU:
```
./glb-renderer -i model.glb -o test_render -d C:\Users\Username\Documents\Renders -f PNG -m CPU
```

Render a GLB file named `model.glb` into a PNG image on a Windows system using the CPU:
```
glb-renderer.exe -i model.glb -o test_render -d C:\Users\Username\Documents\Renders -f PNG -m CPU
```

### Manual complilation

1. Clone the repository:
   ```
   git clone https://github.com/Vixlatio/glb-renderer.git
   ```

2. Navigate to the project directory:
   ```
   cd glb-renderer
   ```

3. Compile the source code
     Option 1 - Using gcc to build for Unix-like systems (Linux, macOS):
     ```
     gcc -o glb-renderer main.c -lglut -lpng -ljpeg -lm
     ```
     Option 2 - Using gcc to build for Windows systems:
     ```
     gcc -o glb-renderer.exe main.c -lglut32 -lpng -ljpeg -lm
     ```

### Using a prebuilt version

Optionally, you may download and use a prebuilt version of the program directly from the Releases section of this repository.

### Requirements

- OpenGL (GLUT)
- libpng
- libjpeg

### Contributing

Contributions are welcome! If you encounter any issues or have suggestions for improvements, feel free to open an issue or submit a pull request.

### License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
