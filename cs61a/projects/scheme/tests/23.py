test = {
  'name': 'Question 23',
  'points': 2,
  'suites': [
    {
      'cases': [
        {
          'code': r"""
          scm> (define p (delay (+ 1 2 3 4 5)))
          p
          scm> p
          #[promise (not forced)]
          scm> (force p)
          15
          scm> p
          #[promise (forced)]
          """,
          'hidden': False,
          'locked': False
        },
        {
          'code': r"""
          scm> (define p (delay (/ 1 0)))
          p
          scm> p
          #[promise (not forced)]
          scm> (force p)
          SchemeError
          scm> p
          #[promise (not forced)]
          """,
          'hidden': False,
          'locked': False
        },
        {
          'code': r"""
          scm> (define s (cons-stream 1 (cons-stream 2 nil)))
          s
          scm> s
          (1 . #[promise (not forced)])
          scm> (cdr s) ; A stream is just a pair
          #[promise (not forced)]
          scm> (stream-cdr s) ; equivalent to (force (cdr s))
          (2 . #[promise (not forced)])
          scm> s
          (1 . #[promise (forced)])
          """,
          'hidden': False,
          'locked': False
        },
        {
          'code': r"""
          scm> (define s (cons-stream 1 (cons-stream 2 nil)))
          s
          scm> s
          (1 . #[promise (not forced)])
          scm> (cdr s)
          #[promise (not forced)]
          """,
          'hidden': False,
          'locked': False
        },
        {
          'code': r"""
          scm> (define (map-stream proc s)
          ....         (if (null? s)
          ....             s
          ....             (cons-stream (proc (car s))
          ....                          (map-stream proc (stream-cdr s)))))
          map-stream
          scm> (define (stream-to-list s n)
          ....         (if (or (null? s) (= n 0))
          ....             nil
          ....             (cons (car s)
          ....                   (stream-to-list (stream-cdr s) (- n 1)))))
          stream-to-list
          scm> (define (naturals)
          ....         (cons-stream 1
          ....                      (map-stream (lambda (x) (+ x 1))
          ....                                  (naturals))))
          naturals
          scm> (stream-to-list (naturals) 10)
          (1 2 3 4 5 6 7 8 9 10)
          scm> (define (scale s n)
          ....         (map-stream (lambda (x) (* x n)) s))
          scale
          scm> (define twos (scale (naturals) 2))
          twos
          scm> (stream-to-list twos 10)
          (2 4 6 8 10 12 14 16 18 20)
          scm> (define (merge s1 s2)
          ....         (cond ((< (car s1) (car s2))
          ....                (cons-stream (car s1)
          ....                             (merge (stream-cdr s1) s2)))
          ....               ((> (car s1) (car s2))
          ....                (cons-stream (car s2)
          ....                             (merge s1 (stream-cdr s2))))
          ....               (else
          ....                (cons-stream (car s1)
          ....                             (merge (stream-cdr s1)
          ....                                    (stream-cdr s2))))))
          merge
          scm> (define threes (scale (naturals) 3))
          threes
          scm> (define twos-threes (merge twos threes))
          twos-threes
          scm> (stream-to-list twos-threes 10)
          (2 3 4 6 8 9 10 12 14 15)
          scm> (define (make-s)
          ....         (cons-stream 1
          ....                      (merge (scale (make-s) 2)
          ....                             (merge (scale (make-s) 3)
          ....                                    (scale (make-s) 5)))))
          make-s
          scm> (stream-to-list (make-s) 20)
          (1 2 3 4 5 6 8 9 10 12 15 16 18 20 24 25 27 30 32 36)
          """,
          'hidden': False,
          'locked': False
        }
      ],
      'scored': True,
      'setup': r"""
      scm> (load 'questions) ; Basic promise and stream tests
      """,
      'teardown': '',
      'type': 'scheme'
    },
    {
      'cases': [
        {
          'code': r"""
          scm> (define foo 0)
          foo
          scm> (define p1 (delay foo))
          p1
          scm> (define p2 (delay foo))
          p2
          scm> (force p1)
          0
          scm> (define foo 10)
          foo
          scm> (force p2)
          10
          scm> (force p1)
          0
          """,
          'hidden': False,
          'locked': False
        },
        {
          'code': r"""
          scm> (define p (delay (begin (print 'evaluate!) 5)))
          p
          scm> p
          #[promise (not forced)]
          scm> (force p)
          evaluate!
          5
          scm> p
          #[promise (forced)]
          scm> (force p) ; Should not evaluate encapsulated expression again
          5
          """,
          'hidden': False,
          'locked': False
        },
        {
          'code': r"""
          scm> (define x 0)
          x
          scm> (define p (delay (define x (+ x 1))))
          p
          scm> (force p)
          x
          scm> x ; Forcing a promise should not change values in original frame
          0
          """,
          'hidden': False,
          'locked': False
        },
        {
          'code': r"""
          scm> (define l (list 1 2 3))
          l
          scm> (define s (cons-stream 1 (cons-stream 2 nil)))
          s
          scm> (define s-alt (cons 1 (delay (cons 2 (delay nil)))))
          s-alt
          scm> (stream? s)
          True
          scm> (stream? s-alt)
          True
          scm> (stream? l)
          False
          scm> (list? s)
          False
          scm> (list? s-alt)
          False
          scm> (pair? s)
          True
          scm> (pair? s-alt)
          True
          """,
          'hidden': False,
          'locked': False
        }
      ],
      'scored': True,
      'setup': r"""
      scm> (load 'questions) ; Technical cases
      """,
      'teardown': '',
      'type': 'scheme'
    }
  ]
}