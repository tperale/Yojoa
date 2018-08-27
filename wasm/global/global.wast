(module
  (global $y (mut i32) (i32.const 0))
  (func $global (param $x i32) (param $y i32) (result i32)
    get_local $x
    get_global $y
    i32.add
  )
  (export "global" (func $global))
  (func $global1 (param $x i32) (param $y i32) (result i32)
    (set_global $y (i32.const 3))
    (i32.add
      (get_local $x)
      (get_global $y)
    )
  )
  (export "global1" (func $global1))
)
