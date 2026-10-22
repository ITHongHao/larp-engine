// NOTE: Comments to boilerplate is for learning purposes,
// especially for viewers unfamiliar with the code.

#include <glad/gl.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>
#include <SDL3/SDL_opengl_glext.h>
#include <SDL3/SDL_opengles2_gl2platform.h>
#include <SDL3/SDL_opengles2_khrplatform.h>
#include <SDL3/SDL_main.h>
#include <iostream>
using namespace std;

//window size
const unsigned int SCR_WIDTH = 640;
const unsigned int SCR_HEIGHT = 480;

const unsigned int characterLogCount = 512;

//vertex and fragment shaders
//GLSL code as a string for C++
//OpenGL 3.3.0+
const char* vertexShaderSource =
R"(
  #version 330 core
  layout (location = 0) in vec3 aPos;
  void main()
  {
    //3D coordinates with a w (1.0) component for converting 3D space to 2D screen.
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
  }
)";
const char* fragmentShaderSource =
R"(
  #version 330 core
  out vec4 FragColor;
  void main()
  {
    //basic color assignment, currently hardcoded colors
    FragColor = vec4(0.8f, 0.3f, 0.02f, 1.0f);
  }
)";

// Main Function
// Deals with initialization of OpenGL, SDL3, and window setup
int main(int argc, char* argv[])
{
  SDL_Window *window; //main window variable
  bool done = false; //check for window startup

  //starts up sdl library and configures use with OpenGL 3.3.0
  SDL_Init(SDL_INIT_VIDEO);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

  //basic window config
  window = SDL_CreateWindow(
    "Window",
    SCR_WIDTH,
    SCR_HEIGHT,
    SDL_WINDOW_OPENGL
  );

  //failstate with resolution or errors with graphics drivers
  if (window == NULL)
  {
    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create window: %s\n", SDL_GetError());
    return 1;
  }

  //state machine for OpenGL that binds to window
  SDL_GLContext glcontext = SDL_GL_CreateContext(window);

  //failstate for if GLAD (loading library) fails to load OpenGL functions from within graphics drivers
  if (!gladLoadGL((GLADloadfunc)SDL_GL_GetProcAddress))
  {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  //build and compile shader program
  //vertex shader
  unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER); //new shader object
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); //pass along 1 string to string array memory address. NULL = string is null-terminated (calculates string length)
  glCompileShader(vertexShader); //tells graphics driver to compile GLSL text into executable GPU code at runtime.
  int success; //1=works, 0=doesn't
  char infoLog[characterLogCount]; //error data storage
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success); //checks driver to see if compilation worked. "iv" means integer vector
  if (!success)
  {
    glGetShaderInfoLog(vertexShader, characterLogCount, NULL, infoLog); //up to 512 characters of error logs will be sent to infoLog buffer
    cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << endl;
  }
  //fragment shader, similar to vertex shader code
  unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(fragmentShader, characterLogCount, NULL, infoLog);
    cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << endl;
  }
  //link shaders
  unsigned int shaderProgram = glCreateProgram(); // shaderProgram object returned with ID handle. program is final executable for drawing visuals
  glAttachShader(shaderProgram, vertexShader); //attach the vertex compiled shaders to new program object
  glAttachShader(shaderProgram, fragmentShader); //attach the fragment compiled shaders to program object
  glLinkProgram(shaderProgram); //link the two shaders together.
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success); //check linking errors
  if (!success) //error usually happens when compiling shaders are successful but cannot link together.
  {
    glGetProgramInfoLog(shaderProgram, characterLogCount, NULL, infoLog);
    cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog;
  }
  //delete compiled shaders to free memory
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  //vertex data and buffer setup
  //currently for a rectangle
  float vertices[] = {
    0.5f,  0.5f, 0.0f,  // top right
    0.5f, -0.5f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f,  // bottom left
    -0.5f,  0.5f, 0.0f   // top left 
  };

  //for the rectangle
  unsigned int indices[] = {
    0, 1, 3,  // first triangle
    1, 2, 3   // second triangle
  };

  //binding: associating a buffer to a target until something else gets bound. When other functions act on the bound buffer, the most recent buffer is used.
  //         essentially.. makes the buffer active. OpenGL is a state machine, so buffer needs to be active to be interacted with
  //VBO- vertex buffer object, an array of data (vertices) that's stored for every vertex in a memory chunk of the GPU
  //VAO- vertex array object, remembers all buffer bindings and attribute configurations. bundles everything (eg. container of pointers) for easier use
  //EBO- element buffer object, an array of data (indices) that describes how every element is constructed in a memory chunk of the GPU
  unsigned int VBO, VAO, EBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO); 
  glGenBuffers(1, &EBO);

  //bind the VAO first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO); //makes VBO the active buffer to VAO
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //carries vertices data from RAM/CPU into GPU memory. GL_STATIC_DRAW is a slight optimization + hint constant

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); //makes EBO the active buffer to VAO
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); //carries indices data from RAM/CPU into GPU memory.

  //0: points to layout (location = 0), refer to VertexShaderSource glsl code.
  //3: tells shader to grab 3 variables (x, y, z)
  //GL_FLOAT: specifies data type. GL_FALSE: OpenGL doesn't normalize numbers
  //3 * sizeof(float): also known as stride, tells us the space between consecutive vertex variables (this case is 12 bytes)
  //(void*)0: offset from beginning of the buffer
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0); //listen to location 0 (turn it on, usually it's all off unless told otherwise) 

  //glVertexAttribPointer already made a permanent link to VBO and VAO. Safe to unbind VBO.
  //0: unbinds VBO
  glBindBuffer(GL_ARRAY_BUFFER, 0); 

  //NOTE: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
  //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); //dont uncomment this

  //as a precaution, unbind so other VAO calls won't modify this VAO by accident.
  glBindVertexArray(0); 

  //MAIN GAME LOOP
  while (!done)
  {
    //every frame, the OS sends events to the window.
    SDL_Event event;
    //SDL_PollEvent pulls the oldest event out of the event queue.
    while (SDL_PollEvent(&event))
    {
      if (event.type == SDL_EVENT_QUIT) //eg. 'X' button of a window
      {
        done = true; //end
      }
    }
    //game logic, present frame, etc
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f); //sets state with hardcoded background color.
    glClear(GL_COLOR_BUFFER_BIT); //executes glClearColor

    glUseProgram(shaderProgram); //tells OpenGL to use the compiled vertex/fragment shader pipelines
    glBindVertexArray(VAO); //rebinds/looks to VAO every frame, no more need to reconfigure as that has all been done prior main loop
    //glDrawElements for EBOS instead of glDrawArrays for generics.
    //6: indices from EBO
    //GL_UNSIGNED INT: formatting
    //0: offset
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    //renders frame to hidden memory buffer, so viewer won't see computation of elements being drawn in real time. 
    //when drawing is complete, switch hidden buffer with visible one. Just switches the memory address pointer, no reallocation or deleting.
    SDL_GL_SwapWindow(window); 
  }

  //removes all active buffers and program. frees memory from GPU
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
  glDeleteProgram(shaderProgram);
  //close and destroy window safely
  SDL_DestroyWindow(window); //delete SDL3 window
  SDL_Quit();
  cout << "closed\n";
  return 0;
}