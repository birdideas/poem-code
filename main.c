/* poem-code -- Poem code encryption program written for fun
   Copyright (C) 2025 Joaquim

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "main.h"

/* The number of words in a st'd poem cipher: */
#define WORDS 5

#define TMP_BUFFER_SIZE 4096
#define MAX_BUFFER_SIZE 32768

struct PoemType {
    char *buf;
    char **word_positions;
    int *word_indices;
};

/* https://stackoverflow.com/questions/8236/ */
off_t fsize(filename)
    const char *filename;
{
    int ret;
    struct stat st;

    ret = stat(filename, &st);

    if (ret != 0) {
        perror("Error reading file stats");
        return -1;
    }

    if (st.st_size <= 0) {
        perror("Bad filesize");
        return -1;
    }

    return st.st_size;
}

/*
 * ingest_file reads a file into a buffer, but to avoid
 * looping through the file twice, it also reads the
 * word_indices parameter, which is not ideal.
 */
char *ingest_file(poem_obj, filename, size)
    struct PoemType *poem_obj;
    char *filename;
    off_t size;
{
    char tmp_buffer[TMP_BUFFER_SIZE] = {0};
    FILE *fp;

    if (size >= MAX_BUFFER_SIZE) {
        fprintf(stderr, "File too big!");
        return NULL;
    }

    fp = fopen(filename, "r");
    if (fp == NULL) {
        perror("Error opening file");
        return NULL;
    }

    while (fgets(tmp_buffer, TMP_BUFFER_SIZE, fp) != NULL) {
        if (tmp_buffer[0] == '#' || tmp_buffer[0] == '\n') {
            continue;
        } else if (tmp_buffer[0] == '>') {
            word_numbers(tmp_buffer, poem_obj->word_indices);
        } else {
            strcat(poem_obj->buf, tmp_buffer);
        }
    }

    fclose(fp);
    return poem_obj->buf;
}

/*
 * word_numbers reads a single line string in the format
 * `> %d %d %d %d %d\n`
 * and loads its numbers into num_list[5]
 *
 * TODO: `strtol` is a great function that allows us to
 * do this without using `strtok`, but it outputs a long,
 * whereas an int is simple and makes more sense.
 * I kept all of the objects as ints to keep the program
 * simple, but I should double check what happens when
 * converting from int to long.
 */
void word_numbers(str, num_list)
    char *str;
    int num_list[WORDS];
{
    int i = 1;
    char *separator;

    ++str;

    num_list[0] = strtol(str, &separator, 10);
    while (i < WORDS) {
        num_list[i++] = strtol(separator, &separator, 10);
    }
}

void filter_text(buf)
    char *buf;
{
    char big_tmp_buffer[MAX_BUFFER_SIZE] = {0};
    char c;
    int i = 0, j = 0;

    while ((c = buf[i++])) {
        if (c == '\n') {
            big_tmp_buffer[j++] = ' ';
        } else if (!isalpha(c) && c != ' ') {
            continue;
        } else if (islower(c)) {
            big_tmp_buffer[j++] = c ^ 32;
        } else {
            big_tmp_buffer[j++] = c;
        }
    }

    strncpy(buf, big_tmp_buffer, j);
    buf[j] = '\0';
}

/*
 * strtok_replace loops through a string and finds the desired
 * words from their indices, all while replacing every space in
 * our big buffer with a null terminator, so that our five words
 * can be addressed as strings without copying any memory.
 *
 * TODO: strlen not explicitly required here. Function could be
 * written a little better;
 */
void strtok_replace(poem)
    struct PoemType *poem;
{
    static size_t i = 0;

    static int j = 0;
    static int counted_spaces = 0;

    char curr;

    for (; (curr = poem->buf[i]); ++i) {
        if (curr != ' ') {
            continue;
        }

        if (++counted_spaces == (poem->word_indices[j] - 1)) {
            poem->word_positions[j++] = (poem->buf + i + 1);
        }

        poem->buf[i] = '\0';
    }
}

int main(argc, argv)
    int argc;
    char *argv[];
{
    /* Declarations */
    int i = 0;

    off_t filesize;

    int word_indices[WORDS] = {0};
    char *word_positions[WORDS] = {0};
    char read_buffer[MAX_BUFFER_SIZE] = {0};
    struct PoemType main_poem;
    /* / */

    main_poem.buf = read_buffer;
    main_poem.word_indices = word_indices;
    main_poem.word_positions = word_positions;

    (void) argc;

    filesize = fsize(argv[1]);
    ingest_file(&main_poem, argv[1], filesize);
    filter_text(main_poem.buf);

    strtok_replace(&main_poem);
    while (i < WORDS) {
        puts(main_poem.word_positions[i++]);
    }

    return 0;
}

