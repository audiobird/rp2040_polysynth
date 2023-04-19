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
        printf("Usage: ./sin_table_gen /path/to/output/ [table width in bits] [sample depth in bits]\n");
        return 1;

        case 1:
        printf("Error: Width of wavetable impractical\n");
        return 1;

        case 2:
        printf("Error: Sample bit depth impractical, try <= 32 && > 1\n");
        return 1;

        case 3:
        printf("Error: Could not allocate memory\n");
        return 1;

        case 4:
        printf("Error: Could not create output files\n");
        return 1;

    }
}

const char table_type[][8] = 
{
    "int8_t",
    "int16_t",
    "int32_t",
};

int main(char argc, char** argv)
{
    if (argc != 4)
    return error(0);

    int table_width = strtol(argv[2], NULL, 10);
    int bit_depth = strtol(argv[3], NULL, 10);

    if (table_width > 16 || table_width < 2)
    return error(1);

    if (bit_depth > 32 || bit_depth < 2)
    return error(2);

    const double mult_factor = (double)(1ul << (bit_depth - 1));

    if (bit_depth > 16)
    bit_depth = 2;
    else if (bit_depth > 8)
    bit_depth = 1;
    else
    bit_depth = 0;

    const char * type_string = table_type[bit_depth];

    const unsigned long count = 1ul << table_width;

    char * header_file_name = malloc(strlen(argv[1]) + strlen("sine_table.h "));
    char * src_file_name = malloc(strlen(argv[1]) + strlen("sine_table.c "));

    if (!header_file_name || !src_file_name)
    return error(3);

    strcpy(header_file_name, argv[1]);
    strcat(header_file_name, "sine_table.h");

    strcpy(src_file_name, argv[1]);
    strcat(src_file_name, "sine_table.c");

    FILE * header_f = fopen(header_file_name, "w");
    FILE * src_f = fopen(src_file_name, "w");

    if (!header_f || !src_f)
    return error(4);

    fprintf(header_f, "#ifndef __SINE_TABLE_H__\n"
            "#define __SINE_TABLE_H__\n\n"
            "#include <inttypes.h>\n\n"
            "extern const %s sine_table[%lu];\n\n"
            "#endif\n\n", type_string, count);
    
    

    fprintf(src_f, "#include \"sine_table.h\"\n\n"
            "const %s sine_table[%lu] = \n"
            "{", type_string, count);
    

    for (int x = 0; x < count; x++)
    {
        if (!(x % 8))
        fprintf(src_f, "\n\t");
        
        double temp = 2.0 * M_PI * ((double)(x) / (double)(count));
        double phase = sin(temp);
        phase *= mult_factor;
        phase -= .5;
        fprintf(src_f, "%i, ", (int32_t)(phase));
    }

    fprintf(src_f, "\n};\n");

    fclose(header_f);
    fclose(src_f);
    free(header_file_name);
    free(src_file_name);

    return 0;
}
