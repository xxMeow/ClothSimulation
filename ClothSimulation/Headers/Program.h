#ifndef Program_h
#define Program_h

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <unistd.h> // To use getcwd()

class Program
{
public:
    // ID of program
    unsigned int ID;
    
    // Read file and construct shader program
    Program(const char *vsFilePath, const char *fsFilePath)
    {
        /** 1. Read file **/
        
        std::ifstream vsFile, fsFile;
        // Make sure that ifstream object can throw exceptions
        vsFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        fsFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        
        std::string vsSrc, fsSrc;
        try {
            // XCode may have special working directory, check it
            char currPath[256];
            char *currPathPtr = getcwd(currPath, sizeof(currPath));
            if (currPathPtr) {
                std::cout << "Working at: " << currPath << std::endl;
            }
            // Open file
            vsFile.open(vsFilePath);
            fsFile.open(fsFilePath);
            std::stringstream vsStream, fsStream;
            // Read file to data stream
            vsStream << vsFile.rdbuf();
            fsStream << fsFile.rdbuf();
            // Close file processor
            vsFile.close();
            fsFile.close();
            // Convert data stream to string
            vsSrc = vsStream.str();
            fsSrc = fsStream.str();
        } catch (std::ifstream::failure e) {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
        }
        
        const char *vsCode = vsSrc.c_str();
        const char *fsCode = fsSrc.c_str();
        
        /** 2. Compile shader **/
        
        // Compile info
        int cFlag; // If compile successed
        char cLog[512]; // Compile log
        
        unsigned int vs, fs; // Shader ID
        
        // Vertex shader
        vs = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vs, 1, &vsCode, NULL);
        glCompileShader(vs);
        // Check for compilation error
        glGetShaderiv(vs, GL_COMPILE_STATUS, &cFlag);
        if (!cFlag) {
            glGetShaderInfoLog(vs, 512, NULL, cLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << cLog << std::endl;
        }
        
        // Fragment shader
        fs = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fs, 1, &fsCode, NULL);
        glCompileShader(fs);
        // Check for compilation error
        glGetShaderiv(fs, GL_COMPILE_STATUS, &cFlag);
        if (!cFlag) {
            glGetShaderInfoLog(fs, 512, NULL, cLog);
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << cLog << std::endl;
        }
        
        // Shader program
        ID = glCreateProgram();
        glAttachShader(ID, vs);
        glAttachShader(ID, fs);
        glLinkProgram(ID);
        // Check for linking error
        glGetProgramiv(ID, GL_LINK_STATUS, &cFlag);
        if (!cFlag) {
            glGetProgramInfoLog(ID, 512, NULL, cLog);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << cLog << std::endl;
        }
        
        // Clean linked shaders (What we actually need is the shader program)
        glDeleteShader(vs);
        glDeleteShader(fs);
    }
};

#endif /* Shader_h */
