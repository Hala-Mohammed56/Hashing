//Hala Mohammed
//1210312
// Sec:3 - Saadeddin Hroub


//includes several libraries
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>


// Constant definition for MaxWord - MaxTableSize  & Prime
#define MaxWord 20
int MaxTableSize =20;
int  Prime=7;


// Hash Table struct to store word and meanings
typedef struct {
    char word[MaxWord];
    char meaning1[MaxWord];
    char meaning2[MaxWord];
    char meaning3[MaxWord];
} HashTable;


// pointers declaration for two-dimensional arrays
HashTable **QuadraticHT;
HashTable **DoubleHT;


//To track number of collisions in Tabels
int collisionCountQHT=0;
int collisionCountDHT=0;


// Hash function that return the index of the string
int hash(char* word, int TableSize) {
    int hashValue = 0;

    while (*word != '\0') {
        hashValue = ((hashValue << 5) + *word++) % TableSize;
    }

    return (hashValue % TableSize);
}


// Quadratic hash function
int QHash(int x, int i, int size) {
    return (x + i * i) % size;
}


// Double hash function
int DHash(int x, int i, int size, int R) {
    int h1 = x % size;
    int h2 = R - (x % R);

    return (h1 + i * h2) % size;
}


// Function to insert an entry into a hash table using quadratic Hash
void QuadraticInsert(char* word, char* m1, char* m2, char* m3) {
    int size = MaxTableSize;

    int hashValue = hash(word, size);
    int index = QHash(hashValue, 0, size);

    int i = 1;
    int Ccount=0;
    while (QuadraticHT[index] != NULL) {
        index = QHash(hashValue, i, size);
        collisionCountQHT++;
        Ccount++;
        if (i >= size) {
            printf("Quadratic Hash Table is full. Cannot insert!\n");
            return;
        }
        printf("Collision occurred at index %d during Quadratic insert. Total collisions: %d\n", index, Ccount);
        i++;
    }


    HashTable* Current = (HashTable*)malloc(sizeof(HashTable));
    if (!Current) {
         perror("Error allocating memory");
         exit(EXIT_FAILURE);
    }

    strcpy(Current->word, word);
    strcpy(Current->meaning1, m1);
    strcpy(Current->meaning2, m2);
    strcpy(Current->meaning3, m3);

    QuadraticHT[index] = Current;
}


// Function to insert an entry into a hash table using Double Hash
void DoubleInsert(char* word , char* m1 , char* m2 , char* m3) {
    int size = MaxTableSize;
    int R = Prime;

    int hashValue = hash(word, size);
    int index = DHash(hashValue, 0, size, R);

    int i = 1;
    int Ccount=0;
    while (DoubleHT[index] != NULL) {
        index = DHash(hashValue, i, size, R);
        collisionCountDHT++;
        Ccount++;
        if (i >= size) {
            printf("Double Hashing Hash Table is full. Cannot insert!\n");
            return;
        }
        printf("Collision occurred at index %d during Double insert. Total collisions: %d\n", index, Ccount);
        i++;
    }

    HashTable* Current = (HashTable*)malloc(sizeof(HashTable));
    if (!Current) {
         perror("Error allocating memory");
         exit(EXIT_FAILURE);
    }
    strcpy(Current->word, word);
    strcpy(Current->meaning1, m1);
    strcpy(Current->meaning2, m2);
    strcpy(Current->meaning3, m3);

    DoubleHT[index] = Current;
}


//Function to counts the number of non-empty lines in words file
int CountLines() {
    FILE *fp = fopen("words.txt", "r");
    if (!fp) {
        perror("Error opening file");
        return -1;
    }

    int count = 0;
    char ch;

    while ((ch = fgetc(fp)) != EOF) {
        if (ch == '\n') {
            count++;
        }
    }

    fclose(fp);

    return count;
}


//Function to checks if the integer is a prime number or not
bool isPrime(int n) {
    if (n <= 1) {
        return false;
    }

    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0) {
            return false;
        }
    }

    return true;
}


//function to calculates the table size
int calculateTableSize() {
    int x = CountLines();
    int size = (2 * x) + 5;

    Prime = size;

    while (!isPrime(Prime)) {
        Prime--;
    }

    return size;
}


//Function to Print hashed tables (including empty spots).
void printTable(HashTable *hashTable[]) {
    printf("Hash Table Contents:\n");
    for (int i = 0; i < MaxTableSize; i++) {
        if (hashTable[i] != NULL) {
            printf("Index %d: Word = %s, Meaning 1 = %s, Meaning 2 = %s, Meaning 3 = %s\n",
                   i, hashTable[i]->word, hashTable[i]->meaning1, hashTable[i]->meaning2,
                   hashTable[i]->meaning3);
        } else {
            printf("Index %d: -----------\n", i);
        }
    }
}


//Function to Print out table size and the load factor.
void printTableSize_LoadFactor(HashTable *hashTable[]) {
    int size = 0;
    for (int i = 0; i < MaxTableSize; i++) {
        if (hashTable[i] != NULL) {
            size++;
        }
    }

    float loadFactor = (float)size / MaxTableSize;

    printf("Size: %d\n", MaxTableSize);
    printf("Load Factor: %.1f\n", loadFactor);
}


//Function to Print out the used hash functions
void printUsedHashFunctions() {
    printf("The used hash functions:\n");
    printf("1) Quadratic Hash: ((x + i * i) %% size)\n");
    printf("2) Hash2:( (h1 + i * h2) %% size )\n");
    printf("h1(x) = x %% size \n");
    printf("h2(x) = Prime - (x %% Prime)\n");
    printf("\n");
}


//Function to read data from the file
void ReadingFromTheFile() {
    FILE *fp = fopen("words.txt", "r");
    if (!fp) {
        perror("Error in 'words.txt' File!");
        exit(EXIT_FAILURE);
    }
    printf("The File Opened Successfully\n");

    char line[100];

    while (fgets(line, sizeof(line), fp) != NULL) {

        char *word = strtok(line, ":");
        char *m1 = strtok(NULL, "#");
        char *m2 = strtok(NULL, "#");
        char *m3 = strtok(NULL, "\n");

        //remove leading and trailing whitespaces
        word = strtok(word, " \t\r\n");
        m1 = strtok(m1, " \t\r\n");
        m2 = strtok(m2, " \t\r\n");
        m3 = strtok(m3, " \t\r\n");

        if (word != NULL) {
            if (m2 == NULL) {
                m2 = "--------";
            }
            if (m3 == NULL) {
                m3 = "--------";
            }

            QuadraticInsert(word, m1, m2, m3);
            DoubleInsert(word, m1, m2, m3);
        } else {
            printf("Error reading line: %s\n", line);
        }
    }

    fclose(fp);
}


//Function To search for a word in Quadratic Hash Table
int QuadraticHashSearch(char *word) {
    for (int initial = 0; initial < MaxTableSize; initial++) {
        int index = initial;
        int i = 0;
        while (QuadraticHT[index] != NULL) {
            if (strcmp(QuadraticHT[index]->word, word) == 0) {
                return index;
            }
            i++;
            index = QHash(hash(word, MaxTableSize), i, MaxTableSize);
            if (index == initial) {
                break;
            }
        }
    }
    return -1;  // Word not found
}


//Function To search for a word in Double Hash Table
int DoubleHashSearch(char *word) {
    for (int initial = 0; initial < MaxTableSize; initial++) {
        int index = initial;
        int i = 0;
        while (DoubleHT[index] != NULL) {
            if (strcmp(DoubleHT[index]->word, word) == 0) {
                return index;
            }
            i++;
            index = DHash(hash(word, MaxTableSize), i, MaxTableSize, Prime);
            if (index == initial) {
                break;
            }
        }
    }
    return -1;  // Word not found
}


//Function to search for word in tables
void Search() {
    printf("Enter word to search: ");
    char word[MaxWord];
    scanf("%s", word);

    printf("Choose table: 1 for Quadratic Hashing, 2 for Double Hashing: ");
    int table;
    scanf("%d", &table);

    if (table == 1) {
        int QIndex = QuadraticHashSearch(word);
        if (QIndex != -1) {
            printf("Word found at Index %d in Quadratic Hash Table!\n", QIndex);
        } else {
            printf("Word is not found in Quadratic Hash Table!\n");
        }
    } else if (table == 2) {
        int DIndex = DoubleHashSearch(word);
        if (DIndex != -1) {
            printf("Word found at Index %d in Double Hash Table!\n", DIndex);
        } else {
            printf("Word is not found in Double Hash Table!\n");
        }
    } else {
        printf("Invalid table choice.\n");
    }
}


//Function to delete the word from the Quadratic Table
int QuadraticDelete(char *word) {
    int index = QuadraticHashSearch(word);
    if (index != -1) {
        free(QuadraticHT[index]);
        QuadraticHT[index] = NULL;
        return 1;
    } else {
        return -1;
    }
}


//Function to delete the word from the Double Table
int DoubleDelete(char *word) {
    int index = DoubleHashSearch(word);
    if (index != -1) {
        free(DoubleHT[index]);
        DoubleHT[index] = NULL;
        return 1;
    } else {
        return -1;
    }
}


//Function to delete the word from both hash tables
void Delete() {
    char word[MaxWord];
    printf("Enter word to delete: ");
    scanf("%s", word);

    int QDeleted = -1;
    int DDeleted = -1;

    if (QuadraticHashSearch(word) != -1) {
        QDeleted = QuadraticDelete(word);
    }

    if (DoubleHashSearch(word) != -1) {
        DDeleted = DoubleDelete(word);
    }

    if (QDeleted != -1 && DDeleted != -1) {
        printf("Word was deleted from both table!\n");
    } else {
        printf("Word not found in any table.\n");
    }
}


//Function to print the number of collisions in each table if it not zero
void CollisionCounts() {

        if (collisionCountQHT != 0) {
            printf("The Number of Collisions in Quadratic Hashing Table is: %d\n", collisionCountQHT);
        }
        if (collisionCountDHT != 0) {
            printf("The Number of Collisions in Double Hashing Table is: %d\n", collisionCountDHT);
        }
}


// Function to save results to an output file
void SaveHashTable(HashTable *hashTable[]) {
    FILE *fp = fopen("saved_words.txt", "w");
    if (fp == NULL) {
        perror("Error in saved_words.txt File!");
        return;
    }

    for (int i = 0; i < MaxTableSize; i++) {
        if (hashTable[i] != NULL) {
            fprintf(fp, "Index:%d  %s:%s#%s#%s \n",
                    i, hashTable[i]->word, hashTable[i]->meaning1,
                    hashTable[i]->meaning2, hashTable[i]->meaning3);
        } else {
            fprintf(fp, "Index:%d  -------------\n", i);
        }
    }

    fclose(fp);
}


//Function to to insert a new record into the hash tables
void InsertInMenu() {
    char word[MaxWord];
    char m1[MaxWord];
    char m2[MaxWord];
    char m3[MaxWord];

    printf("Enter word: ");
    scanf("%s", word);

    printf("Enter meaning1: ");
    scanf("%s", m1);

    printf("Do you want to enter meaning 2? (1 for Yes, 0 for No): ");
    int enterM2;
    scanf("%d", &enterM2);

    if (enterM2) {
        printf("Enter meaning 2: ");
        scanf("%s", m2);
    } else {
        m2[0] = '\0';
    }

    printf("Do you want to enter meaning3? (1 for Yes, 0 for No): ");
    int enterM3;
    scanf("%d", &enterM3);

    if (enterM3) {
        printf("Enter meaning3: ");
        scanf("%s", m3);
    } else {
        m3[0] = '\0';
    }

    int result1 = QuadraticHashSearch(word);
    int result2 = DoubleHashSearch(word);

    if (result1 == -1 && result2 == -1) {
        QuadraticInsert(word, m1, m2, m3);
        DoubleInsert(word, m1, m2, m3);
        printf("The word inserted successfully!\n");
    } else {
        printf("This word already exists in the hash tables!\n");
    }
}


// Function to display the menu options
void view_menu() {
    printf("1. Print hashed tables\n");
    printf("2. Print table size and the load factor\n");
    printf("3. Print the used hash functions\n");
    printf("4. Insert a new record to hash table\n");
    printf("5. Search for a specific word\n");
    printf("6. Delete a specific word (from both tables)\n");
    printf("7. The number of collisions in each tables\n");
    printf("8. Save the hash table\n");
    printf("9. Exit\n\n");
}


// Function to get the user's choice from the menu
int The_Choice() {
    int choice;
    printf("Enter your choice: ");
    scanf("%d", &choice);
    return choice;
}


// Function to perform the chosen menu action
void Menu_Choice(int choice) {
    switch (choice) {
        case 1:
            printf("Quadratic Hashing Table:\n");
            printTable(QuadraticHT);
            printf("\n______________________\n");
            printf("Double Hashing Table\n");
            printTable(DoubleHT);
            printf("\n");
            break;
        case 2:
            printf("Quadratic Hashing Table:\n");
            printTableSize_LoadFactor(QuadraticHT);
            printf("\n______________________\n");
            printf("Double Hashing Table\n");
            printTableSize_LoadFactor(DoubleHT);
            printf("\n");
            break;
        case 3:
            printUsedHashFunctions();
            break;
        case 4:
            InsertInMenu();
            break;
        case 5:
            Search();
            break;
        case 6:
            Delete();
            break;
        case 7:
            CollisionCounts();
            break;
        case 8:
            SaveHashTable(DoubleHT);
            printf("Saved successfully!\n");
            break;
        case 9:

            printf("Exiting...\n");
            break;
        default:
            printf("Invalid choice. Please try again.\n");
    }
}


// main function
int main() {
    MaxTableSize = calculateTableSize();

    // Allocate memory for QuadraticHT and DoubleHT arrays
    QuadraticHT = (HashTable**)malloc(MaxTableSize * sizeof(HashTable*));
    DoubleHT = (HashTable**)malloc(MaxTableSize * sizeof(HashTable*));

    // Initialize the hash tables (updated size)
    for (int i = 0; i < MaxTableSize; i++) {
        QuadraticHT[i] = NULL;
        DoubleHT[i] = NULL;
    }

    ReadingFromTheFile();

    int choice;

    do {
        printf("\n");
        view_menu();
        choice = The_Choice();
        Menu_Choice(choice);
        printf("______________________________________________________________\n");
        printf("\n");
    } while (choice != 9);

    // Free the memory
    for (int i = 0; i < MaxTableSize; i++) {
        if (QuadraticHT[i] != NULL) {
            free(QuadraticHT[i]);
        }
        if (DoubleHT[i] != NULL) {
            free(DoubleHT[i]);
        }
    }

    // Free the memory
    free(QuadraticHT);
    free(DoubleHT);

    return 0;
}
