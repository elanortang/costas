#include <stdio.h>
#include <stdlib.h>

#define MAX	1000

int main (int argc, char** argv) {
  FILE *input;

  input = fopen (argv[1], "r");

  int n;

  int size = 0;
  int* points;
  points = (int*) malloc (sizeof(int) * MAX );

  int max    = 0;
  int tmp;
  while (1) {
    tmp = fscanf(input, "%i", &n);
    if (tmp == EOF) break;

    if (n > max) max = n;
    points[size++] = n;
  }

  printf ("\\documentclass[border=10pt]{standalone}\n");
  printf ("\\usepackage{tikz}\n\n");

  printf ("\\begin{document}\n");
  printf ("\\begin{tikzpicture}\n");
  printf ("\\tikzset{dot/.style={fill=black,circle,minimum size=20pt}}\n");

  printf ("\\def\\limit{%i}\n\n", max);

  printf ("\\foreach \\y in {1,2,...,\\limit}\n");
  printf ("{\n");
  printf ("  \\draw[color=gray] (0.5,\\y) -- (\\limit+.5,\\y);\n");
  printf ("  \\node at (0.25,\\y){\\y};\n");
  printf ("}\n\n");

  printf ("\\foreach \\x in {1,2,...,\\limit}\n");
  printf ("{\n");
  printf ("  \\draw[color=gray] (\\x,0.5) -- (\\x,\\limit+.5);\n");
  printf ("  \\node at (\\x,0.25){\\x};\n");
  printf ("}\n\n");


  for (int p = 0; p < size; p++) {
    int extra = points[p];
//    if ((p+extra) % 2)
//      printf("\\node[dot, color=red] at (%i,%i){};\n", p + 1, points[p]);
//    else
//      printf("\\node[dot, color=blue] at (%i,%i){};\n", p + 1, points[p]);
/*

    if ((p+extra) % 4 == 0)
      printf("\\node[dot, color=red] at (%i,%i){};\n", p + 1, points[p]);
    if ((p+extra) % 4 == 1)
      printf("\\node[dot, color=blue] at (%i,%i){};\n", p + 1, points[p]);
    if ((p+extra) % 4 == 2)
      printf("\\node[dot, color=green!50!black] at (%i,%i){};\n", p + 1, points[p]);
    if ((p+extra) % 4 == 3)
      printf("\\node[dot, color=black] at (%i,%i){};\n", p + 1, points[p]);
*/
    printf("\\node[dot] at (%i,%i){};\n", p + 1, points[p]);
  }

  printf ("\\end{tikzpicture}\n");
  printf ("\\end{document}\n");
}
