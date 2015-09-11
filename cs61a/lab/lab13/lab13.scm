;; Tail Recursion ;;

; Q2
(define (last s)
  (if(null? (cdr s))
    (car s)
    (last (cdr s))
)


; Q3
(define (reverse lst)
  (if (null? lst)
    lst
    (append (reverse (cdr lst)) (car lst)))
)


; Q5
(define (interleave-map s1 s2)
  (cons-stream (car s1)
    (interleave-map s2 (cdr s1)))
)


; Q6
(define (stream-filter s pred)
    (if (= True (pred (car s)) )
      (cons-stream (car s))
      (stream-filter (cdr s) pred)
      )
)


; Q7
(define fibs (make-fib-stream 0 1))
(define (make-fib-stream a b)
  (cons-stream a)
  (make-fib-stream b (+ a b))
    )
)


(define (stream-to-list s num-elements)
  (if (or (null? s) (= num-elements 0))
    nil
    (cons (car s)
          (stream-to-list (stream-cdr s)
                          (- num-elements 1)))))
