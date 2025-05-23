# mgcLisp

The goal of this project is to create a working version of Scheme that I can use
while reading SICP, The only major departure I forsee is that I will _not_ be 
implementing tail recursion, so loop with care.

## primitives:
    
    + - * / % - arithmatic operators
    lt gt lte gte eq - equality and comparison
    not or and - logic operators
    car cdr cons list append join - list operators
    say - prints expression to terminal
    id  - returns type of atom
    min - returns min element of list 
    max - returns max element of list

## Special Forms:

    let - define lexically bound variables: (let ((x 1) (y 2) (z 3)) ((* x y z)))
    cond - (cond ((eq x 3) 2) 
             ((eq x 2) 1)
             ((eq x 7) 3) 
             (else 0))
    if - limited conditional, execute expr1 or expr2 based on result of predicate expr: (if (lt x 5) (+ x 5) (- x 2))
    set! - change the value of a symbol is bound to in the environment (set! z 15)
    do - evaluate a list of expressions, taking the result of the last expressions as the returned value (do ((e1) (e2) (e3) (e4)))
    quote ' - return the quoted expression without evaluateing it: (' x) -> x

    lambda & - create anonymous function: 
                            ((& (x) 
                                (+ x x)) 2)
    define - bind values or functions to names: 
                                (define a 42)
                                (define b 13)
      
there is also a special form of define for creating named functions so you can write the expression:
      
                                (define double-it 
                                        (lambda (x) 
                                                (+ x x)))
or the equivelant expression:

                                (define (double-it x) (+ x x))
    
    
## standard library
        empty? - check if atom is an empty list
        count? - get number of list elements
        map    - map values of one list to their result in another list using a function
        filter - return memebrs of a list matching a predicate
        reduce - run a collect operation on a list
        nth    - return nth list element
        delay  - delay evaluation
        force  - force evaluation