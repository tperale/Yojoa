(module
  (import "console" "write" (func $write (param i32)))
  (memory $memory (data "0"))
  (func $tiny   (result i32)   (local $i i32)
    (i32.store (i32.add (i32.mul (i32.const 0) (i32.const 4)) (i32.const 0) ) (i32.const 72))
    (i32.store (i32.add (i32.mul (i32.const 1) (i32.const 4)) (i32.const 0)) (i32.const 101))
    (i32.store (i32.add (i32.mul (i32.const 2) (i32.const 4)) (i32.const 0)) (i32.const 108))
    (i32.store (i32.add (i32.mul (i32.const 3) (i32.const 4)) (i32.const 0)) (i32.const 108))
    (i32.store (i32.add (i32.mul (i32.const 4) (i32.const 4)) (i32.const 0)) (i32.const 111))
    (i32.store (i32.add (i32.mul (i32.const 5) (i32.const 4)) (i32.const 0)) (i32.const 32))
    (i32.store (i32.add (i32.mul (i32.const 6) (i32.const 4)) (i32.const 0)) (i32.const 87))
    (i32.store (i32.add (i32.mul (i32.const 7) (i32.const 4)) (i32.const 0)) (i32.const 111))
    (i32.store (i32.add (i32.mul (i32.const 8) (i32.const 4)) (i32.const 0)) (i32.const 114))
    (i32.store (i32.add (i32.mul (i32.const 9) (i32.const 4)) (i32.const 0)) (i32.const 108))
    (i32.store (i32.add (i32.mul (i32.const 10) (i32.const 4)) (i32.const 0)) (i32.const 100))
    (i32.store (i32.add (i32.mul (i32.const 11) (i32.const 4)) (i32.const 0)) (i32.const 33))
    (i32.store (i32.add (i32.mul (i32.const 12) (i32.const 4)) (i32.const 0)) (i32.const 92))
    (set_local $i (i32.const 0))
    (block
      (loop
        (br_if 1 (i32.eq (i32.lt_s (get_local $i) (i32.const 12)) (i32.const 0)))
          (call $write (i32.load (i32.add (i32.mul (get_local $i) (i32.const 4)) (i32.const 0))))
          (set_local $i (i32.add (get_local $i) (i32.const 1)))
        (br 0)
      )
    )
    (i32.const 0)
  )
  (export "tiny" (func $tiny))
)