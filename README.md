# BFI: stdin in brainfuck (v0.9.1)

## ... what ?

**BFI** is a brainfuck written in c, with support for real time stdin (no static inputs).

**BFI** is:
- 0% external libraries (because im too lazy to set that up)
- 0% make, cmake or anything (because im too lazy to make it work)
- 100% homemade, poorly documentated, error prone, c newbie code !

## requirements

gcc i guess and that should be it.

## how to run
> BFI has only ever been tested on linux, no guarantee of it ever working anywhere else

first get BFI

```bash
git clone https://github.com/js-all/bfi.git
```

then cd into the dirrectory

```bash
cd bfi
```

next, compile BFI

```bash
./compile.sh
```

this will create the BFI executable, and code.bf if missing and run the executable (might remove).
in most case this will end up in an error but don't worry it **should** be working.

you will just need to write some brainfuck in code.bf

> example (just type back what you wrote, not tested with other keys than alphabet)
> ```brainfuck
> >>+[<,[.[-]]>]
> ```

and rerun BFI

```bash
./bfi
```

and it should work.

> if it doesn't, open an issue on the githup repository, and if you're lucky enough, i might just get to think about it one day.

## added instructions

> those will only be for debugging purposes

### \#:

write the decimal value of the cell

## known issues

### characters are written on new line everytime

exactly what the title says, it is done on purpose, because it just wouldn't output them otherwise

## changelog

### v0.9.6 (lastest)

added # dump instruction (for an easier debugging), see added instruction for more details

and fixed segmentation fault when bf finished its execution.

### v0.9.5

added README.md

### v0.9.0

first working version of BFI

### v0.0.1

begining of the project, nothing really