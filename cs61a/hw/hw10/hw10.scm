
(define (filter pred lst)
    (define (actual lst acc)
        (cond ((null? lst) acc)
              ((pred (car lst))(actual (cdr lst)(append acc (list (car lst)))))
              (else (actual (cdr lst) acc))))
    (actual lst nil))



(define (scale-stream s k)
  (if (null? s)
    nil
    (cons-stream (* k (car s)) (scale-stream (stream-cdr s) k))
    )
  
)


(define (merge s0 s1)
  (cond ((null? s0) s1)
        ((null? s1) s0)
        (else
           (
            cond ((< (car s0) (car s1)) (cons-stream (car s0) (merge (stream-cdr s0) s1)))
                 ((> (car s0) (car s1)) (cons-stream (car s1) (merge s0 (stream-cdr s1))))
                 ((= (car s0) (car s1)) (cons-stream (car s0) (merge (stream-cdr s0) (stream-cdr s1))))
           )
        )
  )

)



(define (make-s)
  (cons-stream 1
      (  merge    (merge (scale-stream (make-s) 2 ) (scale-stream (make-s)  3) )  (scale-stream (make-s) 5 ))
  )
  )


;;; Utilities

(define (int-list n total)
    (if (= n 0)
        total
        (int-list (- n 1) (cons n total))))

(define (equal? s0 s1)
  (cond ((and (null? s0) (null? s1)) True)
        ((or (null? s0) (null? s1)) False)
        (else (and (= (car s0) (car s1))
                   (equal? (cdr s0) (cdr s1))))))


(define (integers first)
  (cons-stream first (integers (+ first 1))))

(define (stream-to-list s num-elements)
  (if (or (null? s) (= num-elements 0))
    nil
    (cons (car s) (stream-to-list (stream-cdr s) (- num-elements 1)))))

