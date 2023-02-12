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
void makeWordBinaryMap();

void makeWordBinaryMap() {
    // if the binary file already exists, return so we don't regenerate
    FILE *binary_file_r = fopen("word_bin_map.bin", "rb");
    if (binary_file_r != NULL) {
      fclose(binary_file_r);
      return;
    }

    struct WordBinaryMap word_binaries[MAX_WORDS];
    int num_words = 0;

    // open the dictionary
    // note: small_dictionary.txt is used in this example
    // but, feel free to use a real dictionary similar to curate.py
    FILE *dict_file = fopen("small_dictionary.txt", "r");
    if (!dict_file) {
        perror("Failed to open dictionary file");
        return;
    }

    // iterate over each word in the dictionary
    char word[32];
    while (fscanf(dict_file, "%s", word) != EOF) {
        uint32_t binary_word = 0;

        // convert the word to its 32-bit representation
        for (int i = 0; i < strlen(word); i++) {
            binary_word |= (1u << (word[i] - 'a'));
        }

        // add the word and its 32-bit representation to the array of structs
        strcpy(word_binaries[num_words].word, word);
        word_binaries[num_words].binary = binary_word;
        num_words++;
    }

    // create the binary file for writing
    FILE *binary_file_w = fopen("word_bin_map.bin", "wb");

    // write the array of structs to the binary file
    fwrite(word_binaries, sizeof(struct WordBinaryMap), num_words, binary_file_w);

    fclose(binary_file_w);
    fclose(dict_file);
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

    FILE* fp = fopen("word_bin_map.bin", "rb");
    if (!fp) {
        perror("Failed to open binary file");
        return;
    }

    // 2 step verification
    struct WordBinaryMap word_binary;
    while (fread(&word_binary, sizeof(word_binary), 1, fp) == 1) {
      // check contains subset <= given letters
      if ((cypher & word_binary.binary) == 0) {
        // check required letter
        if ((word_binary.binary & (1u << (required_letter - 'a'))) != 0) {
          printf("%s is a valid word!\n", word_binary.word);
        }
      }
    }

    fclose(fp);
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