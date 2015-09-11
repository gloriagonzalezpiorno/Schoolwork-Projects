test = {
  'name': 'What would Scheme print?',
  'points': 0,
  'suites': [
    {
      'cases': [
        {
          'code': r"""
          scm> (define ones (cons-stream 1 ones))
          ac617e69bb362320d64986dbad5e6e5e
          # locked
          scm> (define twos (cons-stream 2 twos))
          bec1bafeb4628e1edf583008db853155
          # locked
          scm> (has-even? twos)
          f3731fc96153a8eaada2c20cb22d07df
          # locked
          scm> (define (foo x) (+ x 1))
          e5dafa6b3dcb3ab66c91bc31e78bb954
          # locked
          scm> (define bar (cons-stream (foo 1) (cons-stream (foo 2) bar)))
          484c1d6dd76dfab2e340b06231c61b70
          # locked
          scm> (car bar)
          c8e68b9d4bc0f96d2e9c47efcc96a11d
          # locked
          scm> (define (foo x) (+ x 5))
          e5dafa6b3dcb3ab66c91bc31e78bb954
          # locked
          scm> (car bar)
          c8e68b9d4bc0f96d2e9c47efcc96a11d
          # locked
          scm> (stream-cdr bar)
          04c27cfede04dca073b31e903e6f8cb9
          # locked
          # choice: (7 . #[promise (not forced)])
          # choice: (7 . #[promise (forced)])
          # choice: (2 . #[promise (not forced)])
          # choice: (2 . #[promise (forced)])
          scm> (define (foo x) (+ x 7))
          e5dafa6b3dcb3ab66c91bc31e78bb954
          # locked
          scm> (stream-cdr bar)
          04c27cfede04dca073b31e903e6f8cb9
          # locked
          # choice: (7 . #[promise (not forced)])
          # choice: (7 . #[promise (forced)])
          # choice: (9 . #[promise (not forced)])
          # choice: (9 . #[promise (forced)])
          """,
          'hidden': False,
          'locked': True
        }
      ],
      'scored': True,
      'setup': r"""
      scm> (define (has-even? s)
      ....   (cond ((null? s) False)
      ....         ((even? (car s)) True)
      ....         (else (has-even? (stream-cdr s)))))
      has-even?
      """,
      'teardown': '',
      'type': 'scheme'
    }
  ]
}