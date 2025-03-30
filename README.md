# Projeto EDA – Gestão de Antenas e Efeitos Nefastos

Trabalho realizado no âmbito da unidade curricular de **Estruturas de Dados e Algoritmos**, com o objetivo de aplicar conhecimentos de listas ligadas, manipulação de ficheiros, e estruturas de dados em C.

## 📌 Objetivo

Desenvolver uma aplicação em C capaz de:
- Ler uma matriz de antenas a partir de ficheiro.
- Calcular **efeitos nefastos** entre antenas com a mesma frequência.
- Detetar e tratar **sobreposições** de antenas e efeitos.
- Guardar e carregar ficheiros binários com os dados da aplicação.
- Apresentar visualmente a matriz com diferentes modos de visualização.

## 🧠 Funcionalidades Principais

| Função                           | Descrição                                                                 |
|----------------------------------|---------------------------------------------------------------------------|
| `lerAntenasDoFicheiro()`         | Lê a matriz a partir de um ficheiro `.txt`                               |
| `calcularEfeitoNefasto()`        | Calcula os efeitos nefastos com base na frequência e alinhamento         |
| `salvarBinario()` / `lerBinario()`| Grava e lê os dados em formato binário                                   |
| `redimensionarMatriz()`          | Permite aumentar ou reduzir a matriz (com deslocamento opcional)        |
| `liberarAntena()` / `adicionarAntena()`| Permite modificar as antenas dinamicamente                              |

## 📁 Estrutura do Projeto
ProjetoEDA/ 
├── src/ 
│ ├── Antenas.c 
│ ├── Binario.c 
│ ├── Funções.c
│ ├── Nefasto.c
|  ├── matriz.txt (ficheiro de exemplo) 
│ └── main.c 
├── resources/ 
│ ├── Estruturas.h 
│ └── Funções.h 
├── doxdoc/ 
│ ├── Doxyfile 
│ └── main.tex (relatório LaTeX) 
├── README.md

📝 Relatório (Doxygen + LaTeX)
A documentação técnica e relatório em LaTeX está disponível em doxdoc/main.tex.

Para gerar o PDF:
```bash
cd doxdoc && pdflatex main.tex
```

📌 Observações
- O programa trata linhas vazias e coordenadas inválidas.
- Efeitos nefastos não são desenhados fora dos limites da matriz.
- Sobreposições entre antenas e efeitos são sinalizadas visualmente.

👨‍💻 Autor
Leonel Freitas
Estudante de Engenharia de Sistemas Informáticos – Instituto Politecncio Do Cavado e do Ave
2024–2025
