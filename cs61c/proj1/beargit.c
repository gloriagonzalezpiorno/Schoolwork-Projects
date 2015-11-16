#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <unistd.h>
#include <sys/stat.h>

#include "beargit.h"
#include "util.h"

/* Implementation Notes:
 *
 * - Functions return 0 if successful, 1 if there is an error.
 * - All error conditions in the function description need to be implemented
 *   and written to stderr. We catch some additional errors for you in main.c.
 * - Output to stdout needs to be exactly as specified in the function description.
 * - Only edit this file (beargit.c)
 * - You are given the following helper functions:
 *   * fs_mkdir(dirname): create directory <dirname>
 *   * fs_rm(filename): delete file <filename>
 *   * fs_mv(src,dst): move file <src> to <dst>, overwriting <dst> if it exists
 *   * fs_cp(src,dst): copy file <src> to <dst>, overwriting <dst> if it exists
 *   * write_string_to_file(filename,str): write <str> to filename (overwriting contents)
 *   * read_string_from_file(filename,str,size): read a string of at most <size> (incl.
 *     NULL character) from file <filename> and store it into <str>. Note that <str>
 *     needs to be large enough to hold that string.
 *  - You NEED to test your code. The autograder we provide does not contain the
 *    full set of tests that we will run on your code. See "Step 5" in the homework spec.
 */

/* beargit init
 *
 * - Create .beargit directory
 * - Create empty .beargit/.index file
 * - Create .beargit/.prev file containing 0..0 commit id
 *
 * Output (to stdout):
 * - None if successful
 */

int beargit_init(void) {
  fs_mkdir(".beargit");

  FILE* findex = fopen(".beargit/.index", "w");
  fclose(findex);

  FILE* fbranches = fopen(".beargit/.branches", "w");
  fprintf(fbranches, "%s\n", "master");
  fclose(fbranches);
   
  write_string_to_file(".beargit/.prev", "0000000000000000000000000000000000000000");
  write_string_to_file(".beargit/.current_branch", "master");

  return 0;
}



/* beargit add <filename>
 * 
 * - Append filename to list in .beargit/.index if it isn't in there yet
 *
 * Possible errors (to stderr):
 * >> ERROR: File <filename> already added
 *
 * Output (to stdout):
 * - None if successful
 */

int beargit_add(const char* filename) {
  FILE* findex = fopen(".beargit/.index", "r");
  FILE *fnewindex = fopen(".beargit/.newindex", "w");

  char line[FILENAME_SIZE];
  while(fgets(line, sizeof(line), findex)) {
    strtok(line, "\n");
    if (strcmp(line, filename) == 0) {
      fprintf(stderr, "ERROR: File %s already added\n", filename);
      fclose(findex);
      fclose(fnewindex);
      fs_rm(".beargit/.newindex");
      return 3;
    }

    fprintf(fnewindex, "%s\n", line);
  }

  fprintf(fnewindex, "%s\n", filename);
  fclose(findex);
  fclose(fnewindex);

  fs_mv(".beargit/.newindex", ".beargit/.index");

  return 0;
}

/* beargit status
 *
 * See "Step 1" in the homework 1 spec.
 *
 */

int beargit_status() {
  /* COMPLETE THE REST */
  FILE* findex = fopen(".beargit/.index", "r");
  char line[FILENAME_SIZE];
  int i = 0;
  printf("Tracked files:\n\n");
  while(fgets(line, sizeof(line), findex)) {
    strtok(line, "\n");
    ++i;
    printf ("  %s\n",line);
  }
  printf("\n%d files total\n", i);
  fclose(findex);
  return 0;
}

/* beargit rm <filename>
 * 
 * See "Step 2" in the homework 1 spec.
 *
 */

int beargit_rm(const char* filename) {
  /* COMPLETE THE REST */
  FILE* findex = fopen(".beargit/.index", "r");
  FILE* rmvdIndex = fopen(".beargit/.removedindex", "w");
  char line[FILENAME_SIZE];
  int foundFile = 0;
  int result = 0;
  while (fgets(line, sizeof(line), findex)) {
    strtok(line, "\n");
    if (strcmp(line, filename) == 0) {
      foundFile = 1;
      result = 1;
    } else {
      fprintf(rmvdIndex, "%s\n", line);
    }
  }
  if (foundFile != 1) {
      fprintf(stderr, "ERROR: File %s not tracked\n", filename);
      return result;
  } else {
    fs_mv(".beargit/.removedindex", ".beargit/.index");
  }
  fclose(findex);
  fclose(rmvdIndex);
  return result;
}

/* beargit commit -m <msg>
 *
 * See "Step 3" in the homework 1 spec.
 *
 */

const char* go_bears = "GO BEARS!";

int is_commit_msg_ok(const char* msg) {
  /* COMPLETE THE REST */
  char *sentence = (char*) msg;
  while (*sentence) {
    char *oIndex = sentence;
    char *goBears =  "GO BEARS!";
    while (*sentence && *goBears && (*sentence == *goBears)) {
      ++sentence;
      ++goBears;
    }
    if (!*goBears) {
      return 1;
    }
    sentence = oIndex + 1;
  }
  return 0;
}



char* to_digit(char* digit) {
  int j;
  for (j = 0; j < strlen(digit); j++) {
    if (digit[j] == '6') {
      digit[j] = '0';
    } else if (digit[j] == '1') {
      digit[j] = '1';
    } else if (digit[j] == 'c'){
      digit[j] = '2';
    }
  }
  return digit;

}

void to_61c(char* input, int first) {
  int j;
  for (j = 0; j < 40; j++) {
    if (input[j] == '0') {
      input[j] = '6';
    } else if (input[j] == '1') {
      input[j] = '1';
    } else if (input[j] == '2') {
      input[j] = 'c';
    }
    if (first && (j == strlen(input) - 1)) {
      input[j] = '1';
    }
  }
}

void to_ternary(int number, char* yolo) {
  int digit = 0;
  int i = 0;
  // number++;
  for (; i < 40; ++i) {
    digit = number % 3;
    number = number / 3;
    char* t = calloc(sizeof(char), 2);
    snprintf(t, 2, "%d", digit);
    if (i < 30) {
      yolo[29 - i] = t[0];
    } 
    free(t);
  }
  to_61c(yolo, 0);
}

int to_decimal (int ternary) {
  int sum = 0;
  int digit = 0;
  int mul = 3;
  int i = 0;
  for (; i < 40; ++i) {
    digit = ternary % 10;
    ternary = ternary / 10;
    if (i == 0) {
      sum += digit;
    } else {
      sum += digit * mul;
      mul *= 3;
    }
  }
  return sum;
}


void next_commit_id_part1(char* commit_id) {
  /* COMPLETE THE REST */
  // first commit id
  char * d = "000000000000000000000000000000"; //30
  if (strcmp(commit_id, d) == 0) { 
    // to ternary 
    to_61c(commit_id, 1);
  } else {
    // to digit
    to_digit(commit_id);
    // sum as decimal
    int decimal_sum = to_decimal(atoi(commit_id));
    // decimal sum to ternary binary
    to_ternary(decimal_sum + 1, commit_id);
 }
}


// .beargit/.index          .beargit/commit_id/
void copyTo (char* source, char* dest) {
  FILE* sourceFile = fopen(source, "r");
  char fName[FILENAME_SIZE];
  char toDir[FILENAME_SIZE];
  while (fgets(fName, FILENAME_SIZE, sourceFile)) {
    strtok(fName, "\n");
    strcpy(toDir, dest);
    strcat(toDir, fName); //.beargit/commit_id/fName
    fs_cp(fName, toDir);
  }
  fclose(sourceFile);
}


int beargit_commit(const char* msg) {
  if (!is_commit_msg_ok(msg)) {
    fprintf(stderr, "ERROR: Message must contain \"%s\"\n", go_bears);
    return 1;
  }

  //////////////////////// FIX 
  char curr[BRANCHNAME_SIZE];
  read_string_from_file(".beargit/.current_branch", curr, BRANCHNAME_SIZE);

  // if (strlen(curr) == 0) {
  //   fprintf(stderr,"ERROR: Need to be on HEAD of a branch to commit\n");
  //   return 1;
  // }

  int check = 1;
  FILE* fbranches = fopen(".beargit/.branches", "r");
  char line[FILENAME_SIZE];
  while(fgets(line, sizeof(line), fbranches)) {
    strtok(line, "\n");
    if (strcmp(line, curr) == 0) {
      check = 0;
      break;
    }
  }
  fclose(fbranches);
  if (strlen(curr) == 0 || check == 1) {
    fprintf(stderr,"ERROR: Need to be on HEAD of a branch to commit\n");
    return 1;
  }



  char commit_id[COMMIT_ID_SIZE];
  read_string_from_file(".beargit/.prev", commit_id, COMMIT_ID_SIZE);
  next_commit_id(commit_id); 

  /* COMPLETE THE REST */ 
  // beargitID 
  char beargitID[FILENAME_SIZE];
  strcpy(beargitID, ".beargit/");
  strcat(beargitID, commit_id);    // .beargit/commit_id

  char temp[FILENAME_SIZE];
  strcpy(temp, beargitID);
  strcat(temp, "/"); // .beargit/commit_id/

  fs_mkdir(beargitID);
  // FILE* yohoho = fopen(beargitID, "w");
  char bgIndex[FILENAME_SIZE];
  char bgCommitMSG[FILENAME_SIZE];

  strcpy(bgIndex, beargitID); 
  strcpy(bgCommitMSG, beargitID); 

  strcat(bgIndex, "/.index");      // .beargit/commit_id/.prev
  strcat(beargitID, "/.prev");     // .beargit/commit_id/.index
  strcat(bgCommitMSG, "/.msg");      // .beargit/commit_id/.msg
  // copy from   .beargit/.prev  to   .beargit/commit_id/.prev
  fs_cp(".beargit/.prev", beargitID);

  // copy from   .beargit/.prev  to   .beargit/commit_id/.prev
  fs_cp(".beargit/.index", bgIndex);
  copyTo(".beargit/.index", temp);

  write_string_to_file(bgCommitMSG, msg);
  // printf(">>> msg saved as %s\n", bgCommitMSG);
  write_string_to_file(".beargit/.prev", commit_id);
  // printf(">>> id saved as %s\n", commit_id);
  return 0;
}


void free_char_p(char* lama) {
  int i;
  for (i = 0; i < strlen(lama); i++) {
     lama[i] = '\0';
  }
}

/* beargit log
 *
 * See "Step 4" in the homework 1 spec.
 * 
 */
const   char* d = "0000000000000000000000000000000000000000"; // 40
int beargit_log(int limit) {
  if (limit == 0) {
    return 0;
  }
  int logged = 0;
  /* COMPLETE THE REST */
  // printf(">>default limit num of log: %d\n", limit);
  // 0000000000000000000000000000000000000000


  char cm_id[COMMIT_ID_SIZE];
  char cmMsg[MSG_SIZE];
  read_string_from_file(".beargit/.prev", cm_id, COMMIT_ID_SIZE);
  // printf("%s\n%s",d, cm_id);
  if (strcmp(cm_id, d) == 0) { 
    fprintf(stderr, "ERROR: There are no commits!\n");
    return 1;
  }

  char MSG_index[FILENAME_SIZE];
  strcpy(MSG_index, ".beargit/"); 
  strcat(MSG_index, cm_id); 
  strcat(MSG_index, "/.msg"); 

  read_string_from_file(MSG_index, cmMsg, MSG_SIZE);
  printf("\n");
  printf("commit %s\n    %s\n", cm_id, cmMsg);
  logged++;
  // int i = 3;
  while (logged < limit) {
    // printf("%d < %d ? \n", logged, limit);

    // init 
    char new_dir[FILENAME_SIZE];
    char msgLama[FILENAME_SIZE];

    // dir for commit id
    strcpy(new_dir, ".beargit/"); 
    strcat(new_dir, cm_id); 
    strcat(new_dir, "/.prev"); 
    // printf(">> .beargit/id/.prev \n>>%s\n", new_dir);

    // free cm_id 
    free_char_p(cm_id);
    read_string_from_file(new_dir, cm_id, COMMIT_ID_SIZE);
    free_char_p(new_dir);

    /* .beargit/commit_id/.msg */
    strcpy(msgLama, ".beargit/"); 
    strcat(msgLama, cm_id); 
    strcat(msgLama, "/.msg"); 
    if (strcmp(cm_id, d) == 0) {
      break;
    }
    read_string_from_file(msgLama, cmMsg, MSG_SIZE);
    printf("\ncommit %s\n    %s\n", cm_id, cmMsg);
    logged++;
  }
  printf("\n");
  return 0;
}




const char* digits = "61c";

void next_commit_id(char* commit_id) {
  char current_branch[BRANCHNAME_SIZE];
  read_string_from_file(".beargit/.current_branch", current_branch, BRANCHNAME_SIZE);

  // The first COMMIT_ID_BRANCH_BYTES=10 characters of the commit ID will
  // be used to encode the current branch number. This is necessary to avoid
  // duplicate IDs in different branches, as they can have the same pre-
  // decessor (so next_commit_id has to depend on something else).
  int n = get_branch_number(current_branch);
  for (int i = 0; i < COMMIT_ID_BRANCH_BYTES; i++) {
    commit_id[i] = digits[n%3];
    n /= 3;
  }
  next_commit_id_part1(commit_id + COMMIT_ID_BRANCH_BYTES);
}


// This helper function returns the branch number for a specific branch, or
// returns -1 if the branch does not exist.
int get_branch_number(const char* branch_name) {
  FILE* fbranches = fopen(".beargit/.branches", "r");

  int branch_index = -1;
  int counter = 0;
  char line[FILENAME_SIZE];
  while(fgets(line, sizeof(line), fbranches)) {
    strtok(line, "\n");
    if (strcmp(line, branch_name) == 0) {
      branch_index = counter;
    }
    counter++;
  }

  fclose(fbranches);

  return branch_index;
}

/* beargit branch
 *
 * See "Step 5" in the homework 1 spec.
 *
 */

int beargit_branch() {
  /* COMPLETE THE REST */
 FILE* fbranches = fopen(".beargit/.branches", "r");
 //FILE* current_branch = fopen("./beargit/.current_branch", "r");
 char line[FILENAME_SIZE];
 char current[BRANCHNAME_SIZE];
 printf("%s",line);
 read_string_from_file(".beargit/.current_branch", current, BRANCHNAME_SIZE);

 while(fgets(line, sizeof(line), fbranches)){
  /////////////////////
  strtok(line, "\n");
      if(strcmp(line, current) == 0){
        printf("* %s\n", current);
      }
      else{
        printf("  %s\n", line);
      }

 }
 fclose(fbranches);
 //fclose(current_branch);
  return 0;
}

/* beargit checkout
 *
 * See "Step 6" in the homework 1 spec.
 *
 */

int checkout_commit(const char* commit_id) {
  if (strcmp(commit_id, d) == 0) { 
    FILE* findex = fopen(".beargit/.index", "r");
    char line[FILENAME_SIZE];
    while(fgets(line, sizeof(line), findex)){
      strtok(line, "\n");
      char gg[FILENAME_SIZE];
      sprintf(gg, "%s", line);  
      fs_rm(gg);
    }
    fclose(findex);
    FILE* troll = fopen(".beargit/.index", "w");
    fclose(troll);
  } else {
        
    FILE* findex = fopen(".beargit/.index", "r");
    char line[FILENAME_SIZE];
    while(fgets(line, sizeof(line), findex)){
        strtok(line, "\n");
        char gg[FILENAME_SIZE];
        sprintf(gg, "%s", line);  
        fs_rm(gg);
    }
    fclose(findex);
      char dir[FILENAME_SIZE];
      sprintf(dir, ".beargit/%s/.index", commit_id);

    fs_cp(dir, ".beargit/.index"); // commit id change
    FILE* cIndex = fopen(dir , "r");
    char cline[FILENAME_SIZE];
    while (fgets(cline,sizeof(cline),cIndex)) {
    strtok(cline, "\n");
    char fName[FILENAME_SIZE];
    sprintf(fName, ".beargit/%s/%s", commit_id, cline);
    fs_cp(fName, cline); // needs to be fixed, it's not copying
    }
    fclose(cIndex);
  }
  write_string_to_file(".beargit/.prev", commit_id);
  return 0;



}

int is_it_a_commit_id(const char* commit_id) { //???
  char* good = "61c";
  if(strspn(commit_id, "61c") != strlen(commit_id)){
    return 0;
  }
  return 1;
}

int beargit_checkout(const char* arg, int new_branch) {
  // Get the current branch
  char current_branch[BRANCHNAME_SIZE];
  read_string_from_file(".beargit/.current_branch", current_branch, BRANCHNAME_SIZE); //error 

  // If not detached, update the current branch by storing the current HEAD into that branch's file...
  // Even if we cancel later, this is still ok.
  if (strlen(current_branch)) {
    char current_branch_file[BRANCHNAME_SIZE+50];
    sprintf(current_branch_file, ".beargit/.branch_%s", current_branch);
    fs_cp(".beargit/.prev", current_branch_file);
  }

  // Check whether the argument is a commit ID. If yes, we just stay in detached mode
  // without actually having to change into any other branch.
  if (is_it_a_commit_id(arg)) {
    char commit_dir[FILENAME_SIZE] = ".beargit/";
    strcat(commit_dir, arg);
    if (!fs_check_dir_exists(commit_dir)) {
      fprintf(stderr, "ERROR: Commit %s does not exist\n", arg);
      return 1;
    }

    // Set the current branch to none (i.e., detached).
    write_string_to_file(".beargit/.current_branch", "");

    return checkout_commit(arg);
  }

  // Just a better name, since we now know the argument is a branch name.
  const char* branch_name = arg;

  // Read branches file (giving us the HEAD commit id for that branch).
  int branch_exists = (get_branch_number(branch_name) >= 0);

  // Check for errors.
  if (!(!branch_exists || !new_branch)) {
    fprintf(stderr, "ERROR: A branch named %s already exists\n", branch_name);
    return 1;
  } else if (!branch_exists && !new_branch) {
    fprintf(stderr, "ERROR: No branch %s exists\n", branch_name);
    return 1;
  }

  // File for the branch we are changing into.
  char branch_file [] = ".beargit/.branch_"; 
  strcat(branch_file, branch_name);

  // Update the branch file if new branch is created (now it can't go wrong anymore)
  if (new_branch) {
    FILE* fbranches = fopen(".beargit/.branches", "a"); 
    fprintf(fbranches, "%s\n", branch_name);
    fclose(fbranches);
    fs_cp(".beargit/.prev", branch_file); 
  }

  write_string_to_file(".beargit/.current_branch", branch_name);

  // Read the head commit ID of this branch.
  char branch_head_commit_id[COMMIT_ID_SIZE];
  read_string_from_file(branch_file, branch_head_commit_id, COMMIT_ID_SIZE);

  // Check out the actual commit.
  return checkout_commit(branch_head_commit_id);
}