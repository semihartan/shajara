# shajara (*Tree in Arabish*)

## Description

shajara is a small utility to automate creating hierarchical directory structures. It defines a simple markup language to specify these structures.

## Syntax for the language

The syntax of the language is inspired from Python's syntax for block definitions. You specify each directory name after a plus (**+**) sign by leaving a space between them.

To specify a subdirectory, you have to add an indentation before it. It also supports C and Python-style comments.

Here is the example structure file.

```
+ A
	+ B	// A C-style comment.
	+ C	# A pyhton-style comment.
	+ D
		+ F
		+ G
	+ H
	+ I
+ J
+ K
+ K
	+ L
	+ M
	+ N
```
 
