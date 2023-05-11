#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define MAX_WORDS 234337

struct WordBinaryMap {
    // general word length max
    char word[32];
    // binary representation of word
    uint32_t binary;
};

// prototyping
uint32_t makeCypher(char *letter);
void solve(char *letters, char required_letter);
void makeWordBinaryMap(void);

void makeWordBinaryMap(void) {
    // if the binary file already exists, return so we don't regenerate
    FILE *binary_file_r = fopen("word_bin_map.bin", "rb");
    if (binary_file_r) {
        fclose(binary_file_r);
        return;
    }

    struct WordBinaryMap* word_binaries = NULL;
    int num_words = 0;

    // open the dictionary
    // note: small_dictionary.txt is used in this example
    // but, feel free to use a real dictionary similar to curate.py
    FILE *dict_file = fopen("small_dictionary.txt", "r");
    if (!dict_file) {
        perror("Failed to open dictionary file");
        fclose(dict_file);
        return;
    }

    // iterate over each word in the dictionary
    // we'll use each word to create the array of WordBinaryMap structs
    char word[32];
    while (fscanf(dict_file, "%s", word) != EOF) {
        uint32_t binary_word = 0;

        // convert the word to its 32-bit representation
        for (int i = 0; i < strlen(word); i++) {
            binary_word |= (1u << (word[i] - 'a'));
        }

        // allocate memory for the new WordBinaryMap struct
        struct WordBinaryMap* new_word = malloc(sizeof(struct WordBinaryMap));
        if (new_word == NULL) {
            perror("Failed to allocate memory for new_word");
            fclose(binary_file_r);
            fclose(dict_file);
            return;
        }

        // set the values of the new WordBinaryMap struct
        strcpy(new_word->word, word);
        new_word->binary = binary_word;

        // append the new WordBinaryMap struct to the dynamically allocated array
        struct WordBinaryMap* new_word_binaries = realloc(word_binaries, sizeof(struct WordBinaryMap) * (num_words + 1));
        if (new_word_binaries == NULL) {
            perror("Failed to allocate memory for new_word_binaries");
            fclose(binary_file_r);
            fclose(dict_file);
            return;
        }
        word_binaries = new_word_binaries;
        word_binaries[num_words] = *new_word;
        num_words++;

        free(new_word);
    }

    // create the binary file for writing
    FILE *binary_file_w = fopen("word_bin_map.bin", "wb");
    if (!binary_file_w) {
        perror("Failed to open binary file for writing");
        fclose(binary_file_r);
        fclose(dict_file);
        return;
    }

    // write the array of structs to the binary file
    fwrite(word_binaries, sizeof(struct WordBinaryMap), num_words, binary_file_w);

    fclose(binary_file_r);
    fclose(binary_file_w);
    fclose(dict_file);
    free(word_binaries);
}

uint32_t makeCypher(char *letters) {
  uint32_t cypher = 0;
    for (int i = 0; i < strlen(letters); i++) {
        cypher |= (1u << (letters[i] - 'a'));
    }
    return ~cypher;
}

void solve(char *letters, char required_letter) {
    uint32_t cypher = makeCypher(letters);

    FILE* fpWb = fopen("word_bin_map.bin", "rb");
    if (!fpWb) {
        perror("Failed to open binary file");
        fclose(fpWb);
        return;
    }

    // 2 step verification
    struct WordBinaryMap word_binary;
    while (fread(&word_binary, sizeof(word_binary), 1, fpWb) == 1) {
      // check contains subset <= given letters
      if ((cypher & word_binary.binary) == 0) {
        // check required letter
        if ((word_binary.binary & (1u << (required_letter - 'a'))) != 0) {
          printf("%s is a valid word!\n", word_binary.word);
        }
      }
    }

    fclose(fpWb);
}

int main(int argc, char *argv[]) {
  makeWordBinaryMap();

  if (argc != 3) {
    printf("usage: %s <string> <character>\n", argv[0]);
    exit(1);
  }

  if (strlen(argv[2]) != 1) {
    printf("error: second argument must be a single character.\n");
    exit(2);
  }

  char *letters = argv[1];
  char required_letter = argv[2][0];


  solve(letters, required_letter);
  return 0;
}
