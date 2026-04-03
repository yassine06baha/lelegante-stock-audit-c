#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PRODUCTS 128
#define MAX_FIELD 96
#define MAX_LINE 512

typedef struct {
    char name[MAX_FIELD];
    char category[MAX_FIELD];
    int stockQuantity;
    int reorderLevel;
    int monthlySales;
    double unitCost;
    int reorderGap;
    int priorityScore;
    double estimatedBudget;
} Product;

static void trimNewline(char *text) {
    size_t length = strlen(text);
    while (length > 0 && (text[length - 1] == '\n' || text[length - 1] == '\r')) {
        text[length - 1] = '\0';
        length--;
    }
}

static void enrichProduct(Product *product) {
    product->reorderGap = product->reorderLevel - product->stockQuantity;
    if (product->reorderGap < 0) {
        product->reorderGap = 0;
    }

    product->priorityScore = product->reorderGap * product->monthlySales;
    product->estimatedBudget = product->reorderGap * product->unitCost;
}

static int compareProducts(const void *left, const void *right) {
    const Product *a = (const Product *)left;
    const Product *b = (const Product *)right;
    return b->priorityScore - a->priorityScore;
}

static int loadProducts(const char *inputPath, Product products[], int *count) {
    FILE *file = fopen(inputPath, "r");
    if (file == NULL) {
        fprintf(stderr, "Impossible d'ouvrir %s\n", inputPath);
        return 0;
    }

    char line[MAX_LINE];
    *count = 0;

    if (fgets(line, sizeof(line), file) == NULL) {
        fclose(file);
        return 0;
    }

    while (fgets(line, sizeof(line), file) != NULL && *count < MAX_PRODUCTS) {
        trimNewline(line);

        Product product;
        char *token = strtok(line, ",");
        if (token == NULL) {
            continue;
        }
        strncpy(product.name, token, MAX_FIELD - 1);
        product.name[MAX_FIELD - 1] = '\0';

        token = strtok(NULL, ",");
        if (token == NULL) {
            continue;
        }
        strncpy(product.category, token, MAX_FIELD - 1);
        product.category[MAX_FIELD - 1] = '\0';

        token = strtok(NULL, ",");
        if (token == NULL) {
            continue;
        }
        product.stockQuantity = atoi(token);

        token = strtok(NULL, ",");
        if (token == NULL) {
            continue;
        }
        product.reorderLevel = atoi(token);

        token = strtok(NULL, ",");
        if (token == NULL) {
            continue;
        }
        product.monthlySales = atoi(token);

        token = strtok(NULL, ",");
        if (token == NULL) {
            continue;
        }
        product.unitCost = atof(token);

        enrichProduct(&product);
        products[*count] = product;
        (*count)++;
    }

    fclose(file);
    qsort(products, *count, sizeof(Product), compareProducts);
    return 1;
}

static void writeJsonString(FILE *file, const char *text) {
    fputc('"', file);
    for (size_t i = 0; text[i] != '\0'; i++) {
        if (text[i] == '"' || text[i] == '\\') {
            fputc('\\', file);
        }
        fputc(text[i], file);
    }
    fputc('"', file);
}

static int exportReport(const char *outputPath, const Product products[], int count) {
    FILE *file = fopen(outputPath, "w");
    if (file == NULL) {
        fprintf(stderr, "Impossible d'ecrire %s\n", outputPath);
        return 0;
    }

    fprintf(file, "window.LELEGANTE_STOCK_REPORT = [\n");

    for (int i = 0; i < count; i++) {
        const Product *product = &products[i];
        fprintf(file, "  {\"name\": ");
        writeJsonString(file, product->name);
        fprintf(file, ", \"category\": ");
        writeJsonString(file, product->category);
        fprintf(
            file,
            ", \"stockQuantity\": %d, \"reorderLevel\": %d, \"monthlySales\": %d, "
            "\"priorityScore\": %d, \"estimatedBudget\": %.2f}%s\n",
            product->stockQuantity,
            product->reorderLevel,
            product->monthlySales,
            product->priorityScore,
            product->estimatedBudget,
            i == count - 1 ? "" : ","
        );
    }

    fprintf(file, "];\n");
    fclose(file);
    return 1;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <input_csv> <output_js>\n", argv[0]);
        return 1;
    }

    Product products[MAX_PRODUCTS];
    int count = 0;

    if (!loadProducts(argv[1], products, &count)) {
        return 1;
    }

    if (!exportReport(argv[2], products, count)) {
        return 1;
    }

    printf("Rapport genere pour %d produits dans %s\n", count, argv[2]);
    return 0;
}
