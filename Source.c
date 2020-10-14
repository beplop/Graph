#include <stdio.h>
#include <stdlib.h>

void connectivity(int edges, int vertices) {
    if (edges > (((vertices - 1) * (vertices - 2)) / 2)) {
        printf("This graph is connected\n");
    }
    else {
        printf("This graph is not connected\n");
    }
}

int main(void) {
    int directed = 0;// ориентированность графа
    int is_weight = 0;// взвешенность графа
    int c;
    int vertices = 0;// кол-во вершин
    int edges = 0;// кол-во ребер
   
    FILE* MatrF = fopen("matr.txt", "r");
    if (MatrF == NULL) {
        printf("No such File!!!");
    }
    else {
        printf("File is read successfully!\n");
        while (!feof(MatrF)) {
            if ((fgetc(MatrF)) == ';') {
                vertices++;
            }
        }
        fclose(MatrF);
        FILE* MatrFile = fopen("matr.txt", "r");
        while ((c = fgetc(MatrFile)) != ';') {
            if (c == ',')
                edges++;
        }
        edges++;
        fclose(MatrFile);
      
        int matrix[vertices][edges];
        MatrFile = fopen("matr.txt", "r");
        for (int i = 0; i < vertices; i++) {
            int j = 0;
            while ((c = fgetc(MatrFile)) != ';')
                fscanf(MatrFile, "%d", &matrix[i][j++]);
        }
        fclose(MatrFile);


        for (int i = 0; i < vertices; i++) {
            for (int j = 0; j < edges; j++) {
                printf("%d ", matrix[i][j]);
                if (matrix[i][j] > 1)
                    is_weight = 1;
                if (matrix[i][j] < 0)
                    directed = 1;
            }
            printf("\n");
        }
        char weight[edges];// массив взвешенных узлов
        if (is_weight) {
            for (int j = 0; j < edges; j++) {
                for (int i = 0; i < edges; i++) {
                    if (matrix[i][j] > 0)
                        weight[j] = matrix[i][j];
                }
                printf("%d - %d\n", j, weight[j]);
            }
        }

        char VertNames[vertices]; // массив вершин
        for (int i = 0; i < vertices; i++)
            VertNames[i] = 'a' + i;
        char Connect[edges][2];// массив соединенных вершин
        for (int j = 0; j < edges; j++) {
            int k = 0;
            for (int i = 0; i < vertices; i++) {
                if (matrix[i][j] < 0)
                    Connect[j][1] = VertNames[i];
                if (matrix[i][j] > 0)
                    Connect[j][k++] = VertNames[i];
            }
        }
        printf("\n");
        for (int i = 0; i < edges; i++) {
            for (int j = 0; j < 2; j++) {
                if (!Connect[i][j]) {
                    Connect[i][j] = Connect[i][j - 1];
                }
                printf("%c ", Connect[i][j]);
            }
            printf("\n");
        }

        FILE* GraphF = fopen("graph.dot", "w");
        if (GraphF == NULL) {
            printf("Problem with making a file");
        }
        else {
            printf("Graph file is made successfully!\n");
            if (directed)
                fprintf(GraphF, "di");
            fprintf(GraphF, "graph Test {\n");
            for (int i = 0; i < edges; i++) {
                if (!directed) {
                    fprintf(GraphF, "%c -- %c ", Connect[i][0], Connect[i][1]);
                    if (!is_weight) { fprintf(GraphF, ";\n"); }
                }
                else {
                    fprintf(GraphF, "%c -> %c ", Connect[i][0], Connect[i][1]);
                    if (!is_weight) { fprintf(GraphF, ";\n"); }
                }
                if (is_weight)
                    fprintf(GraphF, "[label = %d] \n", weight[i]);
            }
            fprintf(GraphF, "}\n");
            fclose(GraphF);
            system("dot -Tpng graph.dot -o graph.png");
            system("cygstart graph.png");
        }
        putchar('\n');
        connectivity(edges, vertices); // чек на связность
    }
    return 0;
}
