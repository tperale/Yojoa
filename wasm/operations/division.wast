(module
  ;; Classical version of division in wast
  (func $division (param $x i32) (param $y i32) (result i32)
    get_local $x
    get_local $y
    i32.div_s
  )
  ;; Stack based version of division in wast
  (func $division2 (param $x i32) (param $y i32) (result i32)
    (i32.div_s
      (get_local $x)
      (get_local $y)
    )
  )
  ;; Classical version of division in wast for unsigned integers
  (func $udivision (param $x i32) (param $y i32) (result i32)
    get_local $x
    get_local $y
    i32.div_u
  )
  ;; Stack based version of division in wast for unsigned integers
  (func $udivision2 (param $x i32) (param $y i32) (result i32)
    (i32.div_u
      (get_local $x)
      (get_local $y)
    )
  )
  (export "division" (func $division))
  (export "division2" (func $division2))
  (export "udivision" (func $udivision))
  (export "udivision2" (func $udivision2))
)
