// =====================================
// ============= BASICS ================

// this is the asm code
asm("mov %ecx, %eax");

// this is valid too, no difference
__asm__("mov %ebx, %eax");

// there might be some comments inside the asm code
// comments are one-line, they start with ";"
asm("mov $0, %edx ; I am the comment inside asm code");

// multiline asm
// all the lines (except the last line) must end with '\n' symbol
// (gcc guide says it should be "\n\t" but it's OK to omit tabulation)
asm("mov $1, %al\n"
    "mov $2, %ah");

// ending with ';' also works (why?)
asm("mov $1, %al;"
    "mov $2, %ah");

// if you don't like a lot of '"' (double-quotes)
// you can replace it with a lot of '\' (backslash)
asm("mov %eax, %ebx;\
     mov %ebx, %ecx");


// all the asm uses AT&T synax by default, 
// but this is possible to swich to intel

// either by inserting a special directive
asm(".intel_syntax;\"
     mov eax, ebx;\
     mov ebx, 1");

// or by passing a "-masm=intel" flag to gcc 
$ gcc ./example.c -masm=intel



// =================================================
// ============= INPUTS, OUTPUTS AND CLOBBERS ================

// all we saw above was just a one string of assembler code 
// it's OK when you don't need to interact with C code (simple "sti", "cli" instrucitons)
// but in real world we usually have to load registers from C variables first
// or write a value from register to C variable


int in = 0;
int out;

asm("inc %%eax;\
     mov %%eax, %%ebx"
    : "=b"(out) : "a"(in) );

printf("%d\n", out); // prints "1"


// note double-percent (%%)
// this is because all the asm string becomes a printf-like template
// therefore we have to escape % sign and write it as %
// for this reason all the further examples are written in intel syntax
// (I assume we use -masm=intel flag)


// syntax looks like this:

asm("template-asm" : OUTPUT : INPUT : CLOBBERS);

// OUTPUT and INPUT is a comma-separated lists of "constraints"(var)
//   Constraints says whether an operand may be in a register and which kind of register 
//   or it's a memory reference and which kinds of address, etc...
//   i.e "a" means "value must be stored in eax", 
//   "r" means "put value in any register you want" and so on.
//   "=" means "this variable is a write-only variable", "+" means read-write

//   here is most used constraints

+---+--------------------+
| r |    Register(s)     |
+---+--------------------+
| a |   %eax, %ax, %al   |
| b |   %ebx, %bx, %bl   |
| c |   %ecx, %cx, %cl   |
| d |   %edx, %dx, %dl   |
| S |   %esi, %si        |
| D |   %edi, %di        |
+---+--------------------+

// there are many others, look at http://ibiblio.org/gferg/ldp/GCC-Inline-Assembly-HOWTO.html


// CLOBBERS are comma-separated list of register which you code affects (all the registers you write in your code)
//   it tells to compiler that it shouldn't count on them anymore, it must preserve they values if it needs to

int a = 1;
int c;

asm("mov edx %1;\
     add edx, %2;\
     mov edx %0"
    : "=r"(c) // "c" variable can be placed in any register and will be rewritten by the asm 
    : "r"(a), "r"(2) // input may be an expression 
    : "edx" ); // this code rewrites "edx" register so we mention it in CLOBBERS list 


// %0 .. %2 means "values in order of apperance from output to input, from left to right"
// e.g %0 = a, %1 = b, %2 = c, %3 = d
asm("..." : "=r"(a), "=r"(b) : "r"(c), "r"(d))

// a variable might be input and output same time
asm("add %0, %2" 
    : "=r"(a) // %0
    : "0"(a),  // explicitly set %0 to a but %1 = a anyway
      "r"(b) ); 

