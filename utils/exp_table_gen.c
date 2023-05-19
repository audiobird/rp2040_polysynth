#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <math.h>

int error(int e)
{
    switch(e)
    {
        case 0:
        printf("Usage: ./exp_table_gen /path/to/output/ [exp curve 0 -> 16]\n");
        return 1;

        case 2:
        printf("Error: Could not allocate memory\n");
        return 1;

        case 3:
        printf("Error: Could not create output files\n");
        return 1;

    }
}

int main(char argc, char** argv)
{
    if (argc != 3)
    return error(0);

    int table_size = 16;
    int exp_scale = (double)strtol(argv[2], NULL, 10);

    const unsigned long count = 1ul << table_size;

    char * header_file_name = malloc(strlen(argv[1]) + strlen("exp_table.h "));
    char * src_file_name = malloc(strlen(argv[1]) + strlen("exp_table.c "));

    if (!header_file_name || !src_file_name)
    return error(2);

    strcpy(header_file_name, argv[1]);
    strcat(header_file_name, "exp_table.h");

    strcpy(src_file_name, argv[1]);
    strcat(src_file_name, "exp_table.c");

    FILE * header_f = fopen(header_file_name, "w");
    FILE * src_f = fopen(src_file_name, "w");

    if (!header_f || !src_f)
    return error(3);

    fprintf(header_f, "#ifndef __exp_TABLE_H__\n"
            "#define __exp_TABLE_H__\n\n"
            "#include <inttypes.h>\n\n"
            "extern const uint16_t exp_table[%lu];\n\n"
            "#endif\n\n", count);
    
    

    fprintf(src_f, "#include \"exp_table.h\"\n\n"
            "const uint16_t exp_table[%lu] = \n"
            "{", count);
    
    int m = 65535;
    int b = exp_scale;
    int t = table_size; 
    int s = 1ul << (t - b);
    double d = 20.0 * log10(pow(2, b) + 1);
    double c = (double)m / d;
    

    for (int x = 0; x < count; x++)
    {
        if (!(x % 8))
        fprintf(src_f, "\n\t");

        double temp = pow(10.0, ((double) (x) / c) / 20) - 1.0;
        temp *= s;
        
        //double temp = 20.0 * log10(((double)x / (double)s) + 1) * c;

        uint out = (uint)temp;
        if (out >= 65536)
        out = 65534;
        
        fprintf(src_f, "%u, ", out);
    }

    fprintf(src_f, "\n};\n");

    fclose(header_f);
    fclose(src_f);
    free(header_file_name);
    free(src_file_name);

    return 0;
}
