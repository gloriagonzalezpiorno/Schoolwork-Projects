test = {
  'name': 'merge',
  'points': 1,
  'suites': [
    {
      'cases': [
        {
          'code': r"""
          scm> (define twos (scale-stream (integers 1) 2))
          twos
          scm> (define threes (scale-stream (integers 1) 3))
          threes
          scm> (define m (merge twos threes))
          m
          scm> (stream-to-list m 10)
          (2 3 4 6 8 9 10 12 14 15)
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