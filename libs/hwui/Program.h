/*
 * Copyright (C) 2010 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef ANDROID_UI_PROGRAM_H
#define ANDROID_UI_PROGRAM_H

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include <utils/KeyedVector.h>
#include <utils/RefBase.h>

#include "Matrix.h"

namespace android {
namespace uirenderer {

/**
 * A program holds a vertex and a fragment shader. It offers several utility
 * methods to query attributes and uniforms.
 */
class Program: public LightRefBase<Program> {
public:
    /**
     * Creates a new program with the specified vertex and fragment
     * shaders sources.
     */
    Program(const char* vertex, const char* fragment);
    virtual ~Program();

    /**
     * Binds this program to the GL context.
     */
    virtual void use();

    /**
     * Marks this program as unused. This will not unbind
     * the program from the GL context.
     */
    virtual void remove();

    /**
     * Indicates whether this program is currently in use with
     * the GL context.
     */
    inline bool isInUse() const {
        return mUse;
    }

protected:
    /**
     * Adds an attribute with the specified name.
     *
     * @return The OpenGL name of the attribute.
     */
    int addAttrib(const char* name);
    /**
     * Returns the OpenGL name of the specified attribute.
     */
    int getAttrib(const char* name);

    /**
     * Adds a uniform with the specified name.
     *
     * @return The OpenGL name of the uniform.
     */
    int addUniform(const char* name);
    /**
     * Returns the OpenGL name of the specified uniform.
     */
    int getUniform(const char* name);

private:
    /**
     * Compiles the specified shader of the specified type.
     *
     * @return The name of the compiled shader.
     */
    GLuint buildShader(const char* source, GLenum type);

    // Name of the OpenGL program
    GLuint id;

    // Name of the shaders
    GLuint vertexShader;
    GLuint fragmentShader;

    // Keeps track of attributes and uniforms slots
    KeyedVector<const char*, int> attributes;
    KeyedVector<const char*, int> uniforms;

    bool mUse;
}; // class Program

/**
 * Program used to draw vertices with a simple color. The shaders must
 * specify the following attributes:
 *      vec4 position, position of the vertex
 *      vec4 color, RGBA color of the vertex
 *
 * And the following uniforms:
 *      mat4 projection, the projection matrix
 *      mat4 modelView, the modelView matrix
 *      mat4 transform, an extra transformation matrix
 */
class DrawColorProgram: public Program {
public:
    DrawColorProgram();
    DrawColorProgram(const char* vertex, const char* fragment);

    /**
     * Binds the program with the specified projection, modelView and
     * transform matrices.
     */
    void set(const mat4& projectionMatrix, const mat4& modelViewMatrix,
             const mat4& transformMatrix);

    /**
     * Binds this program to the GL context.
     */
    virtual void use();

    /**
     * Marks this program as unused. This will not unbind
     * the program from the GL context.
     */
    virtual void remove();

    /**
     * Name of the position attribute.
     */
    int position;

    /**
     * Name of the color uniform.
     */
    int color;

    /**
     * Name of the transform uniform.
     */
    int transform;

protected:
    void getAttribsAndUniforms();
};

/**
 * Program used to draw textured vertices. In addition to everything that the
 * DrawColorProgram supports, the following two attributes must be specified:
 *      sampler2D sampler, the texture sampler
 *      vec2 texCoords, the texture coordinates of the vertex
 */
class DrawTextureProgram: public DrawColorProgram {
public:
    DrawTextureProgram();
    DrawTextureProgram(const char* vertex, const char* fragment);

    /**
     * Binds this program to the GL context.
     */
    virtual void use();

    /**
     * Marks this program as unused. This will not unbind
     * the program from the GL context.
     */
    virtual void remove();

    /**
     * Name of the texture sampler uniform.
     */
    int sampler;

    /**
     * Name of the texture coordinates attribute.
     */
    int texCoords;
};

class DrawTextProgram: public DrawTextureProgram {
public:
    DrawTextProgram();
};

/**
 * Program used to draw linear gradients. In addition to everything that the
 * DrawColorProgram supports, the following two attributes must be specified:
 *      vec2 gradient, the vector describing the linear gradient
 *      float gradientLength, the invert of the magnitude of the gradient vector
 *      sampler2D sampler, the texture sampler
 */
class DrawLinearGradientProgram: public DrawColorProgram {
public:
    DrawLinearGradientProgram();

    /**
     * Binds this program to the GL context.
     */
    virtual void use();

    /**
     * Marks this program as unused. This will not unbind
     * the program from the GL context.
     */
    virtual void remove();

    /**
     * Name of the matrix used to compute the screen space coordinates
     * of the vertices.
     */
    int screenSpace;

    /**
     * Name of the linear gradient start point.
     */
    int start;

    /**
     * Name of the linear gradient vector.
     */
    int gradient;

    /**
     * Name of the inverse of linear gradient vector's magnitude.
     */
    int gradientLength;

    /**
     * Name of the texture sampler uniform.
     */
    int sampler;
};

}; // namespace uirenderer
}; // namespace android

#endif // ANDROID_UI_PROGRAM_H
