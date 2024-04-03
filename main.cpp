#include <GL/glut.h> // For OpenGL rendering
#include <iostream> // For standard I/O operations
#include <memory> // For smart pointers
#include <cstring> // For string operations
#include <random> // For generating random file names
#include <dirent.h> // For directory operations
#include <cstdio>   // For file I/O
#include <unistd.h> // For getopt and optarg
#include "stb/stb_image_write.h" // For STB image write operations
#include "tinygltf/tiny_gltf.h" // For loading GLB files
#include <sys/stat.h> // For checking file existence
#include <cmath> // For mathematical operations
#include <GL/glu.h> // For GLU functions

#ifdef _WIN32
#include <Windows.h> // For Windows specific operations
#endif

struct RenderParams {
    std::string glb_file_path;
    std::string output_file_name;
    std::string output_folder_path;
    std::string file_type; // Options: png, jpg, jpeg
    std::string render_mode; // Default: gpu. Options: cpu, gpu
};

std::string to_lowercase(const std::string &str) {
    std::string result;
    for (char c: str) {
        result += std::tolower(c);
    }
    return result;
}

std::string generate_unique_output_file_name(const std::string &output_folder_path) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 35);

    std::string output_file_name;

    while (true) {
        output_file_name.clear(); // Clear the string for generating a new name
        for (int i = 0; i < 32; ++i) {
            output_file_name.push_back("abcdefghijklmnopqrstuvwxyz0123456789"[dis(gen)]);
        }

        // Check for existence of file with the same name, if found, generate a new name
        DIR *dir;
        struct dirent *ent;
        bool found = false;
        if ((dir = opendir(output_folder_path.c_str())) != nullptr) {
            while ((ent = readdir(dir)) != nullptr) {
                if (ent->d_type == DT_REG && std::strcmp(ent->d_name, output_file_name.c_str()) == 0) {
                    found = true;
                    break;
                }
            }
            closedir(dir);
        } else {
            std::cerr << "\nError opening output directory.\n";
            exit(EXIT_FAILURE);
        }

        if (!found) {
            break;
        }
    }

    return output_file_name;
}

// Calculate the bounding box of the scene
void calculateBoundingBox(const tinygltf::Model &model, float &minX, float &minY, float &minZ, float &maxX, float &maxY,
                          float &maxZ) {
    minX = minY = minZ = std::numeric_limits<float>::max();
    maxX = maxY = maxZ = std::numeric_limits<float>::min();

    for (const auto &mesh: model.meshes) {
        for (const auto &primitive: mesh.primitives) {
            const tinygltf::Accessor &accessor = model.accessors[primitive.attributes.at("POSITION")];
            const tinygltf::BufferView &bufferView = model.bufferViews[accessor.bufferView];
            const tinygltf::Buffer &buffer = model.buffers[bufferView.buffer];
            const float *positions = reinterpret_cast<const float *>(&buffer.data[accessor.byteOffset + bufferView.byteOffset]);

            for (size_t i = 0; i < accessor.count; ++i) {
                float x = positions[i * 3];
                float y = positions[i * 3 + 1];
                float z = positions[i * 3 + 2];

                minX = std::min(minX, x);
                minY = std::min(minY, y);
                minZ = std::min(minZ, z);
                maxX = std::max(maxX, x);
                maxY = std::max(maxY, y);
                maxZ = std::max(maxZ, z);
            }
        }
    }
}

// Set up the camera based on the bounding box
void setupCamera(float minX, float minY, float minZ, float maxX, float maxY, float maxZ) {
    // Calculate center of the bounding box
    float centerX = (minX + maxX) / 2.0f;
    float centerY = (minY + maxY) / 2.0f;
    float centerZ = (minZ + maxZ) / 2.0f;

    // Calculate distance from the camera to the bounding box corners
    float distanceX = std::abs(maxX - minX) / 2.0f;
    float distanceY = std::abs(maxY - minY) / 2.0f;
    float distanceZ = std::abs(maxZ - minZ) / 2.0f;
    float distance = std::sqrt(distanceX * distanceX + distanceY * distanceY + distanceZ * distanceZ);

    // Set up the camera
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(centerX, centerY, centerZ + distance, // Eye position
              centerX, centerY, centerZ, // Look-at position
              0.0f, 1.0f, 0.0f); // Up direction
}

// Initialize OpenGL context and perform rendering via GPU
void render_glb_with_opengl_gpu(const char *glb_file_path, const char *output_folder_path, const char *output_file_name,
                                const char *file_type) {
    int argc = 1;
    char *argv[] = {strdup("RenderGLB+"), nullptr}; // strdup() duplicates the string

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("RenderGLB+");
    glutInitDisplayString("rgba double depth>=24");

    tinygltf::Model model;
    tinygltf::TinyGLTF loader;
    std::string err;
    std::string warn;

    // Load the GLB file
    bool ret = loader.LoadBinaryFromFile(&model, &err, &warn, glb_file_path);

    if (!warn.empty()) std::cerr << "\nWarning: " << warn << "\n";
    if (!err.empty()) std::cerr << "\nError: " << err << "\n";
    // Check if the GLB file is loaded successfully
    if (!ret) {
        std::cerr << "\nFailed to load GLB file.\n";
        exit(EXIT_FAILURE);
    }

    // Set up the camera
    float minX, minY, minZ, maxX, maxY, maxZ;
    calculateBoundingBox(model, minX, minY, minZ, maxX, maxY, maxZ);
    setupCamera(minX, minY, minZ, maxX, maxY, maxZ);

    // Set up lighting
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    glShadeModel(GL_SMOOTH);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClearDepth(1.0f);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    // Render the GLB file
    for (size_t i = 0; i < model.meshes.size(); ++i) {
        const tinygltf::Mesh &mesh = model.meshes[i];

        for (size_t j = 0; j < mesh.primitives.size(); ++j) {
            const tinygltf::Primitive &primitive = mesh.primitives[j];

            if (primitive.indices < 0) continue;

            const tinygltf::Accessor &index_accessor = model.accessors[primitive.indices];
            const tinygltf::BufferView &index_buffer_view = model.bufferViews[index_accessor.bufferView];
            const tinygltf::Buffer &index_buffer = model.buffers[index_buffer_view.buffer];
            const void *index_data = &index_buffer.data[index_accessor.byteOffset + index_buffer_view.byteOffset];

            std::vector<unsigned short> indices;

            if (index_accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT) {
                indices.assign((unsigned short *) index_data, (unsigned short *) ((char *) index_data +
                                                                                  index_accessor.count *
                                                                                  sizeof(unsigned short)));
            } else if (index_accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT) {
                const unsigned int *data = (unsigned int *) index_data;
                indices.assign(data, data + index_accessor.count);
            }

            const tinygltf::Accessor &position_accessor = model.accessors[primitive.attributes.at("POSITION")];
            const tinygltf::BufferView &position_buffer_view = model.bufferViews[position_accessor.bufferView];
            const tinygltf::Buffer &position_buffer = model.buffers[position_buffer_view.buffer];
            const void *position_data = &position_buffer.data[position_accessor.byteOffset +
                                                              position_buffer_view.byteOffset];
            const tinygltf::Accessor &normal_accessor = model.accessors[primitive.attributes.at("NORMAL")];
            const tinygltf::BufferView &normal_buffer_view = model.bufferViews[normal_accessor.bufferView];
            const tinygltf::Buffer &normal_buffer = model.buffers[normal_buffer_view.buffer];
            const void *normal_data = &normal_buffer.data[normal_accessor.byteOffset + normal_buffer_view.byteOffset];
            const tinygltf::Accessor &texcoord_accessor = model.accessors[primitive.attributes.at("TEXCOORD_0")];
            const tinygltf::BufferView &texcoord_buffer_view = model.bufferViews[texcoord_accessor.bufferView];
            const tinygltf::Buffer &texcoord_buffer = model.buffers[texcoord_buffer_view.buffer];
            const void *texcoord_data = &texcoord_buffer.data[texcoord_accessor.byteOffset +
                                                              texcoord_buffer_view.byteOffset];

            glEnableClientState(GL_VERTEX_ARRAY);
            glEnableClientState(GL_NORMAL_ARRAY);
            glEnableClientState(GL_TEXTURE_COORD_ARRAY);
            glVertexPointer(3, GL_FLOAT, 0, position_data);
            glNormalPointer(GL_FLOAT, 0, normal_data);
            glTexCoordPointer(2, GL_FLOAT, 0, texcoord_data);
            glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_SHORT, indices.data());
            glDisableClientState(GL_VERTEX_ARRAY);
            glDisableClientState(GL_NORMAL_ARRAY);
            glDisableClientState(GL_TEXTURE_COORD_ARRAY);
            glFlush();
            glReadBuffer(GL_FRONT);

            std::vector<unsigned char> pixels(800 * 600 * 3);
            glReadPixels(0, 0, 800, 600, GL_RGB, GL_UNSIGNED_BYTE, pixels.data());

            std::string output_file_path = output_folder_path;
            output_file_path += output_file_name;
            output_file_path += ".";
            output_file_path += file_type;
            FILE *file = fopen(output_file_path.c_str(), "wb");

            // Check if the file is opened successfully
            if (file == nullptr) {
                std::cerr << "\nFailed to open output file.\n";
                exit(EXIT_FAILURE);
            }
            // Check the file type and write the image
            if (std::strcmp(file_type, "png") == 0) {
                stbi_write_png_to_func([](void *context, void *data, int size) {
                    fwrite(data, 1, size, (FILE *) context);
                }, file, 800, 600, 3, pixels.data(), 0);
            } else if (std::strcmp(file_type, "jpg") == 0 || std::strcmp(file_type, "jpeg") == 0) {
                stbi_write_jpg_to_func([](void *context, void *data, int size) {
                    fwrite(data, 1, size, (FILE *) context);
                }, file, 800, 600, 3, pixels.data(), 0);
            } else {
                std::cerr << "\nUnsupported file type.\n";
                exit(EXIT_FAILURE);
            }

            // Close the file
            fclose(file);

            std::cout << "\nRendering completed successfully.\n";

            break;
        }
    }
}

void *render_glb(void *args) {
    auto *params = reinterpret_cast<RenderParams *>(args);

    if (params->output_file_name.empty()) {
        // If a file name is not provided, generate a unique one
        params->output_file_name = generate_unique_output_file_name(params->output_folder_path);
    } else {
        // Check if a file with the same name already exists
        DIR *dir;
        struct dirent *ent;

        if ((dir = opendir(params->output_folder_path.c_str())) != nullptr) {
            while ((ent = readdir(dir)) != nullptr) {
                if (ent->d_type == DT_REG && std::strcmp(ent->d_name, params->output_file_name.c_str()) == 0) {
                    std::cerr << "\nFile with same name found. Please provide a unique output file name.\n";
                    pthread_exit(nullptr);
                }
            }

            closedir(dir);
        } else {
            std::cerr << "\nError opening output directory. Please double check the path and directory permissions.\n";
            exit(EXIT_FAILURE);
        }
    }

    std::cout << "\nRequest to render " << params->glb_file_path << " into " << params->output_folder_path
              << params->output_file_name << "." << params->file_type << " using " << params->render_mode
              << " mode...\n";

    // Choose the rendering mode
    if (params->render_mode == "cpu") {
        std::cerr
                << "\nCPU rendering is not supported in this version of RenderGLB+. Perhaps, check if there is a newer version: https://github.com/Vixlatio/RenderGLBPlus.\n";
        exit(EXIT_FAILURE);
    } else {
        std::cout << "\nRendering using GPU, please wait...\n";
        render_glb_with_opengl_gpu(params->glb_file_path.c_str(), params->output_folder_path.c_str(),
                                   params->output_file_name.c_str(), params->file_type.c_str());
    }

    pthread_exit(nullptr);
}

// Print usage information
void print_usage(const char *program_name) {
    std::cout << "\nUsage: " << program_name
              << " -i <GLB_File_Path> [-o <Output_File_Name>] -d <Output_Folder_Path> -f <File_Type> [-m <Render_Mode>]\n\n";
    std::cout << "Options:\n";
    std::cout << "  -i <GLB_File_Path>\t\tPath to the GLB file to render (required)\n";
    std::cout << "  -f <Output_File_Name>\t\tName of the output file (optional)\n";
    std::cout << "  -o <Output_Folder_Path>\tPath to the output folder (required)\n";
    std::cout << "  -t <File_Type>\t\tType of the output file (default: png)\n";
    std::cout << "  -m <Render_Mode>\t\tMode of rendering (default: gpu)\n";
    std::cout << "  -h\t\t\t\tPrint this help message\n";
    std::cout << "\nFile Types:\n";
    std::cout << "  png\t\tPNG image file\n";
    std::cout << "  jpg\t\tJPEG image file\n";
    std::cout << "  jpeg\t\tJPEG image file\n";
    std::cout << "\nRender Modes:\n";
    std::cout << "  cpu\t\tCPU rendering\n";
    std::cout << "  gpu\t\tGPU rendering\n";
    std::cout << std::endl;
}

int main(int argc, char **argv) {
    std::string glb_file_path;
    std::string output_folder_path;
    std::string output_file_name;
    std::string file_type = "png";
    std::string render_mode = "gpu";

    int opt;
    while ((opt = getopt(argc, argv, "i:o:f:t:r:h:")) != -1) {
        switch (opt) {
            case '?':
            case 'h':
                print_usage(argv[0]);
                exit(EXIT_SUCCESS);
            case 'i':
                glb_file_path = optarg;
                break;
            case 'o':
                output_folder_path = optarg;
                break;
            case 'f':
                output_file_name = optarg;
                break;
            case 't':
                file_type = to_lowercase(optarg);
                if (file_type != "png" && file_type != "jpg" && file_type != "jpeg") {
                    std::cerr << "\nUnsupported file type. Please use 'png', 'jpg', or 'jpeg'.\n";
                    exit(EXIT_FAILURE);
                }
                break;
            case 'r':
                render_mode = to_lowercase(optarg);
                if (render_mode != "cpu" && render_mode != "gpu") {
                    std::cerr << "\nUnsupported render mode. Please use 'cpu' or 'gpu'.\n";
                    exit(EXIT_FAILURE);
                }
                break;
            default:
                std::cerr << "\nUsage: " << argv[0] << " -i input_glb_file -o output_folder [-f output_file_name] [-t file_type] [-r render_mode]\n";
                exit(EXIT_FAILURE);
        }
    }

    if (glb_file_path.empty() || output_folder_path.empty()) {
        std::cerr << "\nPlease provide input GLB file and output folder.\n";
        exit(EXIT_FAILURE);
    }

    // Ensure the output folder path ends with a '/'
    if (output_folder_path.back() != '/') {
        output_folder_path += '/';
    }

#ifdef _WIN32
    if (output_folder_path.find('\\') != std::string::npos) {
        // Replace backslashes with forward slashes for Windows paths
        std::replace(output_folder_path.begin(), output_folder_path.end(), '\\', '/');
    }
#endif

    RenderParams *params = new RenderParams{glb_file_path, output_file_name, output_folder_path, file_type,
                                            render_mode};

    pthread_t render_thread;
    pthread_create(&render_thread, nullptr, render_glb, reinterpret_cast<void *>(params));
    pthread_join(render_thread, nullptr);

    return 0;
}
