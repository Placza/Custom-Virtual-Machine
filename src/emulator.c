
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>



typedef uint16_t word;

enum
{
  R_PC = 0,   // program counter
  R_SP,       // stack pointer
  R_PV,       // pop value
  R_UF,       // utility flags  
  R_COUNT,
};

int running = 1;

//=======================================================


word stack[128] = {0};            // stack register
word registers[R_COUNT] = {0};    // static registers 


//=======================================================


word memory[65535] = {0};         // memory
word proc = 0;                    // programator counter


//=======================================================


enum
{
  I_PSH,    // push (&a)
  I_POP,    // pop
  I_HLT,    // halt
            
  I_ADD,    // add (a, b)
  I_SUB,    // subtract (a, b)
  I_MUL,    // multiply (a, b)
  I_AND,    // and (a, b)
  I_OR,     // or (a, b)
  I_XOR,    // xor (a, b)
  I_SFL,    // logical shift to the left (a, b)
  I_SFR,    // logical shift to the right (a, b)

  I_JMP,    // jump to relative address (&a)
  I_CJMP,   // conditional jump to relative address (a, &b)
  
  I_LDR,    // load from memory (address)
  I_STR,    // store into memory (data, address)
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



int fetch_instruction();
void evaluate_instruction(word i, int* cycles);
void print_machine_state();
void init_machine();



int main(void)
{

  init_machine();
  int cycles = 0;

  while (running)
  {
    cycles++;
    print_machine_state();
    word i = fetch_instruction();
    evaluate_instruction(i, &cycles);
    usleep(100000);
  }
  return 0;
}





int fetch_instruction()
{
  return memory[registers[R_PC]];
}



int wait(int* cycles, int n)
{
  if (*cycles == n)
  {
    *cycles = 0;
    return 1;
  }
  return 0;
}


void evaluate_instruction(word i, int* cycles)
{
  switch (i) 
  {
    case I_PSH:
      {
        if (!wait(cycles, 1)) break;
        word data = memory[++registers[R_PC]];
        stack[registers[R_SP]] = data;
        registers[R_SP]++;
        registers[R_PC]++;
        break;
      }

    case I_POP:
      {
        if (!wait(cycles, 1)) break;
        registers[R_SP]--;
        registers[R_PV] = stack[registers[R_SP]];
        registers[R_PC]++;
        break;
      }

    case I_HLT:
      {
        running = 0;
        break;
      }

    case I_ADD:
      {
        if (!wait(cycles, 4)) break;
        word a = stack[--registers[R_SP]];
        word b = stack[--registers[R_SP]];
        word res = b + a;
        stack[registers[R_SP]] = res;
        registers[R_SP]++;
        registers[R_PC]++;
        break;
      } 

    case I_SUB:
      {
        if (!wait(cycles, 4)) break;
        word a = stack[--registers[R_SP]];
        word b = stack[--registers[R_SP]];
        word res = b - a;
        stack[registers[R_SP]] = res;
        registers[R_SP]++;
        registers[R_PC]++;
        break;
      }

    case I_MUL:
      {
        if (!wait(cycles, 4)) break;
        word a = stack[--registers[R_SP]];
        word b = stack[--registers[R_SP]];
        word res = b * a;
        stack[registers[R_SP]] = res;
        registers[R_SP]++;
        registers[R_PC]++;
        break;
      }

    case I_AND:
      {
        if (!wait(cycles, 4)) break;
        word a = stack[--registers[R_SP]];
        word b = stack[--registers[R_SP]];
        word res = b & a;
        stack[registers[R_SP]] = res;
        registers[R_SP]++;
        registers[R_PC]++;
        break;
      }

    case I_OR:
      {
        if (!wait(cycles, 4)) break;
        word a = stack[--registers[R_SP]];
        word b = stack[--registers[R_SP]];
        word res = b | a;
        stack[registers[R_SP]] = res;
        registers[R_SP]++;
        registers[R_PC]++;
        break;
      }
    
    case I_XOR:
      {
        if (!wait(cycles, 4)) break;
        word a = stack[--registers[R_SP]];
        word b = stack[--registers[R_SP]];
        word res = b ^ a;
        stack[registers[R_SP]] = res;
        registers[R_SP]++;
        registers[R_PC]++;
        break;
      }

    case I_SFL:
      {
        if (!wait(cycles, 4)) break;
        word a = stack[--registers[R_SP]];
        word b = stack[--registers[R_SP]];
        word res = b << a;
        stack[registers[R_SP]] = res;
        registers[R_SP]++;
        registers[R_PC]++;

        break;
      }
    
    case I_SFR:
      {
        if (!wait(cycles, 4)) break;
        word a = stack[--registers[R_SP]];
        word b = stack[--registers[R_SP]];
        word res = b >> a;
        stack[registers[R_SP]] = res;
        registers[R_SP]++;
        registers[R_PC]++;

        break;
      }

    case I_JMP:
      {
        if (!wait(cycles, 2)) break;
        word dist = memory[++registers[R_PC]];
        registers[R_PC] += dist;
        break;
      }

    case I_CJMP:
      {
        if (!wait(cycles, 4)) break;
        if (stack[registers[R_SP] - 1] == 0) break;
        registers[R_SP]--;
        word dist = memory[++registers[R_PC]];
        registers[R_PC] += dist;
        break;
      }

    case I_LDR:
      {
        if (!wait(cycles, 5)) break;
        word addr = stack[--registers[R_SP]];
        stack[registers[R_SP]] = memory[addr];
        registers[R_SP]++;
        registers[R_PC]++;
        break;
      }

    case I_STR:
      {
        if (!wait(cycles, 5)) break;
        word addr = stack[--registers[R_SP]];
        word data = stack[--registers[R_SP]];
        memory[addr] = data;
        registers[R_PC]++;
        break;
      }
  }
}



void print_machine_state()
{
  for (int i = 0; i < registers[R_SP]; i++)
  {
    printf("%d ", stack[i]);
  }
  printf(" PC: %d SP: %d\n", registers[R_PC], registers[R_SP]);

  for (int i = 100; i < 150; i++)
  {
    printf("%d ", memory[i]);
  }
  printf("\n");
}


void add_instruction(int i)
{
  memory[proc] = i;
  proc++;
}




void init_machine()
{
  FILE *ptr;
  
  ptr = fopen("../assembler/code.bin", "rb");
  
  fread(memory, sizeof(memory), 1, ptr);

  for (int i = 0; i < 100; i++)
  {
    printf("%u ", memory[i]);
  }
  printf("\n");

  fclose(ptr);

  /*add_instruction(I_PSH);
  add_instruction(101);
  add_instruction(I_PSH);
  add_instruction(100);
  add_instruction(I_STR);

  add_instruction(I_PSH);
  add_instruction(1;*/
}
