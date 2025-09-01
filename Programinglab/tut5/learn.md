# OBJDUMP
## what is it? 
it is a command line tool to learn about object file, executables, etcc on unix like systems

## how to install Objdump


## syntax 
objdump [options] [object-files]

Option	Description	Example Usage
-f	Show file headers (architecture, entry point)	objdump -f a.out
-p	Print format-specific header info	objdump -p a.out
-h	Display section headers (.text, .data)	objdump -h a.out
-x	Show all headers (comprehensive)	objdump -x a.out
-d	Disassemble the code section	objdump -d a.out
-D	Disassemble all sections	objdump -D a.out
-s	Display section contents (hex view)	objdump -s a.out
-t	Show symbol table entries	objdump -t a.out
-r	Show relocation entries	objdump -r a.out
-g	Print debugging info if present	objdump -g a.out
-S	Intermix source code with disassembly (if debug info available)	objdump -S a.out
-C	Demangle C++ symbols for readability	objdump -C a.out
-H	List available options (help)	objdump -H
-i	List supported formats/architectures	objdump -i

# GDB

## what is gdb?
Command line tool to help inspect, analyis and control the execution of c/c++ program.