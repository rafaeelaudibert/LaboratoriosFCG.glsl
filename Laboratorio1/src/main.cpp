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

using namespace std;

// Headers abaixo são específicos de C++
#include <string>
#include <fstream>
#include <sstream>

// Headers das bibliotecas OpenGL
#include <glad/glad.h>   // Criação de contexto OpenGL 3.3
#include <GLFW/glfw3.h>  // Criação de janelas do sistema operacional

// Headers locais, definidos na pasta "include/"
#include "utils.h"

// Constants
const double PI   = 3.141592653589793238463;
const float  PI_F = 3.14159265358979f;
const float SPEED_FACTOR = 1.0f;

// Classes for drawings
class Zero
{
public:
    void static createNDC_coefficients(GLfloat* NDC_coefficients, float dX, float dY);
    void static draw(GLushort* indices, GLuint indices_id, int offset);
    const static int POINTS_QUANTITY = 100;
    static constexpr float const& RADIUS_A_OUT = 0.2f;
    static constexpr float const& RADIUS_A_IN = 0.1f;
    static constexpr float const& RADIUS_B_OUT = 0.63f;
    static constexpr float const& RADIUS_B_IN = 0.53f;
private:
    void static translateZero(GLfloat* NDC_coefficients, float dX, float dY);
};

class One
{
public:
    void static createNDC_coefficients(GLfloat* NDC_coefficients, float dX, float dY);
    void static draw(GLushort* indices, GLuint indices_id, int offset);
    const static int POINTS_QUANTITY = 5;
private:
    void static translateOne(GLfloat* NDC_coefficients, float dX, float dY);
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

// Funções de desenho
GLuint buildTriangles(); // Constrói triângulos para renderização
void drawCounter(GLuint vertex_array_object_id); // Propriamente desenha o contador na tela

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
    //
    // Note que o caminho para os arquivos "shader_vertex.glsl" e
    // "shader_fragment.glsl" estão fixados.
    GLuint vertex_shader_id = loadShaderVertex("../../src/shader_vertex.glsl");
    GLuint fragment_shader_id = loadShaderFragment("../../src/shader_fragment.glsl");

    // Criamos um programa de GPU utilizando os shaders carregados acima
    GLuint program_id = createGPUProgram(vertex_shader_id, fragment_shader_id);

    // Construímos a representação dos nossos triângulos
    GLuint vertex_array_object_id = buildTriangles();

    // Ficamos em um loop infinito, renderizando, até que o usuário feche a janela
    while (!glfwWindowShouldClose(window))
    {
        // Aqui executamos as operações de renderização

        // Definimos a cor do "fundo" do framebuffer como branco.  Tal cor é
        // definida como coeficientes RGBA: Red, Green, Blue, Alpha;
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

        // "Pintamos" todos os pixels do framebuffer com a cor definida acima
        glClear(GL_COLOR_BUFFER_BIT);

        // Pedimos para a GPU utilizar o programa de GPU criado acima (contendo
        // os shaders de vértice e fragmentos).
        glUseProgram(program_id);

        // Draw the values on the screen, passing the VAO and the time
        drawCounter(vertex_array_object_id);

        // O framebuffer onde OpenGL executa as operações de renderização não
        // é o mesmo que está sendo mostrado para o usuário, caso contrário
        // seria possível ver artefatos conhecidos como "screen tearing".
        // A chamada abaixo faz a troca dos buffers.
        glfwSwapBuffers(window);

        // Verificamos com o sistema operacional se houve alguma interação do
        // usuário (teclado, mouse, ...) para chamarmos as funções de callback.
        glfwPollEvents();
    }

    // Finalizamos o uso dos recursos do sistema operacional
    glfwTerminate();

    return 0;
}

// Constrói triângulos para futura renderização
GLuint buildTriangles()
{
    // Primeiro, definimos os atributos de cada vértice.

    // Criaremos um 0 e um 1 pra cada posição do contador
    GLfloat NDC_coefficients_zero[4][Zero::POINTS_QUANTITY * 2 * 4] = {{0.0f}};
    GLfloat NDC_coefficients_one[4][One::POINTS_QUANTITY * 4] = {{0.0f}};
    size_t NDC_coefficients_size = sizeof(NDC_coefficients_zero[0]) + sizeof(NDC_coefficients_one[0]);

    /* Posição 0 */
    Zero::createNDC_coefficients(NDC_coefficients_zero[0], 0.68f, 0.0f);
    One::createNDC_coefficients(NDC_coefficients_one[0], 0.68f, 0.0f);

    /* Posição 1 */
    Zero::createNDC_coefficients(NDC_coefficients_zero[1], 0.22f, 0.0f);
    One::createNDC_coefficients(NDC_coefficients_one[1], 0.22f, 0.0f);

    /* Posição 2 */
    Zero::createNDC_coefficients(NDC_coefficients_zero[2], -0.22f, 0.0f);
    One::createNDC_coefficients(NDC_coefficients_one[2], -0.22f, 0.0f);

    /* Posição 3 */
    Zero::createNDC_coefficients(NDC_coefficients_zero[3], -0.68f, 0.0f);
    One::createNDC_coefficients(NDC_coefficients_one[3], -0.68f, 0.0f);

    // Criamos o identificador (ID) de um Vertex Buffer Object (VBO).  Um VBO é
    // um buffer de memória que irá conter os valores de um certo atributo de
    // um conjunto de vértices; por exemplo: posição, cor, normais, coordenadas
    // de textura.
    GLuint VBO_NDC_coefficients_id;
    glGenBuffers(1, &VBO_NDC_coefficients_id);

    // Criamos o identificador (ID) de um Vertex Array Object (VAO).  Um VAO
    // contém a definição de vários atributos de um certo conjunto de vértices;
    // isto é, um VAO irá conter ponteiros para vários VBOs.
    GLuint vertex_array_object_id;
    glGenVertexArrays(1, &vertex_array_object_id);

    // "Ligamos" o VAO ("bind"). Informamos que iremos atualizar o VAO cujo ID
    // está contido na variável "vertex_array_object_id".
    glBindVertexArray(vertex_array_object_id);

    // "Ligamos" o VBO ("bind"). Informamos que o VBO cujo ID está contido na
    // variável VBO_NDC_coefficients_id será modificado a seguir. A
    // constante "GL_ARRAY_BUFFER" informa que esse buffer é de fato um VBO, e
    // irá conter atributos de vértices.
    glBindBuffer(GL_ARRAY_BUFFER, VBO_NDC_coefficients_id);

    // Alocamos memória para o VBO "ligado" acima. Como queremos armazenar
    // nesse VBO todos os valores contidos no array "NDC_coefficients", pedimos
    // para alocar um número de bytes exatamente igual ao tamanho ("size")
    // desse array. A constante "GL_STATIC_DRAW" dá uma dica para o driver da
    // GPU sobre como utilizaremos os dados do VBO. Neste caso, estamos dizendo
    // que não pretendemos alterar tais dados (são estáticos: "STATIC"), e
    // também dizemos que tais dados serão utilizados para renderizar ou
    // desenhar ("DRAW").  Pense que:
    //
    //            glBufferData()  ==  malloc() do C  ==  new do C++.
    //
    glBufferData(GL_ARRAY_BUFFER, NDC_coefficients_size * 4, NULL, GL_STATIC_DRAW);

    // Finalmente, copiamos os valores do "array NDC_coefficients" para dentro do
    // VBO "ligado" acima.  Pense que:
    //
    //            glBufferSubData()  ==  memcpy() do C.
    //
    for (int i = 0; i < 4; i++)
    {
        glBufferSubData(GL_ARRAY_BUFFER, i * NDC_coefficients_size + 0, sizeof(NDC_coefficients_zero[i]), NDC_coefficients_zero[i]);
        glBufferSubData(GL_ARRAY_BUFFER, i * NDC_coefficients_size + sizeof(NDC_coefficients_zero[i]), sizeof(NDC_coefficients_one[i]), NDC_coefficients_one[i]);
    }

    // Precisamos então informar um índice de "local" ("location"), o qual será
    // utilizado no shader "shader_vertex.glsl" para acessar os valores
    // armazenados no VBO "ligado" acima. Também, informamos a dimensão (número de
    // coeficientes) destes atributos. Como em nosso caso são coordenadas NDC
    // homogêneas, temos quatro coeficientes por vértice (X,Y,Z,W). Isto define
    // um tipo de dado chamado de "vec4" em "shader_vertex.glsl": um vetor com
    // quatro coeficientes. Finalmente, informamos que os dados estão em ponto
    // flutuante com 32 bits (GL_FLOAT).
    // Esta função também informa que o VBO "ligado" acima em glBindBuffer()
    // está dentro do VAO "ligado" acima por glBindVertexArray().
    // Veja https://www.khronos.org/opengl/wiki/Vertex_Specification#Vertex_Buffer_Object
    GLuint location = 0; // "(location = 0)" em "shader_vertex.glsl"
    GLint  number_of_dimensions = 4; // vec4 em "shader_vertex.glsl"
    glVertexAttribPointer(location, number_of_dimensions, GL_FLOAT, GL_FALSE, 0, 0);

    // "Ativamos" os atributos. Informamos que os atributos com índice de local
    // definido acima, na variável "location", deve ser utilizado durante o
    // rendering.
    glEnableVertexAttribArray(location);

    // "Desligamos" o VBO, evitando assim que operações posteriores venham a
    // alterar o mesmo. Isto evita bugs.
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Agora repetimos todos os passos acima para atribuir um novo atributo a
    // cada vértice: uma cor (veja slide 115 do documento "Aula_04_Modelagem_Geometrica_3D.pdf").
    // Tal cor é definida como coeficientes RGBA: Red, Green, Blue, Alpha;
    // isto é: Vermelho, Verde, Azul, Alpha (valor de transparência).
    // Conversaremos sobre sistemas de cores nas aulas de Modelos de Iluminação.
    int points_quantity = (Zero::POINTS_QUANTITY * 2 * 4 + One::POINTS_QUANTITY * 4);
    GLfloat color_coefficients[points_quantity * 4] = {0.0f}; // Inicializa todos com 0.0f por padrão
    for(int position = 0; position < 4; position++)
    {
        for(int i = 0; i < Zero::POINTS_QUANTITY * 2 * 4; i += 4)
        {
            color_coefficients[points_quantity * position + i + 0] = 1.0f;
            color_coefficients[points_quantity * position + i + 3] = 1.0f;
        }
        for(int i = 0; i < One::POINTS_QUANTITY * 4; i += 4)
        {
            color_coefficients[points_quantity * position + i + 2 + Zero::POINTS_QUANTITY * 2 * 4] = 1.0f;
            color_coefficients[points_quantity * position + i + 3 + Zero::POINTS_QUANTITY * 2 * 4] = 1.0f;
        }
    }
    GLuint VBO_color_coefficients_id;
    glGenBuffers(1, &VBO_color_coefficients_id);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_color_coefficients_id);
    glBufferData(GL_ARRAY_BUFFER, sizeof(color_coefficients), color_coefficients, GL_STATIC_DRAW); // Aqui copiamos direto pra dentro da array
    location = 1; // "(location = 1)" em "shader_vertex.glsl"
    number_of_dimensions = 4; // vec4 em "shader_vertex.glsl"
    glVertexAttribPointer(location, number_of_dimensions, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(location);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // "Desligamos" o VAO, evitando assim que operações posteriores venham a
    // alterar o mesmo. Isso evita bugs.
    glBindVertexArray(0);

    // Retornamos o ID do VAO. Isso é tudo que será necessário para renderizar
    // os triângulos definidos acima. Veja a chamada glDrawElements() em main().
    return vertex_array_object_id;
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

void drawCounter(GLuint vertex_array_object_id)
{
    // Pega os segundos passados desde o início do programa
    int seconds = (int)(glfwGetTime() * SPEED_FACTOR);

    // "Ligamos" o VAO. Informamos que queremos utilizar os atributos de
    // vértices apontados pelo VAO criado pela função buildTriangles(). Veja
    // comentários detalhados dentro da definição de buildTriangles().
    glBindVertexArray(vertex_array_object_id);

    // Criamos um buffer OpenGL para armazenar os índices abaixo
    GLuint indices_id;
    glGenBuffers(1, &indices_id);

    // Vamos então definir os triângulos utilizando os vértices do array
    // NDC_coefficients e desenhá-los na tela.
    // Este vetor "indices" define a TOPOLOGIA
    // Generate the indices array
    GLushort indices[(Zero::POINTS_QUANTITY + 1)* 2] = {0};
    for (int position = 0; position < 4; position++)
    {
        int offset = (Zero::POINTS_QUANTITY * 2 + One::POINTS_QUANTITY) * position;

        if (((seconds >> position) & 1) == 1)   // Should draw a 1
        {
            One::draw(indices, indices_id, offset);
        }
        else     // Should draw a 0
        {
            Zero::draw(indices, indices_id, offset);
        }

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // Desligamos ele para não ter memory leak
    }

    // Apaga buffers dos indices para não termos memory leak
    glDeleteBuffers(1, &indices_id);

    // "Desligamos" o VAO, evitando assim que operações posteriores venham a
    // alterar o mesmo. Isso evita bugs.
    glBindVertexArray(0);

    return;
}

/* ZERO CLASS IMPLEMENTATION */

// Generate the NDC_coefficients for a letter '0'
void Zero::createNDC_coefficients(GLfloat* NDC_coefficients, float dX, float dY)
{
    int i;
    double theta;

    for(i = 0, theta = 0; i < POINTS_QUANTITY * 2 * 4; i += 8, theta += 2 * PI / POINTS_QUANTITY)
    {
        float radius_out = (RADIUS_A_OUT * RADIUS_B_OUT) / (sqrt(pow(RADIUS_A_OUT, 2) * pow(sin(theta), 2) + pow(RADIUS_B_OUT, 2) * pow(cos(theta), 2)));
        float radius_in = (RADIUS_A_IN * RADIUS_B_IN) / (sqrt(pow(RADIUS_A_IN, 2) * pow(sin(theta), 2) + pow(RADIUS_B_IN, 2) * pow(cos(theta), 2)));
        NDC_coefficients[i + 0] = (float)(radius_out * cos(theta));
        NDC_coefficients[i + 1] = (float)(radius_out * sin(theta));
        NDC_coefficients[i + 3] = 1.0f;
        NDC_coefficients[i + 4] = (float)(radius_in * cos(theta));
        NDC_coefficients[i + 5] = (float)(radius_in * sin(theta));
        NDC_coefficients[i + 7] = 1.0f;
    }

    translateZero(NDC_coefficients, dX, dY);
}

// Move the NDC_coefficients for the letter '0'
void Zero::translateZero(GLfloat* NDC_coefficients, float dX, float dY)
{
    for (int i=0; i < POINTS_QUANTITY * 2 * 4; i += 8)
    {
        NDC_coefficients[i + 0] += dX;
        NDC_coefficients[i + 1] += dY;
        NDC_coefficients[i + 4] += dX;
        NDC_coefficients[i + 5] += dY;
    }
}

void Zero::draw(GLushort* indices, GLuint indices_id, int offset) {
    for(int i=0; i < Zero::POINTS_QUANTITY * 2; i++)
    {
        indices[i] = i + offset;
    }
    indices[Zero::POINTS_QUANTITY * 2] = 0 + offset;
    indices[Zero::POINTS_QUANTITY * 2 + 1] = 1 + offset;

    // "Ligamos" o buffer. Note que o tipo agora é GL_ELEMENT_ARRAY_BUFFER.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_id);

    // Alocamos memória para o buffer, copiando o valor de indices para dentro
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * (Zero::POINTS_QUANTITY + 1) * 2, indices, GL_STATIC_DRAW);

    // Desenhamos elemento na tela
    glDrawElements(GL_TRIANGLE_STRIP, (Zero::POINTS_QUANTITY + 1) * 2, GL_UNSIGNED_SHORT, 0);
}

/* ONE CLASS IMPLEMENTATION */
// Generate the NDC_coefficients for a letter '1'
void One::createNDC_coefficients(GLfloat* NDC_coefficients, float dX, float dY)
{
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

    translateOne(NDC_coefficients, dX, dY);
}

// Move the NDC_coefficients for the letter '0'
void One::translateOne(GLfloat* NDC_coefficients, float dX, float dY)
{
    for (int i=0; i < POINTS_QUANTITY * 4; i += 4)
    {
        NDC_coefficients[i + 0] += dX;
        NDC_coefficients[i + 1] += dY;
    }
}

void One::draw(GLushort* indices, GLuint indices_id, int offset){
    indices[0] = offset + (Zero::POINTS_QUANTITY * 2) + 1;
    indices[1] = offset + (Zero::POINTS_QUANTITY * 2) + 0;
    indices[2] = offset + (Zero::POINTS_QUANTITY * 2) + 2;
    indices[3] = offset + (Zero::POINTS_QUANTITY * 2) + 3;
    indices[4] = offset + (Zero::POINTS_QUANTITY * 2) + 4;

    // "Ligamos" o buffer. Note que o tipo agora é GL_ELEMENT_ARRAY_BUFFER.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_id);

    // Alocamos memória para o buffer.
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * One::POINTS_QUANTITY, NULL, GL_STATIC_DRAW);

    // Copiamos os valores do array indices[] para dentro do buffer.
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(indices[0]) * One::POINTS_QUANTITY, indices);

    // Desenha os elementos na tela
    glDrawElements(GL_TRIANGLE_STRIP, One::POINTS_QUANTITY, GL_UNSIGNED_SHORT, 0);
}

// vim: set spell spelllang=pt_br :

