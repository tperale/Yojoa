#!/usr/bin/env node

// Copyright Joel Martin
// License MIT

const fs = require('fs'),
      assert = require('assert')
const readlineSync = require('./readline-sync');

assert('WebAssembly' in global,
        'WebAssembly global object not detected')

// Convert node Buffer to Uint8Array
function toUint8Array(buf) {
  var u = new Uint8Array(buf.length)
  for (var i = 0; i < buf.length; ++i) {
    u[i] = buf[i]
  }
  return u
}

// Based on:
// https://gist.github.com/kripken/59c67556dc03bb6d57052fedef1e61ab
//   and
// http://thecodebarbarian.com/getting-started-with-webassembly-in-node.js.html

// Loads a WebAssembly dynamic library, returns a promise.
// imports is an optional imports object
function loadWebAssembly(filename, imports) {
  // Fetch the file and compile it
  const buffer = toUint8Array(fs.readFileSync(filename))
  return WebAssembly.compile(buffer)
  .then(module => {
    // Create the imports for the module, including the
    // standard dynamic library imports

    imports = imports || {}
    imports.env = {
      memory: new WebAssembly.Memory({ initial: 256 }),
      table: new WebAssembly.Table({ initial: 0, element: 'anyfunc' })
    };
    imports.console = {
      write: function (byte) {
        const { StringDecoder } = require('string_decoder');
        var string = new StringDecoder('utf8').write(Buffer.from([byte]));
        process.stdout.write(string);
      },
      read: function (offset, length) {
        const answer = readlineSync.question('');
        const buf = Buffer.from(answer);
        for (let i = 0; i < (length < buf.length ? length : buf.length); ++i) {
          new Uint32Array(imports.env.memory.buffer)[offset + i] = buf[i];
        }
      }
    }
    // Create the instance.
    return new WebAssembly.Instance(module, imports)
  })
}

if (module.parent) {
    module.exports.loadWebAssembly = loadWebAssembly
} else {
  assert(process.argv.length >= 4,
          'Usage: ./runwasm.js prog.wasm func INT_ARG...')

  const wasm = process.argv[2],
        func = process.argv[3],
        // Convert args to either floats or ints
        args = process.argv.slice(4).map(
                x => x.match(/[.]/) ? parseFloat(x) : parseInt(x))

  loadWebAssembly(wasm)
  .then(instance => {
    var exports = instance.exports
    assert(exports, 'no exports found')
    assert(func in exports, func + ' not found in wasm module exports')
    //console.log('calling exports.'+func+'('+args+')')
    console.log(exports[func](...args))
  })
  .catch(res => {
    console.log(res)
  })
}
