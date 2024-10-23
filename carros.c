/**
 * PROGRAMAÇÃO IMPERATIVA
 * Nome: Renan da Silva Oliveira Andrade.
 * 
 * 23/10/2024.
 */

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 100 // Tamanho máximo de leitura de linhas.
#define MAX_BRAND_LENGTH 20 // Tamanho máximo da string de marca.
#define MAX_MODEL_LENGTH 30 // Tamanho máximo da string de modelo.
#define EXTRA_CARS_SPAN 10  // Tamanho máximo de carros que podem ser inseridos a mais.

typedef struct
{
    char brand[MAX_BRAND_LENGTH]; // Marca
    char model[MAX_MODEL_LENGTH]; // Modelo
    int year;                     // Ano
    int mileage;                  // Quilometragem
    float price;                  // Preço
} Car;

// Funções de interação com o usuário.

void show_complete_relation(Car cars[], int cars_amount);
void show_brand_only(Car cars[], int cars_amount, char *brand);
void show_between_price_range(Car cars[], int cars_amount, float min, float max);
void insert_new_car(Car cars[], int *cars_amount, Car *new_car);
void remove_overrunned_cars(Car cars[], int *cars_amount, int max_mileage);

// Funções auxiliares.

int count_lines(FILE *stream);
void read_cars(FILE *stream, Car cars[], int vec_size);
void read_string(char dst[], int max, FILE *stream);
void consume_input_garbage(FILE *stream);
void shift_elements_right(Car vec[], int vec_size, int breakpoint);
void strtolower(char str[]);
void print_options();
void print_car_data(Car *car);

int main(void)
{
    char *fname = "carros.txt";
    FILE *file = fopen(fname, "r");

    // Checa se o arquivo foi aberto corretamente.
    if (file == NULL)
    {
        printf("Não foi possível abrir o arquivo. Encerrando.\n");
        exit(1);
    }

    const int LINE_COUNT = count_lines(file);
    
    // Cada carro ocupa 5 linhas.
    const int INITIAL_CARS_AMOUNT = LINE_COUNT / 5;

    int cars_amount = INITIAL_CARS_AMOUNT;

    // Cria um vetor com espaço suficiente para todos os carros presentes no
    // arquivo + os carros extras.
    Car cars[INITIAL_CARS_AMOUNT + EXTRA_CARS_SPAN];

    // Lê todos os registros do arquivo.
    read_cars(file, cars, cars_amount);
    fclose(file);

    system("clear");
    printf("------------------ CARROS ------------------\n");

    bool stop = false;
    while (stop == false)
    {
        print_options();
        printf("Digite a opção desejada: ");
        char c = tolower(fgetc(stdin));
        consume_input_garbage(stdin);
        system("clear");

        switch (c)
        {
        case 'a':
            printf("A - Exibir a relação completa de registros.\n\n");

            show_complete_relation(cars, cars_amount);
            break;
        case 'b':
            printf("B - Exibir todos os registros de carros de uma marca específica, fornecida pelo usuário via teclado.\n\n");

            printf("Digite a marca desejada: ");
            char brand[MAX_BRAND_LENGTH];
            read_string(brand, MAX_BRAND_LENGTH, stdin);

            show_brand_only(cars, cars_amount, brand);
            break;
        case 'c':
            printf("C - Exibir todos os registros de carros cujo preço esteja entre um valor mínimo e um valor máximo, fornecidos pelo usuário via teclado.\n\n");

            float min, max = 0.00;
            printf("Digite o valor mínimo de preço: ");
            fscanf(stdin, "%f", &min);
            consume_input_garbage(stdin);

            printf("Digite o valor máximo de preço: ");
            fscanf(stdin, "%f", &max);
            consume_input_garbage(stdin);

            if (min > max)
                printf("O valor mínimo não pode ser maior que o máximo.\n");
            else
                show_between_price_range(cars, cars_amount, min, max);
            break;
        case 'd':
            printf("D - Inserir um novo registro de carro no vetor; todos os dados do carro devem ser fornecidos pelo usuário via teclado.\n\n");

            // Caso o vetor tenha sido completamente preenchido
            if (cars_amount >= INITIAL_CARS_AMOUNT + EXTRA_CARS_SPAN)
            {
                printf("A quantidade máxima de carros no vetor foi atingida.\n\n");
                break;
            }

            Car car;

            // Lê a marca.
            printf("Digite a marca do novo carro: ");
            read_string(car.brand, MAX_BRAND_LENGTH, stdin);

            // Lê o modelo.
            printf("Digite o modelo do novo carro: ");
            read_string(car.model, MAX_MODEL_LENGTH, stdin);

            // Lê o ano.
            printf("Digite o ano do novo carro: ");
            fscanf(stdin, "%d", &(car.year));
            consume_input_garbage(stdin);

            // Lê a quilometragem.
            printf("Digite a quilometragem do novo carro: ");
            fscanf(stdin, "%d", &(car.mileage));
            consume_input_garbage(stdin);

            // Lê o valor/preço.
            printf("Digite o preço do novo carro: ");
            fscanf(stdin, "%f", &(car.price));
            consume_input_garbage(stdin);

            insert_new_car(cars, &cars_amount, &car);

            break;
        case 'e':
            printf("E - Remover todos os registros de carros do vetor cuja kilometragem seja superior a um valor fornecido pelo usuário via teclado.\n\n");

            int max_mileage = 0;
            printf("Digite a quilometragem de corte para remover carros com quilometragem superior: ");
            fscanf(stdin, "%d", &max_mileage);
            consume_input_garbage(stdin);

            remove_overrunned_cars(cars, &cars_amount, max_mileage);
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

void show_complete_relation(Car cars[], int cars_amount)
{
    for (int i = 0; i < cars_amount; i++)
    {
        printf("CARRO %d\n", i + 1);
        print_car_data(&cars[i]);
    }
}

void show_brand_only(Car cars[], int cars_amount, char *brand)
{
    // Pega a versão lowercase da string passada pelo usuário.
    char user_brand[strlen(brand) + 1];
    strcpy(user_brand, brand);
    strtolower(user_brand);

    bool found = false;
    for (int i = 0; i < cars_amount; i++)
    {
        // Pega a versão lowercase da string do struct.
        char car_brand[strlen(cars[i].brand) + 1];
        strcpy(car_brand, cars[i].brand);
        strtolower(car_brand);

        // Se for a marca desejada pelo usuário
        if (strcmp(car_brand, user_brand) == 0)
        {
            printf("CARRO %d\n", i + 1);
            print_car_data(&cars[i]);
            found = true;
        }
    }

    // Se não encontrou nenhum registro, avisa o usuário.
    if (!found)
        printf("Nenhum carro com a marca \"%s\" encontrado.\n\n", brand);
}

void show_between_price_range(Car cars[], int cars_amount, float min, float max)
{
    bool found = false;
    for (int i = 0; i < cars_amount; i++)
    {
        if (cars[i].price >= min && cars[i].price <= max)
        {
            printf("CARRO %d\n", i + 1);
            print_car_data(&cars[i]);
            found = true;
        }
    }

    // Se não encontrou nenhum registro, avisa o usuário.
    if (!found)
        printf("Nenhum carro de preço entre %.2f e %.2f foi encontrado.\n\n", min, max);
}

void remove_overrunned_cars(Car cars[], int *cars_amount, int max_mileage)
{
    bool found = false;
    for (int i = 0; i < *cars_amount; i++)
    {
        if (cars[i].mileage > max_mileage)
        {
            printf("REMOVENDO CARRO: ");
            print_car_data(&cars[i]);
            found = true;

            shift_elements_left(cars, *cars_amount, i);
            *cars_amount = *cars_amount - 1;
            i--;
        }
    }

    // Se não encontrou nenhum registro, avisa o usuário.
    if (!found)
        printf("Nenhum carro com mais de %d km foi encontrado.\n\n", max_mileage);
}

void insert_new_car(Car cars[], int *cars_amount, Car *new_car)
{
    int front_index = *cars_amount;
    cars[front_index] = *new_car;
    (*cars_amount)++;

    printf("INSERIDO NOVO CARRO: ");
    print_car_data(&cars[front_index]);
}

/**
 * @brief Esta função percorre o arquivo procurando por newlines
 * e contando quantas linhas o mesmo possui.
 *
 * @return int
 */
int count_lines(FILE *stream)
{
    int line_count = 1; // Assumindo que o arquivo possui ao menos uma linha.

    char c;
    do
    {
        c = fgetc(stream);
        if (c == '\n')
            line_count++;
    } while (c != EOF);

    rewind(stream); // Retornando o ponteiro de stream para o início do arquivo.

    return line_count;
}

/**
 * @brief Lê todos os carros presentes no arquivo em structs e os insere em ordem
 * não-decrescente em um dado vetor.
 */
void read_cars(FILE *stream, Car cars[], int max_cars)
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
        read_string(brand, MAX_BRAND_LENGTH, stream);

        // Lê o modelo.
        read_string(model, MAX_MODEL_LENGTH, stream);

        // Lê o ano.
        fscanf(stream, "%d", &year);
        consume_input_garbage(stream);

        // Lê a quilometragem.
        fscanf(stream, "%d", &mileage);
        consume_input_garbage(stream);

        // Lê o valor/preço.
        fscanf(stream, "%f", &price);
        consume_input_garbage(stream);

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
}

/**
 * @brief Lê uma string de um stream com um tamanho máximo, substituindo
 * CRLF por NUL.
 */
void read_string(char dst[], int max, FILE *stream)
{
    fgets(dst, max, stream);

    int CRLF = strcspn(dst, "\r\n");
    dst[CRLF] = '\0';
}

/**
 * @brief Consome quaisquer caracteres de um stream até que encontre um
 * newline ou EOF
 */
void consume_input_garbage(FILE *stream)
{
    char c;
    do
        c = fgetc(stream);
    while (c && c != '\n' && c != EOF);
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

/**
 * @brief Desloca os elementos de um vetor de carros para a esquerda,
 * sobrepondo uma dada posição.
 */
void shift_elements_left(Car vec[], int vec_size, int collapse_point)
{
    for (int i = collapse_point; i < vec_size - 1; i++)
        vec[i] = vec[i + 1];
}

/**
 * @brief Itera sobre uma string (char[]) e converte todos os caracteres
 * para lowercase.
 */
void strtolower(char str[])
{
    int len = strlen(str);
    for (int i = 0; i < len; i++)
        str[i] = tolower(str[i]);
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