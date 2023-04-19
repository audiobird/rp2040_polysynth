#include <stdio.h>
#include <math.h>

int main()
{
    printf( "#include \"log_table.h\"\n\n");
    printf("const uint16_t log_table[128] = \n{\n\t");

    for (int x = 0; x < 128; x++)
    {
        if (!(x % 8) && x != 0)
        printf("\n\t");

        int temp = (int)((pow(((float)(x)/128.0), 4) * (1U << 16)));
        printf("%u, ", temp);
    }
    printf("\n};\n\n");

    printf( "uint16_t log_table_get(uint8_t midi_val)\n"
            "{\n"
            "   return log_table[midi_val];\n"
            "}\n");


    return 0;
}