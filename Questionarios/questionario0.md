# Resolução do Questionário 0
### Rafael Baldasso Audibert (00287695)
 
---
1) Considere a malha poligonal 2D abaixo e seu respectivo array de geometria G: *[Visualizar no questionário 0](https://moodle.inf.ufrgs.br/pluginfile.php/134184/mod_resource/content/4/Questionario_0.pdf)*

    **1.1)** Sendo o modo de rasterização **GL_TRIANGLE_STRIP**, indique com verdadeiro (V) ou falso (F) quais arrays de índices (topologias) abaixo gerariam a malha poligonal acima:

    (<span style="color: red">F</span>) indices[] = { 0, 1, 2, 3, 4, 5 } \
    (<span style="color: #3C3">V</span>) indices[] = { 0, 2, 1, 3, 4, 5 } \
    (<span style="color: red">F</span>) indices[] = { 1, 0, 2, 3, 4, 5 } \
    (<span style="color: #3C3">V</span>) indices[] = { 5, 4, 3, 1, 2, 0 } 

    **1.2)** Sendo o modo de rasterização **GL_TRIANGLES**, indique com verdadeiro (V) ou falso (F) quais arrays de índices (topologias) abaixo gerariam a malha poligonal acima, <u>considerando distribuição anti-horária dos vértices de cada triângulo</u>:

    (<span style="color: red">F</span>) indices[] = { 0, 2, 1, 3, 5, 4 } \
    (<span style="color: #3C3">V</span>) indices[] = { 0,2,1, 1,2,3, 1,3,4, 4,3,5 } \
    (<span style="color: red">F</span>) indices[] = { 0,1,2, 1,2,3, 1,3,4, 3,4,5 }

    **1.3)** Sendo o modo de rasterização **GL_TRIANGLE_FAN**, indique com verdadeiro (V) ou falso (F) a veracidade das afirmações abaixo:

    (<span style="color: red">F</span>) É possível rasterizar a malha poligonal acima com somente um *triangle fan* \
    (<span style="color: #3C3">V</span>) É possível rasterizar a malha poligonal acima com dois *triangle fans* \
    (<span style="color: red">F</span>) É possível rasterizar a malha poligonal acima com dois *triangle fans*, e existe uma maneira única de se fazer isso \
    (<span style="color: #3C3">V</span>) É possível rasterizar a malha poligonal acima com três *triangle fans*

 2) Considerando os pontos **a**, **p**, **q**, e **r**; o vetor 𝑛; e os ângulos radianos 𝛼, 𝛽, e 𝛾 no desenho abaixo ([confira aqui](https://moodle.inf.ufrgs.br/pluginfile.php/134184/mod_resource/content/4/Questionario_0.pdf)), e sabendo que 𝛼 + 𝛽 = 𝜋/2 e 𝛽 + 𝛾 ≤ 𝜋/2, preencha as lacunas indicadas por parenteses utilizando os símbolos <, ≤, >, ≥, ou =, lembrando que 𝑢 ⋅ 𝑣 = ||𝑢||||𝑣|| cos 𝜃, onde 𝜃 é o menor dos ângulos entre os vetores 𝑢 e 𝑣 :

    (𝐩 − 𝐚) ⋅ 𝑛 (≥) 0 \
    (𝐪 − 𝐚) ⋅ 𝑛 (=) 0 \
    (𝐫 − 𝐚) ⋅ 𝑛 (≤) 0 \
    (𝐩 − 𝐚) ⋅ (𝐪 − 𝐚) (≥) 0 \
    (𝐫 − 𝐚) ⋅ (𝐪 − 𝐚) (≥) 0 \
    (𝐫 − 𝐚) ⋅ (𝐩 − 𝐚) (≥) 0 