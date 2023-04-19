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
        printf("Usage: ./phase_step_table_gen /path/to/output/ [table size in bits] [sample rate in hz]\n");
        return 1;

        case 1:
        printf("Error: Size of table impractical\n");
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
    if (argc != 4)
    return error(0);

    int table_size = strtol(argv[2], NULL, 10);
    double sample_rate = (double)strtol(argv[3], NULL, 10);

    if (table_size > 16 || table_size < 7)
    return error(1);

    const unsigned long count = 1ul << table_size;

    char * header_file_name = malloc(strlen(argv[1]) + strlen("phase_step_table.h "));
    char * src_file_name = malloc(strlen(argv[1]) + strlen("phase_step_table.c "));

    if (!header_file_name || !src_file_name)
    return error(2);

    strcpy(header_file_name, argv[1]);
    strcat(header_file_name, "phase_step_table.h");

    strcpy(src_file_name, argv[1]);
    strcat(src_file_name, "phase_step_table.c");

    FILE * header_f = fopen(header_file_name, "w");
    FILE * src_f = fopen(src_file_name, "w");

    if (!header_f || !src_f)
    return error(3);

    fprintf(header_f, "#ifndef __PHASE_STEP_TABLE_H__\n"
            "#define __PHASE_STEP_TABLE_H__\n\n"
            "#include <inttypes.h>\n\n"
            "extern const uint32_t phase_step_table[%lu];\n\n"
            "#endif\n\n", count);
    
    

    fprintf(src_f, "#include \"phase_step_table.h\"\n\n"
            "const uint32_t phase_step_table[%lu] = \n"
            "{", count);
    

    for (int x = 0; x < count; x++)
    {
        if (!(x % 8))
        fprintf(src_f, "\n\t");
        
        double temp = 440.0 * exp(((x / (double)(count / 128)) - 69.0) * log(pow(2.0, 1.0 / 12.0)));	
        temp = (4294967296.0) * temp / sample_rate;
        
        fprintf(src_f, "%u, ", (uint)(temp));
    }

    fprintf(src_f, "\n};\n");

    fclose(header_f);
    fclose(src_f);
    free(header_file_name);
    free(src_file_name);

    return 0;
}
