# AlgoCopypasta
Copy-pasteable code for programming competitions and ICPC notebook templates.

# Usage
If you are on a rather recent version of ubuntu, you should be able to install the required dependencies by simply running install.sh.
```bash
chmod +x install.sh
./install.sh
```

After which, you should be able to generate the notebook by running:
```bash
./generate.sh
```

To modify the source, you can add a new file. 
To ignore a source, add a prefix `_` to its name, e.g: Renaming `./source/04_Geometry/05_Sutherland-Hodgman Algorithm.cpp` to `./source/04_Geometry/_05_Sutherland-Hodgman Algorithm.cpp` will make the script ignore it.

# Contributions
Contributions are welcome. Open a PR and I'll see if I can't merge it into the main branch.

Before you open a PR, make sure you've tested your pasta for correctness e.g. by using it to get ~2-3 ACs in contests or upsolving.

# Disclaimer
The snippets provided in this repo are not meant to be understood but rather plainly copy-pasted into whatever you're working on whenever they are needed. They are also not meant to be used in actual real-world programs without significant changes.

Because of this, the code style used here may be very scuffed while the code quality itself is questionable at best. Issues include:
- Minimalist variable naming
- Minimalist use of brackets
- Comma operator abuse
- Lack of whitespace
- Lack of access specifiers
- Lack of any form of namespacing
- Duplicated sections of code
- STL abuse
- etc.

Feel free to use this repo as an example of what NOT to do when writing code.

That being said, most snippets have already been tested for correctness, and I try to make the snippets I put here at least sort-of understandable (as much as I can anyway, there's stuff I just blatantly stole from other people and restyled) with some effort just in case some modification is required to solve whatever it is you have to.

# PDF Generation
Just run generate.sh. Don't forget to chmod +x it.

You can also change the author for e.g. ICPC team notebooks (or you simply just want your own name in there) in `headfoot.tex`.
