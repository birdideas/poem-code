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
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "main.h"

#define TMP_BUFFER_SIZE 4096
#define MAX_BUFFER_SIZE 32768

/* https://stackoverflow.com/questions/8236/ */
off_t fsize(filename)
    const char *filename;
{
    int ret;
    struct stat st;

    ret = stat(filename, &st);

    if(ret != 0) {
        perror("Error reading file stats");
        return -1;
    }

    if(st.st_size <= 0) {
        perror("Bad filesize");
        return -1;
    }

    return st.st_size;
}

char *ingest_file(filename, buf, size)
    char *filename, *buf;
    off_t size;
{
    char tmp_buffer[TMP_BUFFER_SIZE] = {0};
    FILE *fp;

    if(size >= MAX_BUFFER_SIZE) {
        fprintf(stderr, "File too big!");
        return NULL;
    }

    fp = fopen(filename, "r");
    if(fp == NULL) {
        perror("Error opening file");
        return NULL;
    }

    while(fgets(tmp_buffer, TMP_BUFFER_SIZE, fp) != NULL) {
        if(tmp_buffer[0] == '#' || tmp_buffer[0] == '\n') {
            continue;
        }

        strcat(buf, tmp_buffer);
    }

    fclose(fp);
    return buf;
}

void filter_text(buf)
    char *buf;
{
    char big_tmp_buffer[MAX_BUFFER_SIZE] = {0};
    char c;
    int i = 0, j = 0;

    while((c = buf[i++])) {
        if(c == '\n') {
            big_tmp_buffer[j++] = ' ';
        } else if(!isalpha(c) && c != ' ') {
            continue;
        } else if(islower(c)) {
            big_tmp_buffer[j++] = c ^ 32;
        } else {
            big_tmp_buffer[j++] = c;
        }
    }

    strncpy(buf, big_tmp_buffer, j);
    buf[j] = '\0';
}

int main(argc, argv)
    int argc;
    char *argv[];
{
    int i;

    off_t filesize;

    char read_buffer[MAX_BUFFER_SIZE] = {0};
    char *current;

    for(i = 1; i < argc; ++i) {
        current = argv[i];

        filesize = fsize(current);
        ingest_file(current, read_buffer, filesize);
        filter_text(read_buffer);

        puts(read_buffer);
    }

    return 0;
}

