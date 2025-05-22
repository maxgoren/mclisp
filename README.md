# mgcLisp

The goal of this project is to create a working version of Scheme that I can use
while reading SICP, The only major departure I forsee is that I will _not_ be 
implementing tail recursion, so loop with care.

## primitives:
  + - * / %
  lt gt lte gte eq
  not or and
  car cdr cons list append join
  say id min max

## Special Forms:

  let - define lexically bound variables: (let ((x 1) (y 2) (z 3)) ((* x y z)))
  lambda & - create a function: 
                            ((& (x) 
                                (+ x x)) 2)
  define - bind values or functions to names: 
                                (define double-it 
                                        (lambda (x) 
                                                (+ x x)))
  quote ' - return the quoted expression without evaluateing it: (' x) -> x
  cond - (cond ((eq x 3) 2) 
             ((eq x 2) 1)
             ((eq x 7) 3) 
             (else 0))
  if - limited conditional, execute expr1 or expr2 based on result of predicate expr: (if (lt x 5) (+ x 5) (- x 2))
  set! - change the value of a symbol is bound to in the environment (set! z 15)
  do - evaluate a list of expressions, taking the result of the last expressions as the returned value (do ((e1) (e2) (e3) (e4)))

