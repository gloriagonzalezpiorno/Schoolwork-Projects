#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "tables.h"
#include "translate_utils.h"
#include "translate.h"

/* SOLUTION CODE BELOW */
const int TWO_POW_SEVENTEEN = 131072;    // 2^17
const int UPPER_B = 4294967295;    // 2^32
const int LOWER_B = -2147483648;    // -2^31
const int BYTE = 4;

int is_unsigned_int(long int num) {
  if (0 <= num && num <= 65535) {  // 2^16
    return 1;
  }
  return 0;
}

int is_signed_int(long int num) {
  if (-32768 <= num && num <= 32767) {
    return 1;
  }
  return 0;
}

long int get_high_sixteen(long int thrity_two_bytes) {
  return thrity_two_bytes >> 16; 
}

 int32_t get_low_sixteen(long int thrity_two_bytes) {
  int32_t temp = thrity_two_bytes;
  temp = (temp << 16) >> 16;
  return (temp&0x0000ffff);
}


/* Writes instructions during the assembler's first pass to OUTPUT. The case
   for general instructions has already been completed, but you need to write
   code to translate the li and blt pseudoinstructions. Your pseudoinstruction 
   expansions should not have any side effects.
   NAME is the name of the instruction, ARGS is an array of the arguments, and
   NUM_ARGS specifies the number of items in ARGS.
   Error checking for regular instructions are done in pass two. However, for
   pseudoinstructions, you must make sure that ARGS contains the correct number
   of arguments. You do NOT need to check whether the registers / label are 
   valid, since that will be checked in part two.
   Also for li:
    - make sure that the number is representable by 32 bits. (Hint: the number 
        can be both signed or unsigned).
    - if the immediate can fit in the imm field of an addiu instruction, then
        expand li into a single addiu instruction. Otherwise, expand it into 
        a lui-ori pair.
        
   For move, blt, bgt, and traddu:
    - your expansion should use the fewest number of instructions possible.
   MARS has slightly different translation rules for li, and it allows numbers
   larger than the largest 32 bit number to be loaded with li. You should follow
   the above rules if MARS behaves differently.
   Use fprintf() to write. If writing multiple instructions, make sure that 
   each instruction is on a different line.
   Returns the number of instructions written (so 0 if there were any errors).
 */
unsigned write_pass_one(FILE* output, const char* name, char** args, int num_args) {
  int num_mips_lines = 0;
    // load immd
    if (strcmp(name, "li") == 0) { // number of arguments should be two 
      long int immd;
      // check number of arguments 
      if (num_args != 2) { 
        return 0; // return number of mips code lines here
      } else {
        int t_num = translate_num(&immd, args[1], LOWER_B, UPPER_B);
        if (t_num == -1) {
          return 0;
        }
        // signed or unsigned helper above
        if (is_signed_int(immd) || is_unsigned_int(immd)) {
          fprintf(output, "%s %s %s %ld\n", "addiu", args[0], "$0", immd);
        } else {
          // lui-- higher bits 
          fprintf(output, "%s %s %ld\n", "lui", "$at", get_high_sixteen(immd));
          // fprintf(output, "%s %s %ld\n", "lui", args[0], get_high_sixteen(immd));
          num_mips_lines++;
          // write
          // ori-- lower bits
          fprintf(output, "%s %s %s %ld\n","ori", args[0],"$at", get_low_sixteen(immd));
          num_mips_lines++;
        }
        // write_inst_string(output, name, args, num_args);
        return num_mips_lines;
      }
    } else if (strcmp(name, "move") == 0) { // number of arguments should be two 
        if (num_args != 2) { 
          return num_mips_lines;
        }
        fprintf(output, "%s %s %s %s\n", "addu", args[2], "$0", args[0]);
        num_mips_lines++;
        write_inst_string(output, name, args, num_args);
        return num_mips_lines;  
    } else if (strcmp(name, "blt") == 0) { // number of arguments should be three 
        if (num_args != 3) { 
          return num_mips_lines;
        }
        fprintf(output, "%s %s %s %s\n", "slt","$at", args[0], args[1]);
        num_mips_lines++;
        fprintf(output, "%s %s %s %s\n", "bne", "$at", "$0", args[2]);
        num_mips_lines++;
        // write_inst_string(output, name, args, num_args);
        return num_mips_lines;
    } else if (strcmp(name, "bgt") == 0) { // number of arguments should be three
        if (num_args != 3) { 
          return num_mips_lines;
        }
        fprintf(output, "%s %s %s %s\n", "srt","$at", args[0], args[1]);
        num_mips_lines++;
        fprintf(output, "%s %s %s %s\n", "bne", "$at", "$0", args[2]);
        num_mips_lines++;
        // write_inst_string(output, name, args, num_args);
        return num_mips_lines;

    } else if (strcmp(name, "traddu") == 0) {
        if (num_args != 4) { 
          return num_mips_lines;
        }
        fprintf(output, "%s %s %s %s\n", "addu", args[0], args[1], args[2]);
        num_mips_lines++;
        fprintf(output, "%s %s %s %s\n", "addu", args[0], "$0", args[3]);
        num_mips_lines++;
        // write_inst_string(output, name, args, num_args);
        return num_mips_lines;       
    } 
    write_inst_string(output, name, args, num_args);
    num_mips_lines++;
    return num_mips_lines;

}

/* Writes the instruction in hexadecimal format to OUTPUT during pass #2.
   
   NAME is the name of the instruction, ARGS is an array of the arguments, and
   NUM_ARGS specifies the number of items in ARGS. 
   The symbol table (SYMTBL) is given for any symbols that need to be resolved
   at this step. If a symbol should be relocated, it should be added to the
   relocation table (RELTBL), and the fields for that symbol should be set to
   all zeros. 
   You must perform error checking on all instructions and make sure that their
   arguments are valid. If an instruction is invalid, you should not write 
   anything to OUTPUT but simply return -1. MARS may be a useful resource for
   this step.
   Some function declarations for the write_*() functions are provided in translate.h, and you MUST implement these
   and use these as function headers. You may use helper functions, but you still must implement
   the provided write_* functions declared in translate.h.
   Returns 0 on success and -1 on error. 
 */
int translate_inst(FILE* output, const char* name, char** args, size_t num_args, uint32_t addr,
    SymbolTable* symtbl, SymbolTable* reltbl) {
    if (strcmp(name, "addu") == 0)        return write_rtype (0x21, output, args, num_args);
    else if (strcmp(name, "or") == 0)     return write_rtype (0x25, output, args, num_args);
    else if (strcmp(name, "slt") == 0)    return write_rtype (0x2a, output, args, num_args);
    else if (strcmp(name, "sltu") == 0)   return write_rtype (0x2b, output, args, num_args);
    // write_shift
    else if (strcmp(name, "sll") == 0)    return write_shift (0x00, output, args, num_args);
    else if (strcmp(name, "srl") == 0)    return write_shift (0x02, output, args, num_args);
    // 
    else if (strcmp(name, "jr") == 0)     return write_jr  (0x08, output, args, num_args);
    else if (strcmp(name, "addiu") == 0)  return write_addiu (0x09, output, args, num_args);
    else if (strcmp(name, "ori") == 0)    return write_ori (0x0d, output, args, num_args);
    else if (strcmp(name, "lui") == 0)    return write_lui (0x0f, output, args, num_args);

    else if (strcmp(name, "li") == 0)     return write_pass_one (output, name, args, num_args);
    else if (strcmp(name, "move") == 0)   return write_pass_one (output, name, args, num_args);
    else if (strcmp(name, "blt") == 0)    return write_pass_one (output, name, args, num_args);
    else if (strcmp(name, "bgt") == 0)    return write_pass_one (output, name, args, num_args);
    else if (strcmp(name, "traddu") == 0) return write_pass_one (output, name, args, num_args);

    // write_mem
    else if (strcmp(name, "lb") == 0)     return write_mem (0x20, output, args, num_args);
    else if (strcmp(name, "lh") == 0)     return write_mem (0x21, output, args, num_args);
    else if (strcmp(name, "lwl") == 0)    return write_mem (0x22, output, args, num_args);
    else if (strcmp(name, "lw") == 0)     return write_mem (0x23, output, args, num_args);
    else if (strcmp(name, "lbu") == 0)    return write_mem (0x24, output, args, num_args);
    else if (strcmp(name, "lhu") == 0)    return write_mem (0x25, output, args, num_args);
    else if (strcmp(name, "sb") == 0)     return write_mem (0x28, output, args, num_args);
    else if (strcmp(name, "sh") == 0)     return write_mem (0x29, output, args, num_args);
    else if (strcmp(name, "swl") == 0)    return write_mem (0x2a, output, args, num_args);
    else if (strcmp(name, "sw") == 0)     return write_mem (0x2b, output, args, num_args);
    // write_branch
    else if (strcmp(name, "beq") == 0)    return write_branch(0x04, output, args, num_args, addr, symtbl);
    else if (strcmp(name, "bne") == 0)    return write_branch(0x05, output, args, num_args, addr, symtbl);
    else if (strcmp(name, "blez") == 0)   return write_branch(0x06, output, args, num_args, addr, symtbl);
    else if (strcmp(name, "bgtz") == 0)   return write_branch(0x07, output, args, num_args, addr, symtbl);
    // write_jump
    else if (strcmp(name, "j") == 0)      return write_jump(0x02, output, args, num_args, addr, reltbl);
    else if (strcmp(name, "jal") == 0)    return write_jump(0x03, output, args, num_args, addr, reltbl);
    else                                  return -1;
}

uint32_t inst_gen(int op, int rs, int rt, int rd, int sh, int fnct) {
  return (op << 26) | (rs << 21) | (rt << 16) | (rd << 11) | (sh << 6) | fnct;
}

uint32_t inst_gen_imm(int op, int rs, int rt, int imm) {
  return (op << 26) | (rs << 21) | (rt << 16) | imm;
}

/* A helper function for writing most R-type instructions. You should use
   translate_reg() to parse registers and write_inst_hex() to write to 
   OUTPUT. Both are defined in translate_utils.h.
   This function is INCOMPLETE. Complete the implementation below. You will
   find bitwise operations to be the cleanest way to complete this function.
 */
int write_rtype(uint8_t funct, FILE* output, char** args, size_t num_args) {
    if (args[2] == NULL) {
      return -1;
    }

    int rd = 0, rs = 0, rt = 0;
    if (translate_reg(args[0]) != -1) {
      rd = translate_reg(args[0]);
    }
    if (translate_reg(args[1]) != -1) {
      rs = translate_reg(args[1]);
    }
    if (translate_reg(args[2]) != -1) {
      rt = translate_reg(args[2]);
    }
    if (num_args != 3 || rs == -1 || rt == -1 || rd == -1) {
      return -1;
    }
    uint32_t instruction = inst_gen(0, rs, rt, rd, 0, funct);
    write_inst_hex(output, instruction);
    return 0;
}

/* A helper function for writing shift instructions. You should use 
   translate_num() to parse numerical arguments. translate_num() is defined
   in translate_utils.h.
   This function is INCOMPLETE. Complete the implementation below. You will
   find bitwise operations to be the cleanest way to complete this function.
 */
int write_shift(uint8_t funct, FILE* output, char** args, size_t num_args) {
    long int shamt;
    // if (args[2] == NULL) {
    //   return -1;
    // }
    int rd = translate_reg(args[0]);
    int rt = translate_reg(args[1]);
    int err = translate_num(&shamt, args[2], 0, 31);
    if (num_args != 3 || rd == -1 || rt == -1 || err == -1) {
      return -1;
    }
    uint32_t instruction = inst_gen(0, 0, rt, rd, shamt, funct);
    write_inst_hex(output, instruction);
    return 0;
}

/* The rest of your write_*() functions below */

int write_jr(uint8_t funct, FILE* output, char** args, size_t num_args) {
    // if (args[0] == NULL) {
    //   return -1;
    // }
    int rs = translate_reg(args[0]);
    if (num_args != 1 || rs == -1 ) {
      return -1;
    }
    uint32_t instruction = inst_gen(0, rs, 0, 0, 0, funct);
    write_inst_hex(output, instruction);
    return 0;
}

int write_addiu(uint8_t opcode, FILE* output, char** args, size_t num_args) {
    long int imm;
    // if (args[2] == NULL) {
    //   return -1;
    // }  
    int a = translate_reg(args[0]);
    int b = translate_reg(args[1]);
    int rt, rs;
    if (a == -1) {
      rt = 0;
    } else {
      rt = a;
    }

    if (b == -1) {
      rs = 0;
    } else {
      rs = b;
    }
    int err = translate_num(&imm, args[2], INT16_MIN, INT16_MAX);
    // if (num_args != 3 || rs == -1 || rt == -1 || err == -1) {
    if (err != 0) {
      return -1;
    }
    // int result = translate_num(&imm, args[2], INT16_MIN, INT16_MAX);
    // if (result != -1) {
    //   if (imm < 0 ) {
    //     imm = imm & 0xFFFF;
    //   } 
    uint32_t instruction = inst_gen_imm(opcode, rt, rs, imm);
    write_inst_hex(output, instruction);
    // } else {
    //   return -1;
    // }
    return 0;
}

int write_ori(uint8_t opcode, FILE* output, char** args, size_t num_args) {
    long int imm;
    if (args[2] == NULL) {
      return -1;
    } 
    int rt = translate_reg(args[0]);
    int rs = translate_reg(args[1]);
    int err = translate_num(&imm, args[2], 0, UINT16_MAX);
    if (num_args != 3 || rs == -1 || rt == -1 || err == -1) {
      return -1;
    }
    int result = translate_num(&imm, args[2], 0, TWO_POW_SEVENTEEN/2);
    if (result != -1) {
      uint32_t instruction = inst_gen_imm(opcode, rs, rt, imm);
      write_inst_hex(output, instruction);
    } else {
      return -1;
    }
    return 0;
}

int write_lui(uint8_t opcode, FILE* output, char** args, size_t num_args) {
    // Perhaps perform some error checking?
    long int imm;

    int a = translate_reg(args[0]);
    int rt = a;
    if (a == -1) {
      rt = 0;
    }
    int rs = 0;

    int err = translate_num(&imm, args[1], 0, UINT16_MAX);
    if (num_args != 2 || rt == -1 || err == -1) { 
      return -1;
    }
    int result = translate_num(&imm, args[1], LOWER_B, LOWER_B * (-1));
    if (result != -1) {
      uint32_t instruction;
      instruction = inst_gen_imm(opcode, rs, rt, imm);
      write_inst_hex(output, instruction);
    } else {
      return -1;
    }
    return 0;
}

// uint32_t inst_gen_imm(int op, int rs, int rt, int imm) {
//   return (op << 26) | (rs << 21) | (rt << 16) | imm;
// }


int write_mem(uint8_t opcode, FILE* output, char** args, size_t num_args) {
    long int imm;
    int rt = translate_reg(args[0]);
    int rs = translate_reg(args[2]);
    int err = translate_num(&imm, args[1], INT16_MIN, INT16_MAX);
    if (num_args != 3 || rs == -1 || rt == -1 || err == -1) {
      return -1;
    }
    int result = translate_num(&imm, args[1], INT16_MIN, INT16_MAX);
    if (result != -1) {
      uint32_t instruction = inst_gen_imm(opcode, rt, rs, imm);;
      if (imm < 0 ) {
        imm = imm & 0xFFFF;
      } 
      write_inst_hex(output, instruction);
    } else {
      return -1;
    }
    return 0;
}

/*  A helper function to determine if a destination address
    can be branched to
*/
static int can_branch_to(uint32_t src_addr, uint32_t dest_addr) {
    int32_t diff = dest_addr - src_addr;
    return (diff >= 0 && diff <= TWO_POW_SEVENTEEN) || (diff < 0 && diff >= -(TWO_POW_SEVENTEEN - BYTE));
}


int write_branch(uint8_t opcode, FILE* output, char** args, size_t num_args, uint32_t addr, SymbolTable* symtbl) {
    int rs = translate_reg(args[0]);
    int rt = translate_reg(args[1]);
    if (num_args != 3 || rs == -1 || rt == -1) {
      return -1;
    }
    int label_addr = get_addr_for_symbol(symtbl, args[2]);
    if (label_addr == -1) {
      return -1;
    }
    int32_t address = label_addr - (addr + BYTE);
    int branchable = can_branch_to(addr, label_addr);
    if (branchable) {
      if (address % BYTE == 0) {
        address = (address / 4) & 0xFFFF;  // a / 4
      }
    }
    uint32_t instruction = inst_gen_imm(opcode, rs, rt, address);
    write_inst_hex(output, instruction);        
    return 0;
}

int write_jump(uint8_t opcode, FILE* output, char** args, size_t num_args, uint32_t addr, SymbolTable* reltbl) {
    if (num_args != 1) {
      return -1;
    }
    add_to_table(reltbl, args[0], addr);
    uint8_t opcode_t = opcode << 26;
    uint32_t instruction = opcode_t;
    write_inst_hex(output, instruction);
    return 0;
}