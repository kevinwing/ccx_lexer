# Specification

Modern translation systems often use lexical analysis to divide an input into meaningful units.
Once these meaningful units (or lexemes or tokens) have been derived, other components within
the translation system are used to determine the relationships among the lexemes.
Lexical analyzers (or lexers) are commonly used in compilers, interpreters, and other translation
systems that you have often used. The act of lexical analysis is also known as scanning.
For this assignment you are to build a lexer that will successfully scan through a set of programs
expressed in the CCX programming language. You have never used CCX, and that is just fine:
scanning through CCX programs won't require intimate knowledge of CCX.
Your lexer shall be written in the C or C++ programming language. Your lexer will be compiled and
tested using a Makefile and Linux build tools. Your program must run from the command line.

For C++ you are encouraged to use any standard template library (STL) containers (e.g. <vector>),
algorithms (via <algorithm>), or other facilities in your lexer. Be aware that this may take more
time than it saves if you are not familiar with STL tools. Also, only up to C++ 17 STL is allowed. 

For C you may use any C functions that you like, but beware that use of unsafe functions that
allow buffer overruns is discouraged. 

You may not use any lexer generators or other automated tools to complete this assignment.

With the exception standard C and C++ libraries. All algorithms, structures, and techniques 
employed in your lexer must be of your own making. The appropriate use of containers is
encouraged.

A sample CCX program is shown in [Figure 1](#figure-1-ccx-sample) below. This program simply prints the string
"Hello, world" to the screen and then prints the arguments to the program if any were provided. This program is
also available on [github](ccx/hello_world.ccx)

#### Figure 1 CCX sample
```
/*
 * Hello world with args.
 */
procedure main(argc: integer; argv: string_vector_type) is
begin
    printf("Hello, world\n");
    loop
        argc := argc - 1;
        exit when (argc = 0);
        printf("arg[%d]: %s\n", argc, argv[argc]);
    end loop;
end main;
```

The goal of lexical analysis is to break programs like the one in [Figure 1](#figure-1-ccx-sample) into lexemes that are
eventually used by other components within the translation system to determine things like
whether the program is legal and what the program does. Although the requirements and
constraints imposed upon lexical analysis may vary considerably between different translation
systems, the requirements for most lexers (and for this assignment) are very simple.

Your lexer shall open a file provided on the command-line, discover the lexemes
found in the file, classify each lexeme, and print out each lexeme and its classification to the
output file named with the same name as the input file with “.lexer.out” as the last part of the name.
Your lexer shall classify each lexeme found in a given source file into one of 8 categories.

These categories are:
Comment, string, keyword, character literal, numeric literal, operator, identifier, and UNK.
The details concerning each of these categories is specified later in this document, but for now
let's look at the output for a CCX lexer that meets the requirements of this assignment.
[Figure 2](#figure-2ccx-sample-output), also [ccx/hello_world.ccx.lexer.out](ccx/hello_world.ccx.lexer.out) produced by a lexer when scanning the source code presented in [Figure 1](#figure-1-ccx-sample). Each lexeme and its classification is printed on a separate line. A single space appears
between each lexeme and its classification, and the classification appears in parentheses.
#### Figure 2 CCX sample output
```
/*
 * Hello world with args.
 */ (comment)
procedure (keyword)
main (identifier)
( (operator)
argc (identifier)
: (operator)
integer (keyword)
; (operator)
argv (identifier)
: (operator)
string_vector_type (identifier)
) (operator)
is (keyword)
begin (keyword)
printf (identifier)
( (operator)
"Hello, world\n" (string)
) (operator)
; (operator)
loop (keyword)
argc (identifier)
:= (operator)
argc (identifier)
- (operator)
1 (numeric literal)
; (operator)
exit (keyword)
when (keyword)
( (operator)
argc (identifier)
= (operator)
0 (numeric literal)
) (operator)
; (operator)
printf (identifier)
( (operator)
"arg[%d]: %s\n" (string)
, (operator)
argc (identifier)
, (operator)
argv (identifier)
[ (operator)
argc (identifier)
] (operator)
) (operator)
; (operator)
end (keyword)
loop (keyword)
; (operator)
end (keyword)
main (identifier)
; (operator)
```

Please examine this [output](#figure-2-ccx-sample) closely. Each lexeme must be printed on a separate line, and a
single space must appear between the lexeme and its classification. The lexeme itself must start
in the first column on a given line. The classification of a lexeme must appear in parentheses.
No “blank” or “empty” lines can appear in the output unless they are part of a multi-line
comment. Each lexeme must be printed precisely as it appears in the source file. Do not
bracket the lexeme in quotes or any other characters.

## The Lexeme Categories
As mentioned, your lexer shall classify each lexeme encountered into one of 8 categories.
The details of each category follow.
* __comment__

  Comments in CCX begin with /\* and end with \*/ (C-style comments). Comments can span
  multiple lines. Everything encountered between (and including) the /\* and \*/ delimiters is
  considered part of the comment lexeme.

* __identifier__
  
  Identifiers are used in programs to name entities such as variables. Every programming language
  has its own rules as to what constitutes a legal identifier. In CCX an identifier can be composed
  of letters, digits, and underscores, but must start with a letter. You may assume that your lexer
  will never encounter an identifier that is more than 256 characters long.
* __string__
  
  Strings in CCX are literals delimited by double-quotes "like this". The double-quotes are part of
  the lexeme. When you print a lexeme that has been classified as a string, you must print the
  double-quotes. You may assume that your lexer will never encounter a string that is more than
  256 characters long.  
* __keyword__
  
  CCX contains many keywords. Keywords are sometimes called reserved words. Keywords (like
  all of CCX) are case-sensitive, and may not be used as identifiers in legal programs. It is not the
  job of the lexer to determine whether a keyword is misused; the lexer simply classifies a
  particular lexeme as being a keyword.
  The following are the list of CCX keywords that your lexer must recognize:
  ```
  accessor and array begin bool case character constant else elsif
  end exit function if in integer interface is loop module mutator
  natural null of or others out positive procedure range return
  struct subtype then type when while
  ```
* __character literal__
  
  Character literals in CCX are literals in single-quotes like this: 'x'. CCX allows character escape
  sequences in character literals, such as '\020' but your lexer need not support this.
* __operator__
  
  CCX contains many operators. Some operators consist of a single character, whereas others
  contain multiple characters. The following is a list of the operators that your lexer must
  recognize.
  Each operator is enclosed in double-quotes for the purpose of disambiguation, but these double quotes are not part of the operator:
  ".", "<", ">", "(", ")", "+", "-", "\*", "/", "|", "&", ";", ",", ":", "[", "]", "=", ":=", "..", "<<", ">>", "<>", "<=", ">=",
  "\*\*", "!=", and "=>".
* __numeric literal__
  
  CCX allows numeric literals in multiple forms. Your lexer will recognize a simplified subset of
  CCX numeric literals. Each numeric literal encountered by your lexer will start with a decimal
  digit and will contain only the following:
  - decimal digits (0 through 9)
  - hexadecimal digits (A through F and/or a through f)
  - the special characters ’ ’, ’.’, and ’#’
  - any other character encountered will denote that the numeric literal has ended, and a new lexeme has begun.

* __unknown__
  
  This special category is set aside for lexemes that your lexer cannot classify, and is intended to
  assist you in building and debugging your lexer. This category is composed of all lexemes that
  do not fit in any of the other specified categories. Your lexer will only be tested against legal
  CCX programs, so if the logic in your lexer is correct, you should never encounter an UNK
  lexeme. If, however, your lexer does encounter a lexeme that does not fit the requirements of any
  of the other categories, your lexer must print the offending lexeme, along with its category name
  in parenthesis, and immediately terminate.

*Note that length assumptions are dangerous from a cyber security standpoint
and should never be an acceptable practice in production code.*

### Another CCX Sample
[Figure 3](#figure-3-ccx-sample-2), also [ccx/list.cci](ccx/list.cci) is another CCX sample source file. CCX provides direct support for modular
programming, and as such makes use of the notion of module interfaces. This file encodes the
opaque types and interface for a module to support linked lists.
module list interface
#### Figure 3 CCX Sample 2
```
module list interface
    /*
     * types
     */
    type list_type;
    type listel_type;
    subtype list_size_type is natural;

    /*
     * routines
     */
    mutator  append(l: in out list_type; x: in ptr_type);
    accessor data(e: listel_type) return ptr_type;
    function find(l: list_type; x: ptr_type) return listel_type;
    accessor head(l: list_type) return listel_type;
    mutator  insert(l: in out list_type; e: in out listel_type; x: in ptr_type);
    accessor next(e: listel_type) return listel_type;
    mutator  prepend(l: in out list_type; x: in ptr_type);
    accessor prev(e: listel_type) return listel_type;
    mutator  remove(l: in out list_type; e: in out listel_type);
    function size(l: list_type) return list_size_type;
end list;
```

It is apparent from the sample in [Figure 3](#figure-3-ccx-sample-2) that CCX supports accessors, mutators, and functions
as well as procedures. Your lexer need not be concerned with the differences between each of
these.
[ccx/list.cci.lexer.out](ccx/list.cci.lexer.out) is the output produced by a lexer when scanning the source code presented in
[Figure 3](#figure-3-ccx-sample-2).

Please carefully examine the [output](ccx/list.cci.lexer.out). Note that even though the source 
in [Figure 3](#figure-3-ccx-sample-2) defines list_type as a type, the lexer classifies list_type 
as an identifier. This is correct. This means that your lexer need not be concerned about scalar, aggregate, or user-defined types that
are encountered in source code. The list of keywords and operators provided earlier in this
document will not change regardless of the meaning of the source code that your lexer is
scanning at any given time. Since list_type is not a keyword and satisfies the rules for an
identifier, it is classified as such.
[ccx/list.ccx](ccx/list.ccx) CCX source code is the implementation of a module to support linked lists.
This is the implementation of the module whose interface was shown in [Figure 3](#figure-3-ccx-sample-2) above.

In CCX the interface of a module and its implementation are two distinct entities.
Each of these entities usually appears in a separate file. Files are usually named such that the
interface for a module appears in a file named modulename.cci, and the implementation of the
module appears in a file named modulename.ccx. In the sample code for the list interface shown
in [Figure 3](#figure-3-ccx-sample-2) and the implementation shown above, the interface was placed in a file called
list.cci and the implementation was placed in a file called list.ccx.
[ccx/list.ccx.lexer.out](ccx/list.ccx.lexer.out) list-sample-output) shows the output produced by a lexer when scanning the source code shown above.

### Grading
Your lexer will be built using a Makefile and Ubuntu tools as installed on NIC lab laptops. 
You will test your lexer against the input files listed in [Figure 7](#figure-7-test-source-files):

#### Figure 7 Test Source Files
```
hello_world.ccx
list.cci
list.ccx
complex.cci
complex.ccx
date.cci
date.ccx
natural.cci
natural.ccx
trie.cci
trie.ccx
widget.cci
widget.ccx
```

Each of these source files and the result produced by a correct lexer when scanning the file is available in the github assignment. You will need to compare the output of your lexer with the reference files using diff. If your lexer produces correct results for each of the files listed above, you will earn
100% of the total possible points on this assignment.
Your lexer must compile and run using the supplied Makefile on an Ubuntu system equivalent to what is installed on the NIC lab laptops.
There are 13 files. For every file that your lexer doesn't produce correct output you will lose 10% (at my discretion).
Your lexer must take its arguments from the command line.

```
lexer hello_world.ccx
lexer list.cci
```
Your grade will be determined by a combination output correctness and code readability.
Readable code that doesn't get the right answer is not useful in real life and code
that is not readable can lead to maintainability issues. 
### More Requirements
There are many ways to implement this lexer. Since this lexer is a sort of simple compiler
where input is taken from the command line and output is generated you lexer must take input
from the command line. In our case 1 file at a time.

It will be your responsibility to prove that the output of your program exactly matches the
expected output in the [ccx directory](ccx) of this repository.

Do this using the Linux diff tool, showing the differences between your output and the expected output.
A bash script that runs the entire test suite should be created for convenience. Making this part
of you Makefile under a "test" rule is also recommended. A sample Makefile is provided that does this.

Your program will be evaluated for neatness and clarity as well as correctness. You must
document your program using relevant comments. You must use a consistent programming style.
Using tools such as clang-format to enforce a style can be helpful.
#### Hints
Your lexer need only be tested against the source files listed in [Figure 7](#figure-7-test-source-files). 
Each of these source files is legal CCX. Your lexer is not expected to be bullet proof, so don't spend time trying to handle the
unbounded set of all legal and illegal CCX programs.

You would do well to think of your lexer as a finite state machine that operates on a character-by-character basis. The set of states in such a machine will be relatively small.

Build your lexer incrementally according to the steps provided.
#### Submitting Your Project
Your lexer should be organized into multiple source files for clarity. Figure our a good
naming convention and create an appropriate set of include files for prototypes.

All code will be submitted to GitHub Canvas. Make sure that you use some sort sensible organization.
