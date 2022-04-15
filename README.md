# ccx-parser
This project will require you to write a lexical analyzer program.  You may use C or C++. You will use a Makefile to build the program. A Makefile is provided as a starting point. It may or may not be sufficient for the entire project, but should give you a solid starting point. The requirements for this project are [here](ccx_parser_spec.md)

The project will be broken down into the following steps:

1. Create a program that takes a file as input and outputs a copy of the file with ".lexer.out" appended to the input file name.
   - The output should be an exact copy of the input file.
   - The purpose of this step is to create the necessary pieces to read a file and write a file with the required naming convention. It is intended that this code be used in the subsequent steps.
2. Create a program that parses a file and outputs only the comments in the original file using the same naming convention as in step 1.
   - The output should match the provided ".lexer.out" files comment output.
   - A comment is one of the lexeme categories that you will be parsing. Create a generic output routine that will format any input lexeme category to match the required output. This routine will be used for all subsequent steps.
   - This step builds on step 1 and gets you started in parsing input and classifying the input.
3. Add to your program the ability to identify strings. 
   - The output should match the provided ".lexer.out" files (string) output.
4. Add to your program the ability to identify keywords and operators.
   - The output should match the provided ".lexer.out" files for keyword and operator.
5. Add the ability to identify identifiers and literals etc.
   - At this point you should be more or less done.
   - Write automated testing code to validate your results.
   - Fix errors review the code ensuring that it is correctly documented is clean and understandable.
   - When you are satisfied submit the final program.

There will be a canvas assignment for each step with instructions on what to submit. In particular
it is necessary to distinguish what code in gitHub applies for each step. 

