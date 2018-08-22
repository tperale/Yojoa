(module
  ;; Classical version of addition in wast
  (func $substraction (param $x i32) (param $y i32) (result i32)
    get_local $x
    get_local $y
    i32.sub
  )
  ;; Stack based version of addition in wast
  (func $substraction2 (param $x i32) (param $y i32) (result i32)
    (i32.sub
      (get_local $x)
      (get_local $y)
    )
  )
  (export "substraction" (func $substraction))
  (export "substraction2" (func $substraction2))
)
