(module
  (func $bar (param $a i32) (result i32)
    (i32.add
      (i32.const 42)
      (get_local $a)
    )
  )
  (export "bar" (func $bar))
  (func $tiny (result i32)
    (local $foo i32)
    (set_local $foo
      (i32.add
        (call $bar (i32.const 4))
        (i32.const 2)
      )
    )
    (get_local $foo)
  )
  (export "tiny" (func $tiny))
)
