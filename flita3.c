#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>
int main() {
    FILE *file_ptr;
    file_ptr = fopen("matrix.txt", "r");
    char buffer_str[64];
    int len;
    int *matrix = NULL;
    int n = 0;
    while (fgets(buffer_str, 64, file_ptr) != NULL) {
        n++;
        len = (strlen(buffer_str))/2;
        matrix = (int*)realloc(matrix, n*len*sizeof(int));
        for (int i = 0; i < len; i++) {
            *(matrix + len*(n-1) + i) = buffer_str[i*2]-'0';
        }
    }
    fclose(file_ptr);
    int *matrix2 = NULL;
    matrix2 = (int*)calloc(n*n, sizeof(int));
    int m = 0;
    int prev_start = -1;
    int prev_end = -1;
    file_ptr = fopen("graph.txt", "w");
    fputs("graph graphname {\n", file_ptr);
    for (int i = 0; i < n; i++) {
        fprintf(file_ptr, "%c;\n", 'a'+i);
    }
    for (int i = 0; i < len; i++) {
        int start = -1;
        int end = -1;
        for (int j = 0; j < n; j++) {
            if ((*(matrix + j*len + i) == 1) && (start == -1)) {
                start = j;
                end = j;
            }
            else if (*(matrix + j*len + i) == 1) {
                end = j;
                if ((prev_start != start) || (prev_end != end)) {
                    m++;
                }
            }
        }
        if (start == end){
            *(matrix2 + n*start + start) = 1;
            }
        else if (start != end) {
            *(matrix2 + n*start + end) = 1;
            *(matrix2 + n*end + start) = 1;
        }
        prev_start = start;
        prev_end = end;
        fprintf(file_ptr, "%c -- %c;\n", start+'a', end+'a');
    }
    fputs("}", file_ptr);
    fclose(file_ptr);
    system("dot -Tpng graph.txt > graph_img.png");
    int formula = m > (n-1)*(n-2)/2;
    int *vertexs = NULL;
    vertexs = (int*)calloc(n, sizeof(int));
    vertexs[0] = 1;
    int flag = 1;
    while (flag == 1) {
        for (int i = 0; i < n; i++) {
            if (vertexs[i] == 1) {
                for (int k = 0; k < n; k++) {
                    if (*(matrix2 + i*n + k) == 1 && vertexs[k] == 0) {
                        vertexs[k] = 1;
                    }
                }
                vertexs[i] = 2;
            }
        }
        flag = 0;
        for (int i = 0; i < n; i++) {
            if (vertexs[i] == 1) {
                flag = 1;
            }
        }
    }
    int real = 1;
    for (int i = 0; i < n; i++) {
        if (vertexs[i] == 0) {
            real = 0;
        }
        if (vertexs[i] == 1) {
            return(-1);
        }
    }
    switch (formula) {
    case 0:
        puts("according to the formula, the graph is disconnected");
        break;
    case 1:
        puts("according to the formula, the graph is connected");
        break;
    }
    switch (real) {
    case 0:
        puts("in fact, the graph is disconnected");
        break;
    case 1:
        puts("in fact, the graph is connected");
        break;
    }
    return 0;
}
