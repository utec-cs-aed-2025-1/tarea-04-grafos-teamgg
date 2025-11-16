[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/5zgGDtf4)
[![Open in Visual Studio Code](https://classroom.github.com/assets/open-in-vscode-2e0aaae1b6195c2367325f4f02e2d04e9abb55f0b24a779b69b11b9e10269abc.svg)](https://classroom.github.com/online_ide?assignment_repo_id=21681083&assignment_repo_type=AssignmentRepo)
# Tarea de Grafos

## Integrantes: 
- 1 ____   _____
- 2 ____   _____
- 3 ____   _____

## Objetivo: 
El objetivo de esta tarea es implementar un **Path Finder** para la ciudad de Lima. 

<p align="center">
    <img src=https://github.com/utec-cs-aed/homework_graph/assets/79115974/b63f69db-17eb-417a-8aa1-8483d8dcdaf0 / >
</p>

## Dependencias

Para esta tarea se solicita utilizar ```C++17``` y la librería ```SFML 2.5```

- Para instalar ```SFML 2.5```:

    - [Windows](https://www.youtube.com/watch?v=HkPRG0vfObc)
    - [MacOS y Linux](https://www.youtube.com/playlist?list=PLvv0ScY6vfd95GMoMe2zc4ZgGxWYj3vua)

Cuando se instale la librería, probar que las siguientes líneas del ```CMakeLists.txt``` encuentren la librería adecuadamente.
```cmake
find_package(SFML 2.5 COMPONENTS graphics window REQUIRED)
if(SFML_FOUND)
    target_link_libraries(${PROJECT_NAME} PRIVATE sfml-graphics sfml-window)
else()
    message("SFML not found")
endif()
```

## Dataset
El dataset consiste de dos csv:

- *nodes.csv*

    ![image](https://github.com/utec-cs-aed/homework_graph/assets/79115974/6a68cf06-196a-4605-83a7-3183e9a3f0ec)


- *edges.csv*

    ![image](https://github.com/utec-cs-aed/homework_graph/assets/79115974/247bbbd7-6203-45f4-8196-fcb0434b0f1d)


## Algoritmos
Se les solicita implementar tres algoritmos para busqueda en grafos

- *Dijkstra*

- *Best First Search*

- *A**

Además:
- Analice la complejidad computacional de los tres algoritmos de acuerdo a su propia implementación.
- Puede considere como heuristica la distancia en linea recta.
- **Debe realizar un pequeño video (2 min) mostrando la funcionalidad visual de cada algoritmo**

## Diagrama de clases UML 

![image](https://github.com/utec-cs-aed/homework_graph/assets/79115974/f5a3d89e-cb48-4715-b172-a17e6e27ee24)

----------
> **Créditos:** Juan Diego Castro Padilla [juan.castro.p@utec.edu.pe](mailto:juan.castro.p@utec.edu.pe)




