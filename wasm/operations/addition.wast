(module
  ;; Classical version of addition in wast
  (func $addition (param $x i32) (param $y i32) (result i32)
    get_local $x
    get_local $y
    i32.add
  )
  ;; Stack based version of addition in wast
  (func $addition2 (param $x i32) (param $y i32) (result i32)
    (i32.add
      (get_local $x)
      (get_local $y)
    )
  )
  (export "addition" (func $addition))
  (export "addition2" (func $addition2))
)
