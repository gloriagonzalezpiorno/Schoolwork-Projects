#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <CUnit/Basic.h>

#include "src/utils.h"
#include "src/tables.h"
#include "src/translate_utils.h"
#include "src/translate.h"

const char* TMP_FILE = "test_output.txt";
const int BUF_SIZE = 1024;

/****************************************
 *  Helper functions 
 ****************************************/

int do_nothing() {
    return 0;
}

int init_log_file() {
    set_log_file(TMP_FILE);
    return 0;
}

int check_lines_equal(char **arr, int num) {
    char buf[BUF_SIZE];

    FILE *f = fopen(TMP_FILE, "r");
    if (!f) {
        CU_FAIL("Could not open temporary file");
        return 0;
    }
    for (int i = 0; i < num; i++) {
        if (!fgets(buf, BUF_SIZE, f)) {
            CU_FAIL("Reached end of file");
            return 0;
        }
        CU_ASSERT(!strncmp(buf, arr[i], strlen(arr[i])));
    }
    fclose(f);
    return 0;
}

/****************************************
 *  Test cases for translate_utils.c 
 ****************************************/

void test_translate_reg() {
    CU_ASSERT_EQUAL(translate_reg("$zero"), 0);
    CU_ASSERT_EQUAL(translate_reg("$0"), 0);
    CU_ASSERT_EQUAL(translate_reg("$at"), 1);
    CU_ASSERT_EQUAL(translate_reg("$v0"), 2);
    CU_ASSERT_EQUAL(translate_reg("$a0"), 4);
    CU_ASSERT_EQUAL(translate_reg("$a1"), 5);
    CU_ASSERT_EQUAL(translate_reg("$a2"), 6);
    CU_ASSERT_EQUAL(translate_reg("$a3"), 7);
    CU_ASSERT_EQUAL(translate_reg("$t0"), 8);
    CU_ASSERT_EQUAL(translate_reg("$t1"), 9);
    CU_ASSERT_EQUAL(translate_reg("$t2"), 10);
    CU_ASSERT_EQUAL(translate_reg("$t3"), 11);
    CU_ASSERT_EQUAL(translate_reg("$t4"), 12);
    CU_ASSERT_EQUAL(translate_reg("$t5"), 13);
    CU_ASSERT_EQUAL(translate_reg("$t6"), 14);
    CU_ASSERT_EQUAL(translate_reg("$t7"), 15);
    CU_ASSERT_EQUAL(translate_reg("$s0"), 16);
    CU_ASSERT_EQUAL(translate_reg("$s1"), 17);
    CU_ASSERT_EQUAL(translate_reg("$s2"), 18);
    CU_ASSERT_EQUAL(translate_reg("$s3"), 19);
    CU_ASSERT_EQUAL(translate_reg("$s4"), 20);
    CU_ASSERT_EQUAL(translate_reg("$s5"), 21);
    CU_ASSERT_EQUAL(translate_reg("$s6"), 22);
    CU_ASSERT_EQUAL(translate_reg("$s7"), 23);
    CU_ASSERT_EQUAL(translate_reg("$t8"), 24);
    CU_ASSERT_EQUAL(translate_reg("$t9"), 25);
    CU_ASSERT_EQUAL(translate_reg("$k0"), 26);
    CU_ASSERT_EQUAL(translate_reg("$k1"), 27);
    CU_ASSERT_EQUAL(translate_reg("$gp"), 28);
    CU_ASSERT_EQUAL(translate_reg("$sp"), 29);
    CU_ASSERT_EQUAL(translate_reg("$fp"), 30);
    CU_ASSERT_EQUAL(translate_reg("$ra"), 31);
    CU_ASSERT_EQUAL(translate_reg("$3"), -1);
    CU_ASSERT_EQUAL(translate_reg("$t33"), -1);
    CU_ASSERT_EQUAL(translate_reg("asdf"), -1);
    CU_ASSERT_EQUAL(translate_reg("hey there"), -1);
}

void test_translate_num() {
    long int output;

    CU_ASSERT_EQUAL(translate_num(&output, "35", -1000, 1000), 0);
    CU_ASSERT_EQUAL(output, 35);
    CU_ASSERT_EQUAL(translate_num(&output, "145634236", 0, 9000000000), 0);
    CU_ASSERT_EQUAL(output, 145634236);
    CU_ASSERT_EQUAL(translate_num(&output, "0xC0FFEE", -9000000000, 9000000000), 0);
    CU_ASSERT_EQUAL(output, 12648430);
    CU_ASSERT_EQUAL(translate_num(&output, "72", -16, 72), 0);
    CU_ASSERT_EQUAL(output, 72);
    CU_ASSERT_EQUAL(translate_num(&output, "72", -16, 71), -1);
    CU_ASSERT_EQUAL(translate_num(&output, "72", 72, 150), 0);
    CU_ASSERT_EQUAL(output, 72);
    CU_ASSERT_EQUAL(translate_num(&output, "72", 73, 150), -1);
    CU_ASSERT_EQUAL(translate_num(&output, "35x", -100, 100), -1);
}

void test_translate_num_more() {
    long int output;
    int max = 100;
    for (int i = 0; i < max; i++) {
        char* buf = malloc(sizeof(char) * 16);
        sprintf(buf, "%d", i);
        if (i <= 50) {
            CU_ASSERT_EQUAL(translate_num(&output, buf, 0, 50), 0);
        } else {
            CU_ASSERT_EQUAL(translate_num(&output, buf, 0, 50), -1);
        }
        free(buf);
    }
     // CU_ASSERT_EQUAL(translate_num(&output, "", -1, 255), -1);
     CU_ASSERT_EQUAL(translate_num(&output, "0x0", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x1", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x2", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x3", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x4", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x5", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x6", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x7", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x8", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x9", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0xa", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0xb", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0xc", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0xd", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0xe", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0xf", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x10", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x11", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x12", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x13", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x14", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x15", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x16", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x17", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x18", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x19", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x1a", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x1b", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x1c", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x1d", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x1e", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x1f", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x20", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x21", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x22", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x23", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x24", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x25", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x26", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x27", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x28", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x29", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x2a", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x2b", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x2c", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x2d", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x2e", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x2f", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x30", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x31", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x32", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x33", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x34", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x35", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x36", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x37", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x38", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x39", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x3a", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x3b", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x3c", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x3d", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x3e", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x3f", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x40", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x41", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x42", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x43", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x44", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x45", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x46", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x47", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x48", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x49", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x4a", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x4b", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x4c", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x4d", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x4e", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x4f", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x50", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x51", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x52", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x53", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x54", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x55", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x56", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x57", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x58", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x59", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x5a", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x5b", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x5c", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x5d", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x5e", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x5f", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x60", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x61", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x62", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x63", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x64", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x65", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x66", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x67", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x68", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x69", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x6a", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x6b", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x6c", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x6d", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x6e", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x6f", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x70", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x71", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x72", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x73", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x74", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x75", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x76", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x77", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x78", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x79", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x7a", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x7b", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x7c", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x7d", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x7e", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x7f", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x80", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x81", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x82", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x83", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x84", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x85", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x86", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x87", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x88", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x89", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x8a", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x8b", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x8c", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x8d", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x8e", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x8f", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x90", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x91", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x92", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x93", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x94", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x95", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x96", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x97", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x98", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x99", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x9a", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x9b", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x9c", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x9d", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x9e", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0x9f", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0xa0", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0xa1", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0xa2", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0xa3", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0xa4", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0xa5", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0xa6", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0xa7", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0xa8", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0xa9", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0xaa", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0xab", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0xac", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0xad", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0xae", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0xaf", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0xb0", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0xb1", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0xb2", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0xb3", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0xb4", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0xb5", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0xb6", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0xb7", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0xb8", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0xb9", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0xba", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0xbb", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0xbc", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0xbd", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0xbe", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0xbf", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0xc0", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0xc1", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0xc2", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0xc3", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0xc4", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0xc5", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0xc6", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0xc7", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0xc8", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0xc9", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0xca", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0xcb", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0xcc", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0xcd", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0xce", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0xcf", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0xd0", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0xd1", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0xd2", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0xd3", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0xd4", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0xd5", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0xd6", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0xd7", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0xd8", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0xd9", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0xda", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0xdb", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0xdc", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0xdd", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0xde", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0xdf", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0xe0", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0xe1", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0xe2", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0xe3", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0xe4", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0xe5", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0xe6", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0xe7", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0xe8", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0xe9", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0xea", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0xeb", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0xec", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0xed", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0xee", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0xef", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0xf0", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0xf1", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0xf2", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0xf3", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0xf4", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0xf5", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0xf6", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0xf7", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0xf8", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0xf9", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0xfa", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0xfb", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0xfc", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0xfd", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0xfe", -1, 255), 0);
     CU_ASSERT_EQUAL(translate_num(&output, "0xff", -1, 255), 0);

    CU_ASSERT_EQUAL(translate_num(&output, "0x0", -1, 255), 0);
    CU_ASSERT_EQUAL(output, 0);
    CU_ASSERT_EQUAL(translate_num(&output, "0x0", -1, 255), 0);
    CU_ASSERT_EQUAL(output, 0);
    CU_ASSERT_EQUAL(translate_num(&output, "0x4", -1, 255), 0);
    CU_ASSERT_EQUAL(output, 4);
    CU_ASSERT_EQUAL(translate_num(&output, "0x4", -1, 255), 0);
    CU_ASSERT_EQUAL(output, 4);
    CU_ASSERT_EQUAL(translate_num(&output, "0x8", -1, 255), 0);
    CU_ASSERT_EQUAL(output, 8);
    CU_ASSERT_EQUAL(translate_num(&output, "0x8", -1, 255), 0);
    CU_ASSERT_EQUAL(output, 8);
    CU_ASSERT_EQUAL(translate_num(&output, "0xc", -1, 255), 0);
    CU_ASSERT_EQUAL(output, 12);
    CU_ASSERT_EQUAL(translate_num(&output, "0xc", -1, 255), 0);
    CU_ASSERT_EQUAL(output, 12);
    CU_ASSERT_EQUAL(translate_num(&output, "0x10", -1, 255), 0);
    CU_ASSERT_EQUAL(output, 16);
    CU_ASSERT_EQUAL(translate_num(&output, "0x10", -1, 255), 0);
    CU_ASSERT_EQUAL(output, 16);
    CU_ASSERT_EQUAL(translate_num(&output, "0x14", -1, 255), 0);
    CU_ASSERT_EQUAL(output, 20);
    CU_ASSERT_EQUAL(translate_num(&output, "0x14", -1, 255), 0);
    CU_ASSERT_EQUAL(output, 20);
    CU_ASSERT_EQUAL(translate_num(&output, "0x18", -1, 255), 0);
    CU_ASSERT_EQUAL(output, 24);
    CU_ASSERT_EQUAL(translate_num(&output, "0x18", -1, 255), 0);
    CU_ASSERT_EQUAL(output, 24);
    CU_ASSERT_EQUAL(translate_num(&output, "0x1c", -1, 255), 0);
    CU_ASSERT_EQUAL(output, 28);
    CU_ASSERT_EQUAL(translate_num(&output, "0x1c", -1, 255), 0);
    CU_ASSERT_EQUAL(output, 28);
    CU_ASSERT_EQUAL(translate_num(&output, "0x20", -1, 255), 0);
    CU_ASSERT_EQUAL(output, 32);
    CU_ASSERT_EQUAL(translate_num(&output, "0x20", -1, 255), 0);
    CU_ASSERT_EQUAL(output, 32);
    CU_ASSERT_EQUAL(translate_num(&output, "0x24", -1, 255), 0);
    CU_ASSERT_EQUAL(output, 36);
    CU_ASSERT_EQUAL(translate_num(&output, "0x24", -1, 255), 0);
    CU_ASSERT_EQUAL(output, 36);
    CU_ASSERT_EQUAL(translate_num(&output, "0x28", -1, 255), 0);
    CU_ASSERT_EQUAL(output, 40);
    CU_ASSERT_EQUAL(translate_num(&output, "0x28", -1, 255), 0);
    CU_ASSERT_EQUAL(output, 40);
    CU_ASSERT_EQUAL(translate_num(&output, "0x2c", -1, 255), 0);
    CU_ASSERT_EQUAL(output, 44);
    CU_ASSERT_EQUAL(translate_num(&output, "0x2c", -1, 255), 0);
    CU_ASSERT_EQUAL(output, 44);
    CU_ASSERT_EQUAL(translate_num(&output, "0x30", -1, 255), 0);
    CU_ASSERT_EQUAL(output, 48);
    CU_ASSERT_EQUAL(translate_num(&output, "0x30", -1, 255), 0);
    CU_ASSERT_EQUAL(output, 48);
    CU_ASSERT_EQUAL(translate_num(&output, "0x34", -1, 255), 0);
    CU_ASSERT_EQUAL(output, 52);
    CU_ASSERT_EQUAL(translate_num(&output, "0x34", -1, 255), 0);
    CU_ASSERT_EQUAL(output, 52);
    CU_ASSERT_EQUAL(translate_num(&output, "0x38", -1, 255), 0);
    CU_ASSERT_EQUAL(output, 56);
    CU_ASSERT_EQUAL(translate_num(&output, "0x38", -1, 255), 0);
    CU_ASSERT_EQUAL(output, 56);
    CU_ASSERT_EQUAL(translate_num(&output, "0x3c", -1, 255), 0);
    CU_ASSERT_EQUAL(output, 60);
    CU_ASSERT_EQUAL(translate_num(&output, "0x3c", -1, 255), 0);
    CU_ASSERT_EQUAL(output, 60);
    CU_ASSERT_EQUAL(translate_num(&output, "0x40", -1, 255), 0);
    CU_ASSERT_EQUAL(output, 64);
    CU_ASSERT_EQUAL(translate_num(&output, "0x40", -1, 255), 0);
    CU_ASSERT_EQUAL(output, 64);
    CU_ASSERT_EQUAL(translate_num(&output, "0x44", -1, 255), 0);
    CU_ASSERT_EQUAL(output, 68);
    CU_ASSERT_EQUAL(translate_num(&output, "0x44", -1, 255), 0);
    CU_ASSERT_EQUAL(output, 68);
    CU_ASSERT_EQUAL(translate_num(&output, "0x48", -1, 255), 0);
    CU_ASSERT_EQUAL(output, 72);
    CU_ASSERT_EQUAL(translate_num(&output, "0x48", -1, 255), 0);
    CU_ASSERT_EQUAL(output, 72);
    CU_ASSERT_EQUAL(translate_num(&output, "0x4c", -1, 255), 0);
    CU_ASSERT_EQUAL(output, 76);
    CU_ASSERT_EQUAL(translate_num(&output, "0x4c", -1, 255), 0);
    CU_ASSERT_EQUAL(output, 76);
    CU_ASSERT_EQUAL(translate_num(&output, "0x50", -1, 255), 0);
    CU_ASSERT_EQUAL(output, 80);
    CU_ASSERT_EQUAL(translate_num(&output, "0x50", -1, 255), 0);
    CU_ASSERT_EQUAL(output, 80);
    CU_ASSERT_EQUAL(translate_num(&output, "0x54", -1, 255), 0);
    CU_ASSERT_EQUAL(output, 84);
    CU_ASSERT_EQUAL(translate_num(&output, "0x54", -1, 255), 0);
    CU_ASSERT_EQUAL(output, 84);
    CU_ASSERT_EQUAL(translate_num(&output, "0x58", -1, 255), 0);
    CU_ASSERT_EQUAL(output, 88);
    CU_ASSERT_EQUAL(translate_num(&output, "0x58", -1, 255), 0);
    CU_ASSERT_EQUAL(output, 88);
    CU_ASSERT_EQUAL(translate_num(&output, "0x5c", -1, 255), 0);
    CU_ASSERT_EQUAL(output, 92);
    CU_ASSERT_EQUAL(translate_num(&output, "0x5c", -1, 255), 0);
    CU_ASSERT_EQUAL(output, 92);
    CU_ASSERT_EQUAL(translate_num(&output, "0x60", -1, 255), 0);
    CU_ASSERT_EQUAL(output, 96);
    CU_ASSERT_EQUAL(translate_num(&output, "0x60", -1, 255), 0);
    CU_ASSERT_EQUAL(output, 96);
    CU_ASSERT_EQUAL(translate_num(&output, "0x64", -1, 255), 0);
    CU_ASSERT_EQUAL(output, 100);
    CU_ASSERT_EQUAL(translate_num(&output, "0x64", -1, 255), 0);
    CU_ASSERT_EQUAL(output, 100);
    CU_ASSERT_EQUAL(translate_num(&output, "0x68", -1, 255), 0);
    CU_ASSERT_EQUAL(output, 104);
    CU_ASSERT_EQUAL(translate_num(&output, "0x68", -1, 255), 0);
    CU_ASSERT_EQUAL(output, 104);
    CU_ASSERT_EQUAL(translate_num(&output, "0x6c", -1, 255), 0);
    CU_ASSERT_EQUAL(output, 108);
    CU_ASSERT_EQUAL(translate_num(&output, "0x6c", -1, 255), 0);
    CU_ASSERT_EQUAL(output, 108);
    CU_ASSERT_EQUAL(translate_num(&output, "0x70", -1, 255), 0);
    CU_ASSERT_EQUAL(output, 112);
    CU_ASSERT_EQUAL(translate_num(&output, "0x70", -1, 255), 0);
    CU_ASSERT_EQUAL(output, 112);
    CU_ASSERT_EQUAL(translate_num(&output, "0x74", -1, 255), 0);
    CU_ASSERT_EQUAL(output, 116);
    CU_ASSERT_EQUAL(translate_num(&output, "0x74", -1, 255), 0);
    CU_ASSERT_EQUAL(output, 116);
    CU_ASSERT_EQUAL(translate_num(&output, "0x78", -1, 255), 0);
    CU_ASSERT_EQUAL(output, 120);
    CU_ASSERT_EQUAL(translate_num(&output, "0x78", -1, 255), 0);
    CU_ASSERT_EQUAL(output, 120);
    CU_ASSERT_EQUAL(translate_num(&output, "0x7c", -1, 255), 0);
    CU_ASSERT_EQUAL(output, 124);
    CU_ASSERT_EQUAL(translate_num(&output, "0x7c", -1, 255), 0);
    CU_ASSERT_EQUAL(output, 124);
    CU_ASSERT_EQUAL(translate_num(&output, "0x80", -1, 255), 0);
    CU_ASSERT_EQUAL(output, 128);
    CU_ASSERT_EQUAL(translate_num(&output, "0x80", -1, 255), 0);
    CU_ASSERT_EQUAL(output, 128);
    CU_ASSERT_EQUAL(translate_num(&output, "0x84", -1, 255), 0);
    CU_ASSERT_EQUAL(output, 132);
    CU_ASSERT_EQUAL(translate_num(&output, "0x84", -1, 255), 0);
    CU_ASSERT_EQUAL(output, 132);
    CU_ASSERT_EQUAL(translate_num(&output, "0x88", -1, 255), 0);
    CU_ASSERT_EQUAL(output, 136);
    CU_ASSERT_EQUAL(translate_num(&output, "0x88", -1, 255), 0);
    CU_ASSERT_EQUAL(output, 136);
    CU_ASSERT_EQUAL(translate_num(&output, "0x8c", -1, 255), 0);
    CU_ASSERT_EQUAL(output, 140);
    CU_ASSERT_EQUAL(translate_num(&output, "0x8c", -1, 255), 0);
    CU_ASSERT_EQUAL(output, 140);
    CU_ASSERT_EQUAL(translate_num(&output, "0x90", -1, 255), 0);
    CU_ASSERT_EQUAL(output, 144);
    CU_ASSERT_EQUAL(translate_num(&output, "0x90", -1, 255), 0);
    CU_ASSERT_EQUAL(output, 144);
    CU_ASSERT_EQUAL(translate_num(&output, "0x94", -1, 255), 0);
    CU_ASSERT_EQUAL(output, 148);
    CU_ASSERT_EQUAL(translate_num(&output, "0x94", -1, 255), 0);
    CU_ASSERT_EQUAL(output, 148);
    CU_ASSERT_EQUAL(translate_num(&output, "0x98", -1, 255), 0);
    CU_ASSERT_EQUAL(output, 152);
    CU_ASSERT_EQUAL(translate_num(&output, "0x98", -1, 255), 0);
    CU_ASSERT_EQUAL(output, 152);
    CU_ASSERT_EQUAL(translate_num(&output, "0x9c", -1, 255), 0);
    CU_ASSERT_EQUAL(output, 156);
    CU_ASSERT_EQUAL(translate_num(&output, "0x9c", -1, 255), 0);
    CU_ASSERT_EQUAL(output, 156);
    CU_ASSERT_EQUAL(translate_num(&output, "0xa0", -1, 255), 0);
    CU_ASSERT_EQUAL(output, 160);
    CU_ASSERT_EQUAL(translate_num(&output, "0xa0", -1, 255), 0);
    CU_ASSERT_EQUAL(output, 160);
    CU_ASSERT_EQUAL(translate_num(&output, "0xa4", -1, 255), 0);
    CU_ASSERT_EQUAL(output, 164);
    CU_ASSERT_EQUAL(translate_num(&output, "0xa4", -1, 255), 0);
    CU_ASSERT_EQUAL(output, 164);
    CU_ASSERT_EQUAL(translate_num(&output, "0xa8", -1, 255), 0);
    CU_ASSERT_EQUAL(output, 168);
    CU_ASSERT_EQUAL(translate_num(&output, "0xa8", -1, 255), 0);
    CU_ASSERT_EQUAL(output, 168);
    CU_ASSERT_EQUAL(translate_num(&output, "0xac", -1, 255), 0);
    CU_ASSERT_EQUAL(output, 172);
    CU_ASSERT_EQUAL(translate_num(&output, "0xac", -1, 255), 0);
    CU_ASSERT_EQUAL(output, 172);
    CU_ASSERT_EQUAL(translate_num(&output, "0xb0", -1, 255), 0);
    CU_ASSERT_EQUAL(output, 176);
    CU_ASSERT_EQUAL(translate_num(&output, "0xb0", -1, 255), 0);
    CU_ASSERT_EQUAL(output, 176);
    CU_ASSERT_EQUAL(translate_num(&output, "0xb4", -1, 255), 0);
    CU_ASSERT_EQUAL(output, 180);
    CU_ASSERT_EQUAL(translate_num(&output, "0xb4", -1, 255), 0);
    CU_ASSERT_EQUAL(output, 180);
    CU_ASSERT_EQUAL(translate_num(&output, "0xb8", -1, 255), 0);
    CU_ASSERT_EQUAL(output, 184);
    CU_ASSERT_EQUAL(translate_num(&output, "0xb8", -1, 255), 0);
    CU_ASSERT_EQUAL(output, 184);
    CU_ASSERT_EQUAL(translate_num(&output, "0xbc", -1, 255), 0);
    CU_ASSERT_EQUAL(output, 188);
    CU_ASSERT_EQUAL(translate_num(&output, "0xbc", -1, 255), 0);
    CU_ASSERT_EQUAL(output, 188);
    CU_ASSERT_EQUAL(translate_num(&output, "0xc0", -1, 255), 0);
    CU_ASSERT_EQUAL(output, 192);
    CU_ASSERT_EQUAL(translate_num(&output, "0xc0", -1, 255), 0);
    CU_ASSERT_EQUAL(output, 192);
    CU_ASSERT_EQUAL(translate_num(&output, "0xc4", -1, 255), 0);
    CU_ASSERT_EQUAL(output, 196);
    CU_ASSERT_EQUAL(translate_num(&output, "0xc4", -1, 255), 0);
    CU_ASSERT_EQUAL(output, 196);
    CU_ASSERT_EQUAL(translate_num(&output, "0xc8", -1, 255), 0);
    CU_ASSERT_EQUAL(output, 200);
    CU_ASSERT_EQUAL(translate_num(&output, "0xc8", -1, 255), 0);
    CU_ASSERT_EQUAL(output, 200);
    CU_ASSERT_EQUAL(translate_num(&output, "0xcc", -1, 255), 0);
    CU_ASSERT_EQUAL(output, 204);
    CU_ASSERT_EQUAL(translate_num(&output, "0xcc", -1, 255), 0);
    CU_ASSERT_EQUAL(output, 204);
    CU_ASSERT_EQUAL(translate_num(&output, "0xd0", -1, 255), 0);
    CU_ASSERT_EQUAL(output, 208);
    CU_ASSERT_EQUAL(translate_num(&output, "0xd0", -1, 255), 0);
    CU_ASSERT_EQUAL(output, 208);
    CU_ASSERT_EQUAL(translate_num(&output, "0xd4", -1, 255), 0);
    CU_ASSERT_EQUAL(output, 212);
    CU_ASSERT_EQUAL(translate_num(&output, "0xd4", -1, 255), 0);
    CU_ASSERT_EQUAL(output, 212);
    CU_ASSERT_EQUAL(translate_num(&output, "0xd8", -1, 255), 0);
    CU_ASSERT_EQUAL(output, 216);
    CU_ASSERT_EQUAL(translate_num(&output, "0xd8", -1, 255), 0);
    CU_ASSERT_EQUAL(output, 216);
    CU_ASSERT_EQUAL(translate_num(&output, "0xdc", -1, 255), 0);
    CU_ASSERT_EQUAL(output, 220);
    CU_ASSERT_EQUAL(translate_num(&output, "0xdc", -1, 255), 0);
    CU_ASSERT_EQUAL(output, 220);
    CU_ASSERT_EQUAL(translate_num(&output, "0xe0", -1, 255), 0);
    CU_ASSERT_EQUAL(output, 224);
    CU_ASSERT_EQUAL(translate_num(&output, "0xe0", -1, 255), 0);
    CU_ASSERT_EQUAL(output, 224);
    CU_ASSERT_EQUAL(translate_num(&output, "0xe4", -1, 255), 0);
    CU_ASSERT_EQUAL(output, 228);
    CU_ASSERT_EQUAL(translate_num(&output, "0xe4", -1, 255), 0);
    CU_ASSERT_EQUAL(output, 228);
    CU_ASSERT_EQUAL(translate_num(&output, "0xe8", -1, 255), 0);
    CU_ASSERT_EQUAL(output, 232);
    CU_ASSERT_EQUAL(translate_num(&output, "0xe8", -1, 255), 0);
    CU_ASSERT_EQUAL(output, 232);
    CU_ASSERT_EQUAL(translate_num(&output, "0xec", -1, 255), 0);
    CU_ASSERT_EQUAL(output, 236);
    CU_ASSERT_EQUAL(translate_num(&output, "0xec", -1, 255), 0);
    CU_ASSERT_EQUAL(output, 236);
    CU_ASSERT_EQUAL(translate_num(&output, "0xf0", -1, 255), 0);
    CU_ASSERT_EQUAL(output, 240);
    CU_ASSERT_EQUAL(translate_num(&output, "0xf0", -1, 255), 0);
    CU_ASSERT_EQUAL(output, 240);
    CU_ASSERT_EQUAL(translate_num(&output, "0xf4", -1, 255), 0);
    CU_ASSERT_EQUAL(output, 244);
    CU_ASSERT_EQUAL(translate_num(&output, "0xf4", -1, 255), 0);
    CU_ASSERT_EQUAL(output, 244);
    CU_ASSERT_EQUAL(translate_num(&output, "0xf8", -1, 255), 0);
    CU_ASSERT_EQUAL(output, 248);
    CU_ASSERT_EQUAL(translate_num(&output, "0xf8", -1, 255), 0);
    CU_ASSERT_EQUAL(output, 248);
    CU_ASSERT_EQUAL(translate_num(&output, "0xfc", -1, 255), 0);
    CU_ASSERT_EQUAL(output, 252);
    CU_ASSERT_EQUAL(translate_num(&output, "0xfc", -1, 255), 0);
    CU_ASSERT_EQUAL(output, 252);

}



/****************************************
 *  Test cases for tables.c 
 ****************************************/

void test_table_1() {
    int retval;

    SymbolTable* tbl = create_table(SYMTBL_UNIQUE_NAME);
    CU_ASSERT_PTR_NOT_NULL(tbl);

    retval = add_to_table(tbl, "abc", 8);
    CU_ASSERT_EQUAL(retval, 0);
    retval = add_to_table(tbl, "efg", 12);
    CU_ASSERT_EQUAL(retval, 0);
    retval = add_to_table(tbl, "q45", 16);
    CU_ASSERT_EQUAL(retval, 0);
    retval = add_to_table(tbl, "q45", 24); 
    CU_ASSERT_EQUAL(retval, -1); 
    retval = add_to_table(tbl, "bob", 14); 
    CU_ASSERT_EQUAL(retval, -1); 
    retval = add_to_table(tbl, "cob", 14); 
    CU_ASSERT_EQUAL(retval, -1); 
    retval = add_to_table(tbl, "dob", 15); 
    CU_ASSERT_EQUAL(retval, -1); 
    retval = add_to_table(tbl, "eob", 16); 
    CU_ASSERT_EQUAL(retval, 0); 
    retval = add_to_table(tbl, "fob", 17); 
    CU_ASSERT_EQUAL(retval, -1);     
    retval = get_addr_for_symbol(tbl, "abc");
    CU_ASSERT_EQUAL(retval, 8); 
    retval = get_addr_for_symbol(tbl, "q45");
    CU_ASSERT_EQUAL(retval, 16); 
    retval = get_addr_for_symbol(tbl, "ef");
    CU_ASSERT_EQUAL(retval, -1);

    retval = add_to_table(tbl, "bob", 14); 
    CU_ASSERT_EQUAL(retval, -1); 
    retval = add_to_table(tbl, "abc", 8);
    CU_ASSERT_EQUAL(retval, -1);
    retval = add_to_table(tbl, "efg", 12);
    CU_ASSERT_EQUAL(retval, -1);
    retval = add_to_table(tbl, "q45", 16);
    CU_ASSERT_EQUAL(retval, -1);
    free_table(tbl);

    char* arr[] = { "Error: name 'q45' already exists in table.",
                    "Error: address is not a multiple of 4." };
    check_lines_equal(arr, 2);

    SymbolTable* tbl2 = create_table(SYMTBL_NON_UNIQUE);
    CU_ASSERT_PTR_NOT_NULL(tbl2);

    retval = add_to_table(tbl, "q45", 16);
    CU_ASSERT_EQUAL(retval, 0);
    retval = add_to_table(tbl, "q45", 24); 
    CU_ASSERT_EQUAL(retval, 0);

    free_table(tbl2);

}

void test_table_2() {
    int retval, max = 1000;

    SymbolTable* tbl = create_table(SYMTBL_UNIQUE_NAME);
    CU_ASSERT_PTR_NOT_NULL(tbl);

    char buf[10];
    for (int i = 0; i < max; i++) {
        sprintf(buf, "%d", i);
        retval = add_to_table(tbl, buf, 4 * i);
        CU_ASSERT_EQUAL(retval, 0);
    }

    for (int i = 0; i < max; i++) {
        sprintf(buf, "%d", i);
        retval = get_addr_for_symbol(tbl, buf);
        CU_ASSERT_EQUAL(retval, 4 * i);
    }

    for (int i = 0; i < max; i++) {
        sprintf(buf, "%d", i);
        retval = add_to_table(tbl, buf, 4 * i);
        CU_ASSERT_EQUAL(retval, -1);
    }
    free_table(tbl);
    
    tbl = create_table(SYMTBL_UNIQUE_NAME);
    CU_ASSERT_PTR_NOT_NULL(tbl);

    for (int i = 0; i < max; i++) {
        sprintf(buf, "%d", i);
        retval = add_to_table(tbl, buf, 4 * i);
        CU_ASSERT_EQUAL(retval, 0);
    }
}


void test_table_3() {
    int retval;

    SymbolTable* tbl = create_table(SYMTBL_UNIQUE_NAME);
    CU_ASSERT_PTR_NOT_NULL(tbl);

    int max = 1000;
    for (int i = 0; i < max; i++) {
        char* buf = malloc(sizeof(char) * 5);
        sprintf(buf, "%d", i);
        int j = i % 33;
        retval = add_to_table(tbl, buf, j);
        if (j % 4 == 0) {
            CU_ASSERT_EQUAL(retval, 0);
        } else {
            CU_ASSERT_EQUAL(retval, -1);
        }
        if (j % 4 == 0) {
            retval = get_addr_for_symbol(tbl, buf);
            CU_ASSERT_EQUAL(retval, j);
        }
        free(buf);
    }

}



/****************************************
 *  Add your test cases here
 ****************************************/

int main(int argc, char** argv) {
    CU_pSuite pSuite1 = NULL;
    CU_pSuite pSuite2 = NULL;
    CU_pSuite pSuite3 = NULL;


    if (CUE_SUCCESS != CU_initialize_registry()) {
        return CU_get_error();
    }

    /* Suite 1 */
    pSuite1 = CU_add_suite("Testing translate_utils.c", NULL, NULL);
    if (!pSuite1) {
        goto exit;
    }
    if (!CU_add_test(pSuite1, "test_translate_reg", test_translate_reg)) {
        goto exit;
    }
    if (!CU_add_test(pSuite1, "test_translate_num", test_translate_num)) {
        goto exit;
    }

    if (!CU_add_test(pSuite1, "test_translate_num ridiculously", test_translate_num_more)) {
        goto exit;
    }

    /* Suite 2 */
    pSuite2 = CU_add_suite("Testing tables.c", init_log_file, NULL);
    if (!pSuite2) {
        goto exit;
    }
    if (!CU_add_test(pSuite2, "test_table_1", test_table_1)) {
        goto exit;
    }
    if (!CU_add_test(pSuite2, "test_table_2", test_table_2)) {
        goto exit;
    }

    /* Suite 3 */
    pSuite3 = CU_add_suite("Testing tables.c ridiculously", init_log_file, NULL);
    if (!pSuite3) {
        goto exit;
    }
    if (!CU_add_test(pSuite3, "test_table_3", test_table_3)) {
        goto exit;
    }


    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

exit:
    CU_cleanup_registry();
    return CU_get_error();;
}
