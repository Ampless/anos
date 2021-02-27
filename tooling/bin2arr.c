#include <stdio.h>
#include <errno.h>

int main(int argc, char **argv) {
        if(argc < 4) return 1;
        FILE *i = fopen(argv[1], "r");
        FILE *o = fopen(argv[2], "w");
        if(!(i && o)) return 1;
        fprintf(o, "const char %s[] = {", argv[3]);
        int c;
        while((c = fgetc(i)) != EOF) fprintf(o, "%d,\n", c);
        fputs("};", o);
        fclose(i);
        fclose(o);
}
