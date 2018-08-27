(module
  (func $tiny (result i32)
    (local $foo i32)
    (set_local $foo (i32.const 1))
    (get_local $foo) ;; Pushing the statement condition into the stack.
    (if
      (then
        (set_local $foo (i32.const 0))
        (return (get_local $foo))
      )
      (else )
    )
    (set_local $foo (i32.const 3))
    (get_local $foo)
  )
  (export "tiny" (func $tiny))
)
