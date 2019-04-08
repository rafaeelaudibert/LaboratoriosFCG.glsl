# Resolução do Questionário 2
### Rafael Baldasso Audibert (00287695)
 
---
1) Considere um segmento de reta definido pelos pontos Cartesianos 𝐚 = (𝑎𝑥, 𝑎𝑦) e 𝐛 = (𝑏𝑥, 𝑏𝑦). Suponha um outro ponto 𝐪 = (𝑞𝑥, 𝑞𝑦). Que teste pode ser feito para determinar de qual lado do segmento o ponto 𝐪 se encontra?

    Fazemos (𝐪 - 𝐚) • (𝐛 - 𝐚), e olhamos para o sinal do resultado, seguindo alguma convenção sobre negativo significar que estamos a direita ou a esquerda do segmento de reta

2) Considere os sistemas de coordenadas ortonormais 𝑥 , 𝑦
com origem 𝐨; e 𝑢, 𝑣 com origem 𝐩, conforme a figura abaixo.


    ![Q2-1](./img/Q2-1.png)

    **2.1)**  Como expressamos um ponto 𝐚 em relação à 𝑥 , 𝑦 e 𝐨?

    𝐚 = 𝐨 + a<sub>𝑥</sub>𝑥 + a<sub>𝑦</sub>𝑦


    **2.2)**   Como expressamos um ponto 𝐚 em relação à 𝑢, 𝑣 , 𝐩?

    𝐚 = 𝐩 + a<sub>𝑢</sub>𝑢 + a<sub>𝑣</sub>𝑣

    **2.3)**    Defina 𝑢 e 𝑣 como uma combinação linear de 𝑥 e 𝑦.

    Sabemos que sin(45º) = √2/2 e cos(45º) = √2/2, então: 
    𝑢 = (√2/2)𝑥 + (√2/2)𝑦 e 𝑣 = -(√2/2)𝑥 + (√2/2)𝑦

    **2.4)**   Assumindo que 𝐩 = 𝐨 + 𝑥 , escreva a operação de mudança de sistema de coordenadas do ponto 𝐚, de 𝑢, 𝑣 , 𝐩 para 𝑥 , 𝑦 , 𝐨, como uma operação matricial com coordenadas homogêneas

    ![Q2-2](./img/Q2-2.png)

3)  Considere as matrizes de escalamento 𝑆(𝑠<sub>𝑥</sub>, 𝑠<sub>𝑦</sub>) e de rotação
𝑅(𝛼) definidas abaixo.

    ![Q2-3](./img/Q2-3.png)

    **3.1)** A transformação 𝑆(2,3)𝑆(2,2) é igual a 𝑆(2,2)𝑆(2,3)?

    *Sim*

    **3.2)**  A transformação 𝑅(𝛼)𝑅(𝛽) é sempre igual a 𝑅(𝛽)𝑅(𝛼)?

    *Sim*

    **3.3)** A transformação 𝑆(3,1)𝑅(𝛼) é sempre igual a 𝑅(𝛼)𝑆(3,1)?

    *Não*

4)  Dado um polígono convexo definido por um conjunto de
vértices 𝐚<sub>1</sub>, 𝐚<sub>2</sub>, 𝐚<sub>3</sub>, … , 𝐚<sub>𝑁</sub>, como você pode testar se um dado
ponto está dentro ou fora do polígono?

    ![Q2-4](./img/Q2-4.png)

    Dado um ponto 𝐩, e sabendo que a<sub>N+1</sub> =

    ![Q2-5](./img/Q2-5.png)

5)  Considere a transformação *T* ilustrada abaixo, que mapeia a figura da esquerda na figura da direita.

    ![Q2-6](./img/Q2-6.png)

    Sabendo que os pontos P<sub>*i*</sub> da imagem são representados em coordenadas homogêneas por matrizes coluna da forma ![x y 1](./img/Q2-7.png) e a imagem transformada é obtida por uma pré-multiplicação, isto é P<sub>*i*</sub><sup>*'*</sup> = TP<sub>*i*</sub>, então, a transformação T é dada por: 

    ![Q2-8](./img/Q2-8.png)
