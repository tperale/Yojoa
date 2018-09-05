# Compiler course

## Running WASM exported functions

There is a `runwasm.js` script in the `wasm` folder it help running exported function in a `.wasm` compiled file.

```
# first compile .wat to .wasm
> wat2wasm helloworld.wat
# run it with the script: node runwasm.js <wasm_file> <function> <arguments>
> node runwasm.js helloworld.wasm addHello 2
```

The code used to execute the code was found in this [gist](https://gist.github.com/kanaka/3c9caf38bc4da2ecec38f41ba24b77df)

## References

### Compiler

* [Compiler course page](http://tinf2.vub.ac.be/~dvermeir/courses/compilers/)
* [Compiler example](http://tinf2.vub.ac.be/~dvermeir/courses/compilers/micro/<Paste>)
* [tutorialspoint.com/compiler\_design](https://www.tutorialspoint.com/compiler_design/)
* [Writing your own toy compiler](https://gnuu.org/2009/09/18/writing-your-own-toy-compiler/)

### WASM

* [rsms.me/wasm-intro](https://rsms.me/wasm-intro)
* [research.mozilla.org/webassembly](https://research.mozilla.org/webassembly/)
* [scottlogic.com/webassembly-by-hand](https://blog.scottlogic.com/2018/04/26/webassembly-by-hand.html)
* [Text format spec](https://webassembly.github.io/spec/core/text/index.html)
* [mozilla.org/Understanding_the_text_format](https://developer.mozilla.org/en-US/docs/WebAssembly/Understanding_the_text_format)
* [WASM Semantics](https://github.com/WebAssembly/design/blob/master/Semantics.md)

### Bison

* [wikipedia.org/wiki/GNU\_bison](https://en.wikipedia.org/wiki/GNU_bison)
