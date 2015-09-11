test = {
  'name': 'fibs',
  'points': 1,
  'suites': [
    {
      'cases': [
        {
          'code': r"""
          scm> (define fibs (make-fib-stream 0 1))
          ed9d3ada3cd278fb34e16453c8775e38
          # locked
          scm> (stream-to-list fibs 10)
          7998558ebbe77951b250c19f8331f21d
          # locked
          """,
          'hidden': False,
          'locked': True
        }
      ],
      'scored': True,
      'setup': r"""
      scm> (load 'lab13)
      """,
      'teardown': '',
      'type': 'scheme'
    }
  ]
}