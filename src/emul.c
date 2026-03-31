#include <stdio.h>
#include <stdint.h>
#include <unistd.h>



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


uint16_t stack[128] = {0};            // stack register
uint16_t registers[R_COUNT] = {0};    // static registers 


//=======================================================


uint16_t memory[65535] = {0};         // memory
uint16_t proc = 0;                    // programator counter


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
void evaluate_instruction(int i);
void print_machine_state();
void init_machine();



int main(void)
{

  init_machine();

  while (running)
  {
    print_machine_state();
    int i = fetch_instruction();
    evaluate_instruction(i);
    registers[R_PC]++;
    usleep(1*1000*1000);
  }
  return 0;
}





int fetch_instruction()
{
  return memory[registers[R_PC]];
}



void evaluate_instruction(int i)
{
  switch (i) 
  {
    case I_PSH:
      {
        uint16_t data = memory[++registers[R_PC]];
        stack[registers[R_SP]] = data;
        registers[R_SP]++;
        break;
      }

    case I_POP:
      {
        registers[R_SP]--;
        registers[R_PV] = stack[registers[R_SP]];
        break;
      }

    case I_HLT:
      {
        running = 0;
        break;
      }

    case I_ADD:
      {
        uint16_t a = stack[--registers[R_SP]];
        uint16_t b = stack[--registers[R_SP]];
        uint16_t res = b + a;
        stack[registers[R_SP]] = res;
        registers[R_SP]++;
        break;
      } 

    case I_SUB:
      {
        uint16_t a = stack[--registers[R_SP]];
        uint16_t b = stack[--registers[R_SP]];
        uint16_t res = b - a;
        stack[registers[R_SP]] = res;
        registers[R_SP]++;
        break;
      }

    case I_MUL:
      {
        uint16_t a = stack[--registers[R_SP]];
        uint16_t b = stack[--registers[R_SP]];
        uint16_t res = b * a;
        stack[registers[R_SP]] = res;
        registers[R_SP]++;
        break;
      }

    case I_AND:
      {
        uint16_t a = stack[--registers[R_SP]];
        uint16_t b = stack[--registers[R_SP]];
        uint16_t res = b & a;
        stack[registers[R_SP]] = res;
        registers[R_SP]++;
        break;
      }

    case I_OR:
      {
        uint16_t a = stack[--registers[R_SP]];
        uint16_t b = stack[--registers[R_SP]];
        uint16_t res = b | a;
        stack[registers[R_SP]] = res;
        registers[R_SP]++;
        break;
      }
    
    case I_XOR:
      {
        uint16_t a = stack[--registers[R_SP]];
        uint16_t b = stack[--registers[R_SP]];
        uint16_t res = b ^ a;
        stack[registers[R_SP]] = res;
        registers[R_SP]++;
        break;
      }

    case I_SFL:
      {
        uint16_t a = stack[--registers[R_SP]];
        uint16_t b = stack[--registers[R_SP]];
        uint16_t res = b << a;
        stack[registers[R_SP]] = res;
        registers[R_SP]++;
        break;
      }
    
    case I_SFR:
      {
        uint16_t a = stack[--registers[R_SP]];
        uint16_t b = stack[--registers[R_SP]];
        uint16_t res = b >> a;
        stack[registers[R_SP]] = res;
        registers[R_SP]++;
        break;
      }

    case I_JMP:
      {
        uint16_t dist = memory[++registers[R_PC]];
        registers[R_PC] += dist;
        break;
      }

    case I_CJMP:
      {
        if (stack[registers[R_SP] - 1] == 0) break;
        registers[R_SP]--;
        uint16_t dist = memory[++registers[R_PC]];
        registers[R_PC] += dist;
        break;
      }

    case I_LDR:
      {
        uint16_t addr = stack[--registers[R_SP]];
        stack[registers[R_SP]] = memory[addr];
        registers[R_SP]++;
        break;
      }

    case I_STR:
      {
        uint16_t addr = stack[--registers[R_SP]];
        uint16_t data = stack[--registers[R_SP]];
        memory[addr] = data;
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
  printf(" PC: %d\n", registers[R_PC]);
}


void add_instruction(int i)
{
  memory[proc] = i;
  proc++;
}




void init_machine()
{
  
  add_instruction(I_PSH);
  add_instruction(1);
  add_instruction(I_PSH);
  add_instruction(0);


  add_instruction(I_HLT);
}
