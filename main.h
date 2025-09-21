/* main.h -- Header files
   Copyright (C) 2025 Joaquim

   This file is part of poem-code.

   poem-code is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   poem-code is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

struct PoemType;

off_t fsize(const char *filename);
char *ingest_file(struct PoemType *poem_obj, char *filename, off_t size);
void word_numbers(char *str, int num_list[5]);
void filter_text(struct PoemType *poem_obj);
void strtok_replace(struct PoemType *poem_obj);
int main(int argc, char *argv[]);
