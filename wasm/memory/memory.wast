
(module
  (memory $data (data "0"))
  ;; Run and store a value with no offset
  (func (export "memory1") (result i32)
    (i32.store offset=0 (i32.const 0) (i32.const 2))
    (i32.load offset=0 (i32.const 0))
  )
  ;; Test storing and retrieving in offsetted memory
  (func (export "memory2") (result i32)
    (i32.store offset=0 (i32.const 0) (i32.const 2))
    (i32.store offset=4 (i32.const 0) (i32.const 3))
    (i32.load offset=4 (i32.const 0))
  )
  ;; test the offset in the argument.
  (func (export "memory3") (result i32)
    (i32.store offset=4 (i32.const 0) (i32.const 3))
    (i32.load offset=0 (i32.const 4))
  )
)
