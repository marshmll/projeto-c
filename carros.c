#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define MAX_LINE_LENGTH 100
#define MAX_BRAND_LENGTH 20
#define MAX_MODEL_LENGTH 30

typedef struct
{
    char brand[MAX_BRAND_LENGTH]; // Marca
    char model[MAX_MODEL_LENGTH]; // Modelo
    int year;                     // Ano
    int mileage;                  // Kilometragem
    float price;                  // Preço
} Car;

int count_lines(FILE *stream);
void read_cars(FILE *stream, Car cars[], int vec_size);
void read_string(char dst[], int max, FILE *stream);
void consume_input_garbage(FILE *stream);
void shift_elements_right(Car vec[], int vec_size, int breakpoint);
void print_options();
void print_car_data(Car *car);
void show_complete_relation(Car cars[], int car_amount);

int main(void)
{
    FILE *file = fopen("carros.txt", "r");
    const int LINE_COUNT = count_lines(file);

    // Cada carro ocupa 5 linhas.
    const int CARS_AMOUNT = LINE_COUNT / 5;

    // Cria um vetor com espaço suficiente para todos os carros presentes no arquivo.
    Car cars[CARS_AMOUNT];

    // Lê todos os registros do arquivo.
    read_cars(file, cars, CARS_AMOUNT);
    fclose(file);

    printf("------------------ CARROS ------------------\n");

    bool stop = false;
    while (stop == false)
    {
        print_options();
        printf("Digite a opção desejada: ");
        char c = tolower(fgetc(stdin));
        consume_input_garbage(stdin);
        printf("\n");

        switch (c)
        {
        case 'a':
            printf("A - Exibir a relação completa de registros.\n\n");
            show_complete_relation(cars, CARS_AMOUNT);
            break;
        case 'b':
            printf("B - Exibir todos os registros de carros de uma marca específica, fornecida pelo usuário via teclado.\n\n");
            break;
        case 'c':
            printf("C - Exibir todos os registros de carros cujo preço esteja entre um valor mínimo e um valor máximo, fornecidos pelo usuário via teclado.\n\n");
            break;
        case 'd':
            printf("D - Inserir um novo registro de carro no vetor; todos os dados do carro devem ser fornecidos pelo usuário via teclado.\n\n");
            break;
        case 'e':
            printf("E - Remover todos os registros de carros do vetor cuja kilometragem seja superior a um valor fornecido pelo usuário via teclado.\n\n");
            break;
        case 'f':
            printf("Até mais!\n");
            stop = true;
            break;
        default:
            printf("Opção inválida, tente novamente.\n");
            break;
        }
    }

    return 0;
}

/**
 * @brief Esta função percorre o arquivo procurando por newlines
 * e contando quantas linhas o mesmo possui.
 *
 * @return int
 */
int count_lines(FILE *file)
{
    int line_count = 1; // Assumindo que o arquivo possui ao menos uma linha.

    char c;
    do
    {
        c = fgetc(file);
        if (c == '\n')
            line_count++;
    } while (c != EOF);

    rewind(file); // Retornando o ponteiro de stream para o início do arquivo.

    return line_count;
}

/**
 * @brief Lê todos os carros presentes no arquivo em structs e os insere em ordem
 * não-decrescente em um dado vetor.
 */
void read_cars(FILE *file, Car cars[], int max_cars)
{
    // Variáveis auxiliares.
    int cars_vec_size = 0;
    Car car;

    for (int i = 0; i < max_cars; i++)
    {
        // Dados a serem lidos.
        char brand[MAX_BRAND_LENGTH];
        char model[MAX_MODEL_LENGTH];
        int year;
        int mileage;
        float price;

        // Lê a marca.
        read_string(brand, MAX_BRAND_LENGTH, file);

        // Lê o modelo.
        read_string(model, MAX_MODEL_LENGTH, file);

        // Lê o ano.
        fscanf(file, "%d", &year);
        consume_input_garbage(file);

        // Lê a quilometragem.
        fscanf(file, "%d", &mileage);
        consume_input_garbage(file);

        // Lê o valor/preço.
        fscanf(file, "%f", &price);
        consume_input_garbage(file);

        // Insere os dados no struct.
        strcpy(car.brand, brand);
        strcpy(car.model, model);
        car.year = year;
        car.mileage = mileage;
        car.price = price;

        int insert_pos;     // Variável para guardar a posição de inserção do carro lido.
        bool found = false; // Flag para verificar se foi encontrada uma posição.

        // Procura a primeira posição onde o valor do carro lido é menor do que o valor do
        // carro na posição.
        for (int j = 0; j < cars_vec_size && found == false; j++)
        {
            if (car.price < cars[j].price)
            {
                insert_pos = j;
                found = true;
            }
        }

        // Se não encontrou, insere na frente do vetor.
        if (!found)
        {
            cars[cars_vec_size] = car;
            cars_vec_size++;
        }
        // Se não, desloca os elementos do vetor para a direita até a posição de inserção
        // e insere o carro na posição.
        else
        {
            shift_elements_right(cars, cars_vec_size, insert_pos);
            cars[insert_pos] = car;
            cars_vec_size++;
        }
    }

    // for (int i = 0; i < 10; i++)
    // 	printf("\"%s\" \"%s\" \"%d\" \"%d\" \"%.2f\"\n", cars[i].brand, cars[i].model, cars[i].year, cars[i].mileage, cars[i].price);
}

/**
 * @brief Lê uma string de um stream com um tamanho máximo, substituindo
 * CRLF por NUL.
 */
void read_string(char dst[], int max, FILE *file)
{
    fgets(dst, max, file);

    int CRLF = strcspn(dst, "\r\n");
    dst[CRLF] = '\0';
}

/**
 * @brief Consome quaisquer caracteres de um stream até que encontre um
 * newline ou EOF
 */
void consume_input_garbage(FILE *file)
{
    char c;
    do
        c = fgetc(file);
    while (c != '\n' && c != EOF);
}

/**
 * @brief Desloca os elementos de um vetor de carros para a direita,
 * até que chegue em um ponto de quebra.
 */
void shift_elements_right(Car vec[], int vec_size, int breakpoint)
{
    for (int i = vec_size - 1; i >= breakpoint; i--)
        vec[i + 1] = vec[i];
}

void print_options()
{
    printf("SELECIONE UMA OPÇÃO: \n\n");
    printf("A - Exibir a relação completa de registros.\n");
    printf("B - Exibir todos os registros de carros de uma marca específica, fornecida pelo usuário via teclado.\n");
    printf("C - Exibir todos os registros de carros cujo preço esteja entre um valor mínimo e um valor máximo, fornecidos pelo usuário via teclado.\n");
    printf("D - Inserir um novo registro de carro no vetor; todos os dados do carro devem ser fornecidos pelo usuário via teclado.\n");
    printf("E - Remover todos os registros de carros do vetor cuja kilometragem seja superior a um valor fornecido pelo usuário via teclado.\n");
    printf("F - Sair do programa.\n\n");
}

void print_car_data(Car *car)
{
    printf("------------------------\n");
    printf("MARCA: %s\n", car->brand);
    printf("MODELO: %s\n", car->model);
    printf("ANO DE FABRICAÇÃO: %d\n", car->year);
    printf("QUILOMETRAGEM: %d Km\n", car->mileage);
    printf("PREÇO: R$ %.2f\n\n", car->price);
}

void show_complete_relation(Car cars[], int car_amount)
{
    for (int i = 0; i < car_amount; i++)
    {
        printf("CARRO %d\n", i + 1);
        print_car_data(&cars[i]);
    }
}