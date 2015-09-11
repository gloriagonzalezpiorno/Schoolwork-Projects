;; Tail Recursion Extra ;;

; Q8
(define (insert n s)
  'YOUR-CODE-HERE
)


;; Streams Extra ;;

(define (stream-to-list s num-elements)
  (if (or (null? s) (= num-elements 0))
    nil
    (cons (car s)
          (stream-to-list (stream-cdr s)
                          (- num-elements 1)))))

; Q9
(define (cycle lst)
  'YOUR-CODE-HERE
)


