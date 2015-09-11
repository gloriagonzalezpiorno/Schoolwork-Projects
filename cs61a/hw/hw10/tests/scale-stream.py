test = {
  'name': 'scale-stream',
  'points': 1,
  'suites': [
    {
      'cases': [
        {
          'code': r"""
          scm> (define s (scale-stream (cons-stream 1
          ....                           (cons-stream 5
          ....                             (cons-stream 2 nil)))
          ....                         5))
          s
          scm> (stream-to-list s 3)
          (5 25 10)
          """,
          'hidden': False,
          'locked': False
        },
        {
          'code': r"""
          scm> (define s (scale-stream (integers 1) 2))
          s
          scm> (stream-to-list s 5)
          (2 4 6 8 10)
          """,
          'hidden': False,
          'locked': False
        }
      ],
      'scored': True,
      'setup': r"""
      scm> (load 'hw10)
      """,
      'teardown': '',
      'type': 'scheme'
    }
  ]
}