//     Universidade Federal do Rio Grande do Sul
//             Instituto de Informática
//       Departamento de Informática Aplicada
//
//    INF01047 Fundamentos de Computação Gráfica
//               Prof. Eduardo Gastal
//
//                   LABORATÓRIO 1
//

// Arquivos "headers" padrões de C podem ser incluídos em um
// programa C++, sendo necessário somente adicionar o caractere
// "c" antes de seu nome, e remover o sufixo ".h". Exemplo:
//    #include <stdio.h> // Em C
//  vira
////    #include <cstdio> // Em C++
//
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <iostream>

// Headers abaixo são específicos de C++
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

// Headers das bibliotecas OpenGL
#include <glad/glad.h>   // Criação de contexto OpenGL 3.3
#include <GLFW/glfw3.h>  // Criação de janelas do sistema operacional

// Headers locais, definidos na pasta "include/"
#include "utils.h"

/** Constants
  *
  * PI is self-explanatory
  * SPEED_FACTOR is the speed by which the counter is multiplied by
  *
  */
constexpr double PI   = 3.141592653589793238463;
constexpr float  SPEED_FACTOR = 5.0f;

/** DIGIT CLASS IMPLEMENTATION **/
class Digit
{
protected:
    GLuint VAO_id;
    GLfloat* NDC_coefficients;

    // Translates all the object points in a given delta
    virtual void translate(const float dX, const float dY) = 0;

    // Generates all the object points
    virtual void generate_NDC_coefficients() = 0;
public:
    virtual void draw() = 0;

    // Generates the object VAO
    void create_vao_id()
    {
        glGenVertexArrays(1, &VAO_id);
    }

    // Generates a VBO id
    GLuint create_vbo_id()
    {
        GLuint VBO;
        glGenBuffers(1, &VBO);

        return VBO;
    }

    // Configures a VBO given its size, data, location (on shader_vertex.glsl) and dimensions in it
    void configure_vbo(GLsizeiptr data_size, const void* data, GLuint index, GLint dimensions)
    {
        glBindVertexArray(VAO_id);

        // 1. Criamos o identificador (ID) de um Vertex Buffer Object (VBO).
        // 2. "Ligamos" o VBO ("bind").
        // 3. Alocamos memória para o VBO "ligado" acima, e copiamos o valor que será armazenado
        //    nele para dentro dele.
        // 4. Informamos o "local" desse VBO para o "shader_vertex.glsl". Também informamos sua dimensão
        // 5. Ativamos os atributos, dando sua localização
        // 6. Desligamos o VBO
        GLuint VBO_color_coefficients_id = create_vbo_id();
        glBindBuffer(GL_ARRAY_BUFFER, VBO_color_coefficients_id);
        glBufferData(GL_ARRAY_BUFFER, data_size, data, GL_STATIC_DRAW); // Aqui copiamos direto pra dentro da array
        glVertexAttribPointer(index, dimensions, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(index);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // "Desligamos" o VAO, evitando assim que operações posteriores venham a
        // alterar o mesmo. Isso evita bugs.
        glBindVertexArray(0);
    }
};

/** ZERO CLASS IMPLEMENTATION **/
class Zero : public Digit
{
    void translate(const float dX, const float dY); // Override virtual
    void generate_NDC_coefficients();               // Override virtual

public:
    Zero(const float dX, const float dY);
    void draw();

    // Constants
    static constexpr int const& POINTS_QUANTITY = 150;
    static constexpr float const& RADIUS_A_OUT = 0.2f;
    static constexpr float const& RADIUS_A_IN = 0.1f;
    static constexpr float const& RADIUS_B_OUT = 0.63f;
    static constexpr float const& RADIUS_B_IN = 0.53f;
};

/** ONE CLASS IMPLEMENTATION **/
class One : public Digit
{
    void translate(const float dX, const float dY); // Override virtual
    void generate_NDC_coefficients();               // Override virtual

public:
    One(const float dX, const float dY);
    void draw();

    const static int POINTS_QUANTITY = 5;
};

/** STRUCT COUNTER */
class Counter
{
    std::vector<Zero*> zeros;
    std::vector<One*> ones;

public:
    Counter()
    {
        zeros.push_back(new Zero( 0.68f, 0.0f));
        zeros.push_back(new Zero( 0.22f, 0.0f));
        zeros.push_back(new Zero(-0.22f, 0.0f));
        zeros.push_back(new Zero(-0.68f, 0.0f));
        ones.push_back(new One( 0.68f, 0.0f));
        ones.push_back(new One( 0.22f, 0.0f));
        ones.push_back(new One(-0.22f, 0.0f));
        ones.push_back(new One(-0.68f, 0.0f));
    }

    void draw(GLFWwindow* window)
    {
        // Pega os segundos passados desde o início do programa
        int seconds = (int)(glfwGetTime() * SPEED_FACTOR);

        // Para cada posição do contador, verificamos se devemos desenhar o 0 ou o 1
        for (int position = 0; position < 4; position++)
        {
            ((seconds >> position) & 1) == 1 ? ones.at(position)->draw() : zeros.at(position)->draw();
        }

        // O framebuffer onde OpenGL executa as operações de renderização não
        // é o mesmo que está sendo mostrado para o usuário, caso contrário
        // seria possível ver artefatos conhecidos como "screen tearing".
        // A chamada abaixo faz a troca dos buffers.
        glfwSwapBuffers(window);

        return;
    }

    void reset_screen()
    {
        // Definimos a cor do "fundo" do framebuffer como branco.  Tal cor é
        // definida como coeficientes RGBA: Red, Green, Blue, Alpha;
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

        // "Pintamos" todos os pixels do framebuffer com a cor definida acima
        glClear(GL_COLOR_BUFFER_BIT);
    }
};

// Declaração de várias funções utilizadas em main().  Essas estão definidas
// logo após a definição de main() neste arquivo.
GLuint loadShaderVertex(const char* filename);   // Carrega um vertex shader
GLuint loadShaderFragment(const char* filename); // Carrega um fragment shader
void loadShader(const char* filename, GLuint shader_id); // Função utilizada pelas duas acima
GLuint createGPUProgram(GLuint vertex_shader_id, GLuint fragment_shader_id); // Cria um programa de GPU

// Funções callback para comunicação com o sistema operacional e interação do
// usuário. Veja mais comentários nas definições das mesmas, abaixo.
void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void errorCallback(int error, const char* description);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);

int main()
{
    // Inicializamos a biblioteca GLFW, utilizada para criar uma janela do
    // sistema operacional, onde poderemos renderizar com OpenGL.
    if (!glfwInit())
    {
        fprintf(stderr, "ERROR: glfwInit() failed.\n");
        std::exit(EXIT_FAILURE);
    }

    // Definimos o callback para impressão de erros da GLFW no terminal
    glfwSetErrorCallback(errorCallback);

    // Pedimos para utilizar OpenGL versão 3.3 (ou superior)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // Pedimos para utilizar o perfil "core", isto é, utilizaremos somente as
    // funções modernas de OpenGL.
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Criamos uma janela do sistema operacional, com 700 colunas e 700 linhas
    // de pixels, e com título "INF01047 ...".
    GLFWwindow* window;
    window = glfwCreateWindow(1000, 600, "INF01047 - 00287695 - Rafael Baldasso Audibert", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        fprintf(stderr, "ERROR: glfwCreateWindow() failed.\n");
        std::exit(EXIT_FAILURE);
    }

    // Definimos a função de callback que será chamada sempre que o usuário
    // pressionar alguma tecla do teclado.
    glfwSetKeyCallback(window, keyCallback);

    // Definimos a função de callback que será chamada sempre que a janela for
    // redimensionada, por consequência alterando o tamanho do "framebuffer"
    // (região de memória onde são armazenados os pixels da imagem).
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    // Indicamos que as chamadas OpenGL deverão renderizar nesta janela
    glfwMakeContextCurrent(window);

    // Carregamento de todas funções definidas por OpenGL 3.3, utilizando a
    // biblioteca GLAD.
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);

    // Carregamos os shaders de vértices e de fragmentos que serão utilizados
    // para renderização.
    // Note que o caminho para os arquivos "shader_vertex.glsl" e
    // "shader_fragment.glsl" estão fixados.
    GLuint vertex_shader_id = loadShaderVertex("../../src/shader_vertex.glsl");
    GLuint fragment_shader_id = loadShaderFragment("../../src/shader_fragment.glsl");

    // Criamos um programa de GPU utilizando os shaders carregados acima, e o utilizamos
    glUseProgram(createGPUProgram(vertex_shader_id, fragment_shader_id));

    // Construímos a representação dos nossos triângulos
    Counter* counter = new Counter();

    // Ficamos em um loop infinito, renderizando, até que o usuário feche a janela
    while (!glfwWindowShouldClose(window))
    {
        // Aqui executamos as operações de renderização

        // Resetamos a tela
        counter->reset_screen();

        // Desenhamos os valores na tela
        counter->draw(window);

        // Verificamos com o sistema operacional se houve alguma interação do
        // usuário (teclado, mouse, ...) para chamarmos as funções de callback.
        glfwPollEvents();
    }

    // Finalizamos o uso dos recursos do sistema operacional
    glfwTerminate();

    return 0;
}

// Carrega um Vertex Shader de um arquivo
GLuint loadShaderVertex(const char* filename)
{
    // Criamos um identificador (ID) para este shader, informando que o mesmo
    // será aplicado nos vértices.
    GLuint vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);

    // Carregamos e compilamos o shader
    loadShader(filename, vertex_shader_id);

    // Retorna o ID gerado acima
    return vertex_shader_id;
}

// Carrega um Fragment Shader de um arquivo
GLuint loadShaderFragment(const char* filename)
{
    // Criamos um identificador (ID) para este shader, informando que o mesmo
    // será aplicado nos fragmentos.
    GLuint fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);

    // Carregamos e compilamos o shader
    loadShader(filename, fragment_shader_id);

    // Retorna o ID gerado acima
    return fragment_shader_id;
}

// Função auxilar, utilizada pelas duas funções acima. Carrega código de GPU de
// um arquivo GLSL e faz sua compilação.
void loadShader(const char* filename, GLuint shader_id)
{
    // Lemos o arquivo de texto indicado pela variável "filename"
    // e colocamos seu conteúdo em memória, apontado pela variável
    // "shader_string".
    std::ifstream file;
    try
    {
        file.exceptions(std::ifstream::failbit);
        file.open(filename);
    }
    catch ( std::exception& e )
    {
        fprintf(stderr, "ERROR: Cannot open file \"%s\".\n", filename);
        std::exit(EXIT_FAILURE);
    }
    std::stringstream shader;
    shader << file.rdbuf();
    std::string str = shader.str();
    const GLchar* shader_string = str.c_str();
    const GLint   shader_string_length = static_cast<GLint>( str.length() );

    // Define o código do shader GLSL, contido na string "shader_string"
    glShaderSource(shader_id, 1, &shader_string, &shader_string_length);

    // Compila o código do shader GLSL (em tempo de execução)
    glCompileShader(shader_id);

    // Verificamos se ocorreu algum erro ou "warning" durante a compilação
    GLint compiled_ok;
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &compiled_ok);

    GLint log_length = 0;
    glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &log_length);

    // Alocamos memória para guardar o log de compilação.
    // A chamada "new" em C++ é equivalente ao "malloc()" do C.
    GLchar* log = new GLchar[log_length];
    glGetShaderInfoLog(shader_id, log_length, &log_length, log);

    // Imprime no terminal qualquer erro ou "warning" de compilação
    if ( log_length != 0 )
    {
        std::string  output;

        if ( !compiled_ok )
        {
            output += "ERROR: OpenGL compilation of \"";
            output += filename;
            output += "\" failed.\n";
            output += "== Start of compilation log\n";
            output += log;
            output += "== End of compilation log\n";
        }
        else
        {
            output += "WARNING: OpenGL compilation of \"";
            output += filename;
            output += "\".\n";
            output += "== Start of compilation log\n";
            output += log;
            output += "== End of compilation log\n";
        }

        fprintf(stderr, "%s", output.c_str());
    }

    // A chamada "delete" em C++ é equivalente ao "free()" do C
    delete [] log;
}

// Esta função cria um programa de GPU, o qual contém obrigatoriamente um
// Vertex Shader e um Fragment Shader.
GLuint createGPUProgram(GLuint vertex_shader_id, GLuint fragment_shader_id)
{
    // Criamos um identificador (ID) para este programa de GPU
    GLuint program_id = glCreateProgram();

    // Definição dos dois shaders GLSL que devem ser executados pelo programa
    glAttachShader(program_id, vertex_shader_id);
    glAttachShader(program_id, fragment_shader_id);

    // Linkagem dos shaders acima ao programa
    glLinkProgram(program_id);

    // Verificamos se ocorreu algum erro durante a linkagem
    GLint linked_ok = GL_FALSE;
    glGetProgramiv(program_id, GL_LINK_STATUS, &linked_ok);

    // Imprime no terminal qualquer erro de linkagem
    if ( linked_ok == GL_FALSE )
    {
        GLint log_length = 0;
        glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &log_length);

        // Alocamos memória para guardar o log de compilação.
        // A chamada "new" em C++ é equivalente ao "malloc()" do C.
        GLchar* log = new GLchar[log_length];

        glGetProgramInfoLog(program_id, log_length, &log_length, log);

        std::string output;

        output += "ERROR: OpenGL linking of program failed.\n";
        output += "== Start of link log\n";
        output += log;
        output += "== End of link log\n";

        // A chamada "delete" em C++ é equivalente ao "free()" do C
        delete [] log;

        fprintf(stderr, "%s", output.c_str());
    }

    // Retorna o ID gerado acima
    return program_id;
}

// Definição da função que será chamada sempre que a janela do sistema
// operacional for redimensionada, por consequência alterando o tamanho do
// "framebuffer" (região de memória onde são armazenados os pixels da imagem).
void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    // Indicamos que queremos renderizar em toda região do framebuffer. A
    // função "glViewport" define o mapeamento das "normalized device
    // coordinates" (NDC) para "pixel coordinates".
    glViewport(0, 0, width, height);
}

// Definição da função que será chamada sempre que o usuário pressionar alguma
// tecla do teclado. Veja http://www.glfw.org/docs/latest/input_guide.html#input_key
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    // ==============
    // Não modifique este loop! Ele é utilizando para correção automatizada dos
    // laboratórios. Deve ser sempre o primeiro comando desta função KeyCallback().
    for (int i = 0; i < 10; ++i)
        if (key == GLFW_KEY_0 + i && action == GLFW_PRESS && mode == GLFW_MOD_SHIFT)
            std::exit(100 + i);
    // ==============

    // Se o usuário pressionar a tecla ESC, fechamos a janela.
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

// Definimos o callback para impressão de erros da GLFW no terminal
void errorCallback(int error, const char* description)
{
    fprintf(stderr, "ERROR: GLFW: %s\n", description);
}

/* ZERO CLASS IMPLEMENTATION */
// Zero constructor
Zero::Zero(float dX, float dY)
{
    generate_NDC_coefficients();
    translate(dX, dY);

    // Criaremos o VAO utilizado por esse objeto
    create_vao_id();

    // Configure NDC_coefficients vbo
    configure_vbo(sizeof(NDC_coefficients[0]) * Zero::POINTS_QUANTITY * 2 * 4, NDC_coefficients, 0, 4);

    // Configure color_coefficients vbo
    GLfloat color_coefficients[Zero::POINTS_QUANTITY * 2 * 4] = {0.0f}; // Inicializa todos com 0.0f por padrão
    for(int i = 0; i < Zero::POINTS_QUANTITY * 2 * 4; i += 4)
    {
        color_coefficients[i + 0] = 1.0f;
        color_coefficients[i + 3] = 1.0f;
    }
    configure_vbo(sizeof(color_coefficients), color_coefficients, 1, 4);
}

// Generate the NDC_coefficients for a letter '0'
void Zero::generate_NDC_coefficients()
{
    int i;
    double theta;
    NDC_coefficients = (GLfloat*) malloc(sizeof(GLfloat) * Zero::POINTS_QUANTITY * 2 * 4);

    for(i = 0, theta = 0; i < Zero::POINTS_QUANTITY * 2 * 4; i += 8, theta += 2 * PI / Zero::POINTS_QUANTITY)
    {
        float radius_out = (RADIUS_A_OUT * RADIUS_B_OUT) / (sqrt(pow(RADIUS_A_OUT, 2) * pow(sin(theta), 2) + pow(RADIUS_B_OUT, 2) * pow(cos(theta), 2)));
        float radius_in = (RADIUS_A_IN * RADIUS_B_IN) / (sqrt(pow(RADIUS_A_IN, 2) * pow(sin(theta), 2) + pow(RADIUS_B_IN, 2) * pow(cos(theta), 2)));

        NDC_coefficients[i + 0] = (GLfloat)(radius_out * cos(theta));
        NDC_coefficients[i + 1] = (GLfloat)(radius_out * sin(theta));
        NDC_coefficients[i + 2] = 0.0f;
        NDC_coefficients[i + 3] = 1.0f;
        NDC_coefficients[i + 4] = (GLfloat)(radius_in * cos(theta));
        NDC_coefficients[i + 5] = (GLfloat)(radius_in * sin(theta));
        NDC_coefficients[i + 6] = 0.0f;
        NDC_coefficients[i + 7] = 1.0f;
    }
};

// Move the NDC_coefficients for the letter '0'
void Zero::translate(float dX, float dY)
{
    for (int i=0; i < Zero::POINTS_QUANTITY * 2 * 4; i += 8)
    {
        NDC_coefficients[i + 0] += dX;
        NDC_coefficients[i + 1] += dY;
        NDC_coefficients[i + 4] += dX;
        NDC_coefficients[i + 5] += dY;
    }
}

void Zero::draw()
{

    // "Ligamos" o VAO. Informamos que queremos utilizar os atributos de
    // vértices apontados pelo VAO criado pela função buildNumbers(). Veja
    // comentários detalhados dentro da definição de buildNumbers().
    glBindVertexArray(VAO_id);

    // Criamos um buffer OpenGL para armazenar os índices abaixo
    GLuint indices_id;
    glGenBuffers(1, &indices_id);

    GLushort indices[(Zero::POINTS_QUANTITY + 1) * 2];
    for(int i=0; i < Zero::POINTS_QUANTITY * 2; i++)
    {
        indices[i] = i;
    }
    indices[Zero::POINTS_QUANTITY * 2] = 0;
    indices[Zero::POINTS_QUANTITY * 2 + 1] = 1;

    // "Ligamos" o buffer. Note que o tipo agora é GL_ELEMENT_ARRAY_BUFFER.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_id);

    // Alocamos memória para o buffer, copiando o valor de indices para dentro
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Desenhamos elemento na tela
    glDrawElements(GL_TRIANGLE_STRIP, (Zero::POINTS_QUANTITY + 1) * 2, GL_UNSIGNED_SHORT, 0);

    // Desligamos e deletamos o VBO para não ocorrer memory leak
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &indices_id);

    // Turn off the VAO to prevent bugs
    glBindVertexArray(0);
}

/* ONE CLASS IMPLEMENTATION */
One::One(float dX, float dY)
{
    generate_NDC_coefficients();
    translate(dX, dY);

    // Criaremos o VAO desse objeto
    create_vao_id();

    // Configure NDC_coefficients vbo
    configure_vbo(sizeof(NDC_coefficients[0]) * One::POINTS_QUANTITY * 4, NDC_coefficients, 0, 4);

    // Configure color_coefficients vbo
    GLfloat color_coefficients[One::POINTS_QUANTITY * 4] = {0.0f}; // Inicializa todos com 0.0f por padrão
    for(int i = 0; i < One::POINTS_QUANTITY * 4; i += 4)
    {
        color_coefficients[i + 2] = 1.0f;
        color_coefficients[i + 3] = 1.0f;
    }
    configure_vbo(sizeof(color_coefficients), color_coefficients, 1, 4);
}

// Generate the NDC_coefficients for a letter '1'
void One::generate_NDC_coefficients()
{
    NDC_coefficients = (GLfloat*) malloc(sizeof(GLfloat) * One::POINTS_QUANTITY * 4);

    NDC_coefficients[0]  = -0.04f;
    NDC_coefficients[1]  = -0.64f;
    NDC_coefficients[2]  = 0.0f;
    NDC_coefficients[3]  = 1.0f;
    NDC_coefficients[4]  =  0.04f;
    NDC_coefficients[5]  = -0.64f;
    NDC_coefficients[6]  = 0.0f;
    NDC_coefficients[7]  = 1.0f;
    NDC_coefficients[8]  =  0.04f;
    NDC_coefficients[9]  =  0.64f;
    NDC_coefficients[10] = 0.0f;
    NDC_coefficients[11] = 1.0f;
    NDC_coefficients[12] = -0.04f;
    NDC_coefficients[13] =  0.64f;
    NDC_coefficients[14] = 0.0f;
    NDC_coefficients[15] = 1.0f;
    NDC_coefficients[16] = -0.09f;
    NDC_coefficients[17] =  0.50f;
    NDC_coefficients[18] = 0.0f;
    NDC_coefficients[19] = 1.0f;
}

// Move the NDC_coefficients for the letter '1'
void One::translate(float dX, float dY)
{
    for (int i=0; i < One::POINTS_QUANTITY * 4; i += 4)
    {
        NDC_coefficients[i + 0] += dX;
        NDC_coefficients[i + 1] += dY;
    }
}

void One::draw()
{
    // "Ligamos" o VAO. Informamos que queremos utilizar os atributos de
    // vértices apontados pelo VAO criado pela função buildNumbers(). Veja
    // comentários detalhados dentro da definição de buildNumbers().
    glBindVertexArray(VAO_id);

    // Create the indices que serão usados pela chamada do glDrawElements
    GLushort indices[One::POINTS_QUANTITY] = {1, 0, 2, 3, 4};

    // Criamos um buffer OpenGL para armazenar os índices abaixo
    GLuint indices_id = create_vbo_id();

    // "Ligamos" o buffer. Note que o tipo agora é GL_ELEMENT_ARRAY_BUFFER.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_id);

    // Alocamos memória para o buffer, copiando o valor de indices para dentro
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Desenhamos elemento na tela
    glDrawElements(GL_TRIANGLE_STRIP, One::POINTS_QUANTITY, GL_UNSIGNED_SHORT, 0);

    // Desligamos e deletamos o VBO para não ocorrer memory leak
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &indices_id);

    // Turn off the VAO to prevent bugs
    glBindVertexArray(0);
}
